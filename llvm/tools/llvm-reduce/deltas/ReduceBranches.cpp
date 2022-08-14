//===- ReduceBranches.cpp - Specialized Delta Pass ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a function which calls the Generic Delta pass in order
// to reduce uninteresting branches from defined functions.
//
//===----------------------------------------------------------------------===//

#include "ReduceBranches.h"
#include "Utils.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/SimplifyCFGOptions.h"

using namespace llvm;

/// For each conditional branch, try to turn it into an unconditional
/// branch to the specified successor
static void setConditionalBranchesTo(Oracle &O, Module &Program, unsigned SuccNum) {
  SmallPtrSet<BasicBlock *, 8> TryToDelete;
  for (auto &F : Program) {
    for (auto &BB : F) {
      BranchInst *BI = dyn_cast<BranchInst>(BB.getTerminator());
      if (BI && BI->isConditional() && !O.shouldKeep()) {
        auto Other = BI->getSuccessor(1 - SuccNum);
        TryToDelete.insert(Other);
        Other->removePredecessor(&BB);
        ReplaceInstWithInst(BI, BranchInst::Create(BI->getSuccessor(SuccNum)));
      }
    }
  }
  for (auto *BB : TryToDelete)
    if (!BB->hasNPredecessorsOrMore(1))
      BB->eraseFromParent();
}

static void reduceConditionalBranchesTrue(Oracle &O, Module &Program) {
  setConditionalBranchesTo(O, Program, 0);
}

static void reduceConditionalBranchesFalse(Oracle &O, Module &Program) {
  setConditionalBranchesTo(O, Program, 1);
}

static void reduceUsingSimplifyCFG(Oracle &O, Module &Program) {
  TargetTransformInfo TTI(Program.getDataLayout());
  for (auto &F : Program) {
    for (auto &BB : F)
      if (!O.shouldKeep())
        simplifyCFG(&BB, TTI);
  }
}

void llvm::reduceConditionalBranchesTrueDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing Conditional Branches to true target...\n";
  runDeltaPass(Test, reduceConditionalBranchesTrue);
}

void llvm::reduceConditionalBranchesFalseDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing Conditional Branches to false target...\n";
  runDeltaPass(Test, reduceConditionalBranchesFalse);
}

void llvm::reduceUsingSimplifyCFGDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing using SimplifyCFG...\n";
  runDeltaPass(Test, reduceUsingSimplifyCFG);
}
