#ifndef LLVM_LIB_ANALYSIS_DAGSLICER_H
#define LLVM_LIB_ANALYSIS_DAGSLICER_H

namespace llvm {

class Value;

namespace DAGSlicer {

void recordDAG(const Value *Root, unsigned AnalysisDepth, unsigned MaxDepth);

} // namespace DAGSlicer
} // namespace llvm

#endif // LLVM_LIB_ANALYSIS_DAGSLICER_H
