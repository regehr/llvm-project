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
#include <cassert>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define DEBUG_TYPE "dag-slicer"

using namespace llvm;

namespace {

enum class PatternValueType { DataInt, Bool };

struct PatternOp {
  std::string Name;
  SmallVector<unsigned, 3> OperandIndices;
  PatternValueType ResultType = PatternValueType::DataInt;
  SmallVector<PatternValueType, 3> OperandTypes;
  bool IsCommutative = false;
};

enum class PatternTreeKind { Node, Boundary, Constant };

struct PatternTree {
  PatternTreeKind Kind = PatternTreeKind::Boundary;
  PatternValueType ResultType = PatternValueType::DataInt;
  unsigned BitWidth = 0;
  std::string Name;
  std::vector<PatternTree> Children;
  const Value *BoundaryValue = nullptr;
  bool IsCommutative = false;
};

PatternOp makePatternOp(std::string Name, ArrayRef<unsigned> Operands,
                        PatternValueType ResultType,
                        ArrayRef<PatternValueType> OperandTypes,
                        bool IsCommutative = false) {
  PatternOp Op;
  Op.Name = std::move(Name);
  Op.OperandIndices.assign(Operands.begin(), Operands.end());
  Op.ResultType = ResultType;
  Op.OperandTypes.assign(OperandTypes.begin(), OperandTypes.end());
  Op.IsCommutative = IsCommutative;
  assert(Op.OperandIndices.size() == Op.OperandTypes.size());
  return Op;
}

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
    return makePatternOp(std::move(Name), {0}, PatternValueType::DataInt,
                         {PatternValueType::DataInt});
  };
  auto BinaryDataInt = [](std::string Name, bool IsCommutative = false) {
    return makePatternOp(std::move(Name), {0, 1}, PatternValueType::DataInt,
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
    return BinaryDataInt("SshlSat");
  case Intrinsic::ssub_sat:
    return BinaryDataInt("SsubSat");
  case Intrinsic::uadd_sat:
    return BinaryDataInt("UaddSat", true);
  case Intrinsic::umax:
    return BinaryDataInt("Umax", true);
  case Intrinsic::umin:
    return BinaryDataInt("Umin", true);
  case Intrinsic::ushl_sat:
    return BinaryDataInt("UshlSat");
  case Intrinsic::usub_sat:
    return BinaryDataInt("UsubSat");
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
                bool IsCommutative = false) {
    return makePatternOp(Name.str(), Operands, PatternValueType::Bool,
                         {PatternValueType::DataInt, PatternValueType::DataInt},
                         IsCommutative);
  };

  switch (Inst->getPredicate()) {
  case ICmpInst::ICMP_EQ:
    return Cmp("Eq", {0, 1}, true);
  case ICmpInst::ICMP_NE:
    return Cmp("Ne", {0, 1}, true);
  case ICmpInst::ICMP_SLT:
    return Cmp("Slt", {0, 1});
  case ICmpInst::ICMP_SGT:
    return Cmp("Slt", {1, 0});
  case ICmpInst::ICMP_SLE:
    return Cmp("Sle", {0, 1});
  case ICmpInst::ICMP_SGE:
    return Cmp("Sle", {1, 0});
  case ICmpInst::ICMP_ULT:
    return Cmp("Ult", {0, 1});
  case ICmpInst::ICMP_UGT:
    return Cmp("Ult", {1, 0});
  case ICmpInst::ICMP_ULE:
    return Cmp("Ule", {0, 1});
  case ICmpInst::ICMP_UGE:
    return Cmp("Ule", {1, 0});
  default:
    return std::nullopt;
  }
}

std::optional<PatternOp> getSelectPatternOp(const SelectInst *Inst) {
  if (!isBoolType(Inst->getCondition()->getType()) ||
      !isDataIntType(Inst->getTrueValue()->getType()) ||
      !isDataIntType(Inst->getFalseValue()->getType()))
    return std::nullopt;

  return makePatternOp("Select", {0, 1, 2}, PatternValueType::DataInt,
                       {PatternValueType::Bool, PatternValueType::DataInt,
                        PatternValueType::DataInt});
}

std::optional<PatternOp> getCastPatternOp(const CastInst *Inst) {
  switch (Inst->getOpcode()) {
  case Instruction::Trunc:
    if (isDataIntType(Inst->getOperand(0)->getType()) &&
        isBoolType(Inst->getType()))
      return makePatternOp("TruncToBool", {0}, PatternValueType::Bool,
                           {PatternValueType::DataInt});
    return std::nullopt;
  case Instruction::ZExt:
    if (isBoolType(Inst->getOperand(0)->getType()) &&
        isDataIntType(Inst->getType()))
      return makePatternOp("ZextBool", {0}, PatternValueType::DataInt,
                           {PatternValueType::Bool});
    return std::nullopt;
  case Instruction::SExt:
    if (isBoolType(Inst->getOperand(0)->getType()) &&
        isDataIntType(Inst->getType()))
      return makePatternOp("SextBool", {0}, PatternValueType::DataInt,
                           {PatternValueType::Bool});
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
    Op = makePatternOp(Name, {0, 1}, PatternValueType::DataInt,
                       {PatternValueType::DataInt, PatternValueType::DataInt},
                       Inst->isCommutative());
  }

  if (!Op || Op->ResultType != *ResultType ||
      !hasPatternOperandTypes(Inst, *Op))
    return std::nullopt;
  return Op;
}

PatternTree makeConstantTree(const ConstantInt *C) {
  PatternTree Tree;
  Tree.Kind = PatternTreeKind::Constant;
  Tree.ResultType = isBoolType(C->getType()) ? PatternValueType::Bool
                                             : PatternValueType::DataInt;
  Tree.BitWidth = C->getBitWidth();
  Tree.BoundaryValue = C;
  return Tree;
}

std::optional<PatternTree> makeBoundaryTree(const Value *V) {
  if (auto *C = dyn_cast<ConstantInt>(V))
    return makeConstantTree(C);

  auto Ty = getPatternValueType(V->getType());
  if (!Ty)
    return std::nullopt;

  PatternTree Tree;
  Tree.Kind = PatternTreeKind::Boundary;
  Tree.ResultType = *Ty;
  Tree.BitWidth = getBitWidth(V->getType());
  Tree.BoundaryValue = V;
  return Tree;
}

std::optional<PatternTree> buildNodeTree(const Instruction *Inst,
                                         const PatternOp &Op,
                                         ArrayRef<PatternTree> Children) {
  PatternTree Tree;
  Tree.Kind = PatternTreeKind::Node;
  Tree.ResultType = Op.ResultType;
  Tree.BitWidth = getBitWidth(Inst->getType());
  Tree.Name = Op.Name;
  Tree.Children.assign(Children.begin(), Children.end());
  Tree.BoundaryValue = Inst;
  Tree.IsCommutative = Op.IsCommutative;

  return Tree;
}

int getDAGSize(const Value *Val, DenseMap<const Value *, int> &SizeMap) {
  auto It = SizeMap.find(Val);
  if (It != SizeMap.end())
    return It->second;

  auto *Inst = dyn_cast<Instruction>(Val);
  auto Op = Inst ? getPatternOp(Inst) : std::nullopt;
  int Size = Op ? 1 : 0;
  if (Op) {
    for (unsigned OperandIndex : Op->OperandIndices)
      Size += getDAGSize(Inst->getOperand(OperandIndex), SizeMap);
  }
  SizeMap[Val] = Size;
  return Size;
}

std::vector<PatternTree>
enumeratePatternHelper(const Value *Val, int Size,
                       DenseMap<const Value *, int> &SizeMap);

void enumerateSizePartitions(ArrayRef<int> OperandSizes,
                             MutableArrayRef<int> SizePartition,
                             std::size_t OperandNo, int RemainingSize,
                             function_ref<void(ArrayRef<int>)> Callback) {
  if (OperandNo == OperandSizes.size()) {
    if (RemainingSize == 0)
      Callback(SizePartition);
    return;
  }

  int RemainingCapacity = 0;
  for (std::size_t I = OperandNo + 1; I < OperandSizes.size(); ++I)
    RemainingCapacity += OperandSizes[I];

  int UpperBound = std::min(OperandSizes[OperandNo], RemainingSize);
  int LowerBound = std::max(0, RemainingSize - RemainingCapacity);
  for (int OperandSize = LowerBound; OperandSize <= UpperBound; ++OperandSize) {
    SizePartition[OperandNo] = OperandSize;
    enumerateSizePartitions(OperandSizes, SizePartition, OperandNo + 1,
                            RemainingSize - OperandSize, Callback);
  }
}

std::optional<SmallVector<std::vector<PatternTree>, 3>>
getOperandPatternTreesForPartition(const Instruction *Inst, const PatternOp &Op,
                                   ArrayRef<int> SizePartition,
                                   DenseMap<const Value *, int> &SizeMap) {
  SmallVector<std::vector<PatternTree>, 3> OperandTrees;
  for (std::size_t I = 0; I < Op.OperandIndices.size(); ++I) {
    OperandTrees.push_back(enumeratePatternHelper(
        Inst->getOperand(Op.OperandIndices[I]), SizePartition[I], SizeMap));
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
    if (auto Tree = buildNodeTree(Inst, Op, ChosenOperands))
      Result.push_back(std::move(*Tree));
    return;
  }

  for (const PatternTree &OperandTree : OperandTrees[OperandNo]) {
    ChosenOperands[OperandNo] = OperandTree;
    enumerateOperandProducts(Inst, Op, OperandTrees, ChosenOperands, Result,
                             OperandNo + 1);
  }
}

std::vector<PatternTree>
enumeratePatternHelper(const Value *Val, int Size,
                       DenseMap<const Value *, int> &SizeMap) {
  auto *Inst = dyn_cast<Instruction>(Val);
  if (!Inst || Size == 0) {
    if (auto Boundary = makeBoundaryTree(Val))
      return {*Boundary};
  }

  if (!Inst)
    return {};

  auto Op = getPatternOp(Inst);
  if (!Op) {
    if (auto Boundary = makeBoundaryTree(Val))
      return {*Boundary};
    return {};
  }

  std::vector<PatternTree> Result;
  SmallVector<int, 3> OperandSizes;
  for (unsigned OperandIndex : Op->OperandIndices)
    OperandSizes.push_back(getDAGSize(Inst->getOperand(OperandIndex), SizeMap));

  SmallVector<int, 3> SizePartition(Op->OperandIndices.size(), 0);
  enumerateSizePartitions(
      OperandSizes, SizePartition, 0, Size - 1, [&](ArrayRef<int> Partition) {
        auto OperandTrees =
            getOperandPatternTreesForPartition(Inst, *Op, Partition, SizeMap);
        if (!OperandTrees)
          return;

        SmallVector<PatternTree, 3> ChosenOperands(Op->OperandIndices.size());
        enumerateOperandProducts(Inst, *Op, *OperandTrees, ChosenOperands,
                                 Result);
      });
  return Result;
}

struct RenderState {
  bool SymbolizeConstants = false;
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
    if (State.SymbolizeConstants) {
      OS << "C" << State.NextConstantNumber++;
      return;
    }
    OS << "const";
    return;
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

std::optional<std::string> getCanonicalSortKey(const PatternTree &Tree,
                                               bool SymbolizeConstants);

void canonicalizePatternTree(PatternTree &Tree, bool SymbolizeConstants) {
  for (PatternTree &Child : Tree.Children)
    canonicalizePatternTree(Child, SymbolizeConstants);

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

  auto LHS = getCanonicalSortKey(Tree.Children[0], SymbolizeConstants);
  auto RHS = getCanonicalSortKey(Tree.Children[1], SymbolizeConstants);
  if (LHS && RHS && *RHS < *LHS)
    std::swap(Tree.Children[0], Tree.Children[1]);
}

std::optional<std::string> renderCanonicalPatternTree(PatternTree Tree,
                                                      bool SymbolizeConstants) {
  canonicalizePatternTree(Tree, SymbolizeConstants);

  RenderState State;
  State.SymbolizeConstants = SymbolizeConstants;
  State.ResultBitWidth = Tree.BitWidth;
  return renderPatternWithState(Tree, State);
}

std::optional<std::string> renderPattern(const PatternTree &Tree,
                                         bool SymbolizeConstants) {
  if (Tree.ResultType != PatternValueType::DataInt)
    return std::nullopt;

  return renderCanonicalPatternTree(Tree, SymbolizeConstants);
}

std::optional<std::string> getCanonicalSortKey(const PatternTree &Tree,
                                               bool SymbolizeConstants) {
  return renderCanonicalPatternTree(Tree, SymbolizeConstants);
}

SmallVector<unsigned, 8> getPatternSizes(const DAGSlicer::Config &Cfg) {
  if (!Cfg.PatternSizes.empty())
    return Cfg.PatternSizes;

  SmallVector<unsigned, 8> Sizes;
  for (unsigned Size = 2; Size <= Cfg.MaxPatternSize; ++Size)
    Sizes.push_back(Size);
  return Sizes;
}

} // namespace

namespace llvm::DAGSlicer {

const Config &getDefaultConfig() {
  static const Config Cfg;
  return Cfg;
}

void enumeratePatterns(
    const Value *Root, const Config &Cfg,
    function_ref<void(unsigned PatternSize, StringRef Pattern)> Callback) {
  auto *Inst = dyn_cast_or_null<Instruction>(Root);
  auto RootOp = Inst ? getPatternOp(Inst) : std::nullopt;
  if (!RootOp || RootOp->ResultType != PatternValueType::DataInt)
    return;

  DenseMap<const Value *, int> SizeMap;
  int RootSize = getDAGSize(Root, SizeMap);
  for (unsigned PatternSize : getPatternSizes(Cfg)) {
    if (PatternSize == 0 || static_cast<int>(PatternSize) > RootSize)
      continue;

    for (const PatternTree &Tree :
         enumeratePatternHelper(Root, PatternSize, SizeMap)) {
      if (auto Pattern = renderPattern(Tree, Cfg.SymbolizeConstants))
        Callback(PatternSize, *Pattern);
    }
  }
}

void recordPatterns(const Value *Root, unsigned KnownBitsDepth,
                    const Config &Cfg) {
  LLVM_DEBUG(enumeratePatterns(
      Root, Cfg, [&](unsigned PatternSize, StringRef Pattern) {
        dbgs() << "DAGSLICER\t" << KnownBitsDepth << '\t' << PatternSize << '\t'
               << Pattern << '\n';
      }));
}

} // namespace llvm::DAGSlicer

#undef DEBUG_TYPE
