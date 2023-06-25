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

/// Try to remove returns from this funtion if it has a body and also
/// it isn't called. FIXME: Of course we could relax the second
/// condition.
static bool shouldRemoveReturns(const Function &F) {
  if (F.isDeclaration())
        return false;
  for (const User *U : F.users())
    if (!isa<BlockAddress>(U))
      return false;
   return true;
}

void rewriteReturns(Function &F, std::vector<ReturnInst *> &ToRewrite) {
  auto RetTy = ToRewrite.at(0)->getType();

  // make a copy of the function, with the new return type

  // rewrite the first return to actually return that value, and the
  // others to return 0
  
  // run DCE on the function -- if this variant is interesting, we
  // don't want to subsequently return a dead value
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
          /// FIXME handle non-integer-typed returns
          if (RI && RI->getType()->isIntegerTy() && !O.shouldKeep())
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
