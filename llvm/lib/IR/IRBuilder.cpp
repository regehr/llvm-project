//===- IRBuilder.cpp - Builder for LLVM Instrs ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the IRBuilder class, which is used as a convenient way
// to create LLVM instructions with a consistent and simplified interface.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/None.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/NoFolder.h"
#include "llvm/IR/Statepoint.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/MathExtras.h"
#include <cassert>
#include <cstdint>
#include <vector>

using namespace llvm;

/// CreateGlobalString - Make a new global variable with an initializer that
/// has array of i8 type filled in with the nul terminated string value
/// specified.  If Name is specified, it is the name of the global variable
/// created.
GlobalVariable *IRBuilderBase::CreateGlobalString(StringRef Str,
                                                  const Twine &Name,
                                                  unsigned AddressSpace) {
  Constant *StrConstant = ConstantDataArray::getString(Context, Str);
  Module &M = *BB->getParent()->getParent();
  auto *GV = new GlobalVariable(M, StrConstant->getType(), true,
                                GlobalValue::PrivateLinkage, StrConstant, Name,
                                nullptr, GlobalVariable::NotThreadLocal,
                                AddressSpace);
  GV->setUnnamedAddr(GlobalValue::UnnamedAddr::Global);
  GV->setAlignment(Align(1));
  return GV;
}

Type *IRBuilderBase::getCurrentFunctionReturnType() const {
  assert(BB && BB->getParent() && "No current function!");
  return BB->getParent()->getReturnType();
}

Value *IRBuilderBase::getCastedInt8PtrValue(Value *Ptr) {
  auto *PT = cast<PointerType>(Ptr->getType());
  if (PT->getElementType()->isIntegerTy(8))
    return Ptr;

  // Otherwise, we need to insert a bitcast.
  return CreateBitCast(Ptr, getInt8PtrTy(PT->getAddressSpace()));
}

static CallInst *createCallHelper(Function *Callee, ArrayRef<Value *> Ops,
                                  IRBuilderBase *Builder,
                                  const Twine &Name = "",
                                  Instruction *FMFSource = nullptr) {
  CallInst *CI = Builder->CreateCall(Callee, Ops, Name);
  if (FMFSource)
    CI->copyFastMathFlags(FMFSource);
  return CI;
}

CallInst *IRBuilderBase::CreateMemSet(Value *Ptr, Value *Val, Value *Size,
                                      MaybeAlign Align, bool isVolatile,
                                      MDNode *TBAATag, MDNode *ScopeTag,
                                      MDNode *NoAliasTag) {
  Ptr = getCastedInt8PtrValue(Ptr);
  Value *Ops[] = {Ptr, Val, Size, getInt1(isVolatile)};
  Type *Tys[] = { Ptr->getType(), Size->getType() };
  Module *M = BB->getParent()->getParent();
  Function *TheFn = Intrinsic::getDeclaration(M, Intrinsic::memset, Tys);

  CallInst *CI = createCallHelper(TheFn, Ops, this);

  if (Align)
    cast<MemSetInst>(CI)->setDestAlignment(Align->value());

  // Set the TBAA info if present.
  if (TBAATag)
    CI->setMetadata(LLVMContext::MD_tbaa, TBAATag);

  if (ScopeTag)
    CI->setMetadata(LLVMContext::MD_alias_scope, ScopeTag);

  if (NoAliasTag)
    CI->setMetadata(LLVMContext::MD_noalias, NoAliasTag);

  return CI;
}

CallInst *IRBuilderBase::CreateElementUnorderedAtomicMemSet(
    Value *Ptr, Value *Val, Value *Size, Align Alignment, uint32_t ElementSize,
    MDNode *TBAATag, MDNode *ScopeTag, MDNode *NoAliasTag) {

  Ptr = getCastedInt8PtrValue(Ptr);
  Value *Ops[] = {Ptr, Val, Size, getInt32(ElementSize)};
  Type *Tys[] = {Ptr->getType(), Size->getType()};
  Module *M = BB->getParent()->getParent();
  Function *TheFn = Intrinsic::getDeclaration(
      M, Intrinsic::memset_element_unordered_atomic, Tys);

  CallInst *CI = createCallHelper(TheFn, Ops, this);

  cast<AtomicMemSetInst>(CI)->setDestAlignment(Alignment);

  // Set the TBAA info if present.
  if (TBAATag)
    CI->setMetadata(LLVMContext::MD_tbaa, TBAATag);

  if (ScopeTag)
    CI->setMetadata(LLVMContext::MD_alias_scope, ScopeTag);

  if (NoAliasTag)
    CI->setMetadata(LLVMContext::MD_noalias, NoAliasTag);

  return CI;
}

CallInst *IRBuilderBase::CreateMemCpy(Value *Dst, unsigned DstAlign, Value *Src,
                                      unsigned SrcAlign, Value *Size,
                                      bool isVolatile, MDNode *TBAATag,
                                      MDNode *TBAAStructTag, MDNode *ScopeTag,
                                      MDNode *NoAliasTag) {
  return CreateMemCpy(Dst, MaybeAlign(DstAlign), Src, MaybeAlign(SrcAlign),
                      Size, isVolatile, TBAATag, TBAAStructTag, ScopeTag,
                      NoAliasTag);
}

CallInst *IRBuilderBase::CreateMemCpy(Value *Dst, MaybeAlign DstAlign,
                                      Value *Src, MaybeAlign SrcAlign,
                                      Value *Size, bool isVolatile,
                                      MDNode *TBAATag, MDNode *TBAAStructTag,
                                      MDNode *ScopeTag, MDNode *NoAliasTag) {
  Dst = getCastedInt8PtrValue(Dst);
  Src = getCastedInt8PtrValue(Src);

  Value *Ops[] = {Dst, Src, Size, getInt1(isVolatile)};
  Type *Tys[] = { Dst->getType(), Src->getType(), Size->getType() };
  Module *M = BB->getParent()->getParent();
  Function *TheFn = Intrinsic::getDeclaration(M, Intrinsic::memcpy, Tys);

  CallInst *CI = createCallHelper(TheFn, Ops, this);

  auto* MCI = cast<MemCpyInst>(CI);
  if (DstAlign)
    MCI->setDestAlignment(*DstAlign);
  if (SrcAlign)
    MCI->setSourceAlignment(*SrcAlign);

  // Set the TBAA info if present.
  if (TBAATag)
    CI->setMetadata(LLVMContext::MD_tbaa, TBAATag);

  // Set the TBAA Struct info if present.
  if (TBAAStructTag)
    CI->setMetadata(LLVMContext::MD_tbaa_struct, TBAAStructTag);

  if (ScopeTag)
    CI->setMetadata(LLVMContext::MD_alias_scope, ScopeTag);

  if (NoAliasTag)
    CI->setMetadata(LLVMContext::MD_noalias, NoAliasTag);

  return CI;
}

CallInst *IRBuilderBase::CreateMemCpyInline(Value *Dst, MaybeAlign DstAlign,
                                            Value *Src, MaybeAlign SrcAlign,
                                            Value *Size) {
  Dst = getCastedInt8PtrValue(Dst);
  Src = getCastedInt8PtrValue(Src);
  Value *IsVolatile = getInt1(false);

  Value *Ops[] = {Dst, Src, Size, IsVolatile};
  Type *Tys[] = {Dst->getType(), Src->getType(), Size->getType()};
  Function *F = BB->getParent();
  Module *M = F->getParent();
  Function *TheFn = Intrinsic::getDeclaration(M, Intrinsic::memcpy_inline, Tys);

  CallInst *CI = createCallHelper(TheFn, Ops, this);

  auto *MCI = cast<MemCpyInlineInst>(CI);
  if (DstAlign)
    MCI->setDestAlignment(*DstAlign);
  if (SrcAlign)
    MCI->setSourceAlignment(*SrcAlign);

  return CI;
}

CallInst *IRBuilderBase::CreateElementUnorderedAtomicMemCpy(
    Value *Dst, Align DstAlign, Value *Src, Align SrcAlign, Value *Size,
    uint32_t ElementSize, MDNode *TBAATag, MDNode *TBAAStructTag,
    MDNode *ScopeTag, MDNode *NoAliasTag) {
  assert(DstAlign >= ElementSize &&
         "Pointer alignment must be at least element size");
  assert(SrcAlign >= ElementSize &&
         "Pointer alignment must be at least element size");
  Dst = getCastedInt8PtrValue(Dst);
  Src = getCastedInt8PtrValue(Src);

  Value *Ops[] = {Dst, Src, Size, getInt32(ElementSize)};
  Type *Tys[] = {Dst->getType(), Src->getType(), Size->getType()};
  Module *M = BB->getParent()->getParent();
  Function *TheFn = Intrinsic::getDeclaration(
      M, Intrinsic::memcpy_element_unordered_atomic, Tys);

  CallInst *CI = createCallHelper(TheFn, Ops, this);

  // Set the alignment of the pointer args.
  auto *AMCI = cast<AtomicMemCpyInst>(CI);
  AMCI->setDestAlignment(DstAlign);
  AMCI->setSourceAlignment(SrcAlign);

  // Set the TBAA info if present.
  if (TBAATag)
    CI->setMetadata(LLVMContext::MD_tbaa, TBAATag);

  // Set the TBAA Struct info if present.
  if (TBAAStructTag)
    CI->setMetadata(LLVMContext::MD_tbaa_struct, TBAAStructTag);

  if (ScopeTag)
    CI->setMetadata(LLVMContext::MD_alias_scope, ScopeTag);

  if (NoAliasTag)
    CI->setMetadata(LLVMContext::MD_noalias, NoAliasTag);

  return CI;
}

CallInst *IRBuilderBase::CreateMemMove(Value *Dst, MaybeAlign DstAlign,
                                       Value *Src, MaybeAlign SrcAlign,
                                       Value *Size, bool isVolatile,
                                       MDNode *TBAATag, MDNode *ScopeTag,
                                       MDNode *NoAliasTag) {
  Dst = getCastedInt8PtrValue(Dst);
  Src = getCastedInt8PtrValue(Src);

  Value *Ops[] = {Dst, Src, Size, getInt1(isVolatile)};
  Type *Tys[] = { Dst->getType(), Src->getType(), Size->getType() };
  Module *M = BB->getParent()->getParent();
  Function *TheFn = Intrinsic::getDeclaration(M, Intrinsic::memmove, Tys);

  CallInst *CI = createCallHelper(TheFn, Ops, this);

  auto *MMI = cast<MemMoveInst>(CI);
  if (DstAlign)
    MMI->setDestAlignment(*DstAlign);
  if (SrcAlign)
    MMI->setSourceAlignment(*SrcAlign);

  // Set the TBAA info if present.
  if (TBAATag)
    CI->setMetadata(LLVMContext::MD_tbaa, TBAATag);

  if (ScopeTag)
    CI->setMetadata(LLVMContext::MD_alias_scope, ScopeTag);

  if (NoAliasTag)
    CI->setMetadata(LLVMContext::MD_noalias, NoAliasTag);

  return CI;
}

CallInst *IRBuilderBase::CreateElementUnorderedAtomicMemMove(
    Value *Dst, Align DstAlign, Value *Src, Align SrcAlign, Value *Size,
    uint32_t ElementSize, MDNode *TBAATag, MDNode *TBAAStructTag,
    MDNode *ScopeTag, MDNode *NoAliasTag) {
  assert(DstAlign >= ElementSize &&
         "Pointer alignment must be at least element size");
  assert(SrcAlign >= ElementSize &&
         "Pointer alignment must be at least element size");
  Dst = getCastedInt8PtrValue(Dst);
  Src = getCastedInt8PtrValue(Src);

  Value *Ops[] = {Dst, Src, Size, getInt32(ElementSize)};
  Type *Tys[] = {Dst->getType(), Src->getType(), Size->getType()};
  Module *M = BB->getParent()->getParent();
  Function *TheFn = Intrinsic::getDeclaration(
      M, Intrinsic::memmove_element_unordered_atomic, Tys);

  CallInst *CI = createCallHelper(TheFn, Ops, this);

  // Set the alignment of the pointer args.
  CI->addParamAttr(0, Attribute::getWithAlignment(CI->getContext(), DstAlign));
  CI->addParamAttr(1, Attribute::getWithAlignment(CI->getContext(), SrcAlign));

  // Set the TBAA info if present.
  if (TBAATag)
    CI->setMetadata(LLVMContext::MD_tbaa, TBAATag);

  // Set the TBAA Struct info if present.
  if (TBAAStructTag)
    CI->setMetadata(LLVMContext::MD_tbaa_struct, TBAAStructTag);

  if (ScopeTag)
    CI->setMetadata(LLVMContext::MD_alias_scope, ScopeTag);

  if (NoAliasTag)
    CI->setMetadata(LLVMContext::MD_noalias, NoAliasTag);

  return CI;
}

static CallInst *getReductionIntrinsic(IRBuilderBase *Builder, Intrinsic::ID ID,
                                    Value *Src) {
  Module *M = Builder->GetInsertBlock()->getParent()->getParent();
  Value *Ops[] = {Src};
  Type *Tys[] = { Src->getType() };
  auto Decl = Intrinsic::getDeclaration(M, ID, Tys);
  return createCallHelper(Decl, Ops, Builder);
}

CallInst *IRBuilderBase::CreateFAddReduce(Value *Acc, Value *Src) {
  Module *M = GetInsertBlock()->getParent()->getParent();
  Value *Ops[] = {Acc, Src};
  Type *Tys[] = {Acc->getType(), Src->getType()};
  auto Decl = Intrinsic::getDeclaration(
      M, Intrinsic::experimental_vector_reduce_v2_fadd, Tys);
  return createCallHelper(Decl, Ops, this);
}

CallInst *IRBuilderBase::CreateFMulReduce(Value *Acc, Value *Src) {
  Module *M = GetInsertBlock()->getParent()->getParent();
  Value *Ops[] = {Acc, Src};
  Type *Tys[] = {Acc->getType(), Src->getType()};
  auto Decl = Intrinsic::getDeclaration(
      M, Intrinsic::experimental_vector_reduce_v2_fmul, Tys);
  return createCallHelper(Decl, Ops, this);
}

CallInst *IRBuilderBase::CreateAddReduce(Value *Src) {
  return getReductionIntrinsic(this, Intrinsic::experimental_vector_reduce_add,
                               Src);
}

CallInst *IRBuilderBase::CreateMulReduce(Value *Src) {
  return getReductionIntrinsic(this, Intrinsic::experimental_vector_reduce_mul,
                               Src);
}

CallInst *IRBuilderBase::CreateAndReduce(Value *Src) {
  return getReductionIntrinsic(this, Intrinsic::experimental_vector_reduce_and,
                               Src);
}

CallInst *IRBuilderBase::CreateOrReduce(Value *Src) {
  return getReductionIntrinsic(this, Intrinsic::experimental_vector_reduce_or,
                               Src);
}

CallInst *IRBuilderBase::CreateXorReduce(Value *Src) {
  return getReductionIntrinsic(this, Intrinsic::experimental_vector_reduce_xor,
                               Src);
}

CallInst *IRBuilderBase::CreateIntMaxReduce(Value *Src, bool IsSigned) {
  auto ID = IsSigned ? Intrinsic::experimental_vector_reduce_smax
                     : Intrinsic::experimental_vector_reduce_umax;
  return getReductionIntrinsic(this, ID, Src);
}

CallInst *IRBuilderBase::CreateIntMinReduce(Value *Src, bool IsSigned) {
  auto ID = IsSigned ? Intrinsic::experimental_vector_reduce_smin
                     : Intrinsic::experimental_vector_reduce_umin;
  return getReductionIntrinsic(this, ID, Src);
}

CallInst *IRBuilderBase::CreateFPMaxReduce(Value *Src, bool NoNaN) {
  auto Rdx = getReductionIntrinsic(
      this, Intrinsic::experimental_vector_reduce_fmax, Src);
  if (NoNaN) {
    FastMathFlags FMF;
    FMF.setNoNaNs();
    Rdx->setFastMathFlags(FMF);
  }
  return Rdx;
}

CallInst *IRBuilderBase::CreateFPMinReduce(Value *Src, bool NoNaN) {
  auto Rdx = getReductionIntrinsic(
      this, Intrinsic::experimental_vector_reduce_fmin, Src);
  if (NoNaN) {
    FastMathFlags FMF;
    FMF.setNoNaNs();
    Rdx->setFastMathFlags(FMF);
  }
  return Rdx;
}

CallInst *IRBuilderBase::CreateLifetimeStart(Value *Ptr, ConstantInt *Size) {
  assert(isa<PointerType>(Ptr->getType()) &&
         "lifetime.start only applies to pointers.");
  Ptr = getCastedInt8PtrValue(Ptr);
  if (!Size)
    Size = getInt64(-1);
  else
    assert(Size->getType() == getInt64Ty() &&
           "lifetime.start requires the size to be an i64");
  Value *Ops[] = { Size, Ptr };
  Module *M = BB->getParent()->getParent();
  Function *TheFn =
      Intrinsic::getDeclaration(M, Intrinsic::lifetime_start, {Ptr->getType()});
  return createCallHelper(TheFn, Ops, this);
}

CallInst *IRBuilderBase::CreateLifetimeEnd(Value *Ptr, ConstantInt *Size) {
  assert(isa<PointerType>(Ptr->getType()) &&
         "lifetime.end only applies to pointers.");
  Ptr = getCastedInt8PtrValue(Ptr);
  if (!Size)
    Size = getInt64(-1);
  else
    assert(Size->getType() == getInt64Ty() &&
           "lifetime.end requires the size to be an i64");
  Value *Ops[] = { Size, Ptr };
  Module *M = BB->getParent()->getParent();
  Function *TheFn =
      Intrinsic::getDeclaration(M, Intrinsic::lifetime_end, {Ptr->getType()});
  return createCallHelper(TheFn, Ops, this);
}

CallInst *IRBuilderBase::CreateInvariantStart(Value *Ptr, ConstantInt *Size) {

  assert(isa<PointerType>(Ptr->getType()) &&
         "invariant.start only applies to pointers.");
  Ptr = getCastedInt8PtrValue(Ptr);
  if (!Size)
    Size = getInt64(-1);
  else
    assert(Size->getType() == getInt64Ty() &&
           "invariant.start requires the size to be an i64");

  Value *Ops[] = {Size, Ptr};
  // Fill in the single overloaded type: memory object type.
  Type *ObjectPtr[1] = {Ptr->getType()};
  Module *M = BB->getParent()->getParent();
  Function *TheFn =
      Intrinsic::getDeclaration(M, Intrinsic::invariant_start, ObjectPtr);
  return createCallHelper(TheFn, Ops, this);
}

CallInst *IRBuilderBase::CreateAssumption(Value *Cond) {
  assert(Cond->getType() == getInt1Ty() &&
         "an assumption condition must be of type i1");

  Value *Ops[] = { Cond };
  Module *M = BB->getParent()->getParent();
  Function *FnAssume = Intrinsic::getDeclaration(M, Intrinsic::assume);
  return createCallHelper(FnAssume, Ops, this);
}

/// Create a call to a Masked Load intrinsic.
/// \p Ptr       - base pointer for the load
/// \p Alignment - alignment of the source location
/// \p Mask      - vector of booleans which indicates what vector lanes should
///                be accessed in memory
/// \p PassThru  - pass-through value that is used to fill the masked-off lanes
///                of the result
/// \p Name      - name of the result variable
CallInst *IRBuilderBase::CreateMaskedLoad(Value *Ptr, Align Alignment,
                                          Value *Mask, Value *PassThru,
                                          const Twine &Name) {
  auto *PtrTy = cast<PointerType>(Ptr->getType());
  Type *DataTy = PtrTy->getElementType();
  assert(DataTy->isVectorTy() && "Ptr should point to a vector");
  assert(Mask && "Mask should not be all-ones (null)");
  if (!PassThru)
    PassThru = UndefValue::get(DataTy);
  Type *OverloadedTypes[] = { DataTy, PtrTy };
  Value *Ops[] = {Ptr, getInt32(Alignment.value()), Mask, PassThru};
  return CreateMaskedIntrinsic(Intrinsic::masked_load, Ops,
                               OverloadedTypes, Name);
}

/// Create a call to a Masked Store intrinsic.
/// \p Val       - data to be stored,
/// \p Ptr       - base pointer for the store
/// \p Alignment - alignment of the destination location
/// \p Mask      - vector of booleans which indicates what vector lanes should
///                be accessed in memory
CallInst *IRBuilderBase::CreateMaskedStore(Value *Val, Value *Ptr,
                                           Align Alignment, Value *Mask) {
  auto *PtrTy = cast<PointerType>(Ptr->getType());
  Type *DataTy = PtrTy->getElementType();
  assert(DataTy->isVectorTy() && "Ptr should point to a vector");
  assert(Mask && "Mask should not be all-ones (null)");
  Type *OverloadedTypes[] = { DataTy, PtrTy };
  Value *Ops[] = {Val, Ptr, getInt32(Alignment.value()), Mask};
  return CreateMaskedIntrinsic(Intrinsic::masked_store, Ops, OverloadedTypes);
}

/// Create a call to a Masked intrinsic, with given intrinsic Id,
/// an array of operands - Ops, and an array of overloaded types -
/// OverloadedTypes.
CallInst *IRBuilderBase::CreateMaskedIntrinsic(Intrinsic::ID Id,
                                               ArrayRef<Value *> Ops,
                                               ArrayRef<Type *> OverloadedTypes,
                                               const Twine &Name) {
  Module *M = BB->getParent()->getParent();
  Function *TheFn = Intrinsic::getDeclaration(M, Id, OverloadedTypes);
  return createCallHelper(TheFn, Ops, this, Name);
}

/// Create a call to a Masked Gather intrinsic.
/// \p Ptrs     - vector of pointers for loading
/// \p Align    - alignment for one element
/// \p Mask     - vector of booleans which indicates what vector lanes should
///               be accessed in memory
/// \p PassThru - pass-through value that is used to fill the masked-off lanes
///               of the result
/// \p Name     - name of the result variable
CallInst *IRBuilderBase::CreateMaskedGather(Value *Ptrs, Align Alignment,
                                            Value *Mask, Value *PassThru,
                                            const Twine &Name) {
  auto PtrsTy = cast<VectorType>(Ptrs->getType());
  auto PtrTy = cast<PointerType>(PtrsTy->getElementType());
  unsigned NumElts = PtrsTy->getVectorNumElements();
  Type *DataTy = VectorType::get(PtrTy->getElementType(), NumElts);

  if (!Mask)
    Mask = Constant::getAllOnesValue(VectorType::get(Type::getInt1Ty(Context),
                                     NumElts));

  if (!PassThru)
    PassThru = UndefValue::get(DataTy);

  Type *OverloadedTypes[] = {DataTy, PtrsTy};
  Value *Ops[] = {Ptrs, getInt32(Alignment.value()), Mask, PassThru};

  // We specify only one type when we create this intrinsic. Types of other
  // arguments are derived from this type.
  return CreateMaskedIntrinsic(Intrinsic::masked_gather, Ops, OverloadedTypes,
                               Name);
}

/// Create a call to a Masked Scatter intrinsic.
/// \p Data  - data to be stored,
/// \p Ptrs  - the vector of pointers, where the \p Data elements should be
///            stored
/// \p Align - alignment for one element
/// \p Mask  - vector of booleans which indicates what vector lanes should
///            be accessed in memory
CallInst *IRBuilderBase::CreateMaskedScatter(Value *Data, Value *Ptrs,
                                             Align Alignment, Value *Mask) {
  auto PtrsTy = cast<VectorType>(Ptrs->getType());
  auto DataTy = cast<VectorType>(Data->getType());
  unsigned NumElts = PtrsTy->getVectorNumElements();

#ifndef NDEBUG
  auto PtrTy = cast<PointerType>(PtrsTy->getElementType());
  assert(NumElts == DataTy->getVectorNumElements() &&
         PtrTy->getElementType() == DataTy->getElementType() &&
         "Incompatible pointer and data types");
#endif

  if (!Mask)
    Mask = Constant::getAllOnesValue(VectorType::get(Type::getInt1Ty(Context),
                                     NumElts));

  Type *OverloadedTypes[] = {DataTy, PtrsTy};
  Value *Ops[] = {Data, Ptrs, getInt32(Alignment.value()), Mask};

  // We specify only one type when we create this intrinsic. Types of other
  // arguments are derived from this type.
  return CreateMaskedIntrinsic(Intrinsic::masked_scatter, Ops, OverloadedTypes);
}

template <typename T0, typename T1, typename T2, typename T3>
static std::vector<Value *>
getStatepointArgs(IRBuilderBase &B, uint64_t ID, uint32_t NumPatchBytes,
                  Value *ActualCallee, uint32_t Flags, ArrayRef<T0> CallArgs,
                  ArrayRef<T1> TransitionArgs, ArrayRef<T2> DeoptArgs,
                  ArrayRef<T3> GCArgs) {
  std::vector<Value *> Args;
  Args.push_back(B.getInt64(ID));
  Args.push_back(B.getInt32(NumPatchBytes));
  Args.push_back(ActualCallee);
  Args.push_back(B.getInt32(CallArgs.size()));
  Args.push_back(B.getInt32(Flags));
  Args.insert(Args.end(), CallArgs.begin(), CallArgs.end());
  Args.push_back(B.getInt32(TransitionArgs.size()));
  Args.insert(Args.end(), TransitionArgs.begin(), TransitionArgs.end());
  Args.push_back(B.getInt32(DeoptArgs.size()));
  Args.insert(Args.end(), DeoptArgs.begin(), DeoptArgs.end());
  Args.insert(Args.end(), GCArgs.begin(), GCArgs.end());

  return Args;
}

template <typename T0, typename T1, typename T2, typename T3>
static CallInst *CreateGCStatepointCallCommon(
    IRBuilderBase *Builder, uint64_t ID, uint32_t NumPatchBytes,
    Value *ActualCallee, uint32_t Flags, ArrayRef<T0> CallArgs,
    ArrayRef<T1> TransitionArgs, ArrayRef<T2> DeoptArgs, ArrayRef<T3> GCArgs,
    const Twine &Name) {
  // Extract out the type of the callee.
  auto *FuncPtrType = cast<PointerType>(ActualCallee->getType());
  assert(isa<FunctionType>(FuncPtrType->getElementType()) &&
         "actual callee must be a callable value");

  Module *M = Builder->GetInsertBlock()->getParent()->getParent();
  // Fill in the one generic type'd argument (the function is also vararg)
  Type *ArgTypes[] = { FuncPtrType };
  Function *FnStatepoint =
    Intrinsic::getDeclaration(M, Intrinsic::experimental_gc_statepoint,
                              ArgTypes);

  std::vector<Value *> Args =
      getStatepointArgs(*Builder, ID, NumPatchBytes, ActualCallee, Flags,
                        CallArgs, TransitionArgs, DeoptArgs, GCArgs);
  return createCallHelper(FnStatepoint, Args, Builder, Name);
}

CallInst *IRBuilderBase::CreateGCStatepointCall(
    uint64_t ID, uint32_t NumPatchBytes, Value *ActualCallee,
    ArrayRef<Value *> CallArgs, ArrayRef<Value *> DeoptArgs,
    ArrayRef<Value *> GCArgs, const Twine &Name) {
  return CreateGCStatepointCallCommon<Value *, Value *, Value *, Value *>(
      this, ID, NumPatchBytes, ActualCallee, uint32_t(StatepointFlags::None),
      CallArgs, None /* No Transition Args */, DeoptArgs, GCArgs, Name);
}

CallInst *IRBuilderBase::CreateGCStatepointCall(
    uint64_t ID, uint32_t NumPatchBytes, Value *ActualCallee, uint32_t Flags,
    ArrayRef<Use> CallArgs, ArrayRef<Use> TransitionArgs,
    ArrayRef<Use> DeoptArgs, ArrayRef<Value *> GCArgs, const Twine &Name) {
  return CreateGCStatepointCallCommon<Use, Use, Use, Value *>(
      this, ID, NumPatchBytes, ActualCallee, Flags, CallArgs, TransitionArgs,
      DeoptArgs, GCArgs, Name);
}

CallInst *IRBuilderBase::CreateGCStatepointCall(
    uint64_t ID, uint32_t NumPatchBytes, Value *ActualCallee,
    ArrayRef<Use> CallArgs, ArrayRef<Value *> DeoptArgs,
    ArrayRef<Value *> GCArgs, const Twine &Name) {
  return CreateGCStatepointCallCommon<Use, Value *, Value *, Value *>(
      this, ID, NumPatchBytes, ActualCallee, uint32_t(StatepointFlags::None),
      CallArgs, None, DeoptArgs, GCArgs, Name);
}

template <typename T0, typename T1, typename T2, typename T3>
static InvokeInst *CreateGCStatepointInvokeCommon(
    IRBuilderBase *Builder, uint64_t ID, uint32_t NumPatchBytes,
    Value *ActualInvokee, BasicBlock *NormalDest, BasicBlock *UnwindDest,
    uint32_t Flags, ArrayRef<T0> InvokeArgs, ArrayRef<T1> TransitionArgs,
    ArrayRef<T2> DeoptArgs, ArrayRef<T3> GCArgs, const Twine &Name) {
  // Extract out the type of the callee.
  auto *FuncPtrType = cast<PointerType>(ActualInvokee->getType());
  assert(isa<FunctionType>(FuncPtrType->getElementType()) &&
         "actual callee must be a callable value");

  Module *M = Builder->GetInsertBlock()->getParent()->getParent();
  // Fill in the one generic type'd argument (the function is also vararg)
  Function *FnStatepoint = Intrinsic::getDeclaration(
      M, Intrinsic::experimental_gc_statepoint, {FuncPtrType});

  std::vector<Value *> Args =
      getStatepointArgs(*Builder, ID, NumPatchBytes, ActualInvokee, Flags,
                        InvokeArgs, TransitionArgs, DeoptArgs, GCArgs);
  return Builder->CreateInvoke(FnStatepoint, NormalDest, UnwindDest, Args,
                               Name);
}

InvokeInst *IRBuilderBase::CreateGCStatepointInvoke(
    uint64_t ID, uint32_t NumPatchBytes, Value *ActualInvokee,
    BasicBlock *NormalDest, BasicBlock *UnwindDest,
    ArrayRef<Value *> InvokeArgs, ArrayRef<Value *> DeoptArgs,
    ArrayRef<Value *> GCArgs, const Twine &Name) {
  return CreateGCStatepointInvokeCommon<Value *, Value *, Value *, Value *>(
      this, ID, NumPatchBytes, ActualInvokee, NormalDest, UnwindDest,
      uint32_t(StatepointFlags::None), InvokeArgs, None /* No Transition Args*/,
      DeoptArgs, GCArgs, Name);
}

InvokeInst *IRBuilderBase::CreateGCStatepointInvoke(
    uint64_t ID, uint32_t NumPatchBytes, Value *ActualInvokee,
    BasicBlock *NormalDest, BasicBlock *UnwindDest, uint32_t Flags,
    ArrayRef<Use> InvokeArgs, ArrayRef<Use> TransitionArgs,
    ArrayRef<Use> DeoptArgs, ArrayRef<Value *> GCArgs, const Twine &Name) {
  return CreateGCStatepointInvokeCommon<Use, Use, Use, Value *>(
      this, ID, NumPatchBytes, ActualInvokee, NormalDest, UnwindDest, Flags,
      InvokeArgs, TransitionArgs, DeoptArgs, GCArgs, Name);
}

InvokeInst *IRBuilderBase::CreateGCStatepointInvoke(
    uint64_t ID, uint32_t NumPatchBytes, Value *ActualInvokee,
    BasicBlock *NormalDest, BasicBlock *UnwindDest, ArrayRef<Use> InvokeArgs,
    ArrayRef<Value *> DeoptArgs, ArrayRef<Value *> GCArgs, const Twine &Name) {
  return CreateGCStatepointInvokeCommon<Use, Value *, Value *, Value *>(
      this, ID, NumPatchBytes, ActualInvokee, NormalDest, UnwindDest,
      uint32_t(StatepointFlags::None), InvokeArgs, None, DeoptArgs, GCArgs,
      Name);
}

CallInst *IRBuilderBase::CreateGCResult(Instruction *Statepoint,
                                       Type *ResultType,
                                       const Twine &Name) {
 Intrinsic::ID ID = Intrinsic::experimental_gc_result;
 Module *M = BB->getParent()->getParent();
 Type *Types[] = {ResultType};
 Function *FnGCResult = Intrinsic::getDeclaration(M, ID, Types);

 Value *Args[] = {Statepoint};
 return createCallHelper(FnGCResult, Args, this, Name);
}

CallInst *IRBuilderBase::CreateGCRelocate(Instruction *Statepoint,
                                         int BaseOffset,
                                         int DerivedOffset,
                                         Type *ResultType,
                                         const Twine &Name) {
 Module *M = BB->getParent()->getParent();
 Type *Types[] = {ResultType};
 Function *FnGCRelocate =
     Intrinsic::getDeclaration(M, Intrinsic::experimental_gc_relocate, Types);

 Value *Args[] = {Statepoint,
                  getInt32(BaseOffset),
                  getInt32(DerivedOffset)};
 return createCallHelper(FnGCRelocate, Args, this, Name);
}

CallInst *IRBuilderBase::CreateUnaryIntrinsic(Intrinsic::ID ID, Value *V,
                                              Instruction *FMFSource,
                                              const Twine &Name) {
  Module *M = BB->getModule();
  Function *Fn = Intrinsic::getDeclaration(M, ID, {V->getType()});
  return createCallHelper(Fn, {V}, this, Name, FMFSource);
}

CallInst *IRBuilderBase::CreateBinaryIntrinsic(Intrinsic::ID ID, Value *LHS,
                                               Value *RHS,
                                               Instruction *FMFSource,
                                               const Twine &Name) {
  Module *M = BB->getModule();
  Function *Fn = Intrinsic::getDeclaration(M, ID, { LHS->getType() });
  return createCallHelper(Fn, {LHS, RHS}, this, Name, FMFSource);
}

CallInst *IRBuilderBase::CreateIntrinsic(Intrinsic::ID ID,
                                         ArrayRef<Type *> Types,
                                         ArrayRef<Value *> Args,
                                         Instruction *FMFSource,
                                         const Twine &Name) {
  Module *M = BB->getModule();
  Function *Fn = Intrinsic::getDeclaration(M, ID, Types);
  return createCallHelper(Fn, Args, this, Name, FMFSource);
}

CallInst *IRBuilderBase::CreateConstrainedFPBinOp(
    Intrinsic::ID ID, Value *L, Value *R, Instruction *FMFSource,
    const Twine &Name, MDNode *FPMathTag,
    Optional<fp::RoundingMode> Rounding,
    Optional<fp::ExceptionBehavior> Except) {
  Value *RoundingV = getConstrainedFPRounding(Rounding);
  Value *ExceptV = getConstrainedFPExcept(Except);

  FastMathFlags UseFMF = FMF;
  if (FMFSource)
    UseFMF = FMFSource->getFastMathFlags();

  CallInst *C = CreateIntrinsic(ID, {L->getType()},
                                {L, R, RoundingV, ExceptV}, nullptr, Name);
  setConstrainedFPCallAttr(C);
  setFPAttrs(C, FPMathTag, UseFMF);
  return C;
}

Value *IRBuilderBase::CreateNAryOp(unsigned Opc, ArrayRef<Value *> Ops,
                                   const Twine &Name, MDNode *FPMathTag) {
  if (Instruction::isBinaryOp(Opc)) {
    assert(Ops.size() == 2 && "Invalid number of operands!");
    return CreateBinOp(static_cast<Instruction::BinaryOps>(Opc),
                       Ops[0], Ops[1], Name, FPMathTag);
  }
  if (Instruction::isUnaryOp(Opc)) {
    assert(Ops.size() == 1 && "Invalid number of operands!");
    return CreateUnOp(static_cast<Instruction::UnaryOps>(Opc),
                      Ops[0], Name, FPMathTag);
  }
  llvm_unreachable("Unexpected opcode!");
}

CallInst *IRBuilderBase::CreateConstrainedFPCast(
    Intrinsic::ID ID, Value *V, Type *DestTy,
    Instruction *FMFSource, const Twine &Name, MDNode *FPMathTag,
    Optional<fp::RoundingMode> Rounding,
    Optional<fp::ExceptionBehavior> Except) {
  Value *ExceptV = getConstrainedFPExcept(Except);

  FastMathFlags UseFMF = FMF;
  if (FMFSource)
    UseFMF = FMFSource->getFastMathFlags();

  CallInst *C;
  bool HasRoundingMD = false;
  switch (ID) {
  default:
    break;
#define INSTRUCTION(NAME, NARG, ROUND_MODE, INTRINSIC)        \
  case Intrinsic::INTRINSIC:                                \
    HasRoundingMD = ROUND_MODE;                             \
    break;
#include "llvm/IR/ConstrainedOps.def"
  }
  if (HasRoundingMD) {
    Value *RoundingV = getConstrainedFPRounding(Rounding);
    C = CreateIntrinsic(ID, {DestTy, V->getType()}, {V, RoundingV, ExceptV},
                        nullptr, Name);
  } else
    C = CreateIntrinsic(ID, {DestTy, V->getType()}, {V, ExceptV}, nullptr,
                        Name);

  setConstrainedFPCallAttr(C);

  if (isa<FPMathOperator>(C))
    setFPAttrs(C, FPMathTag, UseFMF);
  return C;
}

Value *IRBuilderBase::CreateFCmpHelper(
    CmpInst::Predicate P, Value *LHS, Value *RHS, const Twine &Name,
    MDNode *FPMathTag, bool IsSignaling) {
  if (IsFPConstrained) {
    auto ID = IsSignaling ? Intrinsic::experimental_constrained_fcmps
                          : Intrinsic::experimental_constrained_fcmp;
    return CreateConstrainedFPCmp(ID, P, LHS, RHS, Name);
  }

  if (auto *LC = dyn_cast<Constant>(LHS))
    if (auto *RC = dyn_cast<Constant>(RHS))
      return Insert(Folder.CreateFCmp(P, LC, RC), Name);
  return Insert(setFPAttrs(new FCmpInst(P, LHS, RHS), FPMathTag, FMF), Name);
}

CallInst *IRBuilderBase::CreateConstrainedFPCmp(
    Intrinsic::ID ID, CmpInst::Predicate P, Value *L, Value *R,
    const Twine &Name, Optional<fp::ExceptionBehavior> Except) {
  Value *PredicateV = getConstrainedFPPredicate(P);
  Value *ExceptV = getConstrainedFPExcept(Except);

  CallInst *C = CreateIntrinsic(ID, {L->getType()},
                                {L, R, PredicateV, ExceptV}, nullptr, Name);
  setConstrainedFPCallAttr(C);
  return C;
}

CallInst *IRBuilderBase::CreateConstrainedFPCall(
    Function *Callee, ArrayRef<Value *> Args, const Twine &Name,
    Optional<fp::RoundingMode> Rounding,
    Optional<fp::ExceptionBehavior> Except) {
  llvm::SmallVector<Value *, 6> UseArgs;

  for (auto *OneArg : Args)
    UseArgs.push_back(OneArg);
  bool HasRoundingMD = false;
  switch (Callee->getIntrinsicID()) {
  default:
    break;
#define INSTRUCTION(NAME, NARG, ROUND_MODE, INTRINSIC)        \
  case Intrinsic::INTRINSIC:                                \
    HasRoundingMD = ROUND_MODE;                             \
    break;
#include "llvm/IR/ConstrainedOps.def"
  }
  if (HasRoundingMD)
    UseArgs.push_back(getConstrainedFPRounding(Rounding));
  UseArgs.push_back(getConstrainedFPExcept(Except));

  CallInst *C = CreateCall(Callee, UseArgs, Name);
  setConstrainedFPCallAttr(C);
  return C;
}

Value *IRBuilderBase::CreateSelect(Value *C, Value *True, Value *False,
                                   const Twine &Name, Instruction *MDFrom) {
  if (auto *CC = dyn_cast<Constant>(C))
    if (auto *TC = dyn_cast<Constant>(True))
      if (auto *FC = dyn_cast<Constant>(False))
        return Insert(Folder.CreateSelect(CC, TC, FC), Name);

  SelectInst *Sel = SelectInst::Create(C, True, False);
  if (MDFrom) {
    MDNode *Prof = MDFrom->getMetadata(LLVMContext::MD_prof);
    MDNode *Unpred = MDFrom->getMetadata(LLVMContext::MD_unpredictable);
    Sel = addBranchMetadata(Sel, Prof, Unpred);
  }
  if (isa<FPMathOperator>(Sel))
    setFPAttrs(Sel, nullptr /* MDNode* */, FMF);
  return Insert(Sel, Name);
}

Value *IRBuilderBase::CreatePtrDiff(Value *LHS, Value *RHS,
                                    const Twine &Name) {
  assert(LHS->getType() == RHS->getType() &&
         "Pointer subtraction operand types must match!");
  auto *ArgType = cast<PointerType>(LHS->getType());
  Value *LHS_int = CreatePtrToInt(LHS, Type::getInt64Ty(Context));
  Value *RHS_int = CreatePtrToInt(RHS, Type::getInt64Ty(Context));
  Value *Difference = CreateSub(LHS_int, RHS_int);
  return CreateExactSDiv(Difference,
                         ConstantExpr::getSizeOf(ArgType->getElementType()),
                         Name);
}

Value *IRBuilderBase::CreateLaunderInvariantGroup(Value *Ptr) {
  assert(isa<PointerType>(Ptr->getType()) &&
         "launder.invariant.group only applies to pointers.");
  // FIXME: we could potentially avoid casts to/from i8*.
  auto *PtrType = Ptr->getType();
  auto *Int8PtrTy = getInt8PtrTy(PtrType->getPointerAddressSpace());
  if (PtrType != Int8PtrTy)
    Ptr = CreateBitCast(Ptr, Int8PtrTy);
  Module *M = BB->getParent()->getParent();
  Function *FnLaunderInvariantGroup = Intrinsic::getDeclaration(
      M, Intrinsic::launder_invariant_group, {Int8PtrTy});

  assert(FnLaunderInvariantGroup->getReturnType() == Int8PtrTy &&
         FnLaunderInvariantGroup->getFunctionType()->getParamType(0) ==
             Int8PtrTy &&
         "LaunderInvariantGroup should take and return the same type");

  CallInst *Fn = CreateCall(FnLaunderInvariantGroup, {Ptr});

  if (PtrType != Int8PtrTy)
    return CreateBitCast(Fn, PtrType);
  return Fn;
}

Value *IRBuilderBase::CreateStripInvariantGroup(Value *Ptr) {
  assert(isa<PointerType>(Ptr->getType()) &&
         "strip.invariant.group only applies to pointers.");

  // FIXME: we could potentially avoid casts to/from i8*.
  auto *PtrType = Ptr->getType();
  auto *Int8PtrTy = getInt8PtrTy(PtrType->getPointerAddressSpace());
  if (PtrType != Int8PtrTy)
    Ptr = CreateBitCast(Ptr, Int8PtrTy);
  Module *M = BB->getParent()->getParent();
  Function *FnStripInvariantGroup = Intrinsic::getDeclaration(
      M, Intrinsic::strip_invariant_group, {Int8PtrTy});

  assert(FnStripInvariantGroup->getReturnType() == Int8PtrTy &&
         FnStripInvariantGroup->getFunctionType()->getParamType(0) ==
             Int8PtrTy &&
         "StripInvariantGroup should take and return the same type");

  CallInst *Fn = CreateCall(FnStripInvariantGroup, {Ptr});

  if (PtrType != Int8PtrTy)
    return CreateBitCast(Fn, PtrType);
  return Fn;
}

Value *IRBuilderBase::CreateVectorSplat(unsigned NumElts, Value *V,
                                        const Twine &Name) {
  assert(NumElts > 0 && "Cannot splat to an empty vector!");

  // First insert it into an undef vector so we can shuffle it.
  Type *I32Ty = getInt32Ty();
  Value *Undef = UndefValue::get(VectorType::get(V->getType(), NumElts));
  V = CreateInsertElement(Undef, V, ConstantInt::get(I32Ty, 0),
                          Name + ".splatinsert");

  // Shuffle the value across the desired number of elements.
  Value *Zeros = ConstantAggregateZero::get(VectorType::get(I32Ty, NumElts));
  return CreateShuffleVector(V, Undef, Zeros, Name + ".splat");
}

Value *IRBuilderBase::CreateExtractInteger(
    const DataLayout &DL, Value *From, IntegerType *ExtractedTy,
    uint64_t Offset, const Twine &Name) {
  auto *IntTy = cast<IntegerType>(From->getType());
  assert(DL.getTypeStoreSize(ExtractedTy) + Offset <=
             DL.getTypeStoreSize(IntTy) &&
         "Element extends past full value");
  uint64_t ShAmt = 8 * Offset;
  Value *V = From;
  if (DL.isBigEndian())
    ShAmt = 8 * (DL.getTypeStoreSize(IntTy) -
                 DL.getTypeStoreSize(ExtractedTy) - Offset);
  if (ShAmt) {
    V = CreateLShr(V, ShAmt, Name + ".shift");
  }
  assert(ExtractedTy->getBitWidth() <= IntTy->getBitWidth() &&
         "Cannot extract to a larger integer!");
  if (ExtractedTy != IntTy) {
    V = CreateTrunc(V, ExtractedTy, Name + ".trunc");
  }
  return V;
}

Value *IRBuilderBase::CreatePreserveArrayAccessIndex(
    Type *ElTy, Value *Base, unsigned Dimension, unsigned LastIndex,
    MDNode *DbgInfo) {
  assert(isa<PointerType>(Base->getType()) &&
         "Invalid Base ptr type for preserve.array.access.index.");
  auto *BaseType = Base->getType();

  Value *LastIndexV = getInt32(LastIndex);
  Constant *Zero = ConstantInt::get(Type::getInt32Ty(Context), 0);
  SmallVector<Value *, 4> IdxList;
  for (unsigned I = 0; I < Dimension; ++I)
    IdxList.push_back(Zero);
  IdxList.push_back(LastIndexV);

  Type *ResultType =
      GetElementPtrInst::getGEPReturnType(ElTy, Base, IdxList);

  Module *M = BB->getParent()->getParent();
  Function *FnPreserveArrayAccessIndex = Intrinsic::getDeclaration(
      M, Intrinsic::preserve_array_access_index, {ResultType, BaseType});

  Value *DimV = getInt32(Dimension);
  CallInst *Fn =
      CreateCall(FnPreserveArrayAccessIndex, {Base, DimV, LastIndexV});
  if (DbgInfo)
    Fn->setMetadata(LLVMContext::MD_preserve_access_index, DbgInfo);

  return Fn;
}

Value *IRBuilderBase::CreatePreserveUnionAccessIndex(
    Value *Base, unsigned FieldIndex, MDNode *DbgInfo) {
  assert(isa<PointerType>(Base->getType()) &&
         "Invalid Base ptr type for preserve.union.access.index.");
  auto *BaseType = Base->getType();

  Module *M = BB->getParent()->getParent();
  Function *FnPreserveUnionAccessIndex = Intrinsic::getDeclaration(
      M, Intrinsic::preserve_union_access_index, {BaseType, BaseType});

  Value *DIIndex = getInt32(FieldIndex);
  CallInst *Fn =
      CreateCall(FnPreserveUnionAccessIndex, {Base, DIIndex});
  if (DbgInfo)
    Fn->setMetadata(LLVMContext::MD_preserve_access_index, DbgInfo);

  return Fn;
}

Value *IRBuilderBase::CreatePreserveStructAccessIndex(
    Type *ElTy, Value *Base, unsigned Index, unsigned FieldIndex,
    MDNode *DbgInfo) {
  assert(isa<PointerType>(Base->getType()) &&
         "Invalid Base ptr type for preserve.struct.access.index.");
  auto *BaseType = Base->getType();

  Value *GEPIndex = getInt32(Index);
  Constant *Zero = ConstantInt::get(Type::getInt32Ty(Context), 0);
  Type *ResultType =
      GetElementPtrInst::getGEPReturnType(ElTy, Base, {Zero, GEPIndex});

  Module *M = BB->getParent()->getParent();
  Function *FnPreserveStructAccessIndex = Intrinsic::getDeclaration(
      M, Intrinsic::preserve_struct_access_index, {ResultType, BaseType});

  Value *DIIndex = getInt32(FieldIndex);
  CallInst *Fn = CreateCall(FnPreserveStructAccessIndex,
                            {Base, GEPIndex, DIIndex});
  if (DbgInfo)
    Fn->setMetadata(LLVMContext::MD_preserve_access_index, DbgInfo);

  return Fn;
}

CallInst *IRBuilderBase::CreateAlignmentAssumptionHelper(
    const DataLayout &DL, Value *PtrValue, Value *Mask, Type *IntPtrTy,
    Value *OffsetValue, Value **TheCheck) {
  Value *PtrIntValue = CreatePtrToInt(PtrValue, IntPtrTy, "ptrint");

  if (OffsetValue) {
    bool IsOffsetZero = false;
    if (const auto *CI = dyn_cast<ConstantInt>(OffsetValue))
      IsOffsetZero = CI->isZero();

    if (!IsOffsetZero) {
      if (OffsetValue->getType() != IntPtrTy)
        OffsetValue = CreateIntCast(OffsetValue, IntPtrTy, /*isSigned*/ true,
                                    "offsetcast");
      PtrIntValue = CreateSub(PtrIntValue, OffsetValue, "offsetptr");
    }
  }

  Value *Zero = ConstantInt::get(IntPtrTy, 0);
  Value *MaskedPtr = CreateAnd(PtrIntValue, Mask, "maskedptr");
  Value *InvCond = CreateICmpEQ(MaskedPtr, Zero, "maskcond");
  if (TheCheck)
    *TheCheck = InvCond;

  return CreateAssumption(InvCond);
}

CallInst *IRBuilderBase::CreateAlignmentAssumption(
    const DataLayout &DL, Value *PtrValue, unsigned Alignment,
    Value *OffsetValue, Value **TheCheck) {
  assert(isa<PointerType>(PtrValue->getType()) &&
         "trying to create an alignment assumption on a non-pointer?");
  assert(Alignment != 0 && "Invalid Alignment");
  auto *PtrTy = cast<PointerType>(PtrValue->getType());
  Type *IntPtrTy = getIntPtrTy(DL, PtrTy->getAddressSpace());

  Value *Mask = ConstantInt::get(IntPtrTy, Alignment - 1);
  return CreateAlignmentAssumptionHelper(DL, PtrValue, Mask, IntPtrTy,
                                         OffsetValue, TheCheck);
}

CallInst *IRBuilderBase::CreateAlignmentAssumption(
    const DataLayout &DL, Value *PtrValue, Value *Alignment,
    Value *OffsetValue, Value **TheCheck) {
  assert(isa<PointerType>(PtrValue->getType()) &&
         "trying to create an alignment assumption on a non-pointer?");
  auto *PtrTy = cast<PointerType>(PtrValue->getType());
  Type *IntPtrTy = getIntPtrTy(DL, PtrTy->getAddressSpace());

  if (Alignment->getType() != IntPtrTy)
    Alignment = CreateIntCast(Alignment, IntPtrTy, /*isSigned*/ false,
                              "alignmentcast");

  Value *Mask = CreateSub(Alignment, ConstantInt::get(IntPtrTy, 1), "mask");

  return CreateAlignmentAssumptionHelper(DL, PtrValue, Mask, IntPtrTy,
                                         OffsetValue, TheCheck);
}

IRBuilderDefaultInserter::~IRBuilderDefaultInserter() {}
IRBuilderCallbackInserter::~IRBuilderCallbackInserter() {}
IRBuilderFolder::~IRBuilderFolder() {}
void ConstantFolder::anchor() {}
void NoFolder::anchor() {}
