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

#include "llvm/Bitcode/BitcodeWriter.h"

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
static void instToArgumentInModule(std::vector<Chunk> ChunksToKeep,
                                   Module *Program) {
  Oracle O(ChunksToKeep);

  for (auto &F : *Program) {
    // Make a list of instructions in the current function that are in
    // the chunk and that do not return void
    std::vector<Instruction *> InstToDelete;
    for (auto &BB : F)
      for (auto &Inst : BB)
        if (!O.shouldKeep() && !Inst.getType()->isVoidTy())
          InstToDelete.push_back(&Inst);

    // Bail early if we're not changing this function
    if (InstToDelete.empty())
      continue;

    // Start with original argument list
    std::vector<Type *> ArgTy;
    for (auto &A : F.args())
      ArgTy.push_back(A.getType());

    // Add an argument corresponding to value produced by each deleted
    // insn
    for (auto &Inst : InstToDelete)
      ArgTy.push_back(Inst->getType());
    
    auto FuncTy = FunctionType::get(F.getReturnType(), ArgTy, F.isVarArg());
    auto ClonedFunc = Function::Create(FuncTy, F.getLinkage(), F.getAddressSpace(),
                                       F.getName(), Program);

    ValueToValueMapTy VMap;
    auto A = ClonedFunc->arg_begin();
    for (auto &V : F.args())
      VMap[&V] = A++;
    for (auto &Inst : InstToDelete)
      VMap[Inst] = A++;

    // Delete any (unique) instruction that uses the argument
    for (Value *V : InstToDelete) {
      auto *I = cast<Instruction>(V);
      I->replaceAllUsesWith(UndefValue::get(I->getType()));
      if (!I->isTerminator())
        I->eraseFromParent();
    }

#if 0
    std::set<int> ArgIndexesToKeep;
    for (auto &Arg : enumerate(F->args()))
      if (ArgsToKeep.count(&Arg.value()))
        ArgIndexesToKeep.insert(Arg.index());
#endif

    SmallVector<ReturnInst *, 8> Returns;
    CloneFunctionInto(ClonedFunc, &F, VMap,
                      CloneFunctionChangeType::LocalChangesOnly, Returns);
      
    // In order to preserve function order, we move Clone after old Function
    ClonedFunc->removeFromParent();
    Program->getFunctionList().insertAfter(F.getIterator(), ClonedFunc);

#if 0
    std::error_code EC;
    llvm::raw_fd_ostream OS("module.bc", EC, llvm::sys::fs::F_None);
    WriteBitcodeToFile(*Program, OS);
    OS.flush();
    llvm::outs() << "exiting after printing\n";
    exit(0);
#endif

#if 0
    replaceFunctionCalls(F, *ClonedFunc, ArgIndexesToKeep);
#endif

    // Rename Cloned Function to Old's name
    std::string FName = std::string(F.getName());
    F.replaceAllUsesWith(ConstantExpr::getBitCast(ClonedFunc, F.getType()));
    F.eraseFromParent();
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
      InstCount += BB.getInstList().size();
  outs() << "Number of instructions: " << InstCount << "\n";

  return InstCount;
}

void llvm::reduceInstsToArgumentsDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing Instructions to Arguments...\n";
  unsigned InstCount = countInstructions(Test.getProgram());
  runDeltaPass(Test, InstCount, instToArgumentInModule);
}
