//===- ReduceArguments.cpp - Specialized Delta Pass -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a function which calls the Generic Delta pass
// in order to make values come from function arguments instead of
// being produced by instructions.
//
//===----------------------------------------------------------------------===//

#include "ReduceInstsToArguments.h"
#include "Delta.h"
#include "llvm/ADT/SmallVector.h"
#include <set>
#include <vector>

using namespace llvm;

/// Goes over OldF calls and replaces them with a call to NewF
static void replaceFunctionCalls(Function &OldF, Function &NewF,
                                 const std::set<int> &ArgIndexesToKeep) {
  const auto &Users = OldF.users();
  for (auto I = Users.begin(), E = Users.end(); I != E; )
    if (auto *CI = dyn_cast<CallInst>(*I++)) {
      SmallVector<Value *, 8> Args;
      for (auto ArgI = CI->arg_begin(), E = CI->arg_end(); ArgI != E; ++ArgI)
        if (ArgIndexesToKeep.count(ArgI - CI->arg_begin()))
          Args.push_back(*ArgI);

      CallInst *NewCI = CallInst::Create(&NewF, Args);
      NewCI->setCallingConv(NewF.getCallingConv());
      if (!CI->use_empty())
        CI->replaceAllUsesWith(NewCI);
      ReplaceInstWithInst(CI, NewCI);
    }
}

/// Removes out-of-chunk arguments from functions, and modifies their calls
/// accordingly. It also removes allocations of out-of-chunk arguments.
static void extractArgumentsFromModule(std::vector<Chunk> ChunksToKeep,
                                       Module *Program) {
  Oracle O(ChunksToKeep);

  std::set<Argument *> ArgsToKeep;
  std::vector<Function *> Funcs;
  // Get inside-chunk arguments, as well as their parent function
  for (auto &F : *Program)
    if (!F.arg_empty()) {
      Funcs.push_back(&F);
      for (auto &A : F.args())
        if (O.shouldKeep())
          ArgsToKeep.insert(&A);
    }

  for (auto *F : Funcs) {
    ValueToValueMapTy VMap;
    std::vector<WeakVH> InstToDelete;
    for (auto &A : F->args())
      if (!ArgsToKeep.count(&A)) {
        // By adding undesired arguments to the VMap, CloneFunction will remove
        // them from the resulting Function
        VMap[&A] = UndefValue::get(A.getType());
        for (auto *U : A.users())
          if (auto *I = dyn_cast<Instruction>(*&U))
            InstToDelete.push_back(I);
      }
    // Delete any (unique) instruction that uses the argument
    for (Value *V : InstToDelete) {
      if (!V)
        continue;
      auto *I = cast<Instruction>(V);
      I->replaceAllUsesWith(UndefValue::get(I->getType()));
      if (!I->isTerminator())
        I->eraseFromParent();
    }

    // No arguments to reduce
    if (VMap.empty())
      continue;

    std::set<int> ArgIndexesToKeep;
    for (auto &Arg : enumerate(F->args()))
      if (ArgsToKeep.count(&Arg.value()))
        ArgIndexesToKeep.insert(Arg.index());

    auto *ClonedFunc = CloneFunction(F, VMap);
    // In order to preserve function order, we move Clone after old Function
    ClonedFunc->removeFromParent();
    Program->getFunctionList().insertAfter(F->getIterator(), ClonedFunc);

    replaceFunctionCalls(*F, *ClonedFunc, ArgIndexesToKeep);
    // Rename Cloned Function to Old's name
    std::string FName = std::string(F->getName());
    F->replaceAllUsesWith(ConstantExpr::getBitCast(ClonedFunc, F->getType()));
    F->eraseFromParent();
    ClonedFunc->setName(FName);
  }
}

/// Removes out-of-chunk arguments from functions, and modifies their calls
/// accordingly. It also removes allocations of out-of-chunk arguments.
static void instToArgumentInModule(std::vector<Chunk> ChunksToKeep,
                                   Module *Program) {
  Oracle O(ChunksToKeep);

  std::set<Instruction *> InstToKeep;

  // We only want to eliminate non-void instructions
  for (auto &F : *Program)
    for (auto &BB : F) {
      for (auto &Inst : BB)
        if (O.shouldKeep() || Inst.getType()->isVoidTy())
          InstToKeep.insert(&Inst);
    }

  std::vector<Instruction *> InstToDelete;
  for (auto &F : *Program)
    for (auto &BB : F)
      for (auto &Inst : BB)
        if (!InstToKeep.count(&Inst)) {
          Inst.replaceAllUsesWith(UndefValue::get(Inst.getType()));
          InstToDelete.push_back(&Inst);
        }

  for (auto &I : InstToDelete)
    I->eraseFromParent();
}  

static void extractArgumentsFromModule(std::vector<Chunk> ChunksToKeep,
                                       Module *Program) {
  Oracle O(ChunksToKeep);

  std::set<Instruction *> InstToKeep;
  std::vector<Function *> Funcs;
  // We only want to eliminate non-void instructions
  for (auto &F : *Program)
    for (auto &BB : F) {
      for (auto &Inst : BB)
        if (O.shouldKeep() || Inst.getType()->isVoidTy())
          InstToKeep.insert(&Inst);
    }

  for (auto *F : Funcs) {
    ValueToValueMapTy VMap;
    std::vector<WeakVH> InstToDelete;
    for (auto &A : F->args())
      if (!ArgsToKeep.count(&A)) {
        // By adding undesired arguments to the VMap, CloneFunction will remove
        // them from the resulting Function
        VMap[&A] = UndefValue::get(A.getType());
        for (auto *U : A.users())
          if (auto *I = dyn_cast<Instruction>(*&U))
            InstToDelete.push_back(I);
      }
    // Delete any (unique) instruction that uses the argument
    for (Value *V : InstToDelete) {
      if (!V)
        continue;
      auto *I = cast<Instruction>(V);
      I->replaceAllUsesWith(UndefValue::get(I->getType()));
      if (!I->isTerminator())
        I->eraseFromParent();
    }

    // No arguments to reduce
    if (VMap.empty())
      continue;

    std::set<int> ArgIndexesToKeep;
    for (auto &Arg : enumerate(F->args()))
      if (ArgsToKeep.count(&Arg.value()))
        ArgIndexesToKeep.insert(Arg.index());

    auto *ClonedFunc = CloneFunction(F, VMap);
    // In order to preserve function order, we move Clone after old Function
    ClonedFunc->removeFromParent();
    Program->getFunctionList().insertAfter(F->getIterator(), ClonedFunc);

    replaceFunctionCalls(*F, *ClonedFunc, ArgIndexesToKeep);
    // Rename Cloned Function to Old's name
    std::string FName = std::string(F->getName());
    F->replaceAllUsesWith(ConstantExpr::getBitCast(ClonedFunc, F->getType()));
    F->eraseFromParent();
    ClonedFunc->setName(FName);
  }
}

/// Counts the amount of basic blocks and prints their name & respective index
static unsigned countInstructions(Module *Program) {
  // TODO: Silence index with --quiet flag
  outs() << "----------------------------\n";
  int InstCount = 0;
  for (auto &F : *Program)
    for (auto &BB : F)
      // Well-formed blocks have terminators, which we cannot remove.
      InstCount += BB.getInstList().size() - 1;
  outs() << "Number of instructions: " << InstCount << "\n";

  return InstCount;
}

void llvm::reduceInstsToArgumentsDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing Instructions to Arguments...\n";
  unsigned InstCount = countInstructions(Test.getProgram());
  runDeltaPass(Test, InstCount, instToArgumentInModule);
}
