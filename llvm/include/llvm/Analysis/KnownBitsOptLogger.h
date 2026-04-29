//===- KnownBitsOptLogger.h -------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Tagged logging for known-bits-driven optimization firings across passes.
// Behavior is controlled by the -instcombine-knownbits-log=<file>
// command-line option (the name is historical; the option is shared by
// InstCombine and InstructionSimplify).
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_KNOWNBITSOPTLOGGER_H
#define LLVM_ANALYSIS_KNOWNBITSOPTLOGGER_H

#include "llvm/ADT/StringRef.h"

namespace llvm {

/// Append \p Tag followed by a newline to the file named by the
/// -instcombine-knownbits-log command-line option. Does nothing if the
/// option is not set. Used to record firings of known-bits-driven
/// transforms for offline analysis.
void logKnownBitsOpt(StringRef Tag);

} // end namespace llvm

#endif // LLVM_ANALYSIS_KNOWNBITSOPTLOGGER_H
