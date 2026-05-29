#ifndef LLVM_LIB_ANALYSIS_DAGSLICER_H
#define LLVM_LIB_ANALYSIS_DAGSLICER_H

#include "llvm/ADT/STLFunctionalExtras.h"
#include "llvm/ADT/StringRef.h"

namespace llvm {

class Value;

namespace DAGSlicer {

void enumeratePatterns(const Value *Root, unsigned MinDepth, unsigned MaxDepth,
                       function_ref<void(StringRef Pattern)> Callback);

void recordPatterns(const Value *Root, unsigned AnalysisDepth,
                    unsigned MinDepth, unsigned MaxDepth);

} // namespace DAGSlicer
} // namespace llvm

#endif // LLVM_LIB_ANALYSIS_DAGSLICER_H
