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
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

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

static void reduceUnconditionalBranches(Oracle &O, Module &Program) {
  for (auto &F : Program) {
    for (auto &BB : F) {
      if (BB.isEntryBlock())
        continue;
      BranchInst *XBI = dyn_cast<BranchInst>(&*BB.begin());
      if (!XBI || !XBI->isUnconditional())
        continue;
      if (O.shouldKeep())
        continue;
      outs() << "ok to go\n";
      BasicBlock *NewTarget = XBI->getSuccessor(0);
    again:
      for (BasicBlock *Pred : predecessors(&BB)) {
        BranchInst *PredBI = dyn_cast<BranchInst>(Pred->getTerminator());
        if (PredBI) {
          outs() << "  removepred " << *Pred << "\n";
          BB.removePredecessor(Pred);
          unsigned index = 0;
          for (BasicBlock *TBB : successors(PredBI)) {
            if (TBB == &BB) {
              //Pred->replacePhiUsesWith(&BB, NewTarget);
              PredBI->setSuccessor(index, NewTarget);
              goto again;
            }
            ++index;
          }
        }
        SwitchInst *SI = dyn_cast<SwitchInst>(Pred->getTerminator());
        if (SI) {
          assert(false);
          unsigned index = 0;
          for (BasicBlock *TBB : successors(SI)) {
            if (TBB == &BB) {
              Pred->replacePhiUsesWith(&BB, NewTarget);
              SI->setSuccessor(index, NewTarget);
            }
            ++index;
          }
        }
      }
    }
  }
  outs() << Program << "\n";
}

void llvm::reduceConditionalBranchesTrueDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing Conditional Branches to true target...\n";
  runDeltaPass(Test, reduceConditionalBranchesTrue);
}

void llvm::reduceConditionalBranchesFalseDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing Conditional Branches to false target...\n";
  runDeltaPass(Test, reduceConditionalBranchesFalse);
}

void llvm::reduceUnconditionalBranchesDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing Unconditional Branches...\n";
  runDeltaPass(Test, reduceUnconditionalBranches);
}
