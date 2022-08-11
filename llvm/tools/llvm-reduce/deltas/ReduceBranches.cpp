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

static void setCondBranchesTo(Oracle &O, Module &Program, unsigned SuccNum) {
  for (auto &F : Program) {
    for (auto &BB : F) {
      BranchInst *BI = dyn_cast<BranchInst>(BB.getTerminator());
      if (BI && BI->isConditional() && !O.shouldKeep()) {
        BI->getSuccessor(1 - SuccNum)->removePredecessor(&BB);
        ReplaceInstWithInst(BI, BranchInst::Create(BI->getSuccessor(SuccNum)));
      }
    }
  }
}

/*
 * for each conditional branch, try to turn it into an unconditional
 * branch to its true target
 */
static void reduceConditionalBranchesTrue(Oracle &O, Module &Program) {
  setCondBranchesTo(O, Program, 0);
}

/*
 * for each conditional branch, try to turn it into an unconditional
 * branch to its false target
 */
static void reduceConditionalBranchesFalse(Oracle &O, Module &Program) {
  setCondBranchesTo(O, Program, 1);
}

///  Given a basic block terminated by an unconditional branch, move
///  its instructions to the head of the successor BB and then update
///  predecessors to skip over the block
static bool pushInsnsToSuccessor(Oracle &O, BasicBlock *BB, BranchInst *BI) {
  /// Feasibility checks before talking to the oracle
  if (BB->isEntryBlock())
    return false;
  for (BasicBlock *Pred : predecessors(BB)) {
    auto *TI = Pred->getTerminator();
    if (!isa<BranchInst>(TI) && !isa<SwitchInst>(TI))
      return false;
  }

  if (O.shouldKeep())
    return false;

  /// Branch around the BB we're trying to get rid of
  BasicBlock *NewTarget = BI->getSuccessor(0);
  for (BasicBlock *Pred : predecessors(BB)) {
    // Pred->replaceSuccessorsPhiUsesWith(BB, NewTarget);
    BranchInst *BI = dyn_cast<BranchInst>(Pred->getTerminator());
    if (BI) {
      unsigned index = 0;
      for (BasicBlock *TBB : successors(BI)) {
        if (TBB == BB) {
          BI->setSuccessor(index, NewTarget);
          Pred->replacePhiUsesWith(BB, NewTarget);
        }
        ++index;
      }
    }
    SwitchInst *SI = dyn_cast<SwitchInst>(Pred->getTerminator());
    if (SI) {
      unsigned index = 0;
      for (BasicBlock *TBB : successors(SI)) {
        if (TBB == BB) {
          SI->setSuccessor(index, NewTarget);
          Pred->replacePhiUsesWith(BB, NewTarget);
        }
        ++index;
      }
    }
  }

  /// Sink all instructions to the front of the new branch target
  std::vector<Instruction *> Insns;
  for (auto &I : *BB)
    if (!I.isTerminator())
      Insns.push_back(&I);
  auto it = NewTarget->getFirstInsertionPt();
  for (auto I : Insns)
    I->moveBefore(*NewTarget, it);

  return true;
}

static void reduceUnconditionalBranches(Oracle &O, Module &Program) {
  SmallSet<BasicBlock *, 4> ToDelete;
  for (auto &F : Program) {
    for (auto &BB : F) {
      BranchInst *BI = dyn_cast<BranchInst>(BB.getTerminator());
      if (BI && BI->isUnconditional())
        if (pushInsnsToSuccessor(O, &BB, BI))
          ToDelete.insert(&BB);
    }
  }
  for (auto *BB : ToDelete)
    if (!BB->hasNPredecessorsOrMore(1))
      BB->eraseFromParent();
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
