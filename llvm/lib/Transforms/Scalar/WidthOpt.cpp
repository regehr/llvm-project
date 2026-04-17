//===- WidthOpt.cpp - Width optimization pass -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a width optimization pass.
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Scalar/WidthOpt.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/InstSimplifyFolder.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/Transforms/Utils/Local.h"

using namespace llvm;
using namespace PatternMatch;

// Returns true if Ty is i1 or <N x i1>.
static bool isI1Type(Type *Ty) {
  if (auto *VTy = dyn_cast<VectorType>(Ty))
    return VTy->getElementType()->isIntegerTy(1);
  return Ty->isIntegerTy(1);
}

// Optimize: icmp [ne|eq] (llvm.expect*(zext i1 %x, C, ...), 0)
//        -> llvm.expect*.i1(%x, trunc C, ...)         [for ne]
//        -> xor (llvm.expect*.i1(%x, trunc C, ...)), true  [for eq]
//
// This removes the zext and (for the 'ne' case) the icmp as well.
// Works for both scalar i1 and vector <N x i1> inputs.
static bool tryNarrowExpectOverZextI1(ICmpInst *Cmp) {
  ICmpInst::Predicate Pred = Cmp->getPredicate();
  if (Pred != ICmpInst::ICMP_EQ && Pred != ICmpInst::ICMP_NE)
    return false;

  // One operand must be zero (scalar or vector zeroinitializer).
  Value *LHS = Cmp->getOperand(0), *RHS = Cmp->getOperand(1);
  Value *ExpVal;
  if (match(RHS, m_Zero()))
    ExpVal = LHS;
  else if (match(LHS, m_Zero()))
    ExpVal = RHS;
  else
    return false;

  // The non-zero operand must be a single-use llvm.expect or
  // llvm.expect.with.probability call.
  auto *Call = dyn_cast<IntrinsicInst>(ExpVal);
  if (!Call || !Call->hasOneUse())
    return false;

  Intrinsic::ID IID = Call->getIntrinsicID();
  if (IID != Intrinsic::expect && IID != Intrinsic::expect_with_probability)
    return false;

  // The first argument of the call must be a zext from i1 (or <N x i1>).
  auto *Zext = dyn_cast<ZExtInst>(Call->getArgOperand(0));
  if (!Zext)
    return false;

  Value *NarrowSrc = Zext->getOperand(0);
  Type *NarrowTy = NarrowSrc->getType();
  if (!isI1Type(NarrowTy))
    return false;

  // For eq, the only instruction saving is the zext; skip if it has other uses.
  if (Pred == ICmpInst::ICMP_EQ && !Zext->hasOneUse())
    return false;

  // The expected-value argument must be a constant so we can truncate it.
  auto *ExpectedConst = dyn_cast<Constant>(Call->getArgOperand(1));
  if (!ExpectedConst)
    return false;

  IRBuilder<> Builder(Cmp);

  // Truncate the expected constant to i1 (or <N x i1>).
  Constant *NarrowExpected =
      cast<Constant>(Builder.CreateTrunc(ExpectedConst, NarrowTy));

  // Build args for the narrowed intrinsic.
  SmallVector<Value *, 3> Args = {NarrowSrc, NarrowExpected};
  if (IID == Intrinsic::expect_with_probability)
    Args.push_back(Call->getArgOperand(2));

  Function *NarrowFn =
      Intrinsic::getOrInsertDeclaration(Call->getModule(), IID, {NarrowTy});
  Value *NarrowCall = Builder.CreateCall(NarrowFn, Args);

  // For 'ne': the i1 call result is the answer directly.
  // For 'eq': negate it with xor.
  Value *Result;
  if (Pred == ICmpInst::ICMP_NE) {
    Result = NarrowCall;
  } else {
    assert(Pred == ICmpInst::ICMP_EQ);
    Result = Builder.CreateXor(NarrowCall, Constant::getAllOnesValue(NarrowTy));
  }

  Cmp->replaceAllUsesWith(Result);
  Cmp->eraseFromParent();
  Call->eraseFromParent();
  if (Zext->use_empty())
    Zext->eraseFromParent();

  return true;
}

// Opcodes that commute truncation: trunc(op(a,b)) == op(trunc(a), trunc(b)).
static bool isTruncCommutingBinop(unsigned Opc) {
  switch (Opc) {
  case Instruction::Add:
  case Instruction::Sub:
  case Instruction::Mul:
  case Instruction::And:
  case Instruction::Or:
  case Instruction::Xor:
    return true;
  default:
    return false;
  }
}

// Phase 1: check whether V can be expressed in NarrowTy without creating any
// instructions.  Returns true iff doNarrow() will succeed.  foundZext is set
// to true the first time a ZExtInst whose source has type NarrowTy is found,
// which proves at least one width-changing instruction will be eliminated.
static bool canNarrow(Value *V, Type *NarrowTy, bool &FoundZext,
                      SmallDenseMap<Value *, bool> &Cache) {
  auto It = Cache.find(V);
  if (It != Cache.end())
    return It->second;

  // Compute result before inserting into Cache: recursive calls may rehash it,
  // invalidating any iterator we hold across the call.
  bool Result = false;
  if (auto *Z = dyn_cast<ZExtInst>(V)) {
    if (Z->getOperand(0)->getType() == NarrowTy) {
      FoundZext = true;
      Result = true;
    }
  } else if (isa<Constant>(V)) {
    Result = true;
  } else if (auto *BO = dyn_cast<BinaryOperator>(V)) {
    if (isTruncCommutingBinop(BO->getOpcode()))
      Result = canNarrow(BO->getOperand(0), NarrowTy, FoundZext, Cache) &&
               canNarrow(BO->getOperand(1), NarrowTy, FoundZext, Cache);
  }

  Cache[V] = Result;
  return Result;
}

// Phase 2: create the narrowed version of V in NarrowTy.  Must only be called
// after canNarrow() returned true for the same V.  Builder's constant folding
// collapses identities (and i8 x, -1 -> x) and annihilators (mul i8 x, 0 -> 0)
// so no explicit folding is needed here.
static Value *doNarrow(Value *V, Type *NarrowTy,
                       IRBuilder<InstSimplifyFolder> &Builder,
                       SmallDenseMap<Value *, Value *> &NarrowCache) {
  auto It = NarrowCache.find(V);
  if (It != NarrowCache.end())
    return It->second;

  // Compute result before inserting: recursive calls may rehash the map.
  Value *Result;
  if (auto *Z = dyn_cast<ZExtInst>(V)) {
    assert(Z->getOperand(0)->getType() == NarrowTy);
    Result = Z->getOperand(0);
  } else if (auto *C = dyn_cast<Constant>(V)) {
    Result = cast<Constant>(Builder.CreateTrunc(C, NarrowTy));
  } else {
    auto *BO = cast<BinaryOperator>(V);
    Value *NA = doNarrow(BO->getOperand(0), NarrowTy, Builder, NarrowCache);
    Value *NB = doNarrow(BO->getOperand(1), NarrowTy, Builder, NarrowCache);
    Result = Builder.CreateBinOp(BO->getOpcode(), NA, NB);
  }

  NarrowCache[V] = Result;
  return Result;
}

// Optimize: trunc (binop-chain of zexts and constants) to NarrowTy
//        -> equivalent binop-chain operating entirely in NarrowTy
//
// Example: trunc(or(and(zext i8 %a to i32, 255), zext i8 %b to i32)) to i8
//       -> or(and(%a, -1), %b) -> or(%a, %b)   [constant-folded by IRBuilder]
//
// Profitable whenever at least one zext is eliminated.  Structurally
// profitable; no cost-model check needed.  Works for scalar and vector types.
static bool tryNarrowTrunc(TruncInst *Trunc) {
  Type *NarrowTy = Trunc->getType();
  Value *Src = Trunc->getOperand(0);

  SmallDenseMap<Value *, bool> CheckCache;
  bool FoundZext = false;
  if (!canNarrow(Src, NarrowTy, FoundZext, CheckCache))
    return false;
  if (!FoundZext)
    return false;

  const DataLayout &DL = Trunc->getDataLayout();
  InstSimplifyFolder Folder(DL);
  IRBuilder<InstSimplifyFolder> Builder(Trunc->getContext(), Folder);
  Builder.SetInsertPoint(Trunc);

  SmallDenseMap<Value *, Value *> NarrowCache;
  Value *Narrowed = doNarrow(Src, NarrowTy, Builder, NarrowCache);

  Trunc->replaceAllUsesWith(Narrowed);
  Trunc->eraseFromParent();

  // Remove wide instructions that are now dead.
  if (auto *SrcInst = dyn_cast<Instruction>(Src))
    RecursivelyDeleteTriviallyDeadInstructions(SrcInst);

  return true;
}

// Optimize: trunc (binop X, (zext Y from NarrowTy)) to NarrowTy
//        -> binop (trunc X to NarrowTy), Y
// for any truncation-commuting binary operator.
//
// Handles the case where X cannot be narrowed by canNarrow/tryNarrowTrunc
// (e.g. X is a function call).  Requires the binop and the zext each have
// exactly one use.  Eliminates zext + wide-binop + trunc and adds narrow-trunc
// + narrow-binop = net -1 instruction.  Structurally profitable; no cost-model
// check needed.  Works for scalar and vector types.
static bool tryNarrowTruncBinopZext(TruncInst *Trunc) {
  Type *NarrowTy = Trunc->getType();
  Value *Src = Trunc->getOperand(0);

  auto *BO = dyn_cast<BinaryOperator>(Src);
  if (!BO || !isTruncCommutingBinop(BO->getOpcode()) || !BO->hasOneUse())
    return false;

  Value *ZextSrc = nullptr, *Other = nullptr;
  for (int I = 0; I < 2; ++I) {
    auto *Z = dyn_cast<ZExtInst>(BO->getOperand(I));
    if (Z && Z->getOperand(0)->getType() == NarrowTy && Z->hasOneUse()) {
      ZextSrc = Z->getOperand(0);
      Other = BO->getOperand(1 - I);
      break;
    }
  }
  if (!ZextSrc)
    return false;

  IRBuilder<> Builder(Trunc);
  Value *NarrowOther = Builder.CreateTrunc(Other, NarrowTy);
  Value *Result = Builder.CreateBinOp(BO->getOpcode(), NarrowOther, ZextSrc);

  Trunc->replaceAllUsesWith(Result);
  Trunc->eraseFromParent();
  RecursivelyDeleteTriviallyDeadInstructions(BO);

  return true;
}

// Optimize: trunc (trunc X from WiderTy to MidTy) to NarrowTy
//        -> trunc X to NarrowTy
//
// Requires the inner trunc to have one use so it becomes dead.
// Net: -1 instruction.  Works for scalar and vector types.
static bool tryFoldTruncTrunc(TruncInst *Outer) {
  auto *Inner = dyn_cast<TruncInst>(Outer->getOperand(0));
  if (!Inner || !Inner->hasOneUse())
    return false;

  IRBuilder<> Builder(Outer);
  Value *Result = Builder.CreateTrunc(Inner->getOperand(0), Outer->getType());

  Outer->replaceAllUsesWith(Result);
  Outer->eraseFromParent();
  RecursivelyDeleteTriviallyDeadInstructions(Inner);

  return true;
}

// Optimize: trunc (call llvm.ctpop (zext X from NarrowTy)) to NarrowTy
//        -> call llvm.ctpop.NarrowTy (X)
//
// ctpop counts set bits; the upper bits of zext(X) are zero, so they
// contribute nothing.  Requires the ctpop call and the zext each have one
// use.  Eliminates zext + wide-ctpop + trunc = net -2 instructions.
// Works for scalar and vector types.
static bool tryNarrowCtpop(TruncInst *Trunc) {
  Type *NarrowTy = Trunc->getType();

  auto *Call = dyn_cast<IntrinsicInst>(Trunc->getOperand(0));
  if (!Call || !Call->hasOneUse())
    return false;
  if (Call->getIntrinsicID() != Intrinsic::ctpop)
    return false;

  auto *Z = dyn_cast<ZExtInst>(Call->getArgOperand(0));
  if (!Z || !Z->hasOneUse() || Z->getOperand(0)->getType() != NarrowTy)
    return false;

  IRBuilder<> Builder(Trunc);
  Function *NarrowFn = Intrinsic::getOrInsertDeclaration(
      Call->getModule(), Intrinsic::ctpop, {NarrowTy});
  Value *Result = Builder.CreateCall(NarrowFn, {Z->getOperand(0)});

  Trunc->replaceAllUsesWith(Result);
  Trunc->eraseFromParent();
  RecursivelyDeleteTriviallyDeadInstructions(Call);

  return true;
}

PreservedAnalyses WidthOptPass::run(Function &F, FunctionAnalysisManager &AM) {
  bool EverChanged = false, Changed;

  do {
    Changed = false;
    for (BasicBlock &BB : F)
      for (Instruction &I : make_early_inc_range(BB))
        if (auto *Cmp = dyn_cast<ICmpInst>(&I))
          Changed |= tryNarrowExpectOverZextI1(Cmp);
        else if (auto *Trunc = dyn_cast<TruncInst>(&I))
          Changed |= tryNarrowTrunc(Trunc) || tryNarrowTruncBinopZext(Trunc) ||
                     tryFoldTruncTrunc(Trunc) || tryNarrowCtpop(Trunc);
    EverChanged |= Changed;
  } while (Changed);

  return EverChanged ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
