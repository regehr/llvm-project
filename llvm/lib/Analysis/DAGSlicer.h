#ifndef LLVM_LIB_ANALYSIS_DAGSLICER_H
#define LLVM_LIB_ANALYSIS_DAGSLICER_H

#include "llvm/ADT/STLFunctionalExtras.h"
#include "llvm/ADT/StringRef.h"

namespace llvm {

class Value;

namespace DAGSlicer {

void enumeratePatterns(
    const Value *Root,
    function_ref<void(unsigned PatternSize, StringRef Pattern)> Callback);

void recordPatterns(const Value *Root, unsigned KnownBitsDepth);

} // namespace DAGSlicer
} // namespace llvm

#endif // LLVM_LIB_ANALYSIS_DAGSLICER_H
