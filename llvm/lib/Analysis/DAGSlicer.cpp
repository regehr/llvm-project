#include "DAGSlicer.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <cassert>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define DEBUG_TYPE "dag-slicer"

using namespace llvm;

namespace {

static constexpr bool AllowExternalBoolPatternValues = true;

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

enum class PatternTreeKind { Node, Boundary };

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
    return Cmp("ICmpEq", {0, 1}, true);
  case ICmpInst::ICMP_NE:
    return Cmp("ICmpNe", {0, 1}, true);
  case ICmpInst::ICMP_SLT:
    return Cmp("ICmpSlt", {0, 1}, false);
  case ICmpInst::ICMP_SGT:
    return Cmp("ICmpSlt", {1, 0}, false);
  case ICmpInst::ICMP_SLE:
    return Cmp("ICmpSle", {0, 1}, false);
  case ICmpInst::ICMP_SGE:
    return Cmp("ICmpSle", {1, 0}, false);
  case ICmpInst::ICMP_ULT:
    return Cmp("ICmpUlt", {0, 1}, false);
  case ICmpInst::ICMP_UGT:
    return Cmp("ICmpUlt", {1, 0}, false);
  case ICmpInst::ICMP_ULE:
    return Cmp("ICmpUle", {0, 1}, false);
  case ICmpInst::ICMP_UGE:
    return Cmp("ICmpUle", {1, 0}, false);
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
  auto Ty = getPatternValueType(V->getType());
  if (!Ty)
    return std::nullopt;

  return PatternTree::makeBoundary(V, *Ty, getBitWidth(V->getType()));
}

std::vector<PatternTree> enumeratePatternHelper(const Value *Val,
                                                unsigned RemainingDepth);

std::optional<SmallVector<std::vector<PatternTree>, 3>>
getOperandPatternTrees(const Instruction *Inst, const PatternOp &Op,
                       unsigned RemainingDepth) {
  SmallVector<std::vector<PatternTree>, 3> OperandTrees;
  for (std::size_t I = 0; I < Op.OperandIndices.size(); ++I) {
    OperandTrees.push_back(enumeratePatternHelper(
        Inst->getOperand(Op.OperandIndices[I]), RemainingDepth));
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
                                                unsigned RemainingDepth) {
  std::vector<PatternTree> Result;

  if (auto Boundary = makeBoundaryTree(Val))
    Result.push_back(*Boundary);

  if (RemainingDepth == 0)
    return Result;

  auto *Inst = dyn_cast<Instruction>(Val);
  if (!Inst)
    return Result;

  auto Op = getPatternOp(Inst);
  if (!Op)
    return Result;

  auto OperandTrees = getOperandPatternTrees(Inst, *Op, RemainingDepth - 1);
  if (!OperandTrees)
    return Result;

  SmallVector<PatternTree, 3> ChosenOperands;
  ChosenOperands.reserve(Op->OperandIndices.size());
  enumerateOperandProducts(Inst, *Op, *OperandTrees, ChosenOperands, Result);

  return Result;
}

unsigned getPatternDepth(const PatternTree &Tree) {
  if (Tree.Kind != PatternTreeKind::Node)
    return 0;

  unsigned Depth = 0;
  for (const PatternTree &Child : Tree.Children)
    Depth = std::max(Depth, getPatternDepth(Child));
  return Depth + 1;
}

struct RenderState {
  std::optional<unsigned> DataIntBitWidth;
  DenseMap<const Value *, unsigned> ArgumentNumbers;
  bool Valid = true;
};

bool notePatternValue(const PatternTree &Tree, RenderState &State) {
  if (Tree.ResultType != PatternValueType::DataInt)
    return true;

  if (!State.DataIntBitWidth) {
    State.DataIntBitWidth = Tree.BitWidth;
    return true;
  }

  return Tree.BitWidth == *State.DataIntBitWidth;
}

bool isValidExternalPatternValue(const PatternTree &Tree, RenderState &State) {
  if (!notePatternValue(Tree, State))
    return false;

  if constexpr (AllowExternalBoolPatternValues) {
    if (Tree.ResultType == PatternValueType::Bool)
      return Tree.BitWidth == 1;
  }

  return Tree.ResultType == PatternValueType::DataInt;
}

void renderPattern(const PatternTree &Tree, RenderState &State,
                   raw_ostream &OS) {
  if (!State.Valid)
    return;

  switch (Tree.Kind) {
  case PatternTreeKind::Boundary: {
    if (!isValidExternalPatternValue(Tree, State)) {
      State.Valid = false;
      return;
    }
    auto Inserted = State.ArgumentNumbers.try_emplace(
        Tree.BoundaryValue, State.ArgumentNumbers.size());
    OS << "arg" << Inserted.first->second;
    return;
  }
  case PatternTreeKind::Node:
    if (!notePatternValue(Tree, State)) {
      State.Valid = false;
      return;
    }
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
  return renderPatternWithState(Tree, State);
}

std::optional<std::string> renderPattern(const PatternTree &Tree) {
  if constexpr (!AllowExternalBoolPatternValues) {
    if (Tree.ResultType != PatternValueType::DataInt)
      return std::nullopt;
  } else {
    if (Tree.ResultType == PatternValueType::Bool && Tree.BitWidth != 1)
      return std::nullopt;
  }

  if (Tree.ResultType != PatternValueType::DataInt &&
      Tree.ResultType != PatternValueType::Bool)
    return std::nullopt;

  return renderCanonicalPatternTree(Tree);
}

std::optional<std::string> getCanonicalSortKey(const PatternTree &Tree) {
  return renderCanonicalPatternTree(Tree);
}

} // namespace

namespace llvm::DAGSlicer {

void enumeratePatterns(const Value *Root, unsigned MinDepth, unsigned MaxDepth,
                       function_ref<void(StringRef Pattern)> Callback) {
  auto *Inst = dyn_cast_or_null<Instruction>(Root);
  auto RootOp = Inst ? getPatternOp(Inst) : std::nullopt;
  if (!RootOp)
    return;

  for (const PatternTree &Tree : enumeratePatternHelper(Root, MaxDepth)) {
    unsigned Depth = getPatternDepth(Tree);
    if (Depth < MinDepth || Depth > MaxDepth)
      continue;

    if (auto Pattern = renderPattern(Tree))
      Callback(*Pattern);
  }
}

void recordPatterns(const Value *Root, unsigned AnalysisDepth,
                    unsigned MinDepth, unsigned MaxDepth) {
  assert(MinDepth <= MaxDepth);
  assert(AnalysisDepth <= MaxDepth && "analysis depth exceeds max depth");
  unsigned RemainingDepth = MaxDepth - AnalysisDepth;
  if (RemainingDepth < MinDepth)
    return;

  LLVM_DEBUG(
      enumeratePatterns(Root, MinDepth, RemainingDepth,
                        [&](StringRef Pattern) { dbgs() << Pattern << '\n'; }));
}

} // namespace llvm::DAGSlicer

#undef DEBUG_TYPE
