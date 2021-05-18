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

#include "ReduceUsingPasses.h"
#include "Delta.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/Scalar/InstSimplifyPass.h"
#include <set>
#include <vector>

using namespace llvm;

/// Removes out-of-chunk arguments from functions, and modifies their calls
/// accordingly. It also removes allocations of out-of-chunk arguments.
static void instToArgumentInModule(std::vector<Chunk> ChunksToKeep,
                                   Module *Program) {
  Oracle O(ChunksToKeep);

  llvm::LoopAnalysisManager LAM;
  llvm::FunctionAnalysisManager FAM;
  llvm::CGSCCAnalysisManager CGAM;
  llvm::ModuleAnalysisManager MAM;

  llvm::PassBuilder PB;
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  llvm::FunctionPassManager FPM;

  if (O.shouldKeep())
    FPM.addPass(InstSimplifyPass());

  llvm::ModulePassManager MPM;
  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
  MPM.run(*Program, MAM);
}  

void llvm::reduceUsingPassesDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing with Optimization Passes...\n";
  runDeltaPass(Test, 1, instToArgumentInModule);
}
