//===- ReduceReturns.h - Specialized Delta Pass -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a function which calls the Generic Delta pass
// in order to return various values in hopes of creating dead
// instructions further down in the function.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_LLVM_REDUCE_DELTAS_REDUCERETURNS_H
#define LLVM_TOOLS_LLVM_REDUCE_DELTAS_REDUCERETURNS_H

#include "Delta.h"
#include "llvm/IR/Argument.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"

namespace llvm {
void reduceReturnsDeltaPass(TestRunner &Test);
} // namespace llvm

#endif
