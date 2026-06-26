//===-- KBOptLog.h - Logging for KnownBits-based optimizations --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_KBOPTLOG_H
#define LLVM_SUPPORT_KBOPTLOG_H

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"

namespace llvm {
// Defined in InstructionSimplify.cpp.
extern cl::opt<bool> LogKBOpts;
} // namespace llvm

#define KBOPT_LOG() \
  do { if (::llvm::LogKBOpts) ::llvm::dbgs() << "KBOPT:" << __FILE__ << ":" << __LINE__ << "\n"; } while (0)

#endif
