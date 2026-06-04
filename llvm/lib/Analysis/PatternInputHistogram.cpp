//===- PatternInputHistogram.cpp - Pattern input histograms ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/PatternInputHistogram.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include <system_error>

using namespace llvm;

// If set, accumulate a histogram of pattern-transformer inputs instead of
// emitting a per-query debug log. For each (pattern id, operand known bits) we
// record the occurrence count, the summed bits-added, and the conflict count,
// then write a compact TSV to this path at process exit. This collapses the
// hundreds of millions of repeated queries into a few thousand distinct rows
// without ever materializing the raw lines (no stderr, no pipe, no per-query
// disk IO). vanilla/combined are deliberately not recorded: combined is
// derivable and vanilla is not a function of the inputs, so per the histogram
// design we drop them and aggregate bits-added/conflict instead.
static cl::opt<std::string> PatternHistFile(
    "value-tracking-pattern-histogram", cl::Hidden, cl::init(""),
    cl::desc("Write a TSV histogram of pattern-transformer inputs to this file "
             "at exit (id + operand known bits, summed bits-added/conflicts)."));

void PatternInputHistogram::record(StringRef OutFile, unsigned ID,
                                   ArrayRef<KnownBits> Inputs,
                                   unsigned BitsAdded, bool Conflict) {
  std::lock_guard<std::mutex> Lock(Mtx);
  // Capture the output path on first use; reading the cl::opt here (during
  // compilation) is safe, whereas reading it in the destructor would race
  // static-destruction order.
  if (Path.empty())
    Path = OutFile.str();
  std::string Key;
  raw_string_ostream OS(Key);
  OS << ID;
  for (const KnownBits &In : Inputs) {
    OS << '\t';
    In.print(OS);
  }
  Row &R = Rows[OS.str()];
  ++R.Count;
  R.BitsAdded += BitsAdded;
  R.Conflicts += Conflict ? 1 : 0;
}

PatternInputHistogram::~PatternInputHistogram() {
  if (Path.empty() || Rows.empty())
    return;
  std::error_code EC;
  raw_fd_ostream Out(Path, EC, sys::fs::OF_Text);
  if (EC)
    return;
  // <id> <arg0> <arg1> ... <count> <bits_added> <conflicts>
  for (const auto &KV : Rows)
    Out << KV.first << '\t' << KV.second.Count << '\t' << KV.second.BitsAdded
        << '\t' << KV.second.Conflicts << '\n';
}

void llvm::recordPatternHistogram(unsigned ID, ArrayRef<KnownBits> Inputs,
                                  unsigned BitsAdded, bool Conflict) {
  if (PatternHistFile.empty())
    return;
  static PatternInputHistogram Histogram;
  Histogram.record(PatternHistFile, ID, Inputs, BitsAdded, Conflict);
}
