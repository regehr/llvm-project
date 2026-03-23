#ifndef LLVM_WIDTH_OPTIMIZATION_WIDTHOPT_H
#define LLVM_WIDTH_OPTIMIZATION_WIDTHOPT_H

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/PassManager.h"

namespace llvm {
class Function;
class Instruction;
class IntegerType;
class raw_ostream;
class Value;
} // namespace llvm

namespace widthopt {

enum class InstClass {
  HardAnchor,
  FreelyWidthPolymorphic,
  ConditionallyRetargetable,
  Ignore
};

struct Component {
  unsigned ID = 0;
  unsigned OrigWidth = 0;
  bool Fixed = false;
  llvm::SmallVector<unsigned, 4> CandidateWidths;
  llvm::SmallVector<llvm::Instruction *, 8> Instructions;
  llvm::SmallVector<llvm::Value *, 8> Values;
};

struct ComponentEdge {
  unsigned From = 0;
  unsigned To = 0;
  unsigned Weight = 0;
};

struct CompareAffinity {
  unsigned LHS = 0;
  unsigned RHS = 0;
  unsigned Weight = 0;
};

struct EqualityCompareRepairPressure {
  unsigned LHS = 0;
  unsigned RHS = 0;
  unsigned Weight = 0;
};

struct AnchorPressure {
  unsigned Component = 0;
  unsigned Width = 0;
  unsigned Weight = 0;
};

struct ExtensionPressure {
  unsigned Source = 0;
  unsigned User = 0;
  unsigned Width = 0;
  bool IsSExt = false;
  unsigned Weight = 0;
};

struct CompareRetargetPressure {
  unsigned Component = 0;
  bool PreferSExt = false;
  unsigned Weight = 0;
};

struct AnalysisResult {
  llvm::DenseMap<const llvm::Value *, unsigned> ValueToComponent;
  llvm::SmallVector<Component, 8> Components;
  llvm::SmallVector<ComponentEdge, 16> Edges;
  llvm::SmallVector<CompareAffinity, 8> CompareAffinities;
  llvm::SmallVector<EqualityCompareRepairPressure, 8>
      EqualityCompareRepairPressures;
  llvm::SmallVector<AnchorPressure, 8> AnchorPressures;
  llvm::SmallVector<ExtensionPressure, 8> ExtensionPressures;
  llvm::SmallVector<CompareRetargetPressure, 8> CompareRetargetPressures;
};

struct PlanResult {
  llvm::SmallVector<unsigned, 8> ChosenWidths;
  unsigned TotalCutCost = 0;
};

class WidthComponentAnalysis
    : public llvm::AnalysisInfoMixin<WidthComponentAnalysis> {
public:
  using Result = AnalysisResult;

  Result run(llvm::Function &F, llvm::FunctionAnalysisManager &AM);

  static llvm::AnalysisKey Key;
};

class WidthPlanAnalysis : public llvm::AnalysisInfoMixin<WidthPlanAnalysis> {
public:
  using Result = PlanResult;

  Result run(llvm::Function &F, llvm::FunctionAnalysisManager &AM);

  static llvm::AnalysisKey Key;
};

class WidthComponentPrinter : public llvm::PassInfoMixin<WidthComponentPrinter> {
  llvm::raw_ostream &OS;

public:
  explicit WidthComponentPrinter(llvm::raw_ostream &OS) : OS(OS) {}
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

class WidthCandidatePrinter : public llvm::PassInfoMixin<WidthCandidatePrinter> {
  llvm::raw_ostream &OS;

public:
  explicit WidthCandidatePrinter(llvm::raw_ostream &OS) : OS(OS) {}
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

class WidthPlanPrinter : public llvm::PassInfoMixin<WidthPlanPrinter> {
  llvm::raw_ostream &OS;

public:
  explicit WidthPlanPrinter(llvm::raw_ostream &OS) : OS(OS) {}
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

class WidthOptPass : public llvm::PassInfoMixin<WidthOptPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

} // namespace widthopt

#endif
