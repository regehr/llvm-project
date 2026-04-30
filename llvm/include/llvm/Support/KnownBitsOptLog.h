//===- KnownBitsOptLog.h - Logging for KnownBits-driven optzns --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Lightweight logging for optimizations driven by KnownBits analysis. When the
// command-line flag -log-knownbits-optzns=<file> is given, each LOG_KB_OPTZN()
// call writes a line tagged with __FILE__:__LINE__ to <file> (opened in append
// mode on first use). When the flag is not set, LOG_KB_OPTZN() is a no-op
// after a single nullptr check.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_KNOWNBITSOPTLOG_H
#define LLVM_SUPPORT_KNOWNBITSOPTLOG_H

namespace llvm {

void logKnownBitsOptzn(const char *File, int Line);

} // end namespace llvm

#define LOG_KB_OPTZN() ::llvm::logKnownBitsOptzn(__FILE__, __LINE__)

#endif // LLVM_SUPPORT_KNOWNBITSOPTLOG_H
