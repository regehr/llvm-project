//===- WidthOpt.h - Width optimization pass --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// \file
// This file defines WidthOptPass for the new pass manager.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_WIDTHOPT_H
#define LLVM_TRANSFORMS_SCALAR_WIDTHOPT_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class Function;

struct WidthOptPass : public PassInfoMixin<WidthOptPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_WIDTHOPT_H
