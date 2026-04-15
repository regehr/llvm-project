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
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/PatternMatch.h"

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

PreservedAnalyses WidthOptPass::run(Function &F, FunctionAnalysisManager &AM) {
  bool EverChanged = false, Changed;

  do {
    Changed = false;
    for (BasicBlock &BB : F)
      for (Instruction &I : make_early_inc_range(BB))
        if (auto *Cmp = dyn_cast<ICmpInst>(&I))
          Changed |= tryNarrowExpectOverZextI1(Cmp);
    EverChanged |= Changed;
  } while (Changed);

  return EverChanged ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
