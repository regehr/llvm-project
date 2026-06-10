#include "DAGSlicer.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/ScopeExit.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

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
    Name = "Modu";
    break;
  case Instruction::SRem:
    Name = "Mods";
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

// A single node of a truncated, sharing-preserving DAG. Mirrors PatternOp's
// view of an instruction, but references its operands by node id so reused
// values are not unfolded.
struct DAGNodeInfo {
  bool IsBoundary = false;
  std::string Name;
  PatternValueType ResultType = PatternValueType::DataInt;
  unsigned BitWidth = 0;
  bool IsCommutative = false;
  SmallVector<unsigned, 3> Operands;
};

// Builds the maximal truncation of the value graph rooted at a given Value,
// assigning one id per distinct Value. A value reached by several paths is
// expanded to the deepest budget any path allows; edges do not carry local
// truncation state, so all uses refer to that maximal node.
class TruncatedDAGBuilder {
public:
  unsigned build(const Value *V, unsigned Remaining) {
    // Some optimization passes temporarily create cyclic instruction graphs and
    // call value analyses before repairing the IR. Cut those backedges at a
    // boundary so mining cannot recurse forever or serialize a cyclic graph.
    if (InProgress.contains(V))
      return makeBoundaryNode(V);

    auto VisitedIt = ExpandedRemaining.find(V);
    bool Known = VisitedIt != ExpandedRemaining.end();
    unsigned Id;
    if (Known) {
      Id = NodeIds[V];
      if (VisitedIt->second >= Remaining)
        return Id;
    } else {
      Id = Nodes.size();
      Nodes.emplace_back();
      NodeIds[V] = Id;
    }
    ExpandedRemaining[V] = Remaining;
    InProgress.insert(V);
    auto RemoveFromInProgress = make_scope_exit([&] { InProgress.erase(V); });

    const auto *Inst = dyn_cast<Instruction>(V);
    std::optional<PatternOp> Op =
        (Remaining > 0 && Inst) ? getPatternOp(Inst) : std::nullopt;

    DAGNodeInfo Info;
    if (!Op) {
      Info = makeBoundaryInfo(V);
    } else {
      Info.Name = Op->Name;
      Info.ResultType = Op->ResultType;
      Info.BitWidth = getBitWidth(Inst->getType());
      Info.IsCommutative = Op->IsCommutative;
      for (unsigned OperandIdx : Op->OperandIndices)
        Info.Operands.push_back(
            build(Inst->getOperand(OperandIdx), Remaining - 1));
    }

    Nodes[Id] = std::move(Info);
    return Id;
  }

  ArrayRef<DAGNodeInfo> nodes() const { return Nodes; }

private:
  DAGNodeInfo makeBoundaryInfo(const Value *V) {
    DAGNodeInfo Info;
    Info.IsBoundary = true;
    auto Ty = getPatternValueType(V->getType());
    Info.ResultType = Ty.value_or(PatternValueType::DataInt);
    Info.BitWidth = Ty ? getBitWidth(V->getType()) : 0;
    return Info;
  }

  unsigned makeBoundaryNode(const Value *V) {
    auto It = BoundaryNodeIds.find(V);
    if (It != BoundaryNodeIds.end())
      return It->second;

    unsigned Id = Nodes.size();
    Nodes.push_back(makeBoundaryInfo(V));
    BoundaryNodeIds[V] = Id;
    return Id;
  }

  std::vector<DAGNodeInfo> Nodes;
  DenseMap<const Value *, unsigned> NodeIds;
  DenseMap<const Value *, unsigned> BoundaryNodeIds;
  DenseMap<const Value *, unsigned> ExpandedRemaining;
  DenseSet<const Value *> InProgress;
};

void serializeTruncatedDAG(const Value *Root, unsigned MaxDepth,
                           raw_ostream &OS) {
  const auto *Inst = dyn_cast_or_null<Instruction>(Root);
  if (!Inst || !getPatternOp(Inst))
    return;

  TruncatedDAGBuilder Builder;
  Builder.build(Root, MaxDepth);
  ArrayRef<DAGNodeInfo> Nodes = Builder.nodes();

  unsigned NonBoundaryNodes = 0;
  for (const DAGNodeInfo &N : Nodes)
    if (!N.IsBoundary)
      ++NonBoundaryNodes;
  if (NonBoundaryNodes < 2)
    return;

  SmallVector<unsigned, 16> Postorder;
  SmallVector<unsigned, 16> SsaIndex(Nodes.size(), ~0u);
  SmallVector<std::pair<unsigned, unsigned>, 16> Stack;
  BitVector Done(Nodes.size());

  Stack.emplace_back(0, 0);
  while (!Stack.empty()) {
    auto &[Id, OpNo] = Stack.back();
    const DAGNodeInfo &N = Nodes[Id];
    if (OpNo == N.Operands.size()) {
      Postorder.push_back(Id);
      Done.set(Id);
      Stack.pop_back();
      continue;
    }

    unsigned Child = N.Operands[OpNo++];
    // Only operation nodes get SSA ids. Sharing means a node may be reached
    // repeatedly, so skip operation nodes that have already been emitted.
    if (!Nodes[Child].IsBoundary && !Done.test(Child))
      Stack.emplace_back(Child, 0);
  }

  for (unsigned I = 0; I < Postorder.size(); ++I)
    SsaIndex[Postorder[Postorder.size() - 1 - I]] = I;

  DenseMap<unsigned, unsigned> ArgNumbers;
  for (unsigned I = 0; I < Postorder.size(); ++I) {
    unsigned Id = Postorder[Postorder.size() - 1 - I];
    const DAGNodeInfo &N = Nodes[Id];
    if (I)
      OS << "; ";
    OS << '%' << I << " = " << N.Name << '(';
    for (unsigned J = 0; J < N.Operands.size(); ++J) {
      if (J)
        OS << ", ";
      unsigned Child = N.Operands[J];
      if (Nodes[Child].IsBoundary) {
        auto Inserted = ArgNumbers.try_emplace(Child, ArgNumbers.size());
        OS << "arg" << Inserted.first->second;
      } else {
        OS << '%' << SsaIndex[Child];
      }
    }
    OS << ')';
  }
  OS << '\n';
}

} // namespace

namespace llvm::DAGSlicer {

void recordDAG(const Value *Root, unsigned AnalysisDepth, unsigned MaxDepth) {
  assert(AnalysisDepth <= MaxDepth && "analysis depth exceeds max depth");
  unsigned RemainingDepth = MaxDepth - AnalysisDepth;
  if (RemainingDepth < 2)
    return;

  LLVM_DEBUG(serializeTruncatedDAG(Root, RemainingDepth, dbgs()));
}

} // namespace llvm::DAGSlicer

#undef DEBUG_TYPE
