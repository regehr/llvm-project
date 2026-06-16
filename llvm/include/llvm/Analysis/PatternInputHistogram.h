#ifndef LLVM_ANALYSIS_PATTERNINPUTHISTOGRAM_H
#define LLVM_ANALYSIS_PATTERNINPUTHISTOGRAM_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/ConstantRange.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/KnownBits.h"
#include <cstdint>
#include <map>
#include <mutex>
#include <string>

namespace llvm {

/// Process-wide aggregator for pattern-transformer inputs.
class LLVM_ABI PatternInputHistogram {
public:
  void record(StringRef OutFile, StringRef Domain, unsigned ID,
              ArrayRef<KnownBits> Inputs);
  void record(StringRef OutFile, StringRef Domain, unsigned ID,
              ArrayRef<ConstantRange> Inputs, bool ForSigned);
  ~PatternInputHistogram();

private:
  struct Row {
    uint64_t Count = 0;
  };

  std::mutex Mtx;
  // Ordered map for deterministic output across runs.
  std::map<std::string, Row> Rows;
  std::string Path;
};

LLVM_ABI void recordKBPatternHistogram(unsigned ID, ArrayRef<KnownBits> Inputs);
LLVM_ABI void recordCRPatternHistogram(unsigned ID,
                                       ArrayRef<ConstantRange> Inputs,
                                       bool ForSigned);

} // namespace llvm

#endif // LLVM_ANALYSIS_PATTERNINPUTHISTOGRAM_H
