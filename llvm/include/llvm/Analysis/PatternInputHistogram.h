//===- PatternInputHistogram.h - Pattern input histograms -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_PATTERNINPUTHISTOGRAM_H
#define LLVM_ANALYSIS_PATTERNINPUTHISTOGRAM_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
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
  void record(StringRef OutFile, unsigned ID, ArrayRef<KnownBits> Inputs,
              unsigned BitsAdded, bool Conflict);
  ~PatternInputHistogram();

private:
  struct Row {
    uint64_t Count = 0;
    uint64_t BitsAdded = 0;
    uint64_t Conflicts = 0;
  };

  std::mutex Mtx;
  // Ordered map for deterministic output across runs.
  std::map<std::string, Row> Rows;
  std::string Path;
};

LLVM_ABI void recordPatternHistogram(unsigned ID, ArrayRef<KnownBits> Inputs,
                                     unsigned BitsAdded, bool Conflict);

} // namespace llvm

#endif // LLVM_ANALYSIS_PATTERNINPUTHISTOGRAM_H
