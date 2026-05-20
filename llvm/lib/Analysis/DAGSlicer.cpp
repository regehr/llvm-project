#include "DAGSlicer.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define DEBUG_TYPE "dag-slicer"

using namespace llvm;

namespace {

// config options
static constexpr unsigned MaxPatternSize = 5;
static constexpr bool SymbolizeConstants = false;
constexpr auto makePatternSizes() {
  std::array<unsigned, MaxPatternSize - 1> Sizes{};
  for (unsigned I = 0; I < Sizes.size(); ++I)
    Sizes[I] = I + 2;
  return Sizes;
}
static constexpr auto PatternSizes = makePatternSizes();
// end config

enum class PatternValueType { DataInt, Bool };

struct PatternOp {
  PatternOp(std::string Name, ArrayRef<unsigned> Operands,
            PatternValueType ResultType,
            ArrayRef<PatternValueType> OperandTypes, bool IsCommutative)
      : Name(std::move(Name)), ResultType(ResultType),
        IsCommutative(IsCommutative) {
    OperandIndices.assign(Operands.begin(), Operands.end());
    this->OperandTypes.assign(OperandTypes.begin(), OperandTypes.end());
    assert(OperandIndices.size() == this->OperandTypes.size());
  }

  std::string Name;
  SmallVector<unsigned, 3> OperandIndices;
  PatternValueType ResultType;
  SmallVector<PatternValueType, 3> OperandTypes;
  bool IsCommutative;
};

enum class PatternTreeKind { Node, Boundary, Constant };

bool isBoolType(Type *Ty) { return Ty->isIntegerTy(1); }

bool isDataIntType(Type *Ty) { return Ty->isIntegerTy() && !isBoolType(Ty); }

std::optional<PatternValueType> getPatternValueType(Type *Ty) {
  if (isDataIntType(Ty))
    return PatternValueType::DataInt;
  if (isBoolType(Ty))
    return PatternValueType::Bool;
  return std::nullopt;
}

unsigned getBitWidth(Type *Ty) {
  auto *IntTy = dyn_cast<IntegerType>(Ty);
  assert(IntTy && "expected integer type");
  return IntTy->getBitWidth();
}

bool getConstantBool(const Value *Val) {
  auto *C = dyn_cast<ConstantInt>(Val);
  return C && !C->isZero();
}

struct PatternTree {
  static PatternTree makeNode(const Instruction *Inst, const PatternOp &Op,
                              ArrayRef<PatternTree> Children) {
    PatternTree Tree(PatternTreeKind::Node, Op.ResultType,
                     getBitWidth(Inst->getType()), Inst);
    Tree.Name = Op.Name;
    Tree.Children.assign(Children.begin(), Children.end());
    Tree.IsCommutative = Op.IsCommutative;
    return Tree;
  }

  static PatternTree makeBoundary(const Value *V, PatternValueType ResultType,
                                  unsigned BitWidth) {
    return PatternTree(PatternTreeKind::Boundary, ResultType, BitWidth, V);
  }

  static PatternTree makeConstant(const ConstantInt *C) {
    PatternTree Tree(PatternTreeKind::Constant,
                     isBoolType(C->getType()) ? PatternValueType::Bool
                                              : PatternValueType::DataInt,
                     C->getBitWidth(), C);
    return Tree;
  }

  PatternTreeKind Kind;
  PatternValueType ResultType;
  unsigned BitWidth;
  std::string Name;
  std::vector<PatternTree> Children;
  const Value *BoundaryValue;
  bool IsCommutative;

private:
  PatternTree(PatternTreeKind Kind, PatternValueType ResultType,
              unsigned BitWidth, const Value *BoundaryValue)
      : Kind(Kind), ResultType(ResultType), BitWidth(BitWidth),
        BoundaryValue(BoundaryValue), IsCommutative(false) {}
};

bool isZeroConstant(const Value *Val) {
  auto *C = dyn_cast<ConstantInt>(Val);
  return C && C->isZero();
}

bool hasPatternOperandTypes(const Instruction *Inst, const PatternOp &Op) {
  for (std::size_t I = 0; I < Op.OperandIndices.size(); ++I) {
    auto OperandType =
        getPatternValueType(Inst->getOperand(Op.OperandIndices[I])->getType());
    if (!OperandType || *OperandType != Op.OperandTypes[I])
      return false;
  }
  return true;
}

std::optional<PatternOp> getIntrinsicPatternOp(const CallBase *Call) {
  auto UnaryDataInt = [](std::string Name) {
    return PatternOp(std::move(Name), {0}, PatternValueType::DataInt,
                     {PatternValueType::DataInt}, false);
  };
  auto BinaryDataInt = [](std::string Name, bool IsCommutative) {
    return PatternOp(std::move(Name), {0, 1}, PatternValueType::DataInt,
                     {PatternValueType::DataInt, PatternValueType::DataInt},
                     IsCommutative);
  };

  switch (Call->getIntrinsicID()) {
  case Intrinsic::abs:
    return UnaryDataInt(getConstantBool(Call->getArgOperand(1)) ? "AbsUndef"
                                                                : "Abs");
  case Intrinsic::ctlz:
    return UnaryDataInt(getConstantBool(Call->getArgOperand(1))
                            ? "CountLZeroUndef"
                            : "CountLZero");
  case Intrinsic::cttz:
    return UnaryDataInt(getConstantBool(Call->getArgOperand(1))
                            ? "CountRZeroUndef"
                            : "CountRZero");
  case Intrinsic::ctpop:
    return UnaryDataInt("PopCount");
  case Intrinsic::sadd_sat:
    return BinaryDataInt("SaddSat", true);
  case Intrinsic::smax:
    return BinaryDataInt("Smax", true);
  case Intrinsic::smin:
    return BinaryDataInt("Smin", true);
  case Intrinsic::sshl_sat:
    return BinaryDataInt("SshlSat", false);
  case Intrinsic::ssub_sat:
    return BinaryDataInt("SsubSat", false);
  case Intrinsic::uadd_sat:
    return BinaryDataInt("UaddSat", true);
  case Intrinsic::umax:
    return BinaryDataInt("Umax", true);
  case Intrinsic::umin:
    return BinaryDataInt("Umin", true);
  case Intrinsic::ushl_sat:
    return BinaryDataInt("UshlSat", false);
  case Intrinsic::usub_sat:
    return BinaryDataInt("UsubSat", false);
  case Intrinsic::smul_fix_sat:
    if (isZeroConstant(Call->getArgOperand(2)))
      return BinaryDataInt("SmulSat", true);
    return std::nullopt;
  case Intrinsic::umul_fix_sat:
    if (isZeroConstant(Call->getArgOperand(2)))
      return BinaryDataInt("UmulSat", true);
    return std::nullopt;
  default:
    return std::nullopt;
  }
}

std::string getOpcodeName(const Instruction *Inst) {
  std::string Name;
  switch (Inst->getOpcode()) {
  case Instruction::Add:
    Name = "Add";
    break;
  case Instruction::Sub:
    Name = "Sub";
    break;
  case Instruction::Mul:
    Name = "Mul";
    break;
  case Instruction::UDiv:
    Name = "Udiv";
    break;
  case Instruction::SDiv:
    Name = "Sdiv";
    break;
  case Instruction::URem:
    Name = "Urem";
    break;
  case Instruction::SRem:
    Name = "Srem";
    break;
  case Instruction::Shl:
    Name = "Shl";
    break;
  case Instruction::LShr:
    Name = "Lshr";
    break;
  case Instruction::AShr:
    Name = "Ashr";
    break;
  case Instruction::And:
    Name = "And";
    break;
  case Instruction::Or:
    Name = "Or";
    break;
  case Instruction::Xor:
    Name = "Xor";
    break;
  default:
    return "";
  }

  if (auto *OBO = dyn_cast<OverflowingBinaryOperator>(Inst)) {
    if (OBO->hasNoSignedWrap())
      Name += "Nsw";
    if (OBO->hasNoUnsignedWrap())
      Name += "Nuw";
  }
  if (auto *PEO = dyn_cast<PossiblyExactOperator>(Inst); PEO && PEO->isExact())
    Name += "Exact";
  if (auto *PDI = dyn_cast<PossiblyDisjointInst>(Inst);
      PDI && PDI->isDisjoint())
    Name += "Disjoint";
  return Name;
}

std::optional<PatternOp> getIcmpPatternOp(const ICmpInst *Inst) {
  if (!isDataIntType(Inst->getOperand(0)->getType()) ||
      !isDataIntType(Inst->getOperand(1)->getType()))
    return std::nullopt;

  auto Cmp = [](StringRef Name, ArrayRef<unsigned> Operands,
                bool IsCommutative) {
    return PatternOp(Name.str(), Operands, PatternValueType::Bool,
                     {PatternValueType::DataInt, PatternValueType::DataInt},
                     IsCommutative);
  };

  switch (Inst->getPredicate()) {
  case ICmpInst::ICMP_EQ:
    return Cmp("Eq", {0, 1}, true);
  case ICmpInst::ICMP_NE:
    return Cmp("Ne", {0, 1}, true);
  case ICmpInst::ICMP_SLT:
    return Cmp("Slt", {0, 1}, false);
  case ICmpInst::ICMP_SGT:
    return Cmp("Slt", {1, 0}, false);
  case ICmpInst::ICMP_SLE:
    return Cmp("Sle", {0, 1}, false);
  case ICmpInst::ICMP_SGE:
    return Cmp("Sle", {1, 0}, false);
  case ICmpInst::ICMP_ULT:
    return Cmp("Ult", {0, 1}, false);
  case ICmpInst::ICMP_UGT:
    return Cmp("Ult", {1, 0}, false);
  case ICmpInst::ICMP_ULE:
    return Cmp("Ule", {0, 1}, false);
  case ICmpInst::ICMP_UGE:
    return Cmp("Ule", {1, 0}, false);
  default:
    return std::nullopt;
  }
}

std::optional<PatternOp> getSelectPatternOp(const SelectInst *Inst) {
  if (!isBoolType(Inst->getCondition()->getType()) ||
      !isDataIntType(Inst->getTrueValue()->getType()) ||
      !isDataIntType(Inst->getFalseValue()->getType()))
    return std::nullopt;

  return PatternOp("Select", {0, 1, 2}, PatternValueType::DataInt,
                   {PatternValueType::Bool, PatternValueType::DataInt,
                    PatternValueType::DataInt},
                   false);
}

std::optional<PatternOp> getCastPatternOp(const CastInst *Inst) {
  switch (Inst->getOpcode()) {
  case Instruction::Trunc:
    if (isDataIntType(Inst->getOperand(0)->getType()) &&
        isBoolType(Inst->getType()))
      return PatternOp("TruncToBool", {0}, PatternValueType::Bool,
                       {PatternValueType::DataInt}, false);
    return std::nullopt;
  case Instruction::ZExt:
    if (isBoolType(Inst->getOperand(0)->getType()) &&
        isDataIntType(Inst->getType()))
      return PatternOp("ZextBool", {0}, PatternValueType::DataInt,
                       {PatternValueType::Bool}, false);
    return std::nullopt;
  case Instruction::SExt:
    if (isBoolType(Inst->getOperand(0)->getType()) &&
        isDataIntType(Inst->getType()))
      return PatternOp("SextBool", {0}, PatternValueType::DataInt,
                       {PatternValueType::Bool}, false);
    return std::nullopt;
  default:
    return std::nullopt;
  }
}

std::optional<PatternOp> getPatternOp(const Instruction *Inst) {
  auto ResultType = getPatternValueType(Inst->getType());
  if (!ResultType)
    return std::nullopt;

  std::optional<PatternOp> Op;
  if (auto *Call = dyn_cast<CallBase>(Inst)) {
    Op = getIntrinsicPatternOp(Call);
  } else if (auto *Icmp = dyn_cast<ICmpInst>(Inst)) {
    Op = getIcmpPatternOp(Icmp);
  } else if (auto *Select = dyn_cast<SelectInst>(Inst)) {
    Op = getSelectPatternOp(Select);
  } else if (auto *Cast = dyn_cast<CastInst>(Inst)) {
    Op = getCastPatternOp(Cast);
  } else if (Inst->isBinaryOp()) {
    if (!isDataIntType(Inst->getType()))
      return std::nullopt;
    std::string Name = getOpcodeName(Inst);
    if (Name.empty())
      return std::nullopt;
    Op = PatternOp(Name, {0, 1}, PatternValueType::DataInt,
                   {PatternValueType::DataInt, PatternValueType::DataInt},
                   Inst->isCommutative());
  }

  if (!Op || Op->ResultType != *ResultType ||
      !hasPatternOperandTypes(Inst, *Op))
    return std::nullopt;
  return Op;
}

std::optional<PatternTree> makeBoundaryTree(const Value *V) {
  if constexpr (SymbolizeConstants) {
    if (auto *C = dyn_cast<ConstantInt>(V))
      return PatternTree::makeConstant(C);
  }

  auto Ty = getPatternValueType(V->getType());
  if (!Ty)
    return std::nullopt;

  return PatternTree::makeBoundary(V, *Ty, getBitWidth(V->getType()));
}

std::vector<PatternTree> enumeratePatternHelper(const Value *Val,
                                                unsigned Size);

void enumerateSizePartitions(unsigned NumOperands,
                             MutableArrayRef<unsigned> SizePartition,
                             std::size_t OperandNo, unsigned RemainingSize,
                             function_ref<void(ArrayRef<unsigned>)> Callback) {
  if (OperandNo == NumOperands) {
    if (RemainingSize == 0)
      Callback(SizePartition);
    return;
  }

  for (unsigned OperandSize = 0; OperandSize <= RemainingSize; ++OperandSize) {
    SizePartition[OperandNo] = OperandSize;
    enumerateSizePartitions(NumOperands, SizePartition, OperandNo + 1,
                            RemainingSize - OperandSize, Callback);
  }
}

std::optional<SmallVector<std::vector<PatternTree>, 3>>
getOperandPatternTreesForPartition(const Instruction *Inst, const PatternOp &Op,
                                   ArrayRef<unsigned> SizePartition) {
  SmallVector<std::vector<PatternTree>, 3> OperandTrees;
  for (std::size_t I = 0; I < Op.OperandIndices.size(); ++I) {
    OperandTrees.push_back(enumeratePatternHelper(
        Inst->getOperand(Op.OperandIndices[I]), SizePartition[I]));
    if (OperandTrees.back().empty())
      return std::nullopt;
  }
  return OperandTrees;
}

void enumerateOperandProducts(const Instruction *Inst, const PatternOp &Op,
                              ArrayRef<std::vector<PatternTree>> OperandTrees,
                              SmallVectorImpl<PatternTree> &ChosenOperands,
                              std::vector<PatternTree> &Result,
                              std::size_t OperandNo = 0) {
  if (OperandNo == Op.OperandIndices.size()) {
    Result.push_back(PatternTree::makeNode(Inst, Op, ChosenOperands));
    return;
  }

  for (const PatternTree &OperandTree : OperandTrees[OperandNo]) {
    ChosenOperands.push_back(OperandTree);
    enumerateOperandProducts(Inst, Op, OperandTrees, ChosenOperands, Result,
                             OperandNo + 1);
    ChosenOperands.pop_back();
  }
}

std::vector<PatternTree> enumeratePatternHelper(const Value *Val,
                                                unsigned Size) {
  if (Size == 0) {
    if (auto Boundary = makeBoundaryTree(Val))
      return {*Boundary};
    return {};
  }

  auto *Inst = dyn_cast<Instruction>(Val);
  if (!Inst)
    return {};

  auto Op = getPatternOp(Inst);
  if (!Op)
    return {};

  std::vector<PatternTree> Result;

  SmallVector<unsigned, 3> SizePartition(Op->OperandIndices.size(), 0);
  enumerateSizePartitions(Op->OperandIndices.size(), SizePartition, 0, Size - 1,
                          [&](ArrayRef<unsigned> Partition) {
                            auto OperandTrees =
                                getOperandPatternTreesForPartition(Inst, *Op,
                                                                   Partition);
                            if (!OperandTrees)
                              return;

                            SmallVector<PatternTree, 3> ChosenOperands;
                            ChosenOperands.reserve(Op->OperandIndices.size());
                            enumerateOperandProducts(Inst, *Op, *OperandTrees,
                                                     ChosenOperands, Result);
                          });

  return Result;
}

struct RenderState {
  unsigned ResultBitWidth = 0;
  DenseMap<const Value *, unsigned> ArgumentNumbers;
  unsigned NextConstantNumber = 0;
  bool Valid = true;
};

void renderPattern(const PatternTree &Tree, RenderState &State,
                   raw_ostream &OS) {
  if (!State.Valid)
    return;

  switch (Tree.Kind) {
  case PatternTreeKind::Constant:
    if constexpr (!SymbolizeConstants) {
      llvm_unreachable("symbolized constants are off, but found a const node");
    } else {
      OS << "C" << State.NextConstantNumber++;
      return;
    }
  case PatternTreeKind::Boundary: {
    if (Tree.ResultType != PatternValueType::DataInt ||
        Tree.BitWidth != State.ResultBitWidth) {
      State.Valid = false;
      return;
    }
    auto Inserted = State.ArgumentNumbers.try_emplace(
        Tree.BoundaryValue, State.ArgumentNumbers.size());
    OS << "arg" << Inserted.first->second;
    return;
  }
  case PatternTreeKind::Node:
    OS << Tree.Name << "(";
    for (std::size_t I = 0; I < Tree.Children.size(); ++I) {
      if (I)
        OS << ", ";
      renderPattern(Tree.Children[I], State, OS);
    }
    OS << ")";
    return;
  }
}

std::optional<std::string> renderPatternWithState(const PatternTree &Tree,
                                                  RenderState &State) {
  std::string Pattern;
  raw_string_ostream OS(Pattern);
  renderPattern(Tree, State, OS);
  if (!State.Valid)
    return std::nullopt;
  return OS.str();
}

std::optional<std::string> getCanonicalSortKey(const PatternTree &Tree);

void canonicalizePatternTree(PatternTree &Tree) {
  for (PatternTree &Child : Tree.Children)
    canonicalizePatternTree(Child);

  // Canonicalization contract:
  // - Patterns are trees; repeated subexpressions are rendered structurally,
  //   not as shared DAG bindings.
  // - Children are canonicalized recursively first.
  // - Only local commutative operands are sorted, by canonical subtree text.
  //   Future matchers should use m_c_* / m_c_SpecificICmp for these nodes.
  // - Swapped icmp predicates are normalized in getIcmpPatternOp().
  // - We intentionally do not canonicalize associativity, algebraic
  //   identities, or sharing constraints.
  if (!Tree.IsCommutative || Tree.Children.size() != 2)
    return;

  auto LHS = getCanonicalSortKey(Tree.Children[0]);
  auto RHS = getCanonicalSortKey(Tree.Children[1]);
  if (LHS && RHS && *RHS < *LHS)
    std::swap(Tree.Children[0], Tree.Children[1]);
}

std::optional<std::string> renderCanonicalPatternTree(PatternTree Tree) {
  canonicalizePatternTree(Tree);

  RenderState State;
  State.ResultBitWidth = Tree.BitWidth;
  return renderPatternWithState(Tree, State);
}

std::optional<std::string> renderPattern(const PatternTree &Tree) {
  if (Tree.ResultType != PatternValueType::DataInt)
    return std::nullopt;

  return renderCanonicalPatternTree(Tree);
}

std::optional<std::string> getCanonicalSortKey(const PatternTree &Tree) {
  return renderCanonicalPatternTree(Tree);
}

} // namespace

namespace llvm::DAGSlicer {

void enumeratePatterns(
    const Value *Root,
    function_ref<void(unsigned PatternSize, StringRef Pattern)> Callback) {
  auto *Inst = dyn_cast_or_null<Instruction>(Root);
  auto RootOp = Inst ? getPatternOp(Inst) : std::nullopt;
  if (!RootOp || RootOp->ResultType != PatternValueType::DataInt)
    return;

  for (unsigned PatternSize : PatternSizes) {
    for (const PatternTree &Tree : enumeratePatternHelper(Root, PatternSize)) {
      if (auto Pattern = renderPattern(Tree))
        Callback(PatternSize, *Pattern);
    }
  }
}

void recordPatterns(const Value *Root, unsigned KnownBitsDepth) {
  LLVM_DEBUG(
      enumeratePatterns(Root, [&](unsigned PatternSize, StringRef Pattern) {
        dbgs() << "DAGSLICER\t" << KnownBitsDepth << '\t' << PatternSize << '\t'
               << Pattern << '\n';
      }));
}

} // namespace llvm::DAGSlicer

#undef DEBUG_TYPE
