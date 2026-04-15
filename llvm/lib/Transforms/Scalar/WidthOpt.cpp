//===- WidthOpt.cpp - Width optimization pass -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a placeholder width optimization pass.
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Scalar/WidthOpt.h"
#include "llvm/IR/Function.h"

using namespace llvm;

PreservedAnalyses WidthOptPass::run(Function &F, FunctionAnalysisManager &AM) {
  (void)F;
  (void)AM;
  return PreservedAnalyses::all();
}
