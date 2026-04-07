#ifndef LLVM_WIDTH_OPTIMIZATION_WIDTHOPT_H
#define LLVM_WIDTH_OPTIMIZATION_WIDTHOPT_H

#include "llvm/IR/PassManager.h"

namespace llvm {
class Function;
} // namespace llvm

namespace widthopt {

class WidthOptPass : public llvm::PassInfoMixin<WidthOptPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

} // namespace widthopt

#endif
