//===- KnownBitsOptLog.cpp - Logging for KnownBits-driven optzns ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/KnownBitsOptLog.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>
#include <string>

using namespace llvm;

static cl::opt<std::string> LogKnownBitsOptzns(
    "log-knownbits-optzns",
    cl::desc("Append a tagged entry to <file> each time a KnownBits-driven "
             "optimization fires (instrumented via LOG_KB_OPTZN())."),
    cl::value_desc("file"), cl::init(""), cl::Hidden);

namespace {

struct LogState {
  std::unique_ptr<raw_fd_ostream> OS;
  bool Tried = false;
};

LogState &getState() {
  static LogState S;
  return S;
}

} // end anonymous namespace

void llvm::logKnownBitsOptzn(const char *File, int Line) {
  // Cheap path when the flag isn't set.
  if (LogKnownBitsOptzns.empty())
    return;
  LogState &S = getState();
  if (!S.Tried) {
    std::error_code EC;
    auto Stream = std::make_unique<raw_fd_ostream>(LogKnownBitsOptzns, EC,
                                                   sys::fs::OF_Append);
    if (EC)
      errs() << "log-knownbits-optzns: failed to open '" << LogKnownBitsOptzns
             << "' for appending: " << EC.message() << "\n";
    else
      S.OS = std::move(Stream);
    S.Tried = true;
  }
  if (S.OS) {
    *S.OS << File << ":" << Line << "\n";
    S.OS->flush();
  }
}
