//===- ReduceReturns.cpp - Specialized Delta Pass -----------------------===//
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

#include "ReduceReturns.h"
#include "Delta.h"
#include "Utils.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include <set>
#include <vector>

using namespace llvm;

/// FIXME: return false for functions that have a call in this module,
/// or are address taken
static bool shouldRemoveReturns(const Function &F) {
  return !F.isDeclaration();
}

void rewriteReturns(Function &F, std::vector<ReturnInst *> &Rets) {
}

/// Removes out-of-chunk arguments from functions, and modifies their calls
/// accordingly. It also removes allocations of out-of-chunk arguments.
static void extractReturnsFromModule(Oracle &O, ReducerWorkItem &WorkItem) {
  Module &Program = WorkItem.getModule();
  for (auto &F : Program) {
    if (shouldRemoveReturns(F)) {
      std::vector<ReturnInst *> RetsToRewrite;
      for (auto &BB : F) {
        for (auto &I : BB) {
          auto RI = dyn_cast<ReturnInst>(&I);
          if (RI && !O.shouldKeep())
            RetsToRewrite.push_back(RI);
        }
      }
      if (!RetsToRewrite.empty())
        rewriteReturns(F, RetsToRewrite);
    }
  }
}

void llvm::reduceReturnsDeltaPass(TestRunner &Test) {
  runDeltaPass(Test, extractReturnsFromModule, "Reducing Returns");
}
