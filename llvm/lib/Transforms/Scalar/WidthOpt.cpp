#include "llvm/Transforms/Scalar/WidthOpt.h"


#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/LazyValueInfo.h"
#include "llvm/Analysis/SimplifyQuery.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/PassManager.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/IR/Verifier.h"
#include <cassert>
#include <optional>

using namespace llvm;

namespace widthopt {

namespace {

enum class ExtKind {
  None,
  ZExt,
  SExt,
};

struct PhiShrinkInfo {
  ExtKind Kind = ExtKind::None;
  unsigned NarrowWidth = 0;
  unsigned WideWidth = 0;
  SmallVector<Instruction *, 8> Producers;
};

struct ExtOperandInfo {
  Value *NarrowValue = nullptr;
  Instruction *Producer = nullptr;
  ExtKind Kind = ExtKind::None;
  unsigned NarrowWidth = 0;
  unsigned WideWidth = 0;
};

IntegerType *getScalarIntegerTy(Type *Ty) {
  if (auto *IT = dyn_cast<IntegerType>(Ty))
    return IT;
  if (auto *VT = dyn_cast<FixedVectorType>(Ty))
    return dyn_cast<IntegerType>(VT->getElementType());
  return nullptr;
}

IntegerType *getScalarIntegerTy(Value *V) {
  return getScalarIntegerTy(V->getType());
}

bool isIntegerValue(Value *V) {
  return getScalarIntegerTy(V) != nullptr;
}

bool isScalarOrFixedVectorIntegerValue(Value *V) {
  return getScalarIntegerTy(V) != nullptr;
}

bool haveSameIntegerShape(Type *A, Type *B) {
  auto *AI = dyn_cast<IntegerType>(A);
  auto *BI = dyn_cast<IntegerType>(B);
  if (AI || BI)
    return AI != nullptr && BI != nullptr;

  auto *AV = dyn_cast<FixedVectorType>(A);
  auto *BV = dyn_cast<FixedVectorType>(B);
  return AV != nullptr && BV != nullptr &&
         isa<IntegerType>(AV->getElementType()) &&
         isa<IntegerType>(BV->getElementType()) &&
         AV->getNumElements() == BV->getNumElements();
}

unsigned getScalarIntegerWidth(Type *Ty) {
  auto *IT = getScalarIntegerTy(Ty);
  assert(IT && "Expected scalar integer or fixed integer vector type");
  return IT->getBitWidth();
}

Type *getSameShapeIntegerType(Type *Ty, unsigned ScalarWidth) {
  auto *ScalarTy = IntegerType::get(Ty->getContext(), ScalarWidth);
  if (isa<IntegerType>(Ty))
    return ScalarTy;

  auto *VT = cast<FixedVectorType>(Ty);
  return FixedVectorType::get(ScalarTy, VT->getNumElements());
}

Constant *getSameShapeIntegerConstant(Type *Ty, const APInt &Value) {
  auto *ScalarTy = getScalarIntegerTy(Ty);
  assert(ScalarTy && "Expected scalar integer or fixed integer vector type");
  Constant *ScalarC = ConstantInt::get(ScalarTy, Value);
  if (isa<IntegerType>(Ty))
    return ScalarC;

  auto *VT = cast<FixedVectorType>(Ty);
  return ConstantVector::getSplat(VT->getElementCount(), ScalarC);
}

Constant *getSameShapeBoolConstant(Type *OperandTy, bool Value) {
  Type *CmpTy = CmpInst::makeCmpResultType(OperandTy);
  return getSameShapeIntegerConstant(CmpTy, APInt(1, Value));
}

ConstantInt *getScalarOrSplatConstantInt(Value *V) {
  auto *C = dyn_cast<Constant>(V);
  if (!C)
    return nullptr;
  if (auto *CI = dyn_cast<ConstantInt>(C))
    return CI;
  if (!C->getType()->isVectorTy())
    return nullptr;
  return dyn_cast_or_null<ConstantInt>(C->getSplatValue());
}

Constant *getLowBitsMaskConstant(Type *Ty, unsigned NarrowWidth) {
  auto *EltTy = getScalarIntegerTy(Ty);
  assert(EltTy && "Expected scalar integer or fixed integer vector type");
  APInt Mask = APInt::getLowBitsSet(EltTy->getBitWidth(), NarrowWidth);
  return getSameShapeIntegerConstant(Ty, Mask);
}

unsigned getValueWidth(const Value *V) {
  return getScalarIntegerWidth(V->getType());
}

std::optional<ExtOperandInfo> getExtOperandInfo(Value *V) {
  if (auto *Z = dyn_cast<ZExtInst>(V)) {
    if (!isIntegerValue(Z->getOperand(0)) || !isIntegerValue(Z))
      return std::nullopt;
    return ExtOperandInfo{Z->getOperand(0), Z, ExtKind::ZExt,
                          getValueWidth(Z->getOperand(0)), getValueWidth(Z)};
  }

  if (auto *S = dyn_cast<SExtInst>(V)) {
    if (!isIntegerValue(S->getOperand(0)) || !isIntegerValue(S))
      return std::nullopt;
    return ExtOperandInfo{S->getOperand(0), S, ExtKind::SExt,
                          getValueWidth(S->getOperand(0)), getValueWidth(S)};
  }

  return std::nullopt;
}

bool canRepresentConstant(Constant &C, ExtKind Kind, unsigned NarrowWidth) {
  if (isa<UndefValue>(C) || isa<PoisonValue>(C))
    return Kind != ExtKind::None;

  if (auto *CI = getScalarOrSplatConstantInt(&C)) {
    APInt Narrow = CI->getValue().trunc(NarrowWidth);
    switch (Kind) {
    case ExtKind::ZExt:
      return CI->getValue() == Narrow.zext(CI->getBitWidth());
    case ExtKind::SExt:
      return CI->getValue() == Narrow.sext(CI->getBitWidth());
    case ExtKind::None:
      return false;
    }
    llvm_unreachable("Unexpected extension kind");
  }

  return false;
}

Constant *convertConstantToNarrow(Constant &C, unsigned NarrowWidth) {
  return ConstantExpr::getTrunc(&C, getSameShapeIntegerType(C.getType(),
                                                            NarrowWidth));
}


bool isEqOrNe(ICmpInst::Predicate Pred) {
  return Pred == ICmpInst::ICMP_EQ || Pred == ICmpInst::ICMP_NE;
}

bool isUnsignedICmp(ICmpInst::Predicate Pred) {
  return ICmpInst::isUnsigned(Pred);
}

bool isSignedICmp(ICmpInst::Predicate Pred) {
  return ICmpInst::isSigned(Pred);
}

unsigned computeShrinkWidth(ICmpInst::Predicate Pred, const ExtOperandInfo &LHS,
                            const ExtOperandInfo &RHS) {
  assert(LHS.Kind != ExtKind::None && RHS.Kind != ExtKind::None &&
         "Shrink rules require explicit extension structure");
  if (LHS.WideWidth != RHS.WideWidth)
    return 0;

  // Same-kind extension pairs are the easy cases: equality is preserved by
  // either extension kind, and signed/unsigned order is preserved by the
  // matching signedness-preserving extension.
  if (LHS.Kind == ExtKind::ZExt && RHS.Kind == ExtKind::ZExt) {
    if (isEqOrNe(Pred) || isUnsignedICmp(Pred))
      return std::max(LHS.NarrowWidth, RHS.NarrowWidth);
    return 0;
  }

  if (LHS.Kind == ExtKind::SExt && RHS.Kind == ExtKind::SExt) {
    if (isEqOrNe(Pred) || isSignedICmp(Pred))
      return std::max(LHS.NarrowWidth, RHS.NarrowWidth);
    return 0;
  }

  if (isEqOrNe(Pred)) {
    // Mixed sign/zero-extension equality also needs one extra distinguishing
    // bit on the zero-extended side. Without it, equal-width cases like
    // sext(i8 0x80) == zext(i8 0x80) would collapse to a wrong true i8 compare.
    if (LHS.Kind == ExtKind::SExt && RHS.Kind == ExtKind::ZExt)
      return std::max(LHS.NarrowWidth, RHS.NarrowWidth + 1);
    if (LHS.Kind == ExtKind::ZExt && RHS.Kind == ExtKind::SExt)
      return std::max(LHS.NarrowWidth + 1, RHS.NarrowWidth);
    llvm_unreachable("Mixed extension pairs should cover all remaining cases");
  }

  if (!isSignedICmp(Pred))
    return 0;

  // Mixed signed/unsigned extension pairs need one extra bit on the zero-
  // extended side so the narrowed compare can still distinguish all
  // non-negative values from the negative signed range.
  if (LHS.Kind == ExtKind::SExt && RHS.Kind == ExtKind::ZExt)
    return std::max(LHS.NarrowWidth, RHS.NarrowWidth + 1);

  if (LHS.Kind == ExtKind::ZExt && RHS.Kind == ExtKind::SExt)
    return std::max(LHS.NarrowWidth + 1, RHS.NarrowWidth);

  return 0;
}

Value *materializeAtWidth(IRBuilder<> &B, const ExtOperandInfo &Info,
                          unsigned TargetWidth,
                          DominatorTree *DT = nullptr) {
  assert(TargetWidth >= Info.NarrowWidth && "Cannot shrink below source width");
  assert(Info.Kind != ExtKind::None && "Expected explicit extension kind");

  if (TargetWidth == Info.NarrowWidth)
    return Info.NarrowValue;

  Type *TargetTy = getSameShapeIntegerType(Info.NarrowValue->getType(),
                                           TargetWidth);
  Instruction::CastOps CastOp =
      (Info.Kind == ExtKind::ZExt) ? Instruction::ZExt : Instruction::SExt;
  BasicBlock *InsertBB = B.GetInsertBlock();
  BasicBlock::iterator InsertPt = B.GetInsertPoint();
  Instruction *InsertInst =
      (InsertPt != InsertBB->end()) ? &*InsertPt : nullptr;

  // ConstantData values (e.g. ConstantInt) have no use list, so skip the
  // existing-cast searches below — they can only find instructions anyway.
  if (!isa<ConstantData>(Info.NarrowValue)) {
    // Prefer an existing cast in the same basic block; move it before the
    // insertion point if needed.
    for (User *U : Info.NarrowValue->users()) {
      auto *Cast = dyn_cast<CastInst>(U);
      if (!Cast || Cast->getOpcode() != CastOp || Cast->getType() != TargetTy)
        continue;
      if (Cast->getParent() != InsertBB)
        continue;
      if (InsertInst != nullptr && !Cast->comesBefore(InsertInst)) {
        // If Cast IS the insertion point, moving it "before itself" is a
        // no-op, and the caller will then insert a new instruction before
        // InsertInst (= Cast), creating a use-before-def violation.
        if (Cast == InsertInst)
          continue;
        // Only move if NarrowValue is already defined before the insertion
        // point; if NarrowValue is an instruction later in the same block,
        // moving Cast here would violate dominance.
        if (auto *NVI = dyn_cast<Instruction>(Info.NarrowValue))
          if (NVI->getParent() == InsertBB && !NVI->comesBefore(InsertInst))
            continue;
        Cast->moveBefore(*InsertInst->getParent(), InsertInst->getIterator());
      }
      return Cast;
    }

    // Next, look for an identical cast in a block that strictly dominates the
    // insertion block.  Such a cast is already in scope and needs no movement.
    if (DT) {
      for (User *U : Info.NarrowValue->users()) {
        auto *Cast = dyn_cast<CastInst>(U);
        if (!Cast || Cast->getOpcode() != CastOp || Cast->getType() != TargetTy)
          continue;
        BasicBlock *CastBB = Cast->getParent();
        if (CastBB != InsertBB && DT->dominates(CastBB, InsertBB))
          return Cast;
      }
    }
  }

  if (Info.Kind == ExtKind::ZExt)
    return B.CreateZExt(Info.NarrowValue, TargetTy);
  if (Info.Kind == ExtKind::SExt)
    return B.CreateSExt(Info.NarrowValue, TargetTy);

  llvm_unreachable("Unexpected extension kind");
}

std::optional<ICmpInst::Predicate>
getNarrowPredicateForZeroCompare(ICmpInst::Predicate Pred, ExtKind Kind) {
  if (Kind == ExtKind::SExt) {
    if (isEqOrNe(Pred) || isSignedICmp(Pred))
      return Pred;
    return std::nullopt;
  }

  if (Kind != ExtKind::ZExt)
    return std::nullopt;

  switch (Pred) {
  case ICmpInst::ICMP_EQ:
    return ICmpInst::ICMP_EQ;
  case ICmpInst::ICMP_NE:
    return ICmpInst::ICMP_NE;
  case ICmpInst::ICMP_UGT:
  case ICmpInst::ICMP_SGT:
    return ICmpInst::ICMP_NE;
  case ICmpInst::ICMP_ULE:
  case ICmpInst::ICMP_SLE:
    return ICmpInst::ICMP_EQ;
  default:
    return std::nullopt;
  }
}

std::optional<ICmpInst::Predicate>
getRetargetedZeroComparePredicate(ICmpInst &Cmp, Value &WideV, ExtKind Kind,
                                  unsigned WideWidth) {
  unsigned WideIdx = 0;
  if (Cmp.getOperand(1) == &WideV)
    WideIdx = 1;
  else if (Cmp.getOperand(0) != &WideV)
    return std::nullopt;

  auto *C = dyn_cast<Constant>(Cmp.getOperand(1 - WideIdx));
  if (!C || !isIntegerValue(Cmp.getOperand(1 - WideIdx)) ||
      !haveSameIntegerShape(C->getType(), WideV.getType()) ||
      getValueWidth(C) != WideWidth || !C->isNullValue())
    return std::nullopt;

  ICmpInst::Predicate Pred = Cmp.getPredicate();
  if (WideIdx == 1)
    Pred = Cmp.getSwappedPredicate();
  return getNarrowPredicateForZeroCompare(Pred, Kind);
}

std::optional<bool>
getKnownCompareResultWithConstantLHS(ICmpInst::Predicate Pred,
                                     const APInt &ConstValue) {
  if (isUnsignedICmp(Pred)) {
    switch (Pred) {
    case ICmpInst::ICMP_ULT:
      if (ConstValue.isMaxValue())
        return false;
      break;
    case ICmpInst::ICMP_ULE:
      if (ConstValue.isMinValue())
        return true;
      break;
    case ICmpInst::ICMP_UGT:
      if (ConstValue.isMinValue())
        return false;
      break;
    case ICmpInst::ICMP_UGE:
      if (ConstValue.isMaxValue())
        return true;
      break;
    default:
      break;
    }
    return std::nullopt;
  }

  if (isSignedICmp(Pred)) {
    switch (Pred) {
    case ICmpInst::ICMP_SLT:
      if (ConstValue.isMaxSignedValue())
        return false;
      break;
    case ICmpInst::ICMP_SLE:
      if (ConstValue.isMinSignedValue())
        return true;
      break;
    case ICmpInst::ICMP_SGT:
      if (ConstValue.isMinSignedValue())
        return false;
      break;
    case ICmpInst::ICMP_SGE:
      if (ConstValue.isMaxSignedValue())
        return true;
      break;
    default:
      break;
    }
  }

  return std::nullopt;
}

std::optional<bool>
getKnownCompareResultWithExtAndConstant(ICmpInst::Predicate Pred, ExtKind Kind,
                                        unsigned NarrowWidth,
                                        const APInt &ConstValue) {
  unsigned WideWidth = ConstValue.getBitWidth();

  if (Kind == ExtKind::ZExt) {
    APInt Max = APInt::getLowBitsSet(WideWidth, NarrowWidth);
    if (ConstValue.ule(Max))
      return std::nullopt;

    if (isEqOrNe(Pred))
      return Pred == ICmpInst::ICMP_NE;
    if (!isUnsignedICmp(Pred))
      return std::nullopt;

    switch (Pred) {
    case ICmpInst::ICMP_ULT:
    case ICmpInst::ICMP_ULE:
      return true;
    case ICmpInst::ICMP_UGT:
    case ICmpInst::ICMP_UGE:
      return false;
    default:
      return std::nullopt;
    }
  }

  if (Kind == ExtKind::SExt) {
    APInt Min = APInt::getSignedMinValue(NarrowWidth).sext(WideWidth);
    APInt Max = APInt::getSignedMaxValue(NarrowWidth).sext(WideWidth);
    if (ConstValue.sge(Min) && ConstValue.sle(Max))
      return std::nullopt;

    if (isEqOrNe(Pred))
      return Pred == ICmpInst::ICMP_NE;
    if (!isSignedICmp(Pred))
      return std::nullopt;

    bool BelowRange = ConstValue.slt(Min);
    bool AboveRange = ConstValue.sgt(Max);
    assert((BelowRange || AboveRange) &&
           "Out-of-range compare constant should not be in range");

    switch (Pred) {
    case ICmpInst::ICMP_SLT:
    case ICmpInst::ICMP_SLE:
      return AboveRange;
    case ICmpInst::ICMP_SGT:
    case ICmpInst::ICMP_SGE:
      return BelowRange;
    default:
      return std::nullopt;
    }
  }

  return std::nullopt;
}

Value *buildConstantAwareICmp(IRBuilder<> &B, ICmpInst::Predicate Pred,
                              Value *LHS, Value *RHS, const Twine &Name = "") {
  if (auto *CLHS = getScalarOrSplatConstantInt(LHS)) {
    if (auto Known = getKnownCompareResultWithConstantLHS(Pred, CLHS->getValue()))
      return getSameShapeBoolConstant(LHS->getType(), *Known);
  }
  if (auto *CRHS = getScalarOrSplatConstantInt(RHS)) {
    ICmpInst::Predicate Swapped = ICmpInst::getSwappedPredicate(Pred);
    if (auto Known =
            getKnownCompareResultWithConstantLHS(Swapped, CRHS->getValue()))
      return getSameShapeBoolConstant(LHS->getType(), *Known);
  }
  return B.CreateICmp(Pred, LHS, RHS, Name);
}

// Returns true when Pred is a valid comparison to narrow through an extension
// of Kind. For zext the unsigned predicates preserve ordering; for sext the
// signed predicates preserve ordering. eq/ne are valid for either kind.
bool isCompatiblePredForExtKind(ICmpInst::Predicate Pred, ExtKind Kind) {
  if (isEqOrNe(Pred))
    return true;
  if (Kind == ExtKind::ZExt)
    return isUnsignedICmp(Pred);
  if (Kind == ExtKind::SExt)
    return isSignedICmp(Pred);
  return false;
}

// Forward declarations for helpers used by tryShrinkICmpZeroBounded.
bool isZeroBoundedAtWidth(Value *V, unsigned Width);
bool isTruncRootedLowBitsPreservingOpcode(unsigned Opcode);
bool collectTruncRootedValueCost(Value *V, unsigned TargetWidth,
                                 SmallPtrSetImpl<Value *> &AddedValues,
                                 SmallPtrSetImpl<Instruction *> &RemovedInstructions,
                                 SmallPtrSetImpl<Value *> &Visited);
Value *materializeTruncRootedValueAtWidth(Value *V, unsigned TargetWidth,
                                          Instruction *InsertBefore,
                                          DenseMap<Value *, Value *> *Cache =
                                              nullptr);

// Return the structural narrow width of V if its high bits are provably zero
// by structure alone (direct zext, bitwise trees of such), or 0 if unknown.
// Constants return 0 (they are width-flexible and not the source of the bound).
unsigned getStructuralNarrowWidth(Value *V,
                                  DenseMap<Value *, unsigned> &Cache) {
  auto [It, Inserted] = Cache.try_emplace(V, 0u);
  if (!Inserted)
    return It->second;

  auto compute = [&]() -> unsigned {
    if (auto Ext = getExtOperandInfo(V))
      return Ext->Kind == ExtKind::ZExt ? Ext->NarrowWidth : 0;
    if (isa<ConstantInt>(V))
      return 0;
    if (auto *BO = dyn_cast<BinaryOperator>(V)) {
      // lshr preserves zero-boundedness.
      if (BO->getOpcode() == Instruction::LShr)
        return getStructuralNarrowWidth(BO->getOperand(0), Cache);
      if (BO->getOpcode() == Instruction::And) {
        unsigned W0 = getStructuralNarrowWidth(BO->getOperand(0), Cache);
        unsigned W1 = getStructuralNarrowWidth(BO->getOperand(1), Cache);
        // and: zero-bounded by whichever operand is bounded; take the narrower.
        if (W0 != 0 && W1 != 0) return std::min(W0, W1);
        return W0 != 0 ? W0 : W1;
      }
      if (BO->getOpcode() == Instruction::Or ||
          BO->getOpcode() == Instruction::Xor) {
        unsigned W0 = getStructuralNarrowWidth(BO->getOperand(0), Cache);
        unsigned W1 = getStructuralNarrowWidth(BO->getOperand(1), Cache);
        if (W0 == 0 || W1 == 0) return 0;
        return std::max(W0, W1);
      }
      // add nuw: the sum fits in max(W0, W1) + 1 bits when both operands are
      // zero-bounded.  For constants, use the number of active bits as the width.
      if (BO->getOpcode() == Instruction::Add && BO->hasNoUnsignedWrap()) {
        auto getOperandWidth = [&](Value *V) -> unsigned {
          if (auto *CI = dyn_cast<ConstantInt>(V)) {
            if (CI->isNegative()) return 0;
            unsigned Bits = (unsigned)CI->getValue().getActiveBits();
            return Bits == 0 ? 0 : Bits; // constant 0 → return 0 (identity)
          }
          return getStructuralNarrowWidth(V, Cache);
        };
        unsigned W0 = getOperandWidth(BO->getOperand(0));
        unsigned W1 = getOperandWidth(BO->getOperand(1));
        // If either is unknown treat as unbounded.  If one is 0 (constant zero)
        // the sum equals the other operand; handle conservatively.
        if (W0 == 0 || W1 == 0) return 0;
        unsigned Result = std::max(W0, W1) + 1;
        // Sanity: don't claim a width ≥ the value's actual type width.
        unsigned ActualWidth = getScalarIntegerWidth(BO->getType());
        if (Result >= ActualWidth) return 0;
        return Result;
      }
      // sub nuw: result <= LHS (no borrow), so bounded by LHS's width.
      if (BO->getOpcode() == Instruction::Sub && BO->hasNoUnsignedWrap()) {
        unsigned W = getStructuralNarrowWidth(BO->getOperand(0), Cache);
        unsigned ActualWidth = getScalarIntegerWidth(BO->getType());
        if (W != 0 && W < ActualWidth) return W;
        return 0;
      }
      // mul nuw: product of W0-bit and W1-bit values fits in W0+W1 bits.
      if (BO->getOpcode() == Instruction::Mul && BO->hasNoUnsignedWrap()) {
        unsigned W0 = getStructuralNarrowWidth(BO->getOperand(0), Cache);
        unsigned W1 = getStructuralNarrowWidth(BO->getOperand(1), Cache);
        if (W0 == 0 || W1 == 0) return 0;
        unsigned Result = W0 + W1;
        unsigned ActualWidth = getScalarIntegerWidth(BO->getType());
        if (Result >= ActualWidth) return 0;
        return Result;
      }
    }
    // umin result <= both operands; bounded by whichever operand is bounded
    // (analogous to `and`).
    // umax result = the larger operand; bounded only if both are bounded
    // (analogous to `or`).
    if (auto *II = dyn_cast<IntrinsicInst>(V)) {
      if (II->getIntrinsicID() == Intrinsic::umin) {
        unsigned W0 = getStructuralNarrowWidth(II->getArgOperand(0), Cache);
        unsigned W1 = getStructuralNarrowWidth(II->getArgOperand(1), Cache);
        if (W0 != 0 && W1 != 0) return std::min(W0, W1);
        return W0 != 0 ? W0 : W1;
      }
      if (II->getIntrinsicID() == Intrinsic::umax) {
        unsigned W0 = getStructuralNarrowWidth(II->getArgOperand(0), Cache);
        unsigned W1 = getStructuralNarrowWidth(II->getArgOperand(1), Cache);
        if (W0 == 0 || W1 == 0) return 0;
        return std::max(W0, W1);
      }
    }
    return 0;
  };

  unsigned Result = compute();
  Cache[V] = Result;
  return Result;
}

unsigned getStructuralNarrowWidth(Value *V) {
  DenseMap<Value *, unsigned> Cache;
  return getStructuralNarrowWidth(V, Cache);
}

// Narrow  icmp pred LHS, RHS  when both sides are structurally zero-bounded
// at a width smaller than the current comparison width.  Valid for eq/ne and
// all unsigned predicates.  Handles cases where at least one operand is a
// bitwise tree of zero-extensions rather than a single direct extension
// (tryShrinkICmp and tryShrinkICmpExtConst cover direct-ext operands).
bool tryShrinkICmpZeroBounded(ICmpInst &Cmp) {
  ICmpInst::Predicate Pred = Cmp.getPredicate();
  if (!isEqOrNe(Pred) && !isUnsignedICmp(Pred))
    return false;

  Value *LHS = Cmp.getOperand(0);
  Value *RHS = Cmp.getOperand(1);
  if (!isIntegerValue(LHS) || !isIntegerValue(RHS))
    return false;

  unsigned WideWidth = getValueWidth(LHS);
  if (WideWidth != getValueWidth(RHS))
    return false;

  // Derive the target width from the non-constant zero-bounded operand.
  unsigned TargetWidth = 0;
  for (Value *V : {LHS, RHS}) {
    if (!isa<ConstantInt>(V)) {
      TargetWidth = getStructuralNarrowWidth(V);
      if (TargetWidth != 0)
        break;
    }
  }
  if (TargetWidth == 0 || TargetWidth >= WideWidth)
    return false;

  // Both sides must be zero-bounded at TargetWidth (constants adapt freely).
  if (!isZeroBoundedAtWidth(LHS, TargetWidth) ||
      !isZeroBoundedAtWidth(RHS, TargetWidth))
    return false;

  // Cost check: require that narrowing doesn't add more instructions than it
  // removes.  We re-use the trunc-rooted cost infrastructure without counting
  // a trunc removal (there is none here; the icmp is merely replaced).
  SmallPtrSet<Value *, 8> AddedValues;
  SmallPtrSet<Instruction *, 8> RemovedInstructions;
  SmallPtrSet<Value *, 8> Visited;
  if (!collectTruncRootedValueCost(LHS, TargetWidth, AddedValues,
                                   RemovedInstructions, Visited) ||
      !collectTruncRootedValueCost(RHS, TargetWidth, AddedValues,
                                   RemovedInstructions, Visited))
    return false;
  if (AddedValues.size() > RemovedInstructions.size())
    return false;

  DenseMap<Value *, Value *> Cache;
  Value *NarrowLHS =
      materializeTruncRootedValueAtWidth(LHS, TargetWidth, &Cmp, &Cache);
  Value *NarrowRHS =
      materializeTruncRootedValueAtWidth(RHS, TargetWidth, &Cmp, &Cache);
  if (!NarrowLHS || !NarrowRHS)
    return false;

  IRBuilder<> B(&Cmp);
  Value *NarrowCmp = B.CreateICmp(Pred, NarrowLHS, NarrowRHS, Cmp.getName());
  if (auto *NCI = dyn_cast<ICmpInst>(NarrowCmp))
    NCI->setDebugLoc(Cmp.getDebugLoc());

  Cmp.replaceAllUsesWith(NarrowCmp);
  Cmp.eraseFromParent();

  // Use WeakTrackingVH so that if deleting LI recursively kills RI (because
  // RI is only used by LI), we don't access a dangling pointer for RI.
  WeakTrackingVH LHSHandle(LHS), RHSHandle(RHS);
  if (auto *LI = dyn_cast_or_null<Instruction>(LHSHandle))
    if (LI->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(LI);
  if (auto *RI = dyn_cast_or_null<Instruction>(RHSHandle))
    if (RI->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(RI);

  return true;
}

// Narrow  icmp pred (ext %x to W), C  →  icmp pred %x, trunc(C)
// when C fits in the source width of the extension and the predicate is
// compatible with the extension kind. Also handles the symmetric case where
// the constant is on the left.
bool tryShrinkICmpExtConst(ICmpInst &Cmp) {
  for (unsigned ExtIdx = 0; ExtIdx != 2; ++ExtIdx) {
    auto ExtInfo = getExtOperandInfo(Cmp.getOperand(ExtIdx));
    if (!ExtInfo)
      continue;

    auto *OrigC = dyn_cast<Constant>(Cmp.getOperand(1 - ExtIdx));
    if (!OrigC)
      continue;
    auto *C = getScalarOrSplatConstantInt(OrigC);
    if (!C)
      continue;

    // Normalize the predicate so the ext operand is always on the left.
    ICmpInst::Predicate NormalizedPred =
        ExtIdx == 0 ? Cmp.getPredicate() : Cmp.getSwappedPredicate();

    // For SExt, unsigned predicates can be narrowed when the constant is
    // non-negative and fits in (NarrowWidth-1) bits (i.e., < 2^(NarrowWidth-1)).
    // sext(X) compared unsigned against such C behaves identically to
    // unsigned comparison of X at the narrow type: negative X values produce
    // huge unsigned sext results (>= 2^63) and are thus above C, matching the
    // behavior of unsigned X (>= 2^(N-1)) also being above C.
    if (ExtInfo->Kind == ExtKind::SExt && isUnsignedICmp(NormalizedPred)) {
      const APInt &CV = C->getValue();
      if (CV.isIntN(ExtInfo->NarrowWidth - 1)) {
        // The constant fits in the non-negative range of the narrow type:
        // convert unsigned predicate to same unsigned predicate at narrow width.
        // (canRepresentConstant for ZExt checks this without sign; we reuse
        // the unsigned narrowing path after adjusting the predicate scope.)
        IRBuilder<> B(&Cmp);
        Constant *NarrowC = convertConstantToNarrow(*OrigC, ExtInfo->NarrowWidth);
        Value *NewCmp = buildConstantAwareICmp(B, NormalizedPred,
                                               ExtInfo->NarrowValue, NarrowC,
                                               Cmp.getName());
        if (auto *NewCmpI = dyn_cast<Instruction>(NewCmp)) {
          NewCmpI->setDebugLoc(Cmp.getDebugLoc());
          if (!NewCmpI->hasName())
            NewCmpI->takeName(&Cmp);
        }
        Cmp.replaceAllUsesWith(NewCmp);
        Cmp.eraseFromParent();
        if (ExtInfo->Producer->use_empty())
          RecursivelyDeleteTriviallyDeadInstructions(ExtInfo->Producer);
        return true;
      }
      // If C >= 2^(N-1) (as unsigned i64): sext(X) (non-negative X) < 2^31
      // is always < C since sext(X) < 2^31 <= C; negative X gives huge values.
      // The result depends on predicate — leave for constant-fold if possible.
      // Fall through to let the general path handle or skip.
    }

    // For ZExt, signed predicates can be handled by converting to unsigned.
    // Since zext(X) is always non-negative, signed and unsigned order agree
    // when comparing against a non-negative constant. Against a negative
    // constant, the result is a trivial constant (zext(X) >= 0 > C_neg).
    if (ExtInfo->Kind == ExtKind::ZExt && isSignedICmp(NormalizedPred)) {
      if (C->getValue().isNonNegative()) {
        NormalizedPred = ICmpInst::getUnsignedPredicate(NormalizedPred);
      } else {
        // Negative constant: zext(X) >= 0 > C, so sgt/sge are always true,
        // slt/sle are always false.
        bool AlwaysTrue = (NormalizedPred == ICmpInst::ICMP_SGT ||
                           NormalizedPred == ICmpInst::ICMP_SGE);
        Value *NewCmp = getSameShapeIntegerConstant(Cmp.getType(),
                                                     APInt(1, AlwaysTrue));
        Cmp.replaceAllUsesWith(NewCmp);
        Cmp.eraseFromParent();
        if (ExtInfo->Producer->use_empty())
          RecursivelyDeleteTriviallyDeadInstructions(ExtInfo->Producer);
        return true;
      }
    }

    if (!isCompatiblePredForExtKind(NormalizedPred, ExtInfo->Kind))
      continue;

    IRBuilder<> B(&Cmp);
    Value *NewCmp = nullptr;
    if (canRepresentConstant(*OrigC, ExtInfo->Kind, ExtInfo->NarrowWidth)) {
      Constant *NarrowC = convertConstantToNarrow(*OrigC, ExtInfo->NarrowWidth);
      NewCmp = buildConstantAwareICmp(B, NormalizedPred, ExtInfo->NarrowValue,
                                      NarrowC, Cmp.getName());
    } else {
      auto Known = getKnownCompareResultWithExtAndConstant(
          NormalizedPred, ExtInfo->Kind, ExtInfo->NarrowWidth, C->getValue());
      if (!Known)
        continue;
      NewCmp = getSameShapeIntegerConstant(Cmp.getType(), APInt(1, *Known));
    }

    if (auto *NewCmpI = dyn_cast<Instruction>(NewCmp)) {
      NewCmpI->setDebugLoc(Cmp.getDebugLoc());
      if (!NewCmpI->hasName())
        NewCmpI->takeName(&Cmp);
    }
    Cmp.replaceAllUsesWith(NewCmp);
    Cmp.eraseFromParent();

    if (ExtInfo->Producer->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(ExtInfo->Producer);

    return true;
  }
  return false;
}

bool tryShrinkICmp(ICmpInst &Cmp, DominatorTree *DT = nullptr) {
  auto LHSInfo = getExtOperandInfo(Cmp.getOperand(0));
  auto RHSInfo = getExtOperandInfo(Cmp.getOperand(1));
  if (!LHSInfo || !RHSInfo)
    return false;

  ICmpInst::Predicate Pred = Cmp.getPredicate();

  // ZExt-ZExt with a signed ordering predicate: since zext always produces a
  // non-negative value, the signed comparison is equivalent to the unsigned
  // comparison at the narrow type.  E.g. icmp slt i32 (zext i8 a), (zext i8 b)
  // ≡ icmp ult i8 a, b.
  ICmpInst::Predicate NewPred = Pred;
  if (LHSInfo->Kind == ExtKind::ZExt && RHSInfo->Kind == ExtKind::ZExt &&
      isSignedICmp(Pred))
    NewPred = ICmpInst::getUnsignedPredicate(Pred);

  unsigned TargetWidth = computeShrinkWidth(NewPred, *LHSInfo, *RHSInfo);
  if (TargetWidth == 0 || TargetWidth >= LHSInfo->WideWidth)
    return false;

  // Profitability: each operand whose TargetWidth differs from its NarrowWidth
  // needs a new cast (materializeAtWidth will emit one).  A producer is
  // removable only if this icmp is its sole user.  If the same producer feeds
  // both operands it is counted once for removal.
  unsigned AddedBoundaryCost = 0;
  unsigned RemovedBoundaryCost = 0;
  if (TargetWidth != LHSInfo->NarrowWidth)
    ++AddedBoundaryCost;
  if (TargetWidth != RHSInfo->NarrowWidth)
    ++AddedBoundaryCost;
  bool SameProducer =
      LHSInfo->Producer && LHSInfo->Producer == RHSInfo->Producer;
  if (!SameProducer && LHSInfo->Producer && LHSInfo->Producer->hasOneUse())
    ++RemovedBoundaryCost;
  if (RHSInfo->Producer && RHSInfo->Producer->hasOneUse())
    ++RemovedBoundaryCost;
  if (AddedBoundaryCost > RemovedBoundaryCost)
    return false;

  IRBuilder<> B(&Cmp);
  Value *NewLHS = materializeAtWidth(B, *LHSInfo, TargetWidth, DT);
  Value *NewRHS = materializeAtWidth(B, *RHSInfo, TargetWidth, DT);
  Value *NewCmp = B.CreateICmp(NewPred, NewLHS, NewRHS);

  SmallVector<Instruction *, 2> DeadRoots;
  if (LHSInfo->Producer != RHSInfo->Producer)
    DeadRoots.push_back(LHSInfo->Producer);
  DeadRoots.push_back(RHSInfo->Producer);

  Cmp.replaceAllUsesWith(NewCmp);
  Cmp.eraseFromParent();

  for (Instruction *I : DeadRoots) {
    if (I != nullptr && I->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(I);
  }

  return true;
}

bool areHighBitsKnownZero(Value *V, unsigned NarrowWidth, const DataLayout &DL,
                          AssumptionCache *AC, DominatorTree *DT,
                          const Instruction *CxtI) {
  unsigned WideWidth = getValueWidth(V);
  if (NarrowWidth >= WideWidth)
    return true;

  SimplifyQuery SQ(DL, DT, AC, CxtI);
  KnownBits KB = computeKnownBits(V, SQ);
  APInt HighBits = APInt::getHighBitsSet(WideWidth, WideWidth - NarrowWidth);
  return (KB.Zero & HighBits) == HighBits;
}

bool tryWidenTruncEqualityICmp(ICmpInst &Cmp, const DataLayout &DL,
                               AssumptionCache *AC, DominatorTree *DT) {
  if (!isa<IntegerType>(Cmp.getOperand(0)->getType()) ||
      !isa<IntegerType>(Cmp.getOperand(1)->getType()))
    return false;
  if (!isEqOrNe(Cmp.getPredicate()))
    return false;
  if (!isIntegerValue(Cmp.getOperand(0)) || !isIntegerValue(Cmp.getOperand(1)))
    return false;

  auto *LHS = dyn_cast<TruncInst>(Cmp.getOperand(0));
  auto *RHS = dyn_cast<TruncInst>(Cmp.getOperand(1));
  if (!LHS || !RHS)
    return false;

  unsigned NarrowWidth = getValueWidth(LHS);
  if (NarrowWidth != getValueWidth(RHS))
    return false;

  Value *WideLHS = LHS->getOperand(0);
  Value *WideRHS = RHS->getOperand(0);
  if (getValueWidth(WideLHS) != getValueWidth(WideRHS))
    return false;

  if (!areHighBitsKnownZero(WideLHS, NarrowWidth, DL, AC, DT, &Cmp) ||
      !areHighBitsKnownZero(WideRHS, NarrowWidth, DL, AC, DT, &Cmp))
    return false;

  IRBuilder<> B(&Cmp);
  Value *NewCmp = B.CreateICmp(Cmp.getPredicate(), WideLHS, WideRHS);
  Cmp.replaceAllUsesWith(NewCmp);
  Cmp.eraseFromParent();

  // Guard against LHS == RHS (same trunc used for both sides): deleting through
  // LHS first would leave RHS dangling.
  if (LHS != RHS) {
    if (LHS->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(LHS);
    if (RHS->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(RHS);
  } else {
    if (LHS->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(LHS);
  }

  return true;
}

bool tryWidenTruncZeroExtendedICmp(ICmpInst &Cmp, const DataLayout &DL,
                                   AssumptionCache *AC, DominatorTree *DT) {
  if (!isa<IntegerType>(Cmp.getOperand(0)->getType()) ||
      !isa<IntegerType>(Cmp.getOperand(1)->getType()))
    return false;
  if (!isEqOrNe(Cmp.getPredicate()) && !isUnsignedICmp(Cmp.getPredicate()))
    return false;
  if (!isIntegerValue(Cmp.getOperand(0)) || !isIntegerValue(Cmp.getOperand(1)))
    return false;

  // This is intentionally asymmetric. We allow one operand to stay wide when
  // its truncated-away high bits are known zero, then zero-extend the other
  // side to meet it. That captures the common "trunc vs narrow" compare
  // patterns without requiring both operands to share the same shape.
  auto tryOneDirection = [&](unsigned TruncIdx) -> bool {
    auto *Tr = dyn_cast<TruncInst>(Cmp.getOperand(TruncIdx));
    if (!Tr)
      return false;

    Value *Wide = Tr->getOperand(0);
    unsigned NarrowWidth = getValueWidth(Tr);
    unsigned WideWidth = getValueWidth(Wide);
    assert(NarrowWidth < WideWidth &&
           "Trunc operands must be narrower than their source");

    Value *Other = Cmp.getOperand(1 - TruncIdx);
    if (!isIntegerValue(Other) || getValueWidth(Other) != NarrowWidth)
      return false;

    if (!areHighBitsKnownZero(Wide, NarrowWidth, DL, AC, DT, &Cmp))
      return false;

    unsigned AddedBoundaryCost = 0;
    unsigned RemovedBoundaryCost = Tr->hasOneUse() ? 1 : 0;
    auto OtherExt = getExtOperandInfo(Other);
    if (OtherExt && OtherExt->Kind == ExtKind::ZExt &&
        OtherExt->WideWidth == NarrowWidth) {
      if (!isa<Constant>(OtherExt->NarrowValue))
        AddedBoundaryCost = 1;
      if (OtherExt->Producer->hasOneUse())
        ++RemovedBoundaryCost;
    } else if (!isa<Constant>(Other)) {
      AddedBoundaryCost = 1;
    }

    // Widening this compare is only worthwhile when any new zero-extension
    // is paid for by removable boundary instructions around the compare.
    if (AddedBoundaryCost > RemovedBoundaryCost)
      return false;

    IRBuilder<> B(&Cmp);
    Value *WideOther = Other;
    if (WideWidth != NarrowWidth) {
      if (OtherExt) {
        if (OtherExt->Kind == ExtKind::ZExt &&
            OtherExt->WideWidth == NarrowWidth) {
          WideOther = materializeAtWidth(B, *OtherExt, WideWidth);
        } else {
          WideOther = B.CreateZExt(Other, IntegerType::get(Cmp.getContext(),
                                                          WideWidth));
        }
      } else {
        WideOther =
            B.CreateZExt(Other, IntegerType::get(Cmp.getContext(), WideWidth));
      }
    }
    assert(getValueWidth(WideOther) == WideWidth &&
           "Widened compare operand should match source width");

    Value *NewOps[2] = {Cmp.getOperand(0), Cmp.getOperand(1)};
    NewOps[TruncIdx] = Wide;
    NewOps[1 - TruncIdx] = WideOther;
    Value *NewCmpVal = B.CreateICmp(Cmp.getPredicate(), NewOps[0], NewOps[1]);
    if (auto *NewCmp = dyn_cast<ICmpInst>(NewCmpVal)) {
      NewCmp->setDebugLoc(Cmp.getDebugLoc());
      NewCmp->takeName(&Cmp);
    }
    Cmp.replaceAllUsesWith(NewCmpVal);
    Cmp.eraseFromParent();

    if (Tr->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(Tr);
    if (auto *OtherI = dyn_cast<Instruction>(Other))
      if (OtherI->use_empty())
        RecursivelyDeleteTriviallyDeadInstructions(OtherI);

    return true;
  };

  return tryOneDirection(0) || tryOneDirection(1);
}

bool tryShrinkPhiOfExts(PHINode &Phi) {
  Type *WideTy = Phi.getType();
  if (!getScalarIntegerTy(WideTy))
    return false;

  PhiShrinkInfo Info;
  bool SawExt = false;

  SmallVector<Value *, 8> NarrowIncomingValues;
  SmallVector<BasicBlock *, 8> IncomingBlocks;
  NarrowIncomingValues.reserve(Phi.getNumIncomingValues());
  IncomingBlocks.reserve(Phi.getNumIncomingValues());

  for (unsigned I = 0, E = Phi.getNumIncomingValues(); I != E; ++I) {
    Value *Incoming = Phi.getIncomingValue(I);
    if (auto Ext = getExtOperandInfo(Incoming)) {
      if (Ext->WideWidth != getScalarIntegerWidth(WideTy))
        return false;

      if (!SawExt) {
        SawExt = true;
        Info.Kind = Ext->Kind;
        Info.NarrowWidth = Ext->NarrowWidth;
        Info.WideWidth = Ext->WideWidth;
      } else if (Info.Kind != Ext->Kind || Info.WideWidth != Ext->WideWidth) {
        return false;
      }
      Info.NarrowWidth = std::max(Info.NarrowWidth, Ext->NarrowWidth);
      continue;
    }

    auto *C = dyn_cast<Constant>(Incoming);
    if (!C || !isIntegerValue(C) || !haveSameIntegerShape(C->getType(), WideTy))
      return false;
  }

  if (!SawExt)
    return false;

  auto *NarrowTy = getSameShapeIntegerType(WideTy, Info.NarrowWidth);
  Instruction::CastOps NarrowCastOp =
      (Info.Kind == ExtKind::ZExt) ? Instruction::ZExt : Instruction::SExt;

  // Account exactly for the instructions this rewrite would add or remove
  // locally. The transform always replaces the original wide phi with:
  //  1. a narrow phi at the merged width, and
  //  2. a recreated wide extension for the original users.
  // It may also need to materialize some incoming values at the merged narrow
  // width, but materializeAtWidth can reuse an existing cast in the same block
  // (or one we already planned earlier for another incoming edge). On the
  // removal side, only count instructions that are guaranteed to become dead as
  // a direct consequence of removing this phi; deeper recursive DCE is a bonus
  // and should not be required for profitability.
  unsigned AddedInstructions = 2;   // NarrowPhi + recreated wide result.
  unsigned RemovedInstructions = 1; // The original wide phi.
  SmallVector<std::pair<BasicBlock *, Value *>, 8>
      PlannedIncomingMaterializations;
  auto wouldAddIncomingMaterialization =
      [&](BasicBlock *BB, const ExtOperandInfo &Ext) {
        if (Info.NarrowWidth == Ext.NarrowWidth)
          return false;

        auto PlannedKey = std::make_pair(BB, Ext.NarrowValue);
        if (llvm::is_contained(PlannedIncomingMaterializations, PlannedKey))
          return false;

        // ConstantData values (e.g. ConstantInt) have no use list; casting
        // them folds to a constant at no instruction cost.
        if (isa<ConstantData>(Ext.NarrowValue))
          return false;

        for (User *U : Ext.NarrowValue->users()) {
          auto *Cast = dyn_cast<CastInst>(U);
          if (!Cast || Cast->getOpcode() != NarrowCastOp ||
              Cast->getType() != NarrowTy)
            continue;
          if (Cast->getParent() == BB)
            return false;
        }

        PlannedIncomingMaterializations.push_back(PlannedKey);
        return true;
      };

  for (unsigned I = 0, E = Phi.getNumIncomingValues(); I != E; ++I) {
    Value *Incoming = Phi.getIncomingValue(I);
    IncomingBlocks.push_back(Phi.getIncomingBlock(I));

    if (auto Ext = getExtOperandInfo(Incoming)) {
      if (wouldAddIncomingMaterialization(IncomingBlocks.back(), *Ext))
        ++AddedInstructions;
      Info.Producers.push_back(Ext->Producer);
      continue;
    }

    auto *C = cast<Constant>(Incoming);
    if (!canRepresentConstant(*C, Info.Kind, Info.NarrowWidth))
      return false;
  }

  SmallPtrSet<Instruction *, 8> CountedProducers;
  for (Instruction *Producer : Info.Producers) {
    if (Producer == nullptr || !CountedProducers.insert(Producer).second)
      continue;

    unsigned UsesFromPhi = 0;
    for (Use &U : Producer->uses())
      if (U.getUser() == &Phi)
        ++UsesFromPhi;

    if (Producer->getNumUses() == UsesFromPhi)
      ++RemovedInstructions;
  }

  if (AddedInstructions > RemovedInstructions)
    return false;

  // Profitability confirmed: now materialise any incomings that need widening.
  // This must happen after the profitability check to avoid leaving dead cast
  // instructions in the IR when the check fails.
  NarrowIncomingValues.clear();
  for (unsigned I = 0, E = Phi.getNumIncomingValues(); I != E; ++I) {
    Value *Incoming = Phi.getIncomingValue(I);
    if (auto Ext = getExtOperandInfo(Incoming)) {
      Value *NarrowIncoming = Ext->NarrowValue;
      if (Info.NarrowWidth != Ext->NarrowWidth) {
        IRBuilder<> B(IncomingBlocks[I]->getTerminator());
        NarrowIncoming = materializeAtWidth(B, *Ext, Info.NarrowWidth);
      }
      NarrowIncomingValues.push_back(NarrowIncoming);
      continue;
    }
    auto *C = cast<Constant>(Incoming);
    NarrowIncomingValues.push_back(convertConstantToNarrow(*C, Info.NarrowWidth));
  }

  auto *NarrowPhi = PHINode::Create(NarrowTy, Phi.getNumIncomingValues(),
                                    Phi.getName() + ".narrow",
                                    Phi.getIterator());
  for (unsigned I = 0, E = NarrowIncomingValues.size(); I != E; ++I)
    NarrowPhi->addIncoming(NarrowIncomingValues[I], IncomingBlocks[I]);

  auto InsertIt = Phi.getParent()->getFirstInsertionPt();
  if (InsertIt == Phi.getParent()->end())
    return false;
  Instruction *InsertPt = &*InsertIt;
  IRBuilder<> B(InsertPt);
  Instruction *Wide = nullptr;
  if (Info.Kind == ExtKind::ZExt)
    Wide = cast<Instruction>(B.CreateZExt(NarrowPhi, WideTy, Phi.getName()));
  else
    Wide = cast<Instruction>(B.CreateSExt(NarrowPhi, WideTy, Phi.getName()));

  Phi.replaceAllUsesWith(Wide);
  Phi.eraseFromParent();

  // Deduplicate before calling RTDI: the same ext can appear on multiple
  // incoming edges; erasing it once frees the memory, making subsequent
  // accesses via the dangling pointer UB.
  SmallPtrSet<Instruction *, 8> SeenProducers;
  for (Instruction *Producer : Info.Producers)
    if (Producer != nullptr && SeenProducers.insert(Producer).second &&
        Producer->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(Producer);

  return true;
}

bool tryShrinkSelectOfExts(SelectInst &Sel) {
  Type *WideTy = Sel.getType();
  if (!getScalarIntegerTy(WideTy))
    return false;

  Value *TV = Sel.getTrueValue();
  Value *FV = Sel.getFalseValue();

  auto TrueExt = getExtOperandInfo(TV);
  auto FalseExt = getExtOperandInfo(FV);
  auto *TrueC = dyn_cast<Constant>(TV);
  auto *FalseC = dyn_cast<Constant>(FV);

  if (!TrueExt && !FalseExt)
    return false;
  if ((TrueExt == std::nullopt &&
       (!TrueC || !isIntegerValue(TrueC) ||
        !haveSameIntegerShape(TrueC->getType(), WideTy))) ||
      (FalseExt == std::nullopt &&
       (!FalseC || !isIntegerValue(FalseC) ||
        !haveSameIntegerShape(FalseC->getType(), WideTy))))
    return false;

  PhiShrinkInfo Info;
  const ExtOperandInfo &Seed = TrueExt ? *TrueExt : *FalseExt;
  Info.Kind = Seed.Kind;
  Info.NarrowWidth = Seed.NarrowWidth;
  Info.WideWidth = Seed.WideWidth;

  if (Info.WideWidth != getScalarIntegerWidth(WideTy))
    return false;

  auto validateExt = [&](const std::optional<ExtOperandInfo> &Ext) {
    return Ext && Ext->Kind == Info.Kind && Ext->WideWidth == Info.WideWidth;
  };

  if (TrueExt && !validateExt(TrueExt))
    return false;
  if (FalseExt && !validateExt(FalseExt))
    return false;

  if (TrueExt)
    Info.NarrowWidth = std::max(Info.NarrowWidth, TrueExt->NarrowWidth);
  if (FalseExt)
    Info.NarrowWidth = std::max(Info.NarrowWidth, FalseExt->NarrowWidth);

  if (TrueC && !canRepresentConstant(*TrueC, Info.Kind, Info.NarrowWidth))
    return false;
  if (FalseC && !canRepresentConstant(*FalseC, Info.Kind, Info.NarrowWidth))
    return false;

  SmallVector<Instruction *, 8> OriginalUsers;
  bool NeedWideResult = false;
  for (User *U : Sel.users()) {
    auto *UserI = dyn_cast<Instruction>(U);
    if (!UserI)
      return false;
    OriginalUsers.push_back(UserI);

    if (auto *Tr = dyn_cast<TruncInst>(UserI))
      if (Tr->getOperand(0) == &Sel && getValueWidth(Tr) == Info.NarrowWidth)
        continue;

    if (auto *Cmp = dyn_cast<ICmpInst>(UserI))
      if (getRetargetedZeroComparePredicate(*Cmp, Sel, Info.Kind,
                                            Info.WideWidth))
        continue;

    NeedWideResult = true;
  }

  unsigned RemovableExts = 0;
  if (TrueExt && TrueExt->Producer->hasOneUse())
    ++RemovableExts;
  if (FalseExt && FalseExt->Producer->hasOneUse() &&
      FalseExt->Producer != (TrueExt ? TrueExt->Producer : nullptr))
    ++RemovableExts;

  unsigned AddedExts = NeedWideResult ? 1 : 0;
  if (TrueExt && TrueExt->NarrowWidth != Info.NarrowWidth &&
      !isa<Constant>(TrueExt->NarrowValue))
    ++AddedExts;
  if (FalseExt && FalseExt->NarrowWidth != Info.NarrowWidth &&
      !isa<Constant>(FalseExt->NarrowValue))
    ++AddedExts;

  // Rebuilding the select at an intermediate width may also need to recreate
  // some arm extensions below the original wide type. Only do that when the
  // removable arm extensions pay for the new casts, so the rewrite does not
  // increase instruction count.
  if (AddedExts > RemovableExts)
    return false;

  IRBuilder<> B(&Sel);
  Value *NarrowTV = TrueExt ? materializeAtWidth(B, *TrueExt, Info.NarrowWidth)
                            : convertConstantToNarrow(*TrueC, Info.NarrowWidth);
  Value *NarrowFV =
      FalseExt ? materializeAtWidth(B, *FalseExt, Info.NarrowWidth)
               : convertConstantToNarrow(*FalseC, Info.NarrowWidth);
  Value *NarrowSel = B.CreateSelect(Sel.getCondition(), NarrowTV, NarrowFV,
                                    Sel.getName() + ".narrow");
  SmallVector<Instruction *, 8> RemainingWideUsers;
  for (Instruction *UserI : OriginalUsers) {
    if (auto *Tr = dyn_cast<TruncInst>(UserI)) {
      if (Tr->getOperand(0) == &Sel && getValueWidth(Tr) == Info.NarrowWidth) {
        Tr->replaceAllUsesWith(NarrowSel);
        Tr->eraseFromParent();
        continue;
      }
    }

    if (auto *Cmp = dyn_cast<ICmpInst>(UserI)) {
      auto NarrowPred = getRetargetedZeroComparePredicate(*Cmp, Sel, Info.Kind,
                                                          Info.WideWidth);
      if (NarrowPred) {
        IRBuilder<> CmpB(Cmp);
        auto *Zero = getSameShapeIntegerConstant(NarrowSel->getType(),
                                                 APInt(Info.NarrowWidth, 0));
        Value *NewCmpVal = CmpB.CreateICmp(*NarrowPred, NarrowSel, Zero,
                                           Cmp->getName());
        if (auto *NewCmp = dyn_cast<ICmpInst>(NewCmpVal))
          NewCmp->setDebugLoc(Cmp->getDebugLoc());
        Cmp->replaceAllUsesWith(NewCmpVal);
        Cmp->eraseFromParent();
        continue;
      }
    }

    RemainingWideUsers.push_back(UserI);
  }

  if (!RemainingWideUsers.empty()) {
    Value *Wide = Info.Kind == ExtKind::ZExt
                      ? B.CreateZExt(NarrowSel, WideTy, Sel.getName())
                      : B.CreateSExt(NarrowSel, WideTy, Sel.getName());
    for (Instruction *UserI : RemainingWideUsers)
      UserI->replaceUsesOfWith(&Sel, Wide);
  }

  Sel.eraseFromParent();

  // Capture producers before any deletion; both arms may share the same
  // producer (e.g. select i1 %c, %ext, %ext), in which case deleting through
  // TrueExt's producer first would leave FalseExt->Producer dangling.
  Instruction *TrueProducer  = TrueExt  ? TrueExt->Producer  : nullptr;
  Instruction *FalseProducer = FalseExt ? FalseExt->Producer : nullptr;

  if (TrueProducer && TrueProducer->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(TrueProducer);
  if (FalseProducer && FalseProducer != TrueProducer &&
      FalseProducer->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(FalseProducer);

  return true;
}

bool mayMergeActualUndef(Value *V) {
  if (isa<UndefValue>(V))
    return true;
  if (auto *Phi = dyn_cast<PHINode>(V))
    return llvm::any_of(Phi->incoming_values(),
                        [](Value *Incoming) { return isa<UndefValue>(Incoming); });
  if (auto *Sel = dyn_cast<SelectInst>(V))
    return isa<UndefValue>(Sel->getTrueValue()) ||
           isa<UndefValue>(Sel->getFalseValue());
  return false;
}

bool tryConvertSExtToNonNegZExt(SExtInst &Ext, LazyValueInfo &LVI) {
  if (!isa<IntegerType>(Ext.getSrcTy()) || !isa<IntegerType>(Ext.getType()))
    return false;
  const Use &Base = Ext.getOperandUse(0);
  if (!LVI.getConstantRangeAtUse(Base, /*UndefAllowed=*/false).isAllNonNegative())
    return false;
  if (mayMergeActualUndef(Base.get()))
    return false;

  // Once the operand is known non-negative at this use, sign extension and
  // zero extension agree. Mark the replacement non-negative as well so later
  // folds can continue to exploit that fact.
  auto *ZExt = CastInst::CreateZExtOrBitCast(Base, Ext.getType(), "",
                                             Ext.getIterator());
  ZExt->takeName(&Ext);
  ZExt->setDebugLoc(Ext.getDebugLoc());
  ZExt->setNonNeg();
  Ext.replaceAllUsesWith(ZExt);
  Ext.eraseFromParent();
  return true;
}

bool tryFoldAndOfSExtToZExt(BinaryOperator &And) {
  ConstantInt *Mask = getScalarOrSplatConstantInt(And.getOperand(0));
  SExtInst *Ext = dyn_cast<SExtInst>(And.getOperand(1));
  if (!Mask || !Ext) {
    Mask = getScalarOrSplatConstantInt(And.getOperand(1));
    Ext = dyn_cast<SExtInst>(And.getOperand(0));
  }
  if (!Mask || !Ext)
    return false;

  // Leave shared sign-extensions to the whole-value conversion path so we
  // preserve a single widened value across all compatible uses.
  if (!Ext->hasOneUse())
    return false;

  unsigned SrcWidth = getValueWidth(Ext->getOperand(0));
  unsigned WideWidth = getValueWidth(Ext);
  assert(Mask->getBitWidth() == WideWidth &&
         "And mask should match operand width");

  APInt DemandedMask = APInt::getLowBitsSet(WideWidth, SrcWidth);
  if ((Mask->getValue() & ~DemandedMask) != 0)
    return false;

  IRBuilder<> B(&And);
  auto *ZExt = CastInst::CreateZExtOrBitCast(Ext->getOperand(0), Ext->getType(),
                                             "", Ext->getIterator());
  ZExt->setDebugLoc(Ext->getDebugLoc());
  ZExt->takeName(Ext);
  // Do NOT setNonNeg here: the transformation is valid because the mask zeroes
  // out all bits above SrcWidth, so sext and zext agree on the unmasked bits.
  // But this says nothing about whether the source value is non-negative.
  And.replaceUsesOfWith(Ext, ZExt);
  if (Ext->use_empty())
    Ext->eraseFromParent();
  return true;
}

bool sextUseAllowsZExt(User &U, SExtInst &Ext) {
  if (auto *BO = dyn_cast<BinaryOperator>(&U)) {
    switch (BO->getOpcode()) {
    case Instruction::And:
      if (auto *Mask = dyn_cast<ConstantInt>(BO->getOperand(0) == &Ext
                                                 ? BO->getOperand(1)
                                                 : BO->getOperand(0))) {
        unsigned SrcWidth = getValueWidth(Ext.getOperand(0));
        unsigned WideWidth = getValueWidth(&Ext);
        if (SrcWidth > WideWidth)
          return false;
        APInt DemandedMask = APInt::getLowBitsSet(WideWidth, SrcWidth);
        return (Mask->getValue() & ~DemandedMask) == 0;
      }
      return false;
    case Instruction::LShr: {
      // lshr(sext(a:N→W), k) is safe to convert sext→zext only when every use
      // of the lshr result accesses bits that fall within the original narrow
      // range (0..N-k-1).  Bits N-k..W-k-1 of the lshr result contain sign
      // bits for sext but zeros for zext, so any use reaching those positions
      // makes sext and zext non-equivalent.
      if (BO->getOperand(0) != &Ext)
        return false;
      auto *AmtC = dyn_cast<ConstantInt>(BO->getOperand(1));
      if (!AmtC)
        return false;
      unsigned N = getValueWidth(Ext.getOperand(0)); // narrow source width
      unsigned W = getValueWidth(&Ext);               // wide width
      uint64_t k = AmtC->getValue().getZExtValue();
      if (k >= N)
        return false;
      // Bits 0..N-k-1 of the lshr result are safe (from original a).
      APInt SafeMask = APInt::getLowBitsSet(W, N - k);
      if (BO->use_empty())
        return false;
      for (User *LshrUser : BO->users()) {
        // and(lshr, const_mask) where mask ⊆ SafeMask is fine.
        if (auto *AndU = dyn_cast<BinaryOperator>(LshrUser)) {
          if (AndU->getOpcode() != Instruction::And)
            return false;
          auto *MaskC = dyn_cast<ConstantInt>(
              AndU->getOperand(0) == BO ? AndU->getOperand(1)
                                        : AndU->getOperand(0));
          if (!MaskC || (MaskC->getValue() & ~SafeMask) != 0)
            return false;
        } else if (auto *TrU = dyn_cast<TruncInst>(LshrUser)) {
          // trunc(lshr, M) where M <= N-k only touches safe bits.
          if (getValueWidth(TrU) > N - k)
            return false;
        } else {
          return false;
        }
      }
      return true;
    }
    default:
      return false;
    }
  }

  return false;
}

bool tryConvertWholeSExtToZExt(SExtInst &Ext) {
  if (Ext.use_empty())
    return false;

  // This is the shared-value variant of the masked-use fold above. Only weaken
  // the defining sext when every use is compatible with zero-extension
  // semantics; otherwise keep the single shared sext.
  for (User *U : Ext.users())
    if (!sextUseAllowsZExt(*U, Ext))
      return false;

  auto *ZExt = CastInst::CreateZExtOrBitCast(Ext.getOperand(0), Ext.getType(),
                                             "", Ext.getIterator());
  ZExt->setDebugLoc(Ext.getDebugLoc());
  ZExt->takeName(&Ext);
  // This shared-value rewrite is justified only because every use ignores the
  // sign-propagated high bits of the sext. That does not imply the source
  // itself is non-negative, so the replacement must not carry nneg.
  Ext.replaceAllUsesWith(ZExt);
  Ext.eraseFromParent();
  return true;
}

unsigned getUnsignedRangeWidth(const Use &OperandUse, LazyValueInfo &LVI) {
  Value *V = OperandUse.get();
  if (auto Ext = getExtOperandInfo(V))
    if (Ext->Kind == ExtKind::ZExt)
      return Ext->NarrowWidth;

  if (auto *CI = getScalarOrSplatConstantInt(V))
    return std::max(1u, CI->getValue().getActiveBits());

  if (!isa<IntegerType>(V->getType()))
    return 0;

  ConstantRange CR = LVI.getConstantRangeAtUse(OperandUse, /*UndefAllowed=*/false);
  if (CR.isFullSet())
    return 0;

  APInt UMax = CR.getUnsignedMax();
  unsigned Bits = UMax.getActiveBits();
  return std::max(1u, Bits);
}

enum class NarrowUDivOperandKind {
  Existing,
  NewZExt,
  NewTrunc,
};

struct NarrowUDivOperandPlan {
  NarrowUDivOperandKind Kind = NarrowUDivOperandKind::Existing;
  Value *Source = nullptr;
  Instruction *RemovableBoundary = nullptr;
  unsigned AddedBoundaryCost = 0;
  unsigned RemovedBoundaryCost = 0;
};

struct NarrowUDivResultPlan {
  TruncInst *TruncUser = nullptr;
  unsigned AddedBoundaryCost = 0;
  unsigned RemovedBoundaryCost = 0;
};

bool tryNarrowUDivWithRange(BinaryOperator &BO, LazyValueInfo &LVI) {
  assert((BO.getOpcode() == Instruction::UDiv ||
          BO.getOpcode() == Instruction::URem) &&
         "UDiv/URem narrowing expects a udiv or urem instruction");
  if (!isIntegerValue(&BO) || !isIntegerValue(BO.getOperand(0)) ||
      !isIntegerValue(BO.getOperand(1)))
    return false;

  unsigned OrigWidth = getValueWidth(&BO);
  unsigned LHSWidth = getUnsignedRangeWidth(BO.getOperandUse(0), LVI);
  unsigned RHSWidth = getUnsignedRangeWidth(BO.getOperandUse(1), LVI);
  if (LHSWidth == 0 || RHSWidth == 0)
    return false;

  unsigned TargetWidth = std::max(LHSWidth, RHSWidth);
  if (TargetWidth >= OrigWidth)
    return false;

  auto planOperand = [&](unsigned OperandIdx) -> std::optional<NarrowUDivOperandPlan> {
    Value *V = BO.getOperand(OperandIdx);

    if (auto *C = dyn_cast<Constant>(V)) {
      if (!isIntegerValue(C) || !haveSameIntegerShape(C->getType(), BO.getType()) ||
          !canRepresentConstant(*C, ExtKind::ZExt, TargetWidth))
        return std::nullopt;
      return NarrowUDivOperandPlan{
          NarrowUDivOperandKind::Existing,
          convertConstantToNarrow(*C, TargetWidth),
          nullptr,
          0,
          0,
      };
    }

    if (auto Ext = getExtOperandInfo(V)) {
      if (Ext->Kind != ExtKind::ZExt)
        return std::nullopt;
      if (Ext->NarrowWidth > TargetWidth)
        return std::nullopt;

      NarrowUDivOperandPlan Plan;
      Plan.Source = Ext->NarrowValue;
      if (Ext->Producer->hasOneUse()) {
        Plan.RemovableBoundary = Ext->Producer;
        Plan.RemovedBoundaryCost = 1;
      }
      if (Ext->NarrowWidth == TargetWidth)
        return Plan;

      Plan.Kind = NarrowUDivOperandKind::NewZExt;
      Plan.AddedBoundaryCost = 1;
      return Plan;
    }

    // Range facts can prove a narrower execution width, but they do not by
    // themselves justify the rewrite. We only use them to legalize a truncation
    // when enough existing boundary instructions around the udiv can be removed.
    return NarrowUDivOperandPlan{
        NarrowUDivOperandKind::NewTrunc,
        V,
        nullptr,
        1,
        0,
    };
  };

  auto planResult = [&]() -> NarrowUDivResultPlan {
    if (BO.hasOneUse()) {
      if (auto *Tr = dyn_cast<TruncInst>(*BO.user_begin())) {
        if (getValueWidth(Tr) == TargetWidth)
          return NarrowUDivResultPlan{Tr, 0, 1};
      }
    }
    return NarrowUDivResultPlan{nullptr, 1, 0};
  };

  auto LHSPlan = planOperand(0);
  auto RHSPlan = planOperand(1);
  if (!LHSPlan || !RHSPlan)
    return false;
  NarrowUDivResultPlan ResultPlan = planResult();

  unsigned AddedBoundaryCost = LHSPlan->AddedBoundaryCost +
                               RHSPlan->AddedBoundaryCost +
                               ResultPlan.AddedBoundaryCost;
  unsigned RemovedBoundaryCost = LHSPlan->RemovedBoundaryCost +
                                 RHSPlan->RemovedBoundaryCost +
                                 ResultPlan.RemovedBoundaryCost;

  // Drive the rewrite from removable boundary instructions. A narrow udiv is
  // worthwhile only if it strictly reduces the number of width changes around
  // the region instead of merely moving or adding them.
  if (RemovedBoundaryCost == 0 || AddedBoundaryCost >= RemovedBoundaryCost)
    return false;

  IRBuilder<> B(&BO);
  auto *TargetTy = getSameShapeIntegerType(BO.getType(), TargetWidth);

  auto materializeOperand = [&](const NarrowUDivOperandPlan &Plan,
                                const Twine &Name) -> Value * {
    switch (Plan.Kind) {
    case NarrowUDivOperandKind::Existing:
      return Plan.Source;
    case NarrowUDivOperandKind::NewZExt: {
      // CreateZExt may fold to a constant when Plan.Source is a ConstantInt
      // (e.g. the NarrowValue of a zext of a literal).  Use dyn_cast so we
      // only call setDebugLoc when the result is actually an instruction.
      Value *ZV = B.CreateZExt(Plan.Source, TargetTy, Name);
      if (auto *Z = dyn_cast<Instruction>(ZV))
        Z->setDebugLoc(BO.getDebugLoc());
      return ZV;
    }
    case NarrowUDivOperandKind::NewTrunc: {
      Value *TrV = B.CreateTrunc(Plan.Source, TargetTy, Name);
      if (auto *Tr = dyn_cast<Instruction>(TrV))
        Tr->setDebugLoc(BO.getDebugLoc());
      return TrV;
    }
    }
    llvm_unreachable("Unexpected narrow udiv operand plan");
  };

  Value *NarrowLHS = materializeOperand(*LHSPlan, BO.getName() + ".lhs.narrow");
  Value *NarrowRHS = materializeOperand(*RHSPlan, BO.getName() + ".rhs.narrow");
  // CreateBinOp may fold to a constant when both operands are constants
  // (e.g. both LHS and RHS plans are Existing ConstantInts).  Use Value* and
  // dyn_cast so we only call setDebugLoc / setIsExact when the result is an
  // actual instruction.
  Value *NarrowDivV = B.CreateBinOp(
      (Instruction::BinaryOps)BO.getOpcode(), NarrowLHS, NarrowRHS,
      BO.getName() + ".narrow");
  if (auto *NarrowDiv = dyn_cast<Instruction>(NarrowDivV)) {
    NarrowDiv->setDebugLoc(BO.getDebugLoc());
    if (BO.getOpcode() == Instruction::UDiv && BO.isExact())
      cast<BinaryOperator>(NarrowDiv)->setIsExact(true);
  }

  if (ResultPlan.TruncUser != nullptr) {
    ResultPlan.TruncUser->replaceAllUsesWith(NarrowDivV);
    ResultPlan.TruncUser->eraseFromParent();
  } else {
    Value *WideDivV = B.CreateZExt(NarrowDivV, BO.getType(), BO.getName());
    if (auto *WideDiv = dyn_cast<Instruction>(WideDivV))
      WideDiv->setDebugLoc(BO.getDebugLoc());
    BO.replaceAllUsesWith(WideDivV);
  }
  BO.eraseFromParent();

  auto tryDeleteBoundary = [](Instruction *I) {
    if (I != nullptr && I->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(I);
  };
  tryDeleteBoundary(LHSPlan->RemovableBoundary);
  tryDeleteBoundary(RHSPlan->RemovableBoundary);

  return true;
}

Value *findExistingZExtToWidth(Value *Src, unsigned TargetWidth) {
  // ConstantData (ConstantInt, undef, poison, …) has no use list;
  // guard before calling users() which asserts hasUseList().
  if (!Src->hasUseList())
    return nullptr;
  for (User *U : Src->users()) {
    auto *Z = dyn_cast<ZExtInst>(U);
    if (!Z)
      continue;
    if (getValueWidth(Z) == TargetWidth)
      return Z;
  }
  return nullptr;
}

bool canWidenAddOperandWithoutOverflow(const ExtOperandInfo &ExtInfo,
                                       ConstantInt &C) {
  if (ExtInfo.Kind != ExtKind::ZExt)
    return false;

  // Require the narrow operand's maximal zero-extended value plus the constant
  // to stay within the intermediate width. That lets us bypass the narrower
  // add without changing modulo arithmetic.
  unsigned MidWidth = ExtInfo.WideWidth;
  APInt Max = APInt::getLowBitsSet(MidWidth, ExtInfo.NarrowWidth).zext(MidWidth);
  APInt Sum = Max + C.getValue().zextOrTrunc(MidWidth);
  return !Sum.ult(Max);
}

bool tryWidenAddThroughZExt(BinaryOperator &BO) {
  assert(BO.getOpcode() == Instruction::Add &&
         "Add widening expects an add instruction");
  if (!isIntegerValue(&BO))
    return false;
  if (!BO.hasOneUse())
    return false;

  auto *WideZ = dyn_cast<ZExtInst>(*BO.user_begin());
  if (!WideZ)
    return false;
  if (!isIntegerValue(WideZ))
    return false;

  unsigned WideWidth = getValueWidth(WideZ);
  unsigned MidWidth = getValueWidth(&BO);
  (void)MidWidth;
  assert(WideWidth > MidWidth &&
         "ZExt users should be wider than their operands");
  if (BO.hasNoUnsignedWrap() || BO.hasNoSignedWrap())
    return false;

  // This is a narrow local widening pattern: if one operand already comes from
  // a zext and the result is immediately zext'ed again, try to reuse an
  // existing wider path and do the add there instead.
  auto trySide = [&](unsigned ExtIdx, unsigned OtherIdx) -> bool {
    auto ExtInfo = getExtOperandInfo(BO.getOperand(ExtIdx));
    auto *C = dyn_cast<ConstantInt>(BO.getOperand(OtherIdx));
    if (!ExtInfo || !C)
      return false;
    if (!canWidenAddOperandWithoutOverflow(*ExtInfo, *C))
      return false;

    Value *WideBase =
        findExistingZExtToWidth(ExtInfo->NarrowValue, WideWidth);
    IRBuilder<> B(WideZ);
    // Only reuse an existing zext if it is guaranteed to dominate the
    // insertion point (immediately before WideZ).  A zext in a different
    // basic block may not dominate WideZ's block, producing a use-before-def
    // verifier error.  NarrowValue dominates WideZ (it flows BO→WideZ), so a
    // freshly created zext here is always safe.
    if (WideBase) {
      auto *WideBaseI = cast<Instruction>(WideBase);
      if (WideBaseI->getParent() != WideZ->getParent() ||
          !WideBaseI->comesBefore(WideZ))
        WideBase = nullptr;
    }
    if (!WideBase)
      WideBase = B.CreateZExt(ExtInfo->NarrowValue,
                              IntegerType::get(BO.getContext(), WideWidth));

    Value *WideC =
        ConstantInt::get(IntegerType::get(BO.getContext(), WideWidth),
                         C->getValue().zextOrTrunc(WideWidth));
    // CreateAdd may fold to a non-Instruction (e.g. ConstantInt) when both
    // operands are constants; use Value* and dyn_cast only for setDebugLoc.
    Value *WideAddV = B.CreateAdd(WideBase, WideC, BO.getName() + ".wide");
    if (auto *WideAdd = dyn_cast<Instruction>(WideAddV))
      WideAdd->setDebugLoc(BO.getDebugLoc());
    WideZ->replaceAllUsesWith(WideAddV);
    WideZ->eraseFromParent();

    if (BO.use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(&BO);
    return true;
  };

  return trySide(0, 1) || trySide(1, 0);
}

// Handle: zext iN(add iN(trunc iW X, C)) where X is zero-bounded at N-1 bits
// and C is a non-negative constant fitting in N-1 bits.
//
// Since X ≤ 2^(N-1)-1 and C ≤ 2^(N-1)-1, the add X+C ≤ 2^N-2 < 2^N, so no
// unsigned overflow in iN.  The trunc is lossless because X ≤ 2^(N-1)-1 < 2^N.
// Therefore: zext iN(add iN(trunc iW X, C)) == add iW(X, zext C to iW).
//
// Eliminates the trunc→add→zext chain, replacing it with a single wider add.
bool tryWidenAddOverTruncThroughZExt(BinaryOperator &BO) {
  if (BO.getOpcode() != Instruction::Add)
    return false;
  if (!isIntegerValue(&BO))
    return false;
  if (!BO.hasOneUse())
    return false;

  auto *WideZ = dyn_cast<ZExtInst>(*BO.user_begin());
  if (!WideZ || !isIntegerValue(WideZ))
    return false;

  unsigned MidWidth = getValueWidth(&BO);
  unsigned WideWidth = getValueWidth(WideZ);
  assert(WideWidth > MidWidth);

  if (MidWidth < 2)
    return false; // Need at least 2 bits for "N-1 bits" check to be meaningful.

  auto trySide = [&](unsigned TruncIdx, unsigned OtherIdx) -> bool {
    auto *Tr = dyn_cast<TruncInst>(BO.getOperand(TruncIdx));
    if (!Tr)
      return false;
    auto *C = dyn_cast<ConstantInt>(BO.getOperand(OtherIdx));
    if (!C)
      return false;

    Value *X = Tr->getOperand(0);
    if (getValueWidth(X) != WideWidth)
      return false;

    // Verify C is a non-negative value fitting in MidWidth-1 bits.
    // isIntN checks unsigned fit, which for iN ConstantInt means C ∈ [0, 2^(N-1)-1].
    if (!C->getValue().isIntN(MidWidth - 1))
      return false;

    // X must be zero-bounded at MidWidth-1 bits so trunc(X, iN) + C < 2^N.
    if (!isZeroBoundedAtWidth(X, MidWidth - 1))
      return false;

    // Build wider add: add iW(X, C_wide)
    Value *WideC = ConstantInt::get(IntegerType::get(BO.getContext(), WideWidth),
                                    C->getValue().zext(WideWidth));
    IRBuilder<> B(WideZ);
    // CreateAdd may fold to a non-Instruction when X is a constant; use Value*
    // and dyn_cast only for setDebugLoc.
    Value *WideAddV = B.CreateAdd(X, WideC, BO.getName() + ".wide");
    if (auto *WideAdd = dyn_cast<Instruction>(WideAddV))
      WideAdd->setDebugLoc(BO.getDebugLoc());
    WideZ->replaceAllUsesWith(WideAddV);
    WideZ->eraseFromParent();
    if (BO.use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(&BO);
    return true;
  };

  return trySide(0, 1) || trySide(1, 0);
}

// Handle: zext nneg iN(sub iN(C, trunc iW X)) or zext nneg iN(sub iN(trunc iW X, C))
// where both operands are known to be in the signed non-negative iN range.
//
// The zext nneg flag alone is not enough: a wrapped iN subtraction can still
// land in the non-negative half of the narrow type (e.g. i8 0 - 253 == 3),
// so widening would be wrong.  To make the narrow subtraction equal the wide
// one, both operands must already fit in the signed non-negative iN range.
// Then `zext nneg` on the result implies the narrow subtraction did not cross
// below zero, so the iN sub equals the iW sub without wrap.
//
// Transforms: zext nneg(sub(C, trunc(X))) → sub iW(C_wide, X)
//             zext nneg(sub(trunc(X), C)) → sub iW(X, C_wide)
bool tryWidenSubOverTruncThroughZExtNneg(ZExtInst &ZExt) {
  if (!ZExt.hasNonNeg())
    return false;
  if (!isIntegerValue(&ZExt))
    return false;

  auto *Sub = dyn_cast<BinaryOperator>(ZExt.getOperand(0));
  if (!Sub || Sub->getOpcode() != Instruction::Sub)
    return false;
  if (!Sub->hasOneUse())
    return false;
  if (!isIntegerValue(Sub))
    return false;

  unsigned MidWidth = getValueWidth(Sub);
  unsigned WideWidth = getValueWidth(&ZExt);
  assert(WideWidth > MidWidth);

  // Try both orderings: sub(C, Tr) and sub(Tr, C).
  auto trySide = [&](unsigned TruncIdx, unsigned ConstIdx) -> bool {
    auto *Tr = dyn_cast<TruncInst>(Sub->getOperand(TruncIdx));
    if (!Tr)
      return false;
    auto *C = dyn_cast<ConstantInt>(Sub->getOperand(ConstIdx));
    if (!C || C->isNegative())
      return false;

    Value *X = Tr->getOperand(0);
    if (getValueWidth(X) != WideWidth)
      return false;

    // Trunc must be lossless and both operands must fit in the signed
    // non-negative iN range so `zext nneg` rules out narrow wraparound.
    if (!isZeroBoundedAtWidth(X, MidWidth - 1))
      return false;
    if (!C->getValue().isIntN(MidWidth - 1))
      return false;

    // Build wider sub: sub iW(Op0_wide, Op1_wide)
    Value *WideC = ConstantInt::get(IntegerType::get(Sub->getContext(), WideWidth),
                                    C->getValue().zext(WideWidth));
    IRBuilder<> B(&ZExt);
    Value *WideOp0 = (TruncIdx == 0) ? X : WideC;
    Value *WideOp1 = (TruncIdx == 0) ? WideC : X;
    Value *WideSubV =
        B.CreateSub(WideOp0, WideOp1, Sub->getName() + ".wide");
    if (auto *WideSub = dyn_cast<Instruction>(WideSubV))
      WideSub->setDebugLoc(Sub->getDebugLoc());
    ZExt.replaceAllUsesWith(WideSubV);
    ZExt.eraseFromParent();
    if (Sub->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(Sub);
    return true;
  };

  return trySide(0, 1) || trySide(1, 0);
}

// When a zext nneg is used only as GEP indices, we can eliminate the zext by
// using the narrow value directly as the GEP index. The nneg flag guarantees
// the value is non-negative, so GEP's signed index extension gives the same
// result as the zero extension: sext(X) == zext(X) when X >= 0.
// Example:
//   %ext = zext nneg i32 %x to i64
//   %gep = getelementptr T, ptr %p, i64 %ext
// => %gep = getelementptr T, ptr %p, i32 %x
// When sext iN X to iM is used only as GEP indices, we can eliminate the sext
// by using X directly as the GEP index. GEP sign-extends its indices to
// pointer width, which is exactly what sext does, so they are always equivalent.
bool tryShrinkSExtGEPIndex(SExtInst &SExt) {
  if (!isa<IntegerType>(SExt.getSrcTy()) || !isa<IntegerType>(SExt.getType()))
    return false;
  if (SExt.use_empty())
    return false;
  // All uses must be GEP index operands (not the base pointer, operand 0).
  for (Use &U : SExt.uses()) {
    auto *GEP = dyn_cast<GetElementPtrInst>(U.getUser());
    if (!GEP || U.getOperandNo() == 0)
      return false;
  }
  Value *NarrowSrc = SExt.getOperand(0);
  SmallVector<GetElementPtrInst *, 4> GEPs;
  for (Use &U : SExt.uses())
    GEPs.push_back(cast<GetElementPtrInst>(U.getUser()));
  for (GetElementPtrInst *GEP : GEPs)
    GEP->replaceUsesOfWith(&SExt, NarrowSrc);
  SExt.eraseFromParent();
  return true;
}

bool tryShrinkZExtGEPIndex(ZExtInst &ZExt) {
  if (!isa<IntegerType>(ZExt.getSrcTy()) || !isa<IntegerType>(ZExt.getType()))
    return false;
  if (!isIntegerValue(&ZExt))
    return false;
  // GEP sign-extends indices to pointer width, so replacing zext(X) with X
  // is valid only when sext(X) == zext(X), i.e., X is non-negative.
  // The nneg flag makes this explicit; alternatively, structural analysis can
  // prove the source fits in (NarrowBits-1) unsigned bits, meaning the sign
  // bit of the narrow type is always 0.
  unsigned NarrowBits = ZExt.getSrcTy()->getIntegerBitWidth();
  if (!ZExt.hasNonNeg() &&
      !isZeroBoundedAtWidth(ZExt.getOperand(0), NarrowBits - 1))
    return false;
  // Only when all uses are GEP instructions using this as an index (not base).
  if (ZExt.use_empty())
    return false;
  for (Use &U : ZExt.uses()) {
    auto *GEP = dyn_cast<GetElementPtrInst>(U.getUser());
    if (!GEP)
      return false;
    // Make sure this use is as an index operand, not the base pointer (op 0).
    if (U.getOperandNo() == 0)
      return false;
  }
  // Replace each GEP's use of this zext with the narrow source value.
  Value *NarrowSrc = ZExt.getOperand(0);
  SmallVector<GetElementPtrInst *, 4> GEPs;
  for (Use &U : ZExt.uses())
    GEPs.push_back(cast<GetElementPtrInst>(U.getUser()));
  for (GetElementPtrInst *GEP : GEPs)
    GEP->replaceUsesOfWith(&ZExt, NarrowSrc);
  ZExt.eraseFromParent();
  return true;
}

// When zext iN X to iM is used only by binops (lshr by constant ≥ 1, or and
// with constant < 2^(N-1)) that themselves are used only as GEP indices, we
// can narrow the entire chain back to iN. For example:
//   %z  = zext i8 %x to i32
//   %lo = lshr i32 %z, 4          ; result in [0,15] → fits in i8, non-neg
//   %hi = and i32 %z, 15          ; result in [0,15] → fits in i8, non-neg
//   %p1 = gep ..., i32 %lo
//   %p2 = gep ..., i32 %hi
// becomes:
//   %lo = lshr i8 %x, 4
//   %hi = and i8 %x, 15
//   %p1 = gep ..., i8 %lo
//   %p2 = gep ..., i8 %hi
bool tryShrinkZExtThroughBinopToGEP(ZExtInst &ZExt) {
  if (!isa<IntegerType>(ZExt.getSrcTy()) || !isa<IntegerType>(ZExt.getType()))
    return false;
  if (!isIntegerValue(&ZExt))
    return false;
  if (ZExt.use_empty())
    return false;

  Value *Src = ZExt.getOperand(0);
  Type *NarrowTy = Src->getType();
  unsigned NarrowBits = NarrowTy->getIntegerBitWidth();

  // Validate all uses: each must be a lshr or and BinaryOperator with a
  // constant that makes the result non-negative when interpreted as iN, and
  // all uses of that BinaryOperator must be GEP index operands.
  for (Use &U : ZExt.uses()) {
    auto *BO = dyn_cast<BinaryOperator>(U.getUser());
    if (!BO)
      return false;
    // ZExt must be the left operand.
    if (U.getOperandNo() != 0)
      return false;
    auto *C = dyn_cast<ConstantInt>(BO->getOperand(1));
    if (!C)
      return false;

    if (BO->getOpcode() == Instruction::LShr) {
      // lshr result has at most (NarrowBits - shift) significant bits.
      // For it to be non-negative as iN we need at least 1 bit shifted out.
      uint64_t Shift = C->getZExtValue();
      if (Shift == 0 || Shift >= NarrowBits)
        return false;
    } else if (BO->getOpcode() == Instruction::And) {
      // and result is bounded by the constant. Non-negative as iN means the
      // constant must be < 2^(NarrowBits-1).
      APInt Mask = C->getValue();
      if (!Mask.ult(APInt::getSignedMaxValue(NarrowBits).zext(
              BO->getType()->getIntegerBitWidth()) + 1))
        return false;
    } else {
      return false;
    }

    // All uses of the binop must be GEP index operands.
    if (BO->use_empty())
      return false;
    for (Use &BOU : BO->uses()) {
      auto *GEP = dyn_cast<GetElementPtrInst>(BOU.getUser());
      if (!GEP || BOU.getOperandNo() == 0)
        return false;
    }
  }

  // Perform the transformation.
  for (Use &U : ZExt.uses()) {
    auto *BO = cast<BinaryOperator>(U.getUser());
    IRBuilder<> B(BO);

    // Build narrowed binop operating on the original i8/iN source.
    Value *NarrowBO = nullptr;
    auto *C = cast<ConstantInt>(BO->getOperand(1));
    if (BO->getOpcode() == Instruction::LShr) {
      Value *NarrowC = ConstantInt::get(NarrowTy, C->getZExtValue());
      NarrowBO = B.CreateLShr(Src, NarrowC, BO->getName());
    } else {
      assert(BO->getOpcode() == Instruction::And);
      APInt NarrowMask = C->getValue().trunc(NarrowBits);
      NarrowBO = B.CreateAnd(Src, ConstantInt::get(NarrowTy, NarrowMask),
                             BO->getName());
    }

    // Update all GEP users of this binop.
    SmallVector<GetElementPtrInst *, 4> GEPs;
    for (Use &BOU : BO->uses())
      GEPs.push_back(cast<GetElementPtrInst>(BOU.getUser()));
    for (GetElementPtrInst *GEP : GEPs)
      GEP->replaceUsesOfWith(BO, NarrowBO);
  }

  // Erase the now-dead wide binops and the zext.
  SmallVector<Instruction *, 4> ToErase;
  for (Use &U : ZExt.uses())
    ToErase.push_back(cast<Instruction>(U.getUser()));
  for (Instruction *I : ToErase)
    I->eraseFromParent();
  ZExt.eraseFromParent();
  return true;
}

// When zext iN X to iM is used only as the condition of switch statements,
// we can narrow the switch to use X directly (replacing iM case constants
// with iN truncations). Cases with values >= 2^N are unreachable since
// zext only produces values in [0, 2^N), so we drop them (setting their
// targets to the switch's default block — they become dead).
bool tryShrinkZExtSwitch(ZExtInst &ZExt) {
  if (!isa<IntegerType>(ZExt.getSrcTy()) || !isa<IntegerType>(ZExt.getType()))
    return false;
  if (!isIntegerValue(&ZExt))
    return false;
  if (ZExt.use_empty())
    return false;

  Value *Src = ZExt.getOperand(0);
  Type *NarrowTy = Src->getType();
  unsigned NarrowBits = NarrowTy->getIntegerBitWidth();
  APInt MaxNarrow = APInt::getMaxValue(NarrowBits).zext(
      ZExt.getType()->getIntegerBitWidth());

  // All uses must be SwitchInsts where ZExt is the condition operand.
  for (Use &U : ZExt.uses()) {
    auto *SI = dyn_cast<SwitchInst>(U.getUser());
    if (!SI || U.getOperandNo() != 0)
      return false;
  }

  // Collect the switches and update them.
  SmallVector<SwitchInst *, 4> Switches;
  for (Use &U : ZExt.uses())
    Switches.push_back(cast<SwitchInst>(U.getUser()));

  for (SwitchInst *SI : Switches) {
    // Change the condition to the narrow source.
    SI->setCondition(Src);

    // Update or remove each case. Cases with values > MaxNarrow are
    // unreachable (zext can't produce them); remove them.
    SmallVector<SwitchInst::CaseIt, 4> ToRemove;
    for (auto It = SI->case_begin(), End = SI->case_end(); It != End; ++It) {
      const APInt &CaseVal = It->getCaseValue()->getValue();
      if (CaseVal.ugt(MaxNarrow)) {
        ToRemove.push_back(It);
      } else {
        // Truncate the constant to the narrow type.
        ConstantInt *NewC = ConstantInt::get(
            ZExt.getContext(), CaseVal.trunc(NarrowBits));
        It->setValue(NewC);
      }
    }
    // Remove in reverse order to keep iterators valid.
    for (auto It : llvm::reverse(ToRemove))
      SI->removeCase(It);
  }

  ZExt.eraseFromParent();
  return true;
}

// When sext iN X to iM is used only as the condition of switch statements,
// we can narrow the switch to use X directly. sext produces values in the
// signed range [-2^(N-1), 2^(N-1) - 1], so cases outside that range are
// unreachable and are removed; in-range constants are truncated to iN.
bool tryShrinkSExtSwitch(SExtInst &SExt) {
  if (!isa<IntegerType>(SExt.getSrcTy()) || !isa<IntegerType>(SExt.getType()))
    return false;
  if (!isIntegerValue(&SExt))
    return false;
  if (SExt.use_empty())
    return false;

  Value *Src = SExt.getOperand(0);
  Type *NarrowTy = Src->getType();
  unsigned NarrowBits = NarrowTy->getIntegerBitWidth();
  unsigned WideBits = SExt.getType()->getIntegerBitWidth();

  // All uses must be SwitchInsts where SExt is the condition operand.
  for (Use &U : SExt.uses()) {
    auto *SI = dyn_cast<SwitchInst>(U.getUser());
    if (!SI || U.getOperandNo() != 0)
      return false;
  }

  // Collect the switches.
  SmallVector<SwitchInst *, 4> Switches;
  for (Use &U : SExt.uses())
    Switches.push_back(cast<SwitchInst>(U.getUser()));

  for (SwitchInst *SI : Switches) {
    SI->setCondition(Src);

    SmallVector<SwitchInst::CaseIt, 4> ToRemove;
    for (auto It = SI->case_begin(), End = SI->case_end(); It != End; ++It) {
      const APInt &CaseVal = It->getCaseValue()->getValue();
      // A case is reachable iff sext(trunc(CaseVal)) == CaseVal.
      APInt Truncated = CaseVal.trunc(NarrowBits);
      if (Truncated.sext(WideBits) != CaseVal) {
        ToRemove.push_back(It);
      } else {
        ConstantInt *NewC = ConstantInt::get(SExt.getContext(), Truncated);
        It->setValue(NewC);
      }
    }
    for (auto It : llvm::reverse(ToRemove))
      SI->removeCase(It);
  }

  SExt.eraseFromParent();
  return true;
}

// Collapse sext(sext(x: iN→iM): iM→iW) → sext(x: iN→iW).
// Profitable only when the inner sext has a single use (this outer sext),
// allowing the inner sext to be removed: net effect is -1 instruction.
bool tryShrinkSExtOfSExt(SExtInst &Outer) {
  if (!isIntegerValue(&Outer))
    return false;

  auto *Inner = dyn_cast<SExtInst>(Outer.getOperand(0));
  if (!Inner || !Inner->hasOneUse())
    return false;

  Value *Src = Inner->getOperand(0);
  unsigned SrcWidth = getValueWidth(Src);
  unsigned OuterWidth = getValueWidth(&Outer);
  if (SrcWidth >= OuterWidth)
    return false;

  IRBuilder<> B(&Outer);
  Value *NewSExt = B.CreateSExt(Src, Outer.getType(), Outer.getName());
  if (auto *I = dyn_cast<Instruction>(NewSExt))
    I->setDebugLoc(Outer.getDebugLoc());

  Outer.replaceAllUsesWith(NewSExt);
  Outer.eraseFromParent();
  Inner->eraseFromParent();
  return true;
}

// Sink sext through a bitwise binop when both operands are sext from the same
// source type and each has exactly one use (this binop):
//
//   %sa = sext iN %a to iW   (one use)
//   %sb = sext iN %b to iW   (one use)
//   %r  = and/or/xor iW %sa, %sb
//   → %narrow = and/or/xor iN %a, %b
//     %r      = sext iN %narrow to iW
//
// Net: removes 3 instructions (2 sext + wide binop), adds 2 (narrow binop +
// sext) → net -1.  Correctness: sext distributes over bitwise ops because each
// output bit depends only on the corresponding input bit, and sign extension
// replicates a single bit position.
bool tryShrinkSExtBitwiseBinop(SExtInst &SExt) {
  if (!isIntegerValue(&SExt))
    return false;
  if (!SExt.hasOneUse())
    return false;

  auto *BO = dyn_cast<BinaryOperator>(SExt.user_back());
  if (!BO)
    return false;

  auto Opc = BO->getOpcode();
  if (Opc != Instruction::And && Opc != Instruction::Or &&
      Opc != Instruction::Xor)
    return false;

  // The other operand must also be a sext with exactly one use (this binop).
  Value *OtherOp =
      (BO->getOperand(0) == &SExt) ? BO->getOperand(1) : BO->getOperand(0);
  auto *OtherSExt = dyn_cast<SExtInst>(OtherOp);
  if (!OtherSExt || !OtherSExt->hasOneUse())
    return false;

  Value *SrcA = SExt.getOperand(0);
  Value *SrcB = OtherSExt->getOperand(0);

  // Sources must have identical types so the narrow binop is well-typed.
  if (SrcA->getType() != SrcB->getType())
    return false;
  if (!isIntegerValue(SrcA))
    return false;

  IRBuilder<> B(BO);
  Value *NarrowBO = B.CreateBinOp(Opc, SrcA, SrcB, BO->getName());
  if (auto *I = dyn_cast<Instruction>(NarrowBO))
    I->setDebugLoc(BO->getDebugLoc());
  Value *NewSExt = B.CreateSExt(NarrowBO, BO->getType());
  if (auto *I = dyn_cast<Instruction>(NewSExt))
    I->setDebugLoc(BO->getDebugLoc());

  BO->replaceAllUsesWith(NewSExt);
  // Erase in dependency order: BO first (removes its uses of SExt/OtherSExt).
  BO->eraseFromParent();
  OtherSExt->eraseFromParent();
  SExt.eraseFromParent();
  return true;
}

// Eliminate a zext when the only use is an llvm.expect.iM call whose only use
// is an icmp ne/eq with zero:
//
//   %z   = zext i1 %x to i64
//   %exp = call i64 @llvm.expect.i64(i64 %z, i64 0)
//   %cmp = icmp ne i64 %exp, 0
//
// becomes:
//
//   %exp = call i1 @llvm.expect.i1(i1 %x, i1 false)
//   %cmp = icmp ne i1 %exp, false
//
// Correctness: zext i1 %x to i64 is 0 or 1, so
//   llvm.expect.i64(%z, 0) != 0  iff  %x != false
// The expect hint is preserved (expected value mapped to same bool).
// Verified with alive-tv.
bool tryShrinkZExtOfLLVMExpect(ZExtInst &ZExt) {
  if (!isa<IntegerType>(ZExt.getSrcTy()) || !isa<IntegerType>(ZExt.getType()))
    return false;
  if (ZExt.use_empty())
    return false;

  // Source must be i1.
  if (!ZExt.getSrcTy()->isIntegerTy(1))
    return false;

  // All uses must be llvm.expect.iM calls where ZExt is the first argument.
  for (Use &U : ZExt.uses()) {
    auto *CI = dyn_cast<CallInst>(U.getUser());
    if (!CI)
      return false;
    Function *Callee = CI->getCalledFunction();
    if (!Callee)
      return false;
    Intrinsic::ID IID = Callee->getIntrinsicID();
    if (IID != Intrinsic::expect && IID != Intrinsic::expect_with_probability)
      return false;
    if (U.getOperandNo() != 0)
      return false;
    // The expect call's only use must be icmp eq/ne with zero.
    if (!CI->hasOneUse())
      return false;
    auto *Cmp = dyn_cast<ICmpInst>(CI->user_back());
    if (!Cmp)
      return false;
    ICmpInst::Predicate Pred = Cmp->getPredicate();
    if (Pred != ICmpInst::ICMP_NE && Pred != ICmpInst::ICMP_EQ)
      return false;
    // One operand must be the expect call, the other must be zero.
    Value *OtherOp = Cmp->getOperand(0) == CI ? Cmp->getOperand(1)
                                               : Cmp->getOperand(0);
    auto *ConstOther = dyn_cast<ConstantInt>(OtherOp);
    if (!ConstOther || !ConstOther->isZero())
      return false;
  }

  // Collect calls to transform.
  SmallVector<CallInst *, 4> Calls;
  for (Use &U : ZExt.uses())
    Calls.push_back(cast<CallInst>(U.getUser()));

  Value *NarrowSrc = ZExt.getOperand(0); // i1 value
  LLVMContext &Ctx = ZExt.getContext();
  Module *M = ZExt.getModule();

  for (CallInst *CI : Calls) {
    Function *Callee = CI->getCalledFunction();
    Intrinsic::ID IID = Callee->getIntrinsicID();

    // Map the expected value: the second argument should be 0 or 1.
    // We convert it to i1 (0→false, nonzero→true).
    Value *ExpectedWide = CI->getArgOperand(1);
    ConstantInt *ExpectedConst = cast<ConstantInt>(ExpectedWide);
    Constant *ExpectedNarrow = ExpectedConst->isZero()
                                   ? ConstantInt::getFalse(Ctx)
                                   : ConstantInt::getTrue(Ctx);

    // Build the narrow expect call.
    Function *NarrowExpect;
    if (IID == Intrinsic::expect_with_probability) {
      NarrowExpect = Intrinsic::getOrInsertDeclaration(
          M, IID, {Type::getInt1Ty(Ctx)});
      // Third argument is the probability (double), keep it unchanged.
      Value *Prob = CI->getArgOperand(2);
      IRBuilder<> B(CI);
      CallInst *NewCall =
          B.CreateCall(NarrowExpect, {NarrowSrc, ExpectedNarrow, Prob});
      NewCall->setDebugLoc(CI->getDebugLoc());
      // Replace the icmp.
      ICmpInst *Cmp = cast<ICmpInst>(CI->user_back());
      Value *NewCmp = B.CreateICmp(Cmp->getPredicate(), NewCall,
                                   ConstantInt::getFalse(Ctx), Cmp->getName());
      if (auto *NewCmpI = dyn_cast<Instruction>(NewCmp))
        NewCmpI->setDebugLoc(Cmp->getDebugLoc());
      Cmp->replaceAllUsesWith(NewCmp);
      Cmp->eraseFromParent();
    } else {
      NarrowExpect = Intrinsic::getOrInsertDeclaration(
          M, IID, {Type::getInt1Ty(Ctx)});
      IRBuilder<> B(CI);
      CallInst *NewCall =
          B.CreateCall(NarrowExpect, {NarrowSrc, ExpectedNarrow});
      NewCall->setDebugLoc(CI->getDebugLoc());
      // Replace the icmp.
      ICmpInst *Cmp = cast<ICmpInst>(CI->user_back());
      Value *NewCmp = B.CreateICmp(Cmp->getPredicate(), NewCall,
                                   ConstantInt::getFalse(Ctx), Cmp->getName());
      if (auto *NewCmpI = dyn_cast<Instruction>(NewCmp))
        NewCmpI->setDebugLoc(Cmp->getDebugLoc());
      Cmp->replaceAllUsesWith(NewCmp);
      Cmp->eraseFromParent();
    }
    CI->eraseFromParent();
  }

  ZExt.eraseFromParent();
  return true;
}

// When the operand of a zext is structurally zero-bounded at a width NW2
// narrower than the zext's source type NW1, we can rebuild the operand at
// NW2 and emit a single zext from NW2 to WW instead.  Example:
//   zext i16 (and (zext i8 a to i16), (zext i8 b to i16)) to i32
//   => zext i8 (and i8 a, b) to i32
// This applies whenever getStructuralNarrowWidth returns a width < source.
bool tryShrinkZExtOfZeroBounded(ZExtInst &ZExt) {
  Value *Src = ZExt.getOperand(0);
  if (!isIntegerValue(&ZExt) || !isIntegerValue(Src))
    return false;

  unsigned SrcWidth = getValueWidth(Src);
  unsigned WideWidth = getValueWidth(&ZExt);
  assert(SrcWidth < WideWidth);

  // Only apply when Src has a single use (this ZExt).  If Src has multiple
  // ZExt users we would create duplicate narrow instructions; those cases are
  // better handled by the component widening system which can produce a single
  // widened value shared by all users.
  if (!Src->hasOneUse())
    return false;

  // Determine the structural narrow width of the operand.
  unsigned NarrowWidth = getStructuralNarrowWidth(Src);
  if (NarrowWidth == 0 || NarrowWidth >= SrcWidth)
    return false;

  // Cost model: we add new narrow instructions (tracked in AddedValues) and
  // one new narrow zext (NarrowWidth→WideWidth); we remove the old wide zext
  // plus any dead intermediate instructions (tracked in RemovedInstructions).
  // Condition: AddedValues.size() + 1 <= 1 + RemovedInstructions.size()
  //   i.e. AddedValues.size() <= RemovedInstructions.size().
  SmallPtrSet<Value *, 8> AddedValues;
  SmallPtrSet<Instruction *, 8> RemovedInstructions;
  SmallPtrSet<Value *, 8> Visited;
  if (!collectTruncRootedValueCost(Src, NarrowWidth, AddedValues,
                                   RemovedInstructions, Visited))
    return false;
  if (AddedValues.size() > RemovedInstructions.size())
    return false;

  DenseMap<Value *, Value *> Cache;
  Value *NarrowSrc =
      materializeTruncRootedValueAtWidth(Src, NarrowWidth, &ZExt, &Cache);
  if (!NarrowSrc)
    return false;

  IRBuilder<> B(&ZExt);
  Value *NewZExt;
  if (NarrowWidth == WideWidth) {
    NewZExt = NarrowSrc;
  } else {
    NewZExt = B.CreateZExt(NarrowSrc, ZExt.getType(), ZExt.getName());
    if (auto *NZ = dyn_cast<Instruction>(NewZExt))
      NZ->setDebugLoc(ZExt.getDebugLoc());
  }

  ZExt.replaceAllUsesWith(NewZExt);
  ZExt.eraseFromParent();

  if (auto *SI = dyn_cast<Instruction>(Src))
    if (SI->use_empty())
      RecursivelyDeleteTriviallyDeadInstructions(SI);

  return true;
}

bool tryFoldZExtOfTruncToMask(ZExtInst &Ext) {
  // Skip dead instructions: there is no benefit to transforming a value that
  // has no uses, and doing so can create new dead instructions that perturb
  // subsequent transforms in the fixpoint loop.
  if (Ext.use_empty())
    return false;

  auto *Tr = dyn_cast<TruncInst>(Ext.getOperand(0));
  if (!Tr)
    return false;
  if (!isScalarOrFixedVectorIntegerValue(&Ext) ||
      !isScalarOrFixedVectorIntegerValue(Tr) ||
      !isScalarOrFixedVectorIntegerValue(Tr->getOperand(0)))
    return false;

  Value *Src = Tr->getOperand(0);
  if (!haveSameIntegerShape(Src->getType(), Tr->getType()) ||
      !haveSameIntegerShape(Src->getType(), Ext.getType()))
    return false;

  unsigned SrcWidth = getScalarIntegerWidth(Src->getType());
  unsigned NarrowWidth = getScalarIntegerWidth(Tr->getType());
  unsigned WideWidth = getScalarIntegerWidth(Ext.getType());
  if (NarrowWidth >= WideWidth)
    return false;

  IRBuilder<> B(&Ext);

  // Materialize the mask in the most convenient width we can without
  // reintroducing the original narrow type. When the original source is at
  // least as wide as the zext result, work directly at the result width.
  // Otherwise keep the source width, mask there, and extend once at the end.
  Value *Masked = nullptr;
  if (SrcWidth >= WideWidth) {
    Value *Base = Src;
    if (SrcWidth != WideWidth)
      Base = B.CreateTrunc(Src, Ext.getType());
    Masked = B.CreateAnd(Base, getLowBitsMaskConstant(Base->getType(),
                                                      NarrowWidth));
  } else {
    Value *Narrowed =
        B.CreateAnd(Src, getLowBitsMaskConstant(Src->getType(), NarrowWidth));
    Masked = B.CreateZExt(Narrowed, Ext.getType());
  }

  if (auto *NewI = dyn_cast<Instruction>(Masked)) {
    NewI->setDebugLoc(Ext.getDebugLoc());
    NewI->takeName(&Ext);
  }
  Ext.replaceAllUsesWith(Masked);
  Ext.eraseFromParent();

  if (Tr->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(Tr);

  return true;
}

// When trunc iM X to iN is used only as GEP indices, and X is provably
// non-negative and fits in iN (i.e., the high M-N bits of X are all zero AND
// bit N-1 of X is zero), we can use X directly in the GEP as an iM index and
// eliminate the trunc. The correctness condition is:
//   sext(trunc(X, N), M) == X
// which holds iff X < 2^(N-1) (non-negative, fits in signed iN).
// We check this via isZeroBoundedAtWidth(X, N-1).
bool tryShrinkTruncGEPIndex(TruncInst &Tr) {
  if (!isa<IntegerType>(Tr.getSrcTy()) || !isa<IntegerType>(Tr.getType()))
    return false;
  if (!isIntegerValue(&Tr))
    return false;
  if (Tr.use_empty())
    return false;

  Value *Src = Tr.getOperand(0);
  unsigned NarrowBits = Tr.getType()->getIntegerBitWidth();

  // All uses must be GEP index operands (operand 0 is the base pointer).
  for (Use &U : Tr.uses()) {
    auto *GEP = dyn_cast<GetElementPtrInst>(U.getUser());
    if (!GEP || U.getOperandNo() == 0)
      return false;
  }

  // Src must be provably < 2^(NarrowBits-1) so that sext(trunc(Src)) == Src.
  // isZeroBoundedAtWidth checks that the value fits in the given bit width.
  if (!isZeroBoundedAtWidth(Src, NarrowBits - 1))
    return false;

  // Replace each GEP's use of the trunc with the wider Src.
  SmallVector<GetElementPtrInst *, 4> GEPs;
  for (Use &U : Tr.uses())
    GEPs.push_back(cast<GetElementPtrInst>(U.getUser()));
  for (GetElementPtrInst *GEP : GEPs)
    GEP->replaceUsesOfWith(&Tr, Src);
  Tr.eraseFromParent();
  return true;
}

bool tryFoldTruncOfExt(TruncInst &Tr) {
  auto Ext = getExtOperandInfo(Tr.getOperand(0));
  if (!Ext)
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);

  Value *Replacement = nullptr;
  IRBuilder<> B(&Tr);

  if (TargetWidth == Ext->NarrowWidth) {
    // trunc(ext(a:N→W), N) = a
    Replacement = Ext->NarrowValue;
  } else if (TargetWidth < Ext->NarrowWidth) {
    // trunc(ext(a:N→W), M) where M < N = trunc(a, M)
    if (auto *C = dyn_cast<Constant>(Ext->NarrowValue)) {
      Replacement = convertConstantToNarrow(*C, TargetWidth);
    } else {
      Value *NewTr =
          B.CreateTrunc(Ext->NarrowValue, Tr.getType(), Tr.getName());
      if (auto *NewTrI = dyn_cast<Instruction>(NewTr))
        NewTrI->setDebugLoc(Tr.getDebugLoc());
      Replacement = NewTr;
    }
  } else if (TargetWidth < Ext->WideWidth) {
    // trunc(ext(a:N→W), M) where N < M < W = re-ext(a:N→M) with the same kind
    if (auto *C = dyn_cast<Constant>(Ext->NarrowValue)) {
      Replacement = Ext->Kind == ExtKind::ZExt
                        ? ConstantExpr::getCast(Instruction::ZExt, C, Tr.getType())
                        : ConstantExpr::getCast(Instruction::SExt, C, Tr.getType());
    } else {
      Value *NewExt;
      if (Ext->Kind == ExtKind::ZExt)
        NewExt = B.CreateZExt(Ext->NarrowValue, Tr.getType(), Tr.getName());
      else
        NewExt = B.CreateSExt(Ext->NarrowValue, Tr.getType(), Tr.getName());
      if (auto *NewExtI = dyn_cast<Instruction>(NewExt))
        NewExtI->setDebugLoc(Tr.getDebugLoc());
      Replacement = NewExt;
    }
  } else {
    return false; // TargetWidth >= WideWidth: not a valid narrowing trunc
  }

  Tr.replaceAllUsesWith(Replacement);
  Tr.eraseFromParent();

  if (Ext->Producer->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(Ext->Producer);

  return true;
}

// trunc(and(x, mask), N) → trunc(x, N) when mask has all N low bits set.
// The AND cannot affect the bits that survive the truncation, so it is dead.
bool tryFoldTruncOfAndMask(TruncInst &Tr) {
  auto *BO = dyn_cast<BinaryOperator>(Tr.getOperand(0));
  if (!BO || BO->getOpcode() != Instruction::And || !BO->hasOneUse())
    return false;
  if (!isIntegerValue(&Tr) || !isIntegerValue(BO))
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned SourceWidth = getValueWidth(BO);
  if (TargetWidth >= SourceWidth)
    return false;
  APInt FullMask = APInt::getLowBitsSet(SourceWidth, TargetWidth);

  // Check if either operand of the AND is a constant that covers all
  // TargetWidth low bits (i.e., mask & FullMask == FullMask).
  Value *Other = nullptr;
  for (unsigned I = 0; I < 2; ++I) {
    if (auto *C = getScalarOrSplatConstantInt(BO->getOperand(I))) {
      if ((C->getValue() & FullMask) == FullMask) {
        Other = BO->getOperand(1 - I);
        break;
      }
    }
  }
  if (!Other)
    return false;

  IRBuilder<> B(&Tr);
  Value *NewTr = B.CreateTrunc(Other, Tr.getType(), Tr.getName());
  if (auto *I = dyn_cast<Instruction>(NewTr))
    I->setDebugLoc(Tr.getDebugLoc());
  Tr.replaceAllUsesWith(NewTr);
  Tr.eraseFromParent();
  if (BO->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(BO);
  return true;
}

// Fold trunc(trunc(a:W→M), N) → trunc(a:W→N) when N < M < W.
// Adjacent truncs of the same underlying value can always be merged.
bool tryFoldTruncOfTrunc(TruncInst &Tr) {
  auto *Inner = dyn_cast<TruncInst>(Tr.getOperand(0));
  if (!Inner || !isIntegerValue(&Tr) || !isIntegerValue(Inner))
    return false;
  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned InnerWidth = getValueWidth(Inner);
  unsigned SourceWidth = getValueWidth(Inner->getOperand(0));
  if (TargetWidth >= InnerWidth || InnerWidth >= SourceWidth)
    return false;
  IRBuilder<> B(&Tr);
  // CreateTrunc may fold to a constant if the source is a ConstantInt.
  Value *NewTrV = B.CreateTrunc(Inner->getOperand(0), Tr.getType(), Tr.getName());
  if (auto *NewTr = dyn_cast<Instruction>(NewTrV))
    NewTr->setDebugLoc(Tr.getDebugLoc());
  Tr.replaceAllUsesWith(NewTrV);
  Tr.eraseFromParent();
  if (Inner->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(Inner);
  return true;
}

// Fold: trunc(lshr(X, K)) to i1  →  icmp ne (and X, 1<<K), 0
//
// When the target width is 1, truncating a value extracts exactly bit 0 of
// that value.  After lshr(X, K), bit 0 is original bit K of X.  We can
// express this without a trunc using a bitmask test:
//   bit K of X ≠ 0   ⟺   (X & (1 << K)) ≠ 0
//
// Requirements:
//  - TargetWidth is 1 (we're producing an i1)
//  - Operand is lshr with a constant shift K satisfying 0 < K < SrcWidth
//  - The single-use condition for lshr is NOT required (we only mask X)
bool tryFoldTruncToI1ViaLShrAndMask(TruncInst &Tr) {
  if (!isIntegerValue(&Tr))
    return false;
  if (getValueWidth(&Tr) != 1)
    return false;

  auto *LShr = dyn_cast<BinaryOperator>(Tr.getOperand(0));
  if (!LShr || LShr->getOpcode() != Instruction::LShr)
    return false;
  if (!isIntegerValue(LShr))
    return false;

  auto *ShiftC = dyn_cast<ConstantInt>(LShr->getOperand(1));
  if (!ShiftC)
    return false;

  unsigned SrcWidth = getValueWidth(LShr);
  uint64_t K = ShiftC->getZExtValue();
  // K must be in [1, SrcWidth-1]: K=0 is identity (no shift to undo),
  // K>=SrcWidth gives poison in LLVM IR.
  if (K == 0 || K >= SrcWidth)
    return false;

  // Build: and X, (1 << K); icmp ne ..., 0
  // CreateAnd/CreateICmpNE may fold to a constant if X is a ConstantInt.
  Value *X = LShr->getOperand(0);
  IRBuilder<> B(&Tr);
  APInt BitMask = APInt::getOneBitSet(SrcWidth, (unsigned)K);
  Value *AndV = B.CreateAnd(X, ConstantInt::get(LShr->getType(), BitMask),
                             Twine("bit") + Twine((unsigned)K));
  if (auto *AndInst = dyn_cast<Instruction>(AndV))
    AndInst->setDebugLoc(Tr.getDebugLoc());
  Value *CmpV = B.CreateICmpNE(AndV, ConstantInt::get(LShr->getType(), 0),
                                Tr.getName());
  if (auto *Cmp = dyn_cast<Instruction>(CmpV))
    Cmp->setDebugLoc(Tr.getDebugLoc());

  Tr.replaceAllUsesWith(CmpV);
  Tr.eraseFromParent();
  if (LShr->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(LShr);
  return true;
}

// Fold: trunc iN X to i1 when X is zero-bounded at 1 bit  →  icmp ne iN X, 0
//
// When X is provably in [0, 2) (via !range metadata or other structural
// analysis), truncating to i1 extracts bit 0 which is X itself.
// Replacing with icmp ne eliminates the trunc.
bool tryFoldTruncToI1WhenSrcIsZeroBounded(TruncInst &Tr) {
  if (!isIntegerValue(&Tr))
    return false;
  if (getValueWidth(&Tr) != 1)
    return false;

  Value *Src = Tr.getOperand(0);
  // nuw case is already handled by tryFoldTruncNuwToI1; only run this check
  // for instructions whose source is zero-bounded via other evidence.
  if (Tr.hasNoUnsignedWrap())
    return false;
  if (!isZeroBoundedAtWidth(Src, 1))
    return false;

  IRBuilder<> B(&Tr);
  // CreateICmpNE may fold to a ConstantInt when Src is a ConstantInt.
  Value *CmpV = B.CreateICmpNE(Src,
                               getSameShapeIntegerConstant(Src->getType(),
                                                           APInt(getValueWidth(Src), 0)),
                               Tr.getName());
  if (auto *Cmp = dyn_cast<Instruction>(CmpV))
    Cmp->setDebugLoc(Tr.getDebugLoc());
  Tr.replaceAllUsesWith(CmpV);
  Tr.eraseFromParent();
  return true;
}

// Fold: trunc nuw iN X to i1  →  icmp ne iN X, 0
//
// The `nuw` flag on a trunc means the truncated-away bits are all zero.
// For a target width of 1, this means X ∈ {0, 1}.  In that case:
//   trunc nuw iN X to i1  is equivalent to  icmp ne iN X, 0
// Both yield 0 when X=0 and 1 when X=1.  Replacing eliminates the trunc.
bool tryFoldTruncNuwToI1(TruncInst &Tr) {
  if (!isIntegerValue(&Tr))
    return false;
  if (getValueWidth(&Tr) != 1)
    return false;
  if (!Tr.hasNoUnsignedWrap())
    return false;

  Value *Src = Tr.getOperand(0);
  IRBuilder<> B(&Tr);
  // CreateICmpNE may fold to a ConstantInt when Src is a ConstantInt.
  Value *CmpV = B.CreateICmpNE(Src,
                               getSameShapeIntegerConstant(Src->getType(),
                                                           APInt(getValueWidth(Src), 0)),
                               Tr.getName());
  if (auto *Cmp = dyn_cast<Instruction>(CmpV))
    Cmp->setDebugLoc(Tr.getDebugLoc());

  Tr.replaceAllUsesWith(CmpV);
  Tr.eraseFromParent();
  return true;
}

// trunc(ctpop(zext(a:N→W)), N) = ctpop(a:N)
// because zext does not add any set bits so ctpop of the zext equals ctpop
// of the original value, and ctpop(a:N) <= N which always fits in N bits.
bool tryFoldTruncOfCtpop(TruncInst &Tr) {
  auto *II = dyn_cast<IntrinsicInst>(Tr.getOperand(0));
  if (!II || !II->hasOneUse())
    return false;
  if (II->getIntrinsicID() != Intrinsic::ctpop)
    return false;
  if (!isIntegerValue(&Tr) || !isIntegerValue(II))
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);
  // The ctpop argument must be a zero-extension from exactly TargetWidth bits.
  // We could also accept any zext from <= TargetWidth bits, but that would
  // require a narrower ctpop followed by zext; keep it simple.
  auto Ext = getExtOperandInfo(II->getArgOperand(0));
  if (!Ext || Ext->Kind != ExtKind::ZExt || Ext->NarrowWidth != TargetWidth)
    return false;

  // ctpop(zext(a:N→W)) fits in N bits (result <= N), so we can compute
  // ctpop at the narrow width and return that directly.
  auto *NarrowTy = getSameShapeIntegerType(Tr.getType(), TargetWidth);
  IRBuilder<> B(&Tr);
  Function *NarrowCtpop = Intrinsic::getOrInsertDeclaration(
      II->getModule(), Intrinsic::ctpop, {NarrowTy});
  // Use Value* rather than Instruction*: IRBuilder may fold ctpop to a
  // ConstantInt when NarrowValue is a constant (e.g., from a pass-created
  // ZExtInst with a constant operand that hasn't been folded yet).
  Value *ResultV = B.CreateCall(NarrowCtpop, {Ext->NarrowValue}, Tr.getName());
  if (auto *ResultI = dyn_cast<Instruction>(ResultV))
    ResultI->setDebugLoc(Tr.getDebugLoc());
  Tr.replaceAllUsesWith(ResultV);
  Tr.eraseFromParent();
  if (II->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(II);
  return true;
}

// Returns true if V is provably zero in all bit positions >= Width.
// This is a conservative structural check: it covers direct zero-extensions,
// bitwise operations (and/or/xor) of zero-bounded operands, lshr of a
// zero-bounded value (lshr can only shift zeros in from the left), and
// constant integers whose value fits in Width bits unsigned.  It does not
// require KnownBits analysis.
bool isZeroBoundedAtWidth(Value *V, unsigned Width) {
  if (auto Ext = getExtOperandInfo(V))
    return Ext->Kind == ExtKind::ZExt && Ext->NarrowWidth <= Width;
  if (auto *C = getScalarOrSplatConstantInt(V))
    return C->getValue().isIntN(Width);
  // Check LLVM !range metadata on loads/calls.  A !range MDNode carries pairs
  // of {lo, hi} ConstantInt values; each pair represents the half-open
  // interval [lo, hi).  hi==0 is special and wraps around (means the interval
  // extends to the type's maximum value).  We accept the metadata only when
  // every interval's exclusive upper bound satisfies hi <= 2^Width.
  if (auto *I = dyn_cast<Instruction>(V)) {
    if (MDNode *RangeMD = I->getMetadata(LLVMContext::MD_range)) {
      unsigned N = RangeMD->getNumOperands();
      assert(N >= 2 && N % 2 == 0 && "malformed !range metadata");
      APInt Limit = APInt::getOneBitSet(getScalarIntegerWidth(V->getType()),
                                        Width);
      bool AllFit = true;
      for (unsigned i = 0; i < N; i += 2) {
        auto *HiC = mdconst::extract<ConstantInt>(RangeMD->getOperand(i + 1));
        APInt Hi = HiC->getValue();
        // hi==0 wraps (covers max value); hi>Limit means some values exceed Width bits.
        if (Hi.isZero() || Hi.ugt(Limit)) {
          AllFit = false;
          break;
        }
      }
      if (AllFit)
        return true;
    }
  }
  if (auto *BO = dyn_cast<BinaryOperator>(V)) {
    // lshr with a known constant shift amount k produces a result with at most
    // (SrcBits - k) significant bits, so it is bounded at Width when
    // SrcBits - k <= Width, regardless of the dividend's boundedness.
    // Without a constant shift, fall back to checking the dividend.
    if (BO->getOpcode() == Instruction::LShr) {
      if (auto *ShiftC = dyn_cast<ConstantInt>(BO->getOperand(1))) {
        unsigned Shift = (unsigned)ShiftC->getZExtValue();
        unsigned SrcBits = getScalarIntegerWidth(BO->getType());
        if (Shift < SrcBits && SrcBits - Shift <= Width)
          return true;
      }
      return isZeroBoundedAtWidth(BO->getOperand(0), Width);
    }
    // and can only clear bits, so it is zero-bounded if either operand is.
    if (BO->getOpcode() == Instruction::And)
      return isZeroBoundedAtWidth(BO->getOperand(0), Width) ||
             isZeroBoundedAtWidth(BO->getOperand(1), Width);
    // or and xor can set bits from either side, so both must be zero-bounded.
    if (BO->getOpcode() == Instruction::Or ||
        BO->getOpcode() == Instruction::Xor)
      return isZeroBoundedAtWidth(BO->getOperand(0), Width) &&
             isZeroBoundedAtWidth(BO->getOperand(1), Width);
    // add nuw: sum < 2^Width when both operands are < 2^(Width-1).
    if (BO->getOpcode() == Instruction::Add && BO->hasNoUnsignedWrap()) {
      if (Width == 0) return false;
      return isZeroBoundedAtWidth(BO->getOperand(0), Width - 1) &&
             isZeroBoundedAtWidth(BO->getOperand(1), Width - 1);
    }
    // sub nuw: result <= LHS (no borrow), so bounded if LHS is bounded.
    if (BO->getOpcode() == Instruction::Sub && BO->hasNoUnsignedWrap())
      return isZeroBoundedAtWidth(BO->getOperand(0), Width);
    // mul nuw: product of W0-bit and W1-bit values fits in W0+W1 bits.
    // Use getStructuralNarrowWidth to determine the widths.
    if (BO->getOpcode() == Instruction::Mul && BO->hasNoUnsignedWrap()) {
      unsigned W0 = getStructuralNarrowWidth(BO->getOperand(0));
      unsigned W1 = getStructuralNarrowWidth(BO->getOperand(1));
      return W0 != 0 && W1 != 0 && W0 + W1 <= Width;
    }
    // udiv result <= dividend, so bounded if dividend is bounded.
    if (BO->getOpcode() == Instruction::UDiv)
      return isZeroBoundedAtWidth(BO->getOperand(0), Width);
    // urem result < divisor and <= dividend, so bounded if either is bounded.
    if (BO->getOpcode() == Instruction::URem)
      return isZeroBoundedAtWidth(BO->getOperand(0), Width) ||
             isZeroBoundedAtWidth(BO->getOperand(1), Width);
  }
  // umin result <= both operands; bounded if either operand is bounded.
  // umax result = one of the operands; bounded if both operands are bounded.
  if (auto *II = dyn_cast<IntrinsicInst>(V)) {
    if (II->getIntrinsicID() == Intrinsic::umin)
      return isZeroBoundedAtWidth(II->getArgOperand(0), Width) ||
             isZeroBoundedAtWidth(II->getArgOperand(1), Width);
    if (II->getIntrinsicID() == Intrinsic::umax)
      return isZeroBoundedAtWidth(II->getArgOperand(0), Width) &&
             isZeroBoundedAtWidth(II->getArgOperand(1), Width);
    // ctlz/cttz(iW, is_zero_poison): result is at most W (if false) or W-1 (if true).
    // Check whether that maximum fits in Width bits unsigned.
    if (II->getIntrinsicID() == Intrinsic::ctlz ||
        II->getIntrinsicID() == Intrinsic::cttz) {
      unsigned ArgBits = getScalarIntegerWidth(II->getArgOperand(0)->getType());
      auto *PoisonFlag = dyn_cast<ConstantInt>(II->getArgOperand(1));
      // With is_zero_poison=true, result ≤ ArgBits-1; otherwise result ≤ ArgBits.
      unsigned MaxVal = (PoisonFlag && PoisonFlag->isOne()) ? ArgBits - 1 : ArgBits;
      return APInt(64, MaxVal).isIntN(Width);
    }
  }
  return false;
}

// Returns true when V's value is guaranteed to fit in a Width-bit signed
// integer, i.e. trunc(V, Width) sign-extends back to V's original value.
// This is the signed analogue of isZeroBoundedAtWidth.
bool isSextBoundedAtWidth(Value *V, unsigned Width) {
  if (auto Ext = getExtOperandInfo(V))
    return Ext->Kind == ExtKind::SExt && Ext->NarrowWidth <= Width;
  if (auto *C = getScalarOrSplatConstantInt(V))
    return C->getValue().isSignedIntN(Width);
  return false;
}

/// Handle the SROA i128 construct/destruct pattern:
///   %mask   = and i128 %hi_src, HIGH_MASK   (HIGH_MASK has bits only in positions >= TargetWidth)
///   %lo_ext = zext iN %lo to i128
///   %combined = or i128 %mask, %lo_ext      (has exactly 2 uses: Tr and a shift)
///   Tr      = trunc i128 %combined to iN    ← this trunc, replaced with %lo
///   %shift  = lshr i128 %combined, N
///   %hi_result = trunc i128 %shift to iN    (kept; %shift now reads %mask directly)
///
/// Handle the SROA high-half extract pattern:
///   hi_ext  = zext iN %hi to i2N
///   hi_shl  = shl i2N hi_ext, N
///   lo_part = <value zero-bounded at N bits>
///   combined = or i2N lo_part, hi_shl
///   mask    = and i2N combined, HIGH_MASK    (HIGH_MASK has zeros in 0..N-1)
///   shift   = lshr i2N mask, N
///   Tr      = trunc i2N shift to iN          ← replaced by %hi
///
/// Semantics: and(or(lo, shl(zext(hi), N)), HIGH_MASK) = shl(zext(hi), N)
/// because lo is zero in bits N+, and HIGH_MASK kills bits 0..N-1.
/// Then lshr(shl(zext(hi), N), N) = zext(hi), and trunc(zext(hi), N) = hi.
bool tryShrinkHighHalfSROA(TruncInst &Tr) {
  if (!isa<IntegerType>(Tr.getSrcTy()) || !isa<IntegerType>(Tr.getType()))
    return false;
  if (!isIntegerValue(&Tr))
    return false;
  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned SrcWidth = Tr.getSrcTy()->getIntegerBitWidth();
  if (SrcWidth != 2 * TargetWidth)
    return false;

  // Tr feeds from lshr by TargetWidth with one use (Tr itself).
  auto *LShr = dyn_cast<BinaryOperator>(Tr.getOperand(0));
  if (!LShr || LShr->getOpcode() != Instruction::LShr || !LShr->hasOneUse())
    return false;
  auto *ShiftAmt = dyn_cast<ConstantInt>(LShr->getOperand(1));
  if (!ShiftAmt || ShiftAmt->getZExtValue() != TargetWidth)
    return false;

  // lshr's source is and(V, HIGH_MASK) where HIGH_MASK has zeros in bits 0..N-1
  // and ones in bits N..2N-1.  Both conditions are required: the low zeros
  // discard the low half, and the high ones preserve all of the high half.
  // Checking only the low zeros is not enough: a partial mask like 0x0F00
  // would lose the top 4 bits of the high half, making the result hi & 0x0F
  // rather than hi.
  auto *MaskBO = dyn_cast<BinaryOperator>(LShr->getOperand(0));
  if (!MaskBO || MaskBO->getOpcode() != Instruction::And)
    return false;
  Value *V = nullptr;
  for (unsigned Idx = 0; Idx < 2; ++Idx) {
    if (auto *C = dyn_cast<ConstantInt>(MaskBO->getOperand(Idx))) {
      APInt MaskVal = C->getValue();
      // Low N bits must be zero.
      if (!MaskVal.trunc(TargetWidth).isZero())
        continue;
      // High N bits must be all ones — ensures the full hi half is kept.
      if (!MaskVal.lshr(TargetWidth).trunc(TargetWidth).isAllOnes())
        continue;
      V = MaskBO->getOperand(1 - Idx);
      break;
    }
  }
  if (!V)
    return false;

  // V = or i2N LowPart, HiShl   (or HiShl, LowPart)
  auto *OrBO = dyn_cast<BinaryOperator>(V);
  if (!OrBO || OrBO->getOpcode() != Instruction::Or)
    return false;

  // Find which or-operand is shl(zext(hi), N) and which is the low part.
  Value *HiVal = nullptr;
  for (unsigned Idx = 0; Idx < 2; ++Idx) {
    auto *ShlBO = dyn_cast<BinaryOperator>(OrBO->getOperand(Idx));
    if (!ShlBO || ShlBO->getOpcode() != Instruction::Shl || !ShlBO->hasOneUse())
      continue;
    auto *ShlAmt = dyn_cast<ConstantInt>(ShlBO->getOperand(1));
    if (!ShlAmt || ShlAmt->getZExtValue() != TargetWidth)
      continue;
    auto *ZE = dyn_cast<ZExtInst>(ShlBO->getOperand(0));
    if (!ZE || ZE->getSrcTy()->getIntegerBitWidth() != TargetWidth ||
        !ZE->hasOneUse())
      continue;
    // Verify the other or-operand contributes nothing to bits >= TargetWidth.
    Value *LowPart = OrBO->getOperand(1 - Idx);
    if (!isZeroBoundedAtWidth(LowPart, TargetWidth))
      continue;
    HiVal = ZE->getOperand(0);
    break;
  }
  if (!HiVal)
    return false;

  // Replace Tr with HiVal and clean up dead instructions.
  Tr.replaceAllUsesWith(HiVal);
  Tr.eraseFromParent();
  if (LShr->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(LShr);
  return true;
}

/// Semantics: trunc(or(and(X, HIGH_MASK), zext(lo)), N) == lo  because the and
/// masks out all low bits, so only the zext contributes to positions 0..N-1.
/// Also: trunc(lshr(or(%mask, %lo_ext), N), N) == trunc(lshr(%mask, N), N)
/// because %lo_ext occupies only bits 0..N-1 which shift out entirely.
bool tryShrinkSROAI128Destruct(TruncInst &Tr) {
  if (!isa<IntegerType>(Tr.getSrcTy()) || !isa<IntegerType>(Tr.getType()))
    return false;
  if (!isIntegerValue(&Tr))
    return false;
  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned SourceWidth = Tr.getSrcTy()->getIntegerBitWidth();
  if (TargetWidth >= SourceWidth)
    return false;

  auto *OR_BO = dyn_cast<BinaryOperator>(Tr.getOperand(0));
  if (!OR_BO || OR_BO->getOpcode() != Instruction::Or)
    return false;

  // OR_BO may have multiple trunc uses (all replaced with LoVal) plus exactly
  // one lshr by TargetWidth use (redirected to MaskVal). Collect them.
  // Other users (icmp, and, etc.) are left unchanged.
  SmallVector<BinaryOperator *, 4> HighShifts;
  SmallVector<TruncInst *, 4> LowTruncs;
  for (User *U : OR_BO->users()) {
    if (auto *T = dyn_cast<TruncInst>(U)) {
      if (getValueWidth(T) != TargetWidth)
        continue; // different-width trunc: leave alone
      LowTruncs.push_back(T);
    } else if (auto *LShr = dyn_cast<BinaryOperator>(U)) {
      if (LShr->getOpcode() != Instruction::LShr)
        continue; // other binop use: leave alone
      auto *ShiftAmt = dyn_cast<ConstantInt>(LShr->getOperand(1));
      if (!ShiftAmt || ShiftAmt->getZExtValue() != TargetWidth)
        continue;
      if (!LShr->hasOneUse())
        continue;
      auto *HighTrunc = dyn_cast<TruncInst>(*LShr->user_begin());
      if (!HighTrunc || getValueWidth(HighTrunc) != TargetWidth)
        continue;
      HighShifts.push_back(LShr);
    }
    // Other users (icmp, and, etc.) are left unchanged.
  }
  if (HighShifts.empty() && LowTruncs.empty())
    return false;

  // Identify: which operand of OR is zext(lo) and which is the HIGH_MASK side?
  Value *LoVal = nullptr;
  Value *MaskVal = nullptr;

  for (unsigned Idx = 0; Idx < 2; ++Idx) {
    Value *Op = OR_BO->getOperand(Idx);
    if (auto *ZE = dyn_cast<ZExtInst>(Op)) {
      if (ZE->getSrcTy()->getIntegerBitWidth() == TargetWidth &&
          ZE->getDestTy() == OR_BO->getType()) {
        LoVal = ZE->getOperand(0);
        MaskVal = OR_BO->getOperand(1 - Idx);
        break;
      }
    }
  }
  if (!LoVal || !MaskVal)
    return false;

  // Verify MaskVal is an `and X, C` where C has all zeros in the low TargetWidth bits.
  // This guarantees low bits of OR come entirely from the zext(lo).
  auto *MaskBO = dyn_cast<BinaryOperator>(MaskVal);
  if (!MaskBO || MaskBO->getOpcode() != Instruction::And)
    return false;
  bool FoundHighMask = false;
  for (unsigned Idx = 0; Idx < 2; ++Idx) {
    if (auto *C = dyn_cast<ConstantInt>(MaskBO->getOperand(Idx))) {
      if (C->getValue().trunc(TargetWidth).isZero()) {
        FoundHighMask = true;
        break;
      }
    }
  }
  if (!FoundHighMask)
    return false;

  // Transform:
  // 1. Replace all low truncs with LoVal — the low TargetWidth bits of OR
  //    come only from zext(lo).
  // 2. Redirect HighShift from OR_BO to MaskVal — the low bits contributed
  //    by zext(lo) are shifted out entirely by TargetWidth.
  for (TruncInst *LT : LowTruncs) {
    LT->replaceAllUsesWith(LoVal);
    LT->eraseFromParent();
  }
  for (BinaryOperator *HighShift : HighShifts)
    HighShift->replaceUsesOfWith(OR_BO, MaskVal);
  if (OR_BO->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(OR_BO);
  return true;
}

bool tryShrinkTruncOfLowBitsBinOp(TruncInst &Tr) {
  auto *BO = dyn_cast<BinaryOperator>(Tr.getOperand(0));
  if (!BO)
    return false;
  if (!isIntegerValue(&Tr) || !isIntegerValue(BO) ||
      !isIntegerValue(BO->getOperand(0)) || !isIntegerValue(BO->getOperand(1)))
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);

  // Allow multi-use BOs when all uses are truncs to the same target width.
  // In that case we narrow the BO once and replace all trunc uses together.
  SmallVector<TruncInst *, 4> AllTruncUses;
  if (!BO->hasOneUse()) {
    for (User *U : BO->users()) {
      auto *T = dyn_cast<TruncInst>(U);
      if (!T || getValueWidth(T) != TargetWidth)
        return false;
      AllTruncUses.push_back(T);
    }
  }
  unsigned SourceWidth = getValueWidth(BO);
  if (TargetWidth >= SourceWidth)
    return false;

  // shl with a constant shift amount less than TargetWidth is low-bit
  // preserving: (a << k) mod 2^N = ((a mod 2^N) << k) mod 2^N. Requiring
  // the amount to be less than TargetWidth keeps the narrow shl well-defined.
  //
  // lshr by constant k < TargetWidth is safe when the LHS is a
  // zero-extension from at most TargetWidth bits. That guarantees the bits
  // above position TargetWidth+k-1 are zero, so the logical shift cannot
  // bring nonzero high bits into the truncated region.
  if (!isTruncRootedLowBitsPreservingOpcode(BO->getOpcode())) {
    if (BO->getOpcode() == Instruction::Shl) {
      auto *AmtC = getScalarOrSplatConstantInt(BO->getOperand(1));
      if (!AmtC || AmtC->getValue().uge(TargetWidth))
        return false;
    } else if (BO->getOpcode() == Instruction::LShr) {
      auto *AmtC = getScalarOrSplatConstantInt(BO->getOperand(1));
      if (!AmtC)
        return false;
      // lshr of a zero-bounded value by >= TargetWidth bits shifts out all
      // the value bits, producing 0.
      if (AmtC->getValue().uge(TargetWidth)) {
        if (!isZeroBoundedAtWidth(BO->getOperand(0), TargetWidth))
          return false;
        Value *Zero = ConstantInt::get(Tr.getType(), 0);
        Tr.replaceAllUsesWith(Zero);
        Tr.eraseFromParent();
        if (BO->use_empty())
          RecursivelyDeleteTriviallyDeadInstructions(BO);
        return true;
      }
      // Special case: trunc(lshr*(sext(a:N→W), k_total), N) = ashr(a, k_total)
      // where lshr* denotes a chain of one or more lshrs with constant amounts
      // summing to k_total < N.  At every bit position p of the result:
      //   p < N-k_total : bit p+k_total of a (shifted bits)
      //   p >= N-k_total: sign bit of a (sext fills above N; ashr sign-extends)
      // Walk the lshr chain (each must have a single use) to find the sext.
      {
        uint64_t TotalShift = AmtC->getValue().getZExtValue();
        Value *LshrChainBase = BO->getOperand(0);
        SmallVector<BinaryOperator *, 4> ChainLinks; // inner lshrs, not BO
        while (auto *Inner = dyn_cast<BinaryOperator>(LshrChainBase)) {
          if (Inner->getOpcode() != Instruction::LShr || !Inner->hasOneUse())
            break;
          auto *InnerAmt = getScalarOrSplatConstantInt(Inner->getOperand(1));
          if (!InnerAmt)
            break;
          TotalShift += InnerAmt->getValue().getZExtValue();
          if (TotalShift >= TargetWidth)
            break; // overflow: can't produce valid ashr
          ChainLinks.push_back(Inner);
          LshrChainBase = Inner->getOperand(0);
        }
        auto LHSInfo = getExtOperandInfo(LshrChainBase);
        if (LHSInfo && LHSInfo->Kind == ExtKind::SExt &&
            LHSInfo->NarrowWidth == TargetWidth &&
            TotalShift < TargetWidth) {
          IRBuilder<> B(&Tr);
          auto *NarrowTy = getSameShapeIntegerType(Tr.getType(), TargetWidth);
          Constant *NarrowAmt =
              getSameShapeIntegerConstant(NarrowTy, APInt(TargetWidth, TotalShift));
          // CreateAShr may fold to a non-Instruction when NarrowValue is a constant.
          Value *NewAShrV =
              B.CreateAShr(LHSInfo->NarrowValue, NarrowAmt, Tr.getName());
          if (auto *NewAShr = dyn_cast<Instruction>(NewAShrV))
            NewAShr->setDebugLoc(Tr.getDebugLoc());
          Tr.replaceAllUsesWith(NewAShrV);
          Tr.eraseFromParent();
          // Clean up the lshr chain bottom-up.
          if (BO->use_empty())
            RecursivelyDeleteTriviallyDeadInstructions(BO);
          return true;
        }
      }
      if (!isZeroBoundedAtWidth(BO->getOperand(0), TargetWidth))
        return false;
    } else if (BO->getOpcode() == Instruction::AShr) {
      // ashr by constant k < TargetWidth is safe when the LHS is a
      // sign-extension from at most TargetWidth bits. The upper bits are all
      // copies of the sign bit, so the arithmetic shift cannot pull an
      // incorrect sign bit into the truncated region.
      auto *AmtC = getScalarOrSplatConstantInt(BO->getOperand(1));
      if (!AmtC || AmtC->getValue().uge(TargetWidth))
        return false;
      // Walk a chain of single-use constant ashrs to find the sext root.
      // trunc(ashr(ashr(sext(a:N→W), k1), k2), N) = ashr(a, k1+k2).
      {
        uint64_t TotalShift = AmtC->getValue().getZExtValue();
        Value *AshrChainBase = BO->getOperand(0);
        SmallVector<BinaryOperator *, 4> ChainLinks;
        while (auto *Inner = dyn_cast<BinaryOperator>(AshrChainBase)) {
          if (Inner->getOpcode() != Instruction::AShr || !Inner->hasOneUse())
            break;
          auto *InnerAmt = getScalarOrSplatConstantInt(Inner->getOperand(1));
          if (!InnerAmt)
            break;
          TotalShift += InnerAmt->getValue().getZExtValue();
          if (TotalShift >= TargetWidth)
            break;
          ChainLinks.push_back(Inner);
          AshrChainBase = Inner->getOperand(0);
        }
        auto LHSInfo = getExtOperandInfo(AshrChainBase);
        if (LHSInfo && LHSInfo->NarrowWidth == TargetWidth &&
            TotalShift < TargetWidth) {
          IRBuilder<> B(&Tr);
          auto *NarrowTy2 = getSameShapeIntegerType(Tr.getType(), TargetWidth);
          Constant *NarrowAmt =
              getSameShapeIntegerConstant(NarrowTy2, APInt(TargetWidth, TotalShift));
          // CreateAShr/CreateLShr may fold to a non-Instruction when NarrowValue
          // is a constant; use Value* and dyn_cast only for setDebugLoc.
          Value *NewShiftV;
          if (LHSInfo->Kind == ExtKind::SExt) {
            // trunc(ashr*(sext(a:N→W), k_total), N) = ashr(a, k_total)
            NewShiftV =
                B.CreateAShr(LHSInfo->NarrowValue, NarrowAmt, Tr.getName());
          } else {
            // trunc(ashr*(zext(a:N→W), k_total), N) = lshr(a, k_total)
            // because zext fills upper bits with 0, making ashr == lshr.
            NewShiftV =
                B.CreateLShr(LHSInfo->NarrowValue, NarrowAmt, Tr.getName());
          }
          if (auto *NewShift = dyn_cast<Instruction>(NewShiftV))
            NewShift->setDebugLoc(Tr.getDebugLoc());
          Tr.replaceAllUsesWith(NewShiftV);
          Tr.eraseFromParent();
          if (BO->use_empty())
            RecursivelyDeleteTriviallyDeadInstructions(BO);
          return true;
        }
      }
      // Single-ashr case (no chain). Handle both SExt and ZExt roots.
      auto LHSInfo = getExtOperandInfo(BO->getOperand(0));
      if (!LHSInfo || LHSInfo->NarrowWidth > TargetWidth)
        return false;
      uint64_t ShiftAmt = AmtC->getValue().getZExtValue();
      if (ShiftAmt < TargetWidth && LHSInfo->Kind == ExtKind::ZExt &&
          LHSInfo->NarrowWidth == TargetWidth) {
        // trunc(ashr(zext(a:N→W), k), N) = lshr(a, k)
        IRBuilder<> B(&Tr);
        auto *NarrowTy3 = getSameShapeIntegerType(Tr.getType(), TargetWidth);
        Constant *NarrowAmt =
            getSameShapeIntegerConstant(NarrowTy3, APInt(TargetWidth, ShiftAmt));
        // CreateLShr may fold to a non-Instruction when NarrowValue is a constant.
        Value *NewLShrV =
            B.CreateLShr(LHSInfo->NarrowValue, NarrowAmt, Tr.getName());
        if (auto *NewLShr = dyn_cast<Instruction>(NewLShrV))
          NewLShr->setDebugLoc(Tr.getDebugLoc());
        Tr.replaceAllUsesWith(NewLShrV);
        Tr.eraseFromParent();
        if (BO->use_empty())
          RecursivelyDeleteTriviallyDeadInstructions(BO);
        return true;
      }
      if (LHSInfo->Kind != ExtKind::SExt)
        return false;
    } else {
      return false;
    }
  }

  SmallPtrSet<Value *, 8> AddedValues;
  SmallPtrSet<Instruction *, 8> RemovedInstructions;
  SmallPtrSet<Value *, 8> VisitedValues;
  if (!collectTruncRootedValueCost(BO->getOperand(0), TargetWidth, AddedValues,
                                   RemovedInstructions, VisitedValues) ||
      !collectTruncRootedValueCost(BO->getOperand(1), TargetWidth, AddedValues,
                                   RemovedInstructions, VisitedValues))
    return false;

  unsigned AddedInstructionCost = AddedValues.size();
  unsigned RemovedInstructionCost = 1 + RemovedInstructions.size();

  // Rebuild the add at the narrow width only when removable instructions
  // around the region pay for any recursive narrowing we introduce.
  if (AddedInstructionCost > RemovedInstructionCost)
    return false;

  // For multi-use case insert right after the BO (dominates all its users).
  // For single-use case insert before Tr (the only user) as before.
  Instruction *InsertPt =
      AllTruncUses.empty() ? &Tr : BO->getNextNode();

  DenseMap<Value *, Value *> Cache;
  Value *LHS =
      materializeTruncRootedValueAtWidth(BO->getOperand(0), TargetWidth,
                                         InsertPt, &Cache);
  Value *RHS =
      materializeTruncRootedValueAtWidth(BO->getOperand(1), TargetWidth,
                                         InsertPt, &Cache);
  if (!LHS || !RHS)
    return false;

  // Check for identity operations before emitting the narrow binop.
  auto *LHSC = dyn_cast<ConstantInt>(LHS);
  auto *RHSC = dyn_cast<ConstantInt>(RHS);
  Value *FoldedResult = nullptr;
  unsigned Opc = BO->getOpcode();
  if (Opc == Instruction::And) {
    if (RHSC && RHSC->getValue().isAllOnes()) FoldedResult = LHS;
    else if (LHSC && LHSC->getValue().isAllOnes()) FoldedResult = RHS;
    else if ((LHSC && LHSC->isZero()) || (RHSC && RHSC->isZero()))
      FoldedResult = ConstantInt::get(LHS->getType(), 0);
  } else if (Opc == Instruction::Or) {
    if (RHSC && RHSC->isZero()) FoldedResult = LHS;
    else if (LHSC && LHSC->isZero()) FoldedResult = RHS;
    else if ((LHSC && LHSC->getValue().isAllOnes()) ||
             (RHSC && RHSC->getValue().isAllOnes()))
      FoldedResult = ConstantInt::get(LHS->getType(), APInt::getAllOnes(TargetWidth));
  } else if (Opc == Instruction::Xor) {
    if (RHSC && RHSC->isZero()) FoldedResult = LHS;
    else if (LHSC && LHSC->isZero()) FoldedResult = RHS;
  } else if (Opc == Instruction::Add) {
    if (RHSC && RHSC->isZero()) FoldedResult = LHS;
    else if (LHSC && LHSC->isZero()) FoldedResult = RHS;
  } else if (Opc == Instruction::Sub) {
    if (RHSC && RHSC->isZero()) FoldedResult = LHS;
  } else if (Opc == Instruction::Mul) {
    if (RHSC && RHSC->isOne()) FoldedResult = LHS;
    else if (LHSC && LHSC->isOne()) FoldedResult = RHS;
    else if ((LHSC && LHSC->isZero()) || (RHSC && RHSC->isZero()))
      FoldedResult = ConstantInt::get(LHS->getType(), 0);
  } else if (Opc == Instruction::Shl || Opc == Instruction::LShr ||
             Opc == Instruction::AShr) {
    if (RHSC && RHSC->isZero()) FoldedResult = LHS;
  }

  Value *Result;
  if (FoldedResult) {
    Result = FoldedResult;
  } else {
    IRBuilder<> B(InsertPt);
    Result = B.CreateBinOp(
        (Instruction::BinaryOps)BO->getOpcode(), LHS, RHS, Tr.getName());
    if (auto *NewBO = dyn_cast<Instruction>(Result))
      NewBO->setDebugLoc(Tr.getDebugLoc());
  }

  // Replace all trunc uses (either just Tr, or all collected multi-use truncs).
  if (AllTruncUses.empty()) {
    Tr.replaceAllUsesWith(Result);
    Tr.eraseFromParent();
  } else {
    for (TruncInst *T : AllTruncUses) {
      T->replaceAllUsesWith(Result);
      T->eraseFromParent();
    }
  }

  if (BO->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(BO);

  return true;
}

bool isTruncRootedLowBitsPreservingOpcode(unsigned Opcode) {
  switch (Opcode) {
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

Value *materializeTruncRootedValueAtWidth(Value *V, unsigned TargetWidth,
                                          Instruction *InsertBefore,
                                          DenseMap<Value *, Value *> *Cache) {
  // This helper is intentionally tiny. It rebuilds only the narrow patterns we
  // currently know how to prove under a final truncation, and otherwise lets
  // the caller give up instead of speculating about general arithmetic.
  if (!isIntegerValue(V))
    return nullptr;

  if (Cache)
    if (Value *Cached = Cache->lookup(V))
      return Cached;

  unsigned Width = getValueWidth(V);
  auto *TargetTy = getSameShapeIntegerType(V->getType(), TargetWidth);
  if (Width == TargetWidth) {
    if (Cache)
      (*Cache)[V] = V;
    return V;
  }

  if (auto Ext = getExtOperandInfo(V)) {
    if (TargetWidth > Ext->WideWidth)
      return nullptr;
    if (TargetWidth < Ext->NarrowWidth) {
      // Transitive chain: e.g. trunc(sext(sext(a:i8→i16)→i32), i8).
      // Recurse into the extension's source to narrow it further.
      Value *Result = materializeTruncRootedValueAtWidth(Ext->NarrowValue,
                                                         TargetWidth,
                                                         InsertBefore, Cache);
      if (Cache && Result)
        (*Cache)[V] = Result;
      return Result;
    }
    IRBuilder<> B(InsertBefore);
    Value *Result = materializeAtWidth(B, *Ext, TargetWidth);
    if (Cache && Result)
      (*Cache)[V] = Result;
    return Result;
  }

  if (auto *C = dyn_cast<Constant>(V)) {
    Value *Result = convertConstantToNarrow(*C, TargetWidth);
    if (Cache)
      (*Cache)[V] = Result;
    return Result;
  }

  if (auto *BO = dyn_cast<BinaryOperator>(V)) {
    // Special case for `and X, C` where C has no bits in the low TargetWidth
    // positions: trunc(and X, C, TargetWidth) = 0, a free constant.
    if (BO->getOpcode() == Instruction::And) {
      for (unsigned Idx = 0; Idx < 2; ++Idx) {
        if (auto *C = getScalarOrSplatConstantInt(BO->getOperand(Idx))) {
          APInt LowBits = C->getValue().trunc(TargetWidth);
          if (LowBits.isZero()) {
            Value *Zero = getSameShapeIntegerConstant(TargetTy,
                                                     APInt(TargetWidth, 0));
            if (Cache)
              (*Cache)[V] = Zero;
            return Zero;
          }
        }
      }
    }
    if (!isTruncRootedLowBitsPreservingOpcode(BO->getOpcode())) {
      if (BO->getOpcode() == Instruction::Shl) {
        // shl with a constant amount < TargetWidth is also low-bit preserving.
        auto *AmtC = getScalarOrSplatConstantInt(BO->getOperand(1));
        if (!AmtC || AmtC->getValue().uge(TargetWidth))
          return nullptr;
      } else if (BO->getOpcode() == Instruction::LShr) {
        auto *AmtC = getScalarOrSplatConstantInt(BO->getOperand(1));
        if (!AmtC)
          return nullptr;
        if (AmtC->getValue().uge(TargetWidth)) {
          // lshr of a zero-bounded value by >= TargetWidth bits is 0.
          if (!isZeroBoundedAtWidth(BO->getOperand(0), TargetWidth))
            return nullptr;
          Value *Zero = getSameShapeIntegerConstant(TargetTy,
                                                   APInt(TargetWidth, 0));
          if (Cache)
            (*Cache)[V] = Zero;
          return Zero;
        }
        if (!isZeroBoundedAtWidth(BO->getOperand(0), TargetWidth))
          return nullptr;
      } else if (BO->getOpcode() == Instruction::AShr) {
        auto *AmtC = getScalarOrSplatConstantInt(BO->getOperand(1));
        if (!AmtC || AmtC->getValue().uge(TargetWidth))
          return nullptr;
        auto LHSInfo = getExtOperandInfo(BO->getOperand(0));
        if (!LHSInfo || LHSInfo->Kind != ExtKind::SExt ||
            LHSInfo->NarrowWidth > TargetWidth)
          return nullptr;
      } else if (BO->getOpcode() == Instruction::UDiv ||
                 BO->getOpcode() == Instruction::URem) {
        if (!isZeroBoundedAtWidth(BO->getOperand(0), TargetWidth) ||
            !isZeroBoundedAtWidth(BO->getOperand(1), TargetWidth))
          return nullptr;
      } else {
        return nullptr;
      }
    }
    Value *NarrowLHS =
        materializeTruncRootedValueAtWidth(BO->getOperand(0), TargetWidth,
                                           InsertBefore, Cache);
    Value *NarrowRHS =
        materializeTruncRootedValueAtWidth(BO->getOperand(1), TargetWidth,
                                           InsertBefore, Cache);
    if (!NarrowLHS || !NarrowRHS)
      return nullptr;
    // Fold identity cases before emitting the narrow instruction.
    auto AllOnes = [&](Value *V) -> bool {
      auto *C = dyn_cast<Constant>(V);
      return C && C->isAllOnesValue();
    };
    auto IsZero = [&](Value *V) -> bool {
      auto *C = dyn_cast<Constant>(V);
      return C && C->isNullValue();
    };
    auto IsOne = [&](Value *V) -> bool {
      auto *C = dyn_cast<Constant>(V);
      return C && C->isOneValue();
    };
    Value *FoldedResult = nullptr;
    unsigned Opc = BO->getOpcode();
    if (Opc == Instruction::And) {
      if (AllOnes(NarrowRHS)) FoldedResult = NarrowLHS;
      else if (AllOnes(NarrowLHS)) FoldedResult = NarrowRHS;
      else if (IsZero(NarrowLHS) || IsZero(NarrowRHS))
        FoldedResult = getSameShapeIntegerConstant(NarrowLHS->getType(),
                                                  APInt(TargetWidth, 0));
    } else if (Opc == Instruction::Or) {
      if (IsZero(NarrowRHS)) FoldedResult = NarrowLHS;
      else if (IsZero(NarrowLHS)) FoldedResult = NarrowRHS;
      else if (AllOnes(NarrowLHS) || AllOnes(NarrowRHS))
        FoldedResult = getSameShapeIntegerConstant(NarrowLHS->getType(),
                                                  APInt::getAllOnes(TargetWidth));
    } else if (Opc == Instruction::Xor) {
      if (IsZero(NarrowRHS)) FoldedResult = NarrowLHS;
      else if (IsZero(NarrowLHS)) FoldedResult = NarrowRHS;
    } else if (Opc == Instruction::Add) {
      if (IsZero(NarrowRHS)) FoldedResult = NarrowLHS;
      else if (IsZero(NarrowLHS)) FoldedResult = NarrowRHS;
    } else if (Opc == Instruction::Sub) {
      if (IsZero(NarrowRHS)) FoldedResult = NarrowLHS;
    } else if (Opc == Instruction::Mul) {
      if (IsOne(NarrowRHS)) FoldedResult = NarrowLHS;
      else if (IsOne(NarrowLHS)) FoldedResult = NarrowRHS;
      else if (IsZero(NarrowLHS) || IsZero(NarrowRHS))
        FoldedResult = ConstantInt::get(NarrowLHS->getType(), 0);
    } else if (Opc == Instruction::Shl || Opc == Instruction::LShr ||
               Opc == Instruction::AShr) {
      if (IsZero(NarrowRHS)) FoldedResult = NarrowLHS;
    }
    if (FoldedResult) {
      if (Cache)
        (*Cache)[V] = FoldedResult;
      return FoldedResult;
    }
    IRBuilder<> B(InsertBefore);
    Value *Result = B.CreateBinOp(
        (Instruction::BinaryOps)BO->getOpcode(), NarrowLHS, NarrowRHS,
        BO->getName() + ".narrow");
    if (auto *ResultI = dyn_cast<Instruction>(Result))
      ResultI->setDebugLoc(BO->getDebugLoc());
    if (Cache && Result)
      (*Cache)[V] = Result;
    return Result;
  }

  // Handle narrowable min/max/abs intrinsics.
  if (auto *II = dyn_cast<IntrinsicInst>(V)) {
    auto IID = II->getIntrinsicID();
    switch (IID) {
    case Intrinsic::umin:
    case Intrinsic::umax:
    case Intrinsic::smin:
    case Intrinsic::smax: {
      Value *NarrowA = materializeTruncRootedValueAtWidth(
          II->getArgOperand(0), TargetWidth, InsertBefore, Cache);
      Value *NarrowB = materializeTruncRootedValueAtWidth(
          II->getArgOperand(1), TargetWidth, InsertBefore, Cache);
      if (!NarrowA || !NarrowB)
        return nullptr;
      IRBuilder<> B(InsertBefore);
      Function *NarrowFn =
          Intrinsic::getOrInsertDeclaration(II->getModule(), IID, {TargetTy});
      Value *Result =
          B.CreateCall(NarrowFn, {NarrowA, NarrowB}, II->getName() + ".narrow");
      if (auto *ResultI = dyn_cast<Instruction>(Result))
        ResultI->setDebugLoc(II->getDebugLoc());
      if (Cache)
        (*Cache)[V] = Result;
      return Result;
    }
    case Intrinsic::abs: {
      Value *NarrowA = materializeTruncRootedValueAtWidth(
          II->getArgOperand(0), TargetWidth, InsertBefore, Cache);
      if (!NarrowA)
        return nullptr;
      IRBuilder<> B(InsertBefore);
      Function *NarrowFn =
          Intrinsic::getOrInsertDeclaration(II->getModule(), Intrinsic::abs, {TargetTy});
      Value *FalseC = ConstantInt::getFalse(II->getContext());
      Value *Result =
          B.CreateCall(NarrowFn, {NarrowA, FalseC}, II->getName() + ".narrow");
      if (auto *ResultI = dyn_cast<Instruction>(Result))
        ResultI->setDebugLoc(II->getDebugLoc());
      if (Cache)
        (*Cache)[V] = Result;
      return Result;
    }
    default:
      break;
    }
  }

  if (Width > TargetWidth) {
    IRBuilder<> B(InsertBefore);
    Value *Result = B.CreateTrunc(V, TargetTy);
    if (Cache && Result)
      (*Cache)[V] = Result;
    return Result;
  }

  return nullptr;
}

bool collectTruncRootedValueCost(
    Value *V, unsigned TargetWidth, SmallPtrSetImpl<Value *> &AddedValues,
    SmallPtrSetImpl<Instruction *> &RemovedInstructions,
    SmallPtrSetImpl<Value *> &Visited) {
  if (!isIntegerValue(V))
    return false;
  if (!Visited.insert(V).second)
    return true;

  unsigned Width = getValueWidth(V);
  if (Width == TargetWidth)
    return true;

  if (auto Ext = getExtOperandInfo(V)) {
    if (TargetWidth > Ext->WideWidth)
      return false;
    if (TargetWidth < Ext->NarrowWidth) {
      // Transitive chain: e.g. trunc(sext(sext(a:i8→i16)→i32), i8).
      // Recurse into the extension's source to narrow it further.
      if (!collectTruncRootedValueCost(Ext->NarrowValue, TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      if (Ext->Producer->hasOneUse())
        RemovedInstructions.insert(Ext->Producer);
      return true;
    }
    // TargetWidth in [NarrowWidth, WideWidth].
    if (TargetWidth != Ext->NarrowWidth)
      AddedValues.insert(V);
    if (Ext->Producer->hasOneUse())
      RemovedInstructions.insert(Ext->Producer);
    return true;
  }

  if (isa<Constant>(V))
    return true;

  if (auto *BO = dyn_cast<BinaryOperator>(V)) {
    if (isTruncRootedLowBitsPreservingOpcode(BO->getOpcode())) {
      // Special case for `and X, C` where C has no bits in the low TargetWidth
      // positions: trunc(and X, C, TargetWidth) = 0, a free constant.
      // This enables the SROA construct/destruct pattern:
      //   trunc(or(and(X, HIGH_MASK), zext(a)), TargetWidth) == a
      // because trunc(and(X, HIGH_MASK), TargetWidth) == 0.
      if (BO->getOpcode() == Instruction::And) {
        for (unsigned Idx = 0; Idx < 2; ++Idx) {
          if (auto *C = getScalarOrSplatConstantInt(BO->getOperand(Idx))) {
            APInt LowBits = C->getValue().trunc(TargetWidth);
            if (LowBits.isZero()) {
              // The and contributes 0 to the low TargetWidth bits.
              // No recursion needed; this is a free constant fold.
              if (BO->hasOneUse())
                RemovedInstructions.insert(BO);
              return true; // Contributes 0, no AddedValues.
            }
          }
        }
      }
      if (!collectTruncRootedValueCost(BO->getOperand(0), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited) ||
          !collectTruncRootedValueCost(BO->getOperand(1), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      AddedValues.insert(V);
      if (BO->hasOneUse())
        RemovedInstructions.insert(BO);
      return true;
    }
    // shl with a constant shift amount < TargetWidth is also low-bit
    // preserving. The constant operand is free; only recurse on the value.
    if (BO->getOpcode() == Instruction::Shl) {
      auto *AmtC = dyn_cast<ConstantInt>(BO->getOperand(1));
      if (!AmtC || AmtC->getValue().uge(TargetWidth))
        return false;
      if (!collectTruncRootedValueCost(BO->getOperand(0), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      AddedValues.insert(V);
      if (BO->hasOneUse())
        RemovedInstructions.insert(BO);
      return true;
    }
    // lshr by constant k < TargetWidth is safe when the LHS has all bits
    // above TargetWidth-1 provably zero (same condition as the
    // tryShrinkTruncOfLowBitsBinOp entry check).
    // lshr by constant k >= TargetWidth with a zero-bounded LHS gives 0,
    // which is a free constant fold (no added instruction).
    if (BO->getOpcode() == Instruction::LShr) {
      auto *AmtC = getScalarOrSplatConstantInt(BO->getOperand(1));
      if (!AmtC)
        return false;
      if (AmtC->getValue().uge(TargetWidth)) {
        // lshr of a zero-bounded value by >= TargetWidth bits yields 0.
        if (!isZeroBoundedAtWidth(BO->getOperand(0), TargetWidth))
          return false;
        if (BO->hasOneUse())
          RemovedInstructions.insert(BO);
        return true; // Result is 0 -- no AddedValues entry needed.
      }
      if (!isZeroBoundedAtWidth(BO->getOperand(0), TargetWidth))
        return false;
      if (!collectTruncRootedValueCost(BO->getOperand(0), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      AddedValues.insert(V);
      if (BO->hasOneUse())
        RemovedInstructions.insert(BO);
      return true;
    }
    // ashr by constant k < TargetWidth is safe when the LHS is a
    // sign-extension from at most TargetWidth bits.
    if (BO->getOpcode() == Instruction::AShr) {
      auto *AmtC = getScalarOrSplatConstantInt(BO->getOperand(1));
      if (!AmtC || AmtC->getValue().uge(TargetWidth))
        return false;
      auto LHSInfo = getExtOperandInfo(BO->getOperand(0));
      if (!LHSInfo || LHSInfo->Kind != ExtKind::SExt ||
          LHSInfo->NarrowWidth > TargetWidth)
        return false;
      if (!collectTruncRootedValueCost(BO->getOperand(0), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      AddedValues.insert(V);
      if (BO->hasOneUse())
        RemovedInstructions.insert(BO);
      return true;
    }
    // udiv/urem are narrowable when both operands are zero-bounded at
    // TargetWidth: udiv(a,b)<=a and urem(a,b)<b, so the result fits.
    if (BO->getOpcode() == Instruction::UDiv ||
        BO->getOpcode() == Instruction::URem) {
      if (!isZeroBoundedAtWidth(BO->getOperand(0), TargetWidth) ||
          !isZeroBoundedAtWidth(BO->getOperand(1), TargetWidth))
        return false;
      if (!collectTruncRootedValueCost(BO->getOperand(0), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited) ||
          !collectTruncRootedValueCost(BO->getOperand(1), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      AddedValues.insert(V);
      if (BO->hasOneUse())
        RemovedInstructions.insert(BO);
      return true;
    }
  }

  // Handle min/max/abs intrinsics that are narrowable.
  if (auto *II = dyn_cast<IntrinsicInst>(V)) {
    auto IID = II->getIntrinsicID();
    switch (IID) {
    case Intrinsic::umin:
    case Intrinsic::umax:
      // Narrowable when both args are zero-bounded at TargetWidth.
      if (!isZeroBoundedAtWidth(II->getArgOperand(0), TargetWidth) ||
          !isZeroBoundedAtWidth(II->getArgOperand(1), TargetWidth))
        return false;
      if (!collectTruncRootedValueCost(II->getArgOperand(0), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited) ||
          !collectTruncRootedValueCost(II->getArgOperand(1), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      AddedValues.insert(V);
      if (II->hasOneUse())
        RemovedInstructions.insert(II);
      return true;
    case Intrinsic::smin:
    case Intrinsic::smax:
      // Narrowable only when both args are sext-bounded at TargetWidth,
      // i.e. trunc(arg, TargetWidth) sign-extends back to the original value.
      // Without this check the signed comparison order can change after
      // truncation (e.g. smin.i32(129, 0) = 0 but smin.i8(trunc(129), 0) =
      // smin.i8(-127, 0) = -127).
      if (!isSextBoundedAtWidth(II->getArgOperand(0), TargetWidth) ||
          !isSextBoundedAtWidth(II->getArgOperand(1), TargetWidth))
        return false;
      if (!collectTruncRootedValueCost(II->getArgOperand(0), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited) ||
          !collectTruncRootedValueCost(II->getArgOperand(1), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      AddedValues.insert(V);
      if (II->hasOneUse())
        RemovedInstructions.insert(II);
      return true;
    case Intrinsic::abs: {
      // abs(a, false) is narrowable when a is sext-bounded at TargetWidth.
      // Without this check, abs(add(sext(x), 100)) with TargetWidth=8 could
      // narrow to abs(add.i8(x, 100)) even when the add overflows i8, giving
      // the wrong sign and thus the wrong absolute value.
      // e.g. a=34: abs.i32(134)=134, trunc.i8=-122; but abs.i8(-122)=122.
      auto *PoisonFlag = dyn_cast<ConstantInt>(II->getArgOperand(1));
      if (!PoisonFlag || !PoisonFlag->isZero())
        return false;
      if (!isSextBoundedAtWidth(II->getArgOperand(0), TargetWidth))
        return false;
      if (!collectTruncRootedValueCost(II->getArgOperand(0), TargetWidth,
                                       AddedValues, RemovedInstructions,
                                       Visited))
        return false;
      AddedValues.insert(V);
      if (II->hasOneUse())
        RemovedInstructions.insert(II);
      return true;
    }
    default:
      break;
    }
  }

  if (Width > TargetWidth) {
    AddedValues.insert(V);
    return true;
  }

  return false;
}

bool tryShrinkTruncOfSelect(TruncInst &Tr) {
  auto *Sel = dyn_cast<SelectInst>(Tr.getOperand(0));
  if (!Sel || !Sel->hasOneUse())
    return false;
  if (!isIntegerValue(&Tr) || !isIntegerValue(Sel) ||
      !isIntegerValue(Sel->getTrueValue()) || !isIntegerValue(Sel->getFalseValue()))
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned SourceWidth = getValueWidth(Sel);
  if (TargetWidth >= SourceWidth)
    return false;

  SmallPtrSet<Value *, 8> AddedValues;
  SmallPtrSet<Instruction *, 8> RemovedInstructions;
  SmallPtrSet<Value *, 8> VisitedValues;
  if (!collectTruncRootedValueCost(Sel->getTrueValue(), TargetWidth,
                                   AddedValues, RemovedInstructions,
                                   VisitedValues) ||
      !collectTruncRootedValueCost(Sel->getFalseValue(), TargetWidth,
                                   AddedValues, RemovedInstructions,
                                   VisitedValues))
    return false;

  unsigned AddedInstructionCost = AddedValues.size();
  unsigned RemovedInstructionCost = 1 + RemovedInstructions.size();

  // Rebuild the select at the narrow width only when removable instructions
  // around the region pay for any new arm materialization we introduce.
  if (AddedInstructionCost > RemovedInstructionCost)
    return false;

  // Materialize each arm at the truncated width first, then rebuild the select
  // directly in that type. The small cache keeps shared arm structure shared
  // after rewriting.
  DenseMap<Value *, Value *> Cache;
  Value *NarrowTV =
      materializeTruncRootedValueAtWidth(Sel->getTrueValue(), TargetWidth, &Tr,
                                         &Cache);
  Value *NarrowFV = materializeTruncRootedValueAtWidth(Sel->getFalseValue(),
                                                       TargetWidth, &Tr, &Cache);
  if (!NarrowTV || !NarrowFV)
    return false;

  IRBuilder<> B(&Tr);
  Value *NarrowSelV =
      B.CreateSelect(Sel->getCondition(), NarrowTV, NarrowFV, Tr.getName());
  if (auto *NarrowSel = dyn_cast<SelectInst>(NarrowSelV))
    NarrowSel->setDebugLoc(Tr.getDebugLoc());
  Tr.replaceAllUsesWith(NarrowSelV);
  Tr.eraseFromParent();

  if (Sel->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(Sel);

  return true;
}

bool tryShrinkTruncOfShiftRecurrence(TruncInst &Tr) {
  auto *Shl = dyn_cast<BinaryOperator>(Tr.getOperand(0));
  if (!Shl || Shl->getOpcode() != Instruction::Shl)
    return false;

  auto *Phi = dyn_cast<PHINode>(Shl->getOperand(0));
  auto *AmtC = dyn_cast<ConstantInt>(Shl->getOperand(1));
  if (!Phi || !AmtC || Phi->getParent() != Shl->getParent())
    return false;
  if (Phi->getNumIncomingValues() != 2)
    return false;

  BasicBlock *LoopBB = Phi->getParent();
  int BackedgeIdx = -1;
  int InitIdx = -1;
  for (unsigned I = 0; I != 2; ++I) {
    if (Phi->getIncomingBlock(I) == LoopBB)
      BackedgeIdx = I;
    else
      InitIdx = I;
  }
  if (BackedgeIdx < 0 || InitIdx < 0)
    return false;
  if (Phi->getIncomingValue(BackedgeIdx) != Shl)
    return false;

  // Keep this first loop rewrite narrow and explicit: one self-recurrence with
  // a constant shift amount, rooted at a final truncation.
  if (!Phi->hasOneUse())
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned SourceWidth = getValueWidth(Shl);
  if (TargetWidth >= SourceWidth)
    return false;

  BasicBlock *InitBB = Phi->getIncomingBlock(InitIdx);
  Value *Init = Phi->getIncomingValue(InitIdx);
  Value *NarrowInit =
      materializeTruncRootedValueAtWidth(Init, TargetWidth,
                                         InitBB->getTerminator());
  if (!NarrowInit)
    return false;

  // If the shift amount is >= the target width, the narrow shift shl iN x, K
  // would be poison (shift by >= bit width is UB). The original
  // trunc(shl.iW(phi, K), N) would be 0, not poison — don't narrow.
  if (AmtC->getValue().uge(TargetWidth))
    return false;

  auto *TargetTy = IntegerType::get(Tr.getContext(), TargetWidth);
  Constant *NarrowAmt = ConstantInt::get(TargetTy, AmtC->getValue().trunc(TargetWidth));

  auto *NarrowPhi = PHINode::Create(TargetTy, 2, Phi->getName() + ".narrow",
                                    Phi->getIterator());
  NarrowPhi->setDebugLoc(Phi->getDebugLoc());
  NarrowPhi->addIncoming(NarrowInit, InitBB);

  IRBuilder<> B(Shl);
  auto *NarrowShl =
      cast<Instruction>(B.CreateShl(NarrowPhi, NarrowAmt, Shl->getName() + ".narrow"));
  NarrowShl->setDebugLoc(Shl->getDebugLoc());
  NarrowPhi->addIncoming(NarrowShl, LoopBB);

  Tr.replaceAllUsesWith(NarrowShl);
  Tr.eraseFromParent();
  return true;
}

// Narrow a loop-carried recurrence  trunc(binop(phi, step))  to TargetWidth
// when binop is low-bit-preserving (add, sub, mul, and, or, xor) and both the
// phi's init value and the step can be materialized at TargetWidth.  The phi
// must have exactly one use (the binop) so we can remove the wide versions.
bool tryShrinkTruncOfLowBitsRecurrence(TruncInst &Tr) {
  auto *BO = dyn_cast<BinaryOperator>(Tr.getOperand(0));
  if (!BO || !isTruncRootedLowBitsPreservingOpcode(BO->getOpcode()))
    return false;

  // One operand of the binop must be the loop-carried phi.
  PHINode *Phi = nullptr;
  unsigned PhiIdx = 0;
  for (unsigned I = 0; I < 2; ++I) {
    if (auto *P = dyn_cast<PHINode>(BO->getOperand(I))) {
      Phi = P;
      PhiIdx = I;
      break;
    }
  }
  if (!Phi || Phi->getParent() != BO->getParent())
    return false;
  if (Phi->getNumIncomingValues() != 2)
    return false;

  BasicBlock *LoopBB = Phi->getParent();
  int BackedgeIdx = -1;
  int InitIdx = -1;
  for (unsigned I = 0; I != 2; ++I) {
    if (Phi->getIncomingBlock(I) == LoopBB)
      BackedgeIdx = I;
    else
      InitIdx = I;
  }
  if (BackedgeIdx < 0 || InitIdx < 0)
    return false;
  if (Phi->getIncomingValue(BackedgeIdx) != BO)
    return false;

  // Require the phi to have only one use (the binop) so we can remove it.
  if (!Phi->hasOneUse())
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned SourceWidth = getValueWidth(BO);
  if (TargetWidth >= SourceWidth)
    return false;

  BasicBlock *InitBB = Phi->getIncomingBlock(InitIdx);
  Value *Init = Phi->getIncomingValue(InitIdx);
  Value *Step = BO->getOperand(1 - PhiIdx);

  Value *NarrowInit = materializeTruncRootedValueAtWidth(
      Init, TargetWidth, InitBB->getTerminator());
  if (!NarrowInit)
    return false;

  Value *NarrowStep =
      materializeTruncRootedValueAtWidth(Step, TargetWidth, BO);
  if (!NarrowStep)
    return false;

  auto *TargetTy = IntegerType::get(Tr.getContext(), TargetWidth);
  auto *NarrowPhi = PHINode::Create(TargetTy, 2, Phi->getName() + ".narrow",
                                    Phi->getIterator());
  NarrowPhi->setDebugLoc(Phi->getDebugLoc());
  NarrowPhi->addIncoming(NarrowInit, InitBB);

  IRBuilder<> B(BO);
  Value *NarrowLHS = PhiIdx == 0 ? (Value *)NarrowPhi : NarrowStep;
  Value *NarrowRHS = PhiIdx == 0 ? NarrowStep : (Value *)NarrowPhi;
  auto *NarrowBO = cast<Instruction>(B.CreateBinOp(
      (Instruction::BinaryOps)BO->getOpcode(), NarrowLHS, NarrowRHS,
      BO->getName() + ".narrow"));
  NarrowBO->setDebugLoc(BO->getDebugLoc());
  NarrowPhi->addIncoming(NarrowBO, LoopBB);

  Tr.replaceAllUsesWith(NarrowBO);
  Tr.eraseFromParent();
  return true;
}

// Narrow  trunc(phi(v0, v1, ...))  when every incoming value is materializable
// at TargetWidth via the trunc-rooted cost infrastructure.  Handles arms that
// are zero-bounded (zext trees) as well as sext-bounded (direct sext or
// sext-rooted low-bit-preserving ops), since both are correctly narrowed by
// collectTruncRootedValueCost / materializeTruncRootedValueAtWidth.
// Complements tryShrinkPhiOfExts which requires all arms to have the same
// extension kind; this function allows mixed sext/zext arms.
bool tryShrinkTruncOfZeroBoundedPhi(TruncInst &Tr) {
  auto *Phi = dyn_cast<PHINode>(Tr.getOperand(0));
  if (!Phi || !Phi->hasOneUse())
    return false;
  if (!isIntegerValue(&Tr) || !isIntegerValue(Phi))
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned SourceWidth = getValueWidth(Phi);
  if (TargetWidth >= SourceWidth)
    return false;

  unsigned N = Phi->getNumIncomingValues();

  // Cost check: use the trunc-rooted infrastructure on each arm.
  SmallPtrSet<Value *, 16> AddedValues;
  SmallPtrSet<Instruction *, 16> RemovedInstructions;
  SmallPtrSet<Value *, 16> Visited;
  for (unsigned I = 0; I != N; ++I) {
    if (!collectTruncRootedValueCost(Phi->getIncomingValue(I), TargetWidth,
                                     AddedValues, RemovedInstructions, Visited))
      return false;
  }
  // The phi itself and the trunc are being replaced; require net non-increase.
  unsigned RemovedCost = 1 + RemovedInstructions.size(); // 1 for the trunc
  if (AddedValues.size() > RemovedCost)
    return false;

  // Materialize each incoming value at TargetWidth, inserting before the
  // terminator of the incoming block.
  auto *TargetTy = getSameShapeIntegerType(Phi->getType(), TargetWidth);
  auto *NarrowPhi = PHINode::Create(TargetTy, N, Phi->getName() + ".narrow",
                                    Phi->getIterator());
  NarrowPhi->setDebugLoc(Phi->getDebugLoc());

  // Keep a result per predecessor block so that duplicate predecessors (e.g.
  // multiple switch cases targeting the same block) reuse the same materialized
  // value.  A fresh instruction-level cache is still used per block so that
  // values materialized in one predecessor don't leak into a sibling block
  // where they don't dominate.
  DenseMap<BasicBlock *, Value *> PerBlockResult;
  for (unsigned I = 0; I != N; ++I) {
    BasicBlock *BB = Phi->getIncomingBlock(I);
    // This rewrite materializes narrowed incoming values immediately before the
    // predecessor terminator. That is not a legal placement when the incoming
    // value is the predecessor's invoke result, because the invoke does not
    // dominate instructions inserted before itself.
    if (Phi->getIncomingValue(I) == BB->getTerminator() &&
        isa<InvokeInst>(BB->getTerminator())) {
      NarrowPhi->eraseFromParent();
      return false;
    }
    Value *NarrowVal;
    auto It = PerBlockResult.find(BB);
    if (It != PerBlockResult.end()) {
      NarrowVal = It->second;
    } else {
      DenseMap<Value *, Value *> Cache;
      NarrowVal = materializeTruncRootedValueAtWidth(
          Phi->getIncomingValue(I), TargetWidth, BB->getTerminator(), &Cache);
      if (!NarrowVal) {
        // Bail out: remove the partially-built phi.
        NarrowPhi->eraseFromParent();
        return false;
      }
      PerBlockResult[BB] = NarrowVal;
    }
    NarrowPhi->addIncoming(NarrowVal, BB);
  }

  Tr.replaceAllUsesWith(NarrowPhi);
  Tr.eraseFromParent();

  if (Phi->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(Phi);

  return true;
}

// Narrow trunc(umin/umax/smin/smax/abs(...)) by applying the intrinsic at the
// narrower width. For umin/umax the args must be zero-bounded; for smin/smax
// and abs(false) the args must be sext-bounded at the target width.
bool tryShrinkTruncOfMinMaxAbs(TruncInst &Tr) {
  auto *II = dyn_cast<IntrinsicInst>(Tr.getOperand(0));
  if (!II || !II->hasOneUse())
    return false;
  if (!isIntegerValue(&Tr) || !isIntegerValue(II))
    return false;

  unsigned TargetWidth = getValueWidth(&Tr);
  unsigned SourceWidth = getValueWidth(II);
  if (TargetWidth >= SourceWidth)
    return false;

  auto IID = II->getIntrinsicID();
  switch (IID) {
  case Intrinsic::umin:
  case Intrinsic::umax:
    if (!isZeroBoundedAtWidth(II->getArgOperand(0), TargetWidth) ||
        !isZeroBoundedAtWidth(II->getArgOperand(1), TargetWidth))
      return false;
    break;
  case Intrinsic::smin:
  case Intrinsic::smax:
    break; // collectTruncRootedValueCost will verify both args below
  case Intrinsic::abs: {
    auto *PoisonFlag = dyn_cast<ConstantInt>(II->getArgOperand(1));
    if (!PoisonFlag || !PoisonFlag->isZero())
      return false;
    break;
  }
  default:
    return false;
  }

  // Cost check: all args (and transitively their subexpressions) must be
  // materializable at TargetWidth without increasing instruction count.
  SmallPtrSet<Value *, 8> AddedValues;
  SmallPtrSet<Instruction *, 8> RemovedInstructions;
  SmallPtrSet<Value *, 8> Visited;
  if (!collectTruncRootedValueCost(II, TargetWidth, AddedValues,
                                   RemovedInstructions, Visited))
    return false;
  // +1 for the trunc itself being removed.
  if (AddedValues.size() > RemovedInstructions.size() + 1)
    return false;

  DenseMap<Value *, Value *> Cache;
  Value *NarrowResult =
      materializeTruncRootedValueAtWidth(II, TargetWidth, &Tr, &Cache);
  if (!NarrowResult)
    return false;

  Tr.replaceAllUsesWith(NarrowResult);
  Tr.eraseFromParent();
  if (II->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(II);
  return true;
}

bool tryPushFreezeThroughExt(FreezeInst &FI) {
  // Canonicalize freeze(ext x) into ext(freeze x) for simple integer casts.
  // This follows the safe direction used by InstCombine: the cast only
  // propagates poison from its operand, so freezing the narrow operand is
  // sufficient to stop poison without inventing a wider arbitrary value.
  auto *Cast = dyn_cast<CastInst>(FI.getOperand(0));
  if (!Cast || !Cast->hasOneUse())
    return false;

  if (!isa<ZExtInst>(Cast) && !isa<SExtInst>(Cast) && !isa<TruncInst>(Cast))
    return false;

  Value *Src = Cast->getOperand(0);
  if (!isIntegerValue(Src) || !isIntegerValue(&FI))
    return false;

  IRBuilder<> B(Cast);
  auto *FrozenSrc = cast<FreezeInst>(
      B.CreateFreeze(Src, Src->hasName() ? Src->getName() + ".fr" : ""));

  Instruction *NewCast = CastInst::Create(Cast->getOpcode(), FrozenSrc,
                                          FI.getType(), "",
                                          FI.getIterator());
  NewCast->setDebugLoc(FI.getDebugLoc());
  NewCast->takeName(&FI);
  FI.replaceAllUsesWith(NewCast);
  FI.eraseFromParent();

  if (Cast->use_empty())
    RecursivelyDeleteTriviallyDeadInstructions(Cast);

  return true;
}

struct LocalRewriteWorklists {
  // Instructions collected here may be erased by earlier transformations in the
  // same round (via RecursivelyDeleteTriviallyDeadInstructions). Use
  // WeakTrackingVH so that deleted instructions auto-null rather than leaving
  // dangling pointers.
  SmallVector<WeakTrackingVH, 16> Compares;
  SmallVector<WeakTrackingVH, 16> Selects;
  SmallVector<WeakTrackingVH, 16> Phis;
  SmallVector<WeakTrackingVH, 16> SExts;
  SmallVector<WeakTrackingVH, 16> Adds;
  SmallVector<WeakTrackingVH, 16> Ands;
  SmallVector<WeakTrackingVH, 16> UDivs;
  SmallVector<WeakTrackingVH, 16> ZExts;
  SmallVector<WeakTrackingVH, 16> Truncs;
  SmallVector<WeakTrackingVH, 16> Freezes;
};

LocalRewriteWorklists collectLocalRewriteWorklists(Function &F) {
  LocalRewriteWorklists WL;
  for (Instruction &I : instructions(F))
    if (auto *Cmp = dyn_cast<ICmpInst>(&I))
      WL.Compares.push_back(Cmp);
    else if (auto *Sel = dyn_cast<SelectInst>(&I))
      WL.Selects.push_back(Sel);
    else if (auto *Phi = dyn_cast<PHINode>(&I))
      WL.Phis.push_back(Phi);
    else if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
      if (BO->getOpcode() == Instruction::Add)
        WL.Adds.push_back(BO);
      else if (BO->getOpcode() == Instruction::And)
        WL.Ands.push_back(BO);
      else if (BO->getOpcode() == Instruction::UDiv ||
               BO->getOpcode() == Instruction::URem)
        WL.UDivs.push_back(BO);
    } else if (auto *ZExt = dyn_cast<ZExtInst>(&I))
      WL.ZExts.push_back(ZExt);
    else if (auto *SExt = dyn_cast<SExtInst>(&I))
      WL.SExts.push_back(SExt);
    else if (auto *Tr = dyn_cast<TruncInst>(&I))
      WL.Truncs.push_back(Tr);
    else if (auto *FI = dyn_cast<FreezeInst>(&I))
      WL.Freezes.push_back(FI);
  return WL;
}

bool runAnalysisAwareLocalRewrites(Function &F, LazyValueInfo &LVI,
                                   AssumptionCache &AC, DominatorTree &DT) {
  const DataLayout &DL = F.getDataLayout();
  LocalRewriteWorklists WL = collectLocalRewriteWorklists(F);
  bool Changed = false;

  for (WeakTrackingVH &VH : WL.UDivs) {
    auto *UDiv = dyn_cast_or_null<BinaryOperator>(VH);
    if (!UDiv || UDiv->getParent() == nullptr)
      continue;
    Changed |= tryNarrowUDivWithRange(*UDiv, LVI);
  }

  for (WeakTrackingVH &VH : WL.SExts) {
    auto *SExt = dyn_cast_or_null<SExtInst>(VH);
    if (!SExt || SExt->getParent() == nullptr)
      continue;
    Changed |= tryConvertSExtToNonNegZExt(*SExt, LVI);
  }

  // When LVI proves the source of a zext is non-negative at this use, mark
  // the zext nneg so that subsequent structural passes (tryShrinkZExtGEPIndex,
  // etc.) can eliminate it.
  for (WeakTrackingVH &VH : WL.ZExts) {
    auto *ZExt = dyn_cast_or_null<ZExtInst>(VH);
    if (!ZExt || ZExt->getParent() == nullptr)
      continue;
    if (ZExt->hasNonNeg())
      continue;
    const Use &SrcUse = ZExt->getOperandUse(0);
    if (LVI.getConstantRangeAtUse(SrcUse, /*UndefAllowed=*/false)
            .isAllNonNegative()) {
      ZExt->setNonNeg();
      Changed = true;
    }
  }

  for (WeakTrackingVH &VH : WL.Compares) {
    auto *Cmp = dyn_cast_or_null<ICmpInst>(VH);
    if (!Cmp || Cmp->getParent() == nullptr)
      continue;
    if (tryWidenTruncEqualityICmp(*Cmp, DL, &AC, &DT)) {
      Changed = true;
      continue;
    }
    Changed |= tryWidenTruncZeroExtendedICmp(*Cmp, DL, &AC, &DT);
  }

  return Changed;
}

bool runStructuralLocalRewritesToFixpoint(Function &F,
                                          DominatorTree *DT = nullptr) {
  bool ChangedAny = false;
  // Build the worklist once. WeakTrackingVH handles auto-null when instructions
  // are deleted mid-round, so the existing dyn_cast_or_null / getParent()
  // checks in each loop body still handle that correctly. New instructions
  // created by transforms are not added to the worklist, but they will be
  // picked up on the next invocation of this function.
  LocalRewriteWorklists WL = collectLocalRewriteWorklists(F);

  while (true) {
    bool ChangedThisRound = false;

    for (WeakTrackingVH &VH : WL.Adds) {
      auto *Add = dyn_cast_or_null<BinaryOperator>(VH);
      if (!Add || Add->getParent() == nullptr)
        continue;
      if (tryWidenAddThroughZExt(*Add)) {
        ChangedThisRound = true;
        continue;
      }
      ChangedThisRound |= tryWidenAddOverTruncThroughZExt(*Add);
    }

    for (WeakTrackingVH &VH : WL.Ands) {
      auto *And = dyn_cast_or_null<BinaryOperator>(VH);
      if (!And || And->getParent() == nullptr)
        continue;
      ChangedThisRound |= tryFoldAndOfSExtToZExt(*And);
    }

    for (WeakTrackingVH &VH : WL.SExts) {
      auto *SExt = dyn_cast_or_null<SExtInst>(VH);
      if (!SExt || SExt->getParent() == nullptr)
        continue;
      if (tryShrinkSExtGEPIndex(*SExt)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkSExtSwitch(*SExt)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkSExtOfSExt(*SExt)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkSExtBitwiseBinop(*SExt)) {
        ChangedThisRound = true;
        continue;
      }
    }

    for (WeakTrackingVH &VH : WL.ZExts) {
      auto *ZExt = dyn_cast_or_null<ZExtInst>(VH);
      if (!ZExt || ZExt->getParent() == nullptr)
        continue;
      if (tryShrinkZExtGEPIndex(*ZExt)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkZExtThroughBinopToGEP(*ZExt)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkZExtSwitch(*ZExt)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkZExtOfLLVMExpect(*ZExt)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkZExtOfZeroBounded(*ZExt)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryWidenSubOverTruncThroughZExtNneg(*ZExt)) {
        ChangedThisRound = true;
        continue;
      }
      ChangedThisRound |= tryFoldZExtOfTruncToMask(*ZExt);
    }

    for (WeakTrackingVH &VH : WL.Truncs) {
      auto *Tr = dyn_cast_or_null<TruncInst>(VH);
      if (!Tr || Tr->getParent() == nullptr)
        continue;
      // Constant-fold trunc iN C to iM when the operand is a constant.
      // Also handles trunc(lshr/and/or/shl(C1, C2)) by first folding the binop.
      {
        Value *TrSrc = Tr->getOperand(0);
        // If the source is a binop with constant operands, compute the constant.
        if (auto *BI = dyn_cast<BinaryOperator>(TrSrc)) {
          auto *C1 = dyn_cast<ConstantInt>(BI->getOperand(0));
          auto *C2 = dyn_cast<ConstantInt>(BI->getOperand(1));
          if (C1 && C2) {
            APInt V;
            switch (BI->getOpcode()) {
            case Instruction::LShr:
              V = C1->getValue().lshr(C2->getValue());
              break;
            case Instruction::AShr:
              V = C1->getValue().ashr(C2->getValue());
              break;
            case Instruction::Shl:
              V = C1->getValue().shl(C2->getValue());
              break;
            case Instruction::And:
              V = C1->getValue() & C2->getValue();
              break;
            case Instruction::Or:
              V = C1->getValue() | C2->getValue();
              break;
            default:
              goto done_const_fold;
            }
            TrSrc = ConstantInt::get(BI->getType(), V);
          }
        }
        if (auto *C = dyn_cast<ConstantInt>(TrSrc)) {
          auto *Folded = ConstantInt::get(Tr->getType(),
                                          C->getValue().trunc(getValueWidth(Tr)));
          Tr->replaceAllUsesWith(Folded);
          Tr->eraseFromParent();
          ChangedThisRound = true;
          continue;
        }
        done_const_fold:;
      }
      if (tryShrinkTruncGEPIndex(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryFoldTruncOfExt(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryFoldTruncOfAndMask(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryFoldTruncOfTrunc(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryFoldTruncToI1WhenSrcIsZeroBounded(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryFoldTruncNuwToI1(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryFoldTruncOfCtpop(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryFoldTruncToI1ViaLShrAndMask(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkTruncOfShiftRecurrence(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkTruncOfLowBitsRecurrence(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkTruncOfSelect(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkTruncOfZeroBoundedPhi(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkTruncOfMinMaxAbs(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkHighHalfSROA(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkSROAI128Destruct(*Tr)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkTruncOfLowBitsBinOp(*Tr)) {
        ChangedThisRound = true;
      }
    }

    for (WeakTrackingVH &VH : WL.SExts) {
      auto *SExt = dyn_cast_or_null<SExtInst>(VH);
      if (!SExt || SExt->getParent() == nullptr)
        continue;
      ChangedThisRound |= tryConvertWholeSExtToZExt(*SExt);
    }

    for (WeakTrackingVH &VH : WL.Freezes) {
      auto *FI = dyn_cast_or_null<FreezeInst>(VH);
      if (!FI || FI->getParent() == nullptr)
        continue;
      ChangedThisRound |= tryPushFreezeThroughExt(*FI);
    }

    for (WeakTrackingVH &VH : WL.Compares) {
      auto *Cmp = dyn_cast_or_null<ICmpInst>(VH);
      if (!Cmp || Cmp->getParent() == nullptr)
        continue;
      if (tryShrinkICmp(*Cmp, DT)) {
        ChangedThisRound = true;
        continue;
      }
      if (tryShrinkICmpExtConst(*Cmp)) {
        ChangedThisRound = true;
        continue;
      }
      ChangedThisRound |= tryShrinkICmpZeroBounded(*Cmp);
    }

    for (WeakTrackingVH &VH : WL.Selects) {
      auto *Sel = dyn_cast_or_null<SelectInst>(VH);
      if (!Sel || Sel->getParent() == nullptr)
        continue;
      ChangedThisRound |= tryShrinkSelectOfExts(*Sel);
    }

    for (WeakTrackingVH &VH : WL.Phis) {
      auto *Phi = dyn_cast_or_null<PHINode>(VH);
      if (!Phi || Phi->getParent() == nullptr)
        continue;
      ChangedThisRound |= tryShrinkPhiOfExts(*Phi);
    }

    if (!ChangedThisRound)
      break;
    ChangedAny = true;
  }

  return ChangedAny;
}

} // namespace

PreservedAnalyses WidthOptPass::run(Function &F, FunctionAnalysisManager &AM) {
  LazyValueInfo &LVI = AM.getResult<LazyValueAnalysis>(F);
  AssumptionCache &AC = AM.getResult<AssumptionAnalysis>(F);
  DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);

  bool Changed = false;

  // Run analysis-driven local rewrites once using the current analysis
  // snapshots, then iterate the purely structural local rewrites to a fixed
  // point so one local fold can expose another later in the pass.
  Changed |= runAnalysisAwareLocalRewrites(F, LVI, AC, DT);
  Changed |= runStructuralLocalRewritesToFixpoint(F, &DT);

  // The structural pass may have converted sext-based comparisons (e.g.,
  // icmp sgt i32 (sext i8 %x), 0) to narrower forms (icmp sgt i8 %x, 0).
  // LVI can now prove nneg for more zext instructions using these tighter
  // branch constraints. Invalidate the LVI cache and re-run.
  LVI.clear();
  if (runAnalysisAwareLocalRewrites(F, LVI, AC, DT)) {
    Changed = true;
    runStructuralLocalRewritesToFixpoint(F, &DT);
  }

  std::string Err;
  raw_string_ostream OS(Err);
  if (verifyFunction(F, &OS)) {
    errs() << "VERIFY FAILED:\n"
           << Err << "\n";
    for (auto &BB : F) {
      errs() << BB.getName() << ":\n";
      for (auto &I : BB)
        errs() << "  " << I << "\n";
    }
    llvm_unreachable("IR broken");
  }

  return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace widthopt
