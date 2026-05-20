#ifndef LLVM_LIB_ANALYSIS_DAGSLICER_H
#define LLVM_LIB_ANALYSIS_DAGSLICER_H

#include "llvm/ADT/STLFunctionalExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

namespace llvm {

class Value;

namespace DAGSlicer {

struct Config {
  unsigned MaxPatternSize = 5;
  bool SymbolizeConstants = false;
  SmallVector<unsigned, 8> PatternSizes;
};

const Config &getDefaultConfig();

void enumeratePatterns(
    const Value *Root, const Config &Cfg,
    function_ref<void(unsigned PatternSize, StringRef Pattern)> Callback);

void recordPatterns(const Value *Root, unsigned KnownBitsDepth,
                    const Config &Cfg = getDefaultConfig());

} // namespace DAGSlicer
} // namespace llvm

#endif // LLVM_LIB_ANALYSIS_DAGSLICER_H
