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
#include "llvm/Transforms/AggressiveInstCombine/AggressiveInstCombine.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/IPO/DeadArgumentElimination.h"
#include "llvm/Transforms/IPO/GlobalDCE.h"
#include "llvm/Transforms/IPO/GlobalOpt.h"
#include "llvm/Transforms/IPO/Internalize.h"
#include "llvm/Transforms/IPO/SCCP.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/ADCE.h"
#include "llvm/Transforms/Scalar/BDCE.h"
#include "llvm/Transforms/Scalar/CorrelatedValuePropagation.h"
#include "llvm/Transforms/Scalar/DCE.h"
#include "llvm/Transforms/Scalar/LoopInstSimplify.h"
#include "llvm/Transforms/Scalar/InstSimplifyPass.h"
#include "llvm/Transforms/Scalar/IndVarSimplify.h"
#include "llvm/Transforms/Scalar/LoopSimplifyCFG.h"
#include "llvm/Transforms/Scalar/TailRecursionElimination.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Scalar/EarlyCSE.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimpleLoopUnswitch.h"
#include "llvm/Transforms/Scalar/LoopIdiomRecognize.h"
#include "llvm/Transforms/Scalar/DeadStoreElimination.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/InstSimplifyPass.h"
#include "llvm/Transforms/Scalar/JumpThreading.h"
#include "llvm/Transforms/Scalar/LICM.h"
#include "llvm/Transforms/Scalar/LoopDeletion.h"
#include "llvm/Transforms/Scalar/MemCpyOptimizer.h"
#include "llvm/Transforms/Scalar/NewGVN.h"
#include "llvm/Transforms/Scalar/SCCP.h"
#include "llvm/Transforms/Scalar/SROA.h"

using namespace llvm;

/// Removes out-of-chunk arguments from functions, and modifies their calls
/// accordingly. It also removes allocations of out-of-chunk arguments.
static void runOptPasses(std::vector<Chunk> ChunksToKeep, Module *Program) {
  Oracle O(ChunksToKeep);

  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;

  PassBuilder PB;
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  LoopPassManager LPM;

  if (!O.shouldKeep()) {
    outs() << "LICM\n";
    LPM.addPass(LICMPass());
  }
  if (!O.shouldKeep()) {
    outs() << "LoopDeletion\n";
    LPM.addPass(LoopDeletionPass());
  }
  if (!O.shouldKeep()) {
    outs() << "LoopInstSimplify\n";
    LPM.addPass(LoopInstSimplifyPass());
  }
  if (!O.shouldKeep()) {
    outs() << "LoopSimplifyCFG\n";
    LPM.addPass(LoopSimplifyCFGPass());
  }
  if (!O.shouldKeep()) {
    outs() << "SimpleLoopUnswitch\n";
    LPM.addPass(SimpleLoopUnswitchPass());
  }
  if (!O.shouldKeep()) {
    outs() << "LoopIdiomRecognize\n";
    LPM.addPass(LoopIdiomRecognizePass());
  }
  if (!O.shouldKeep()) {
    outs() << "IndVarSimplify\n";
    LPM.addPass(IndVarSimplifyPass());
  }

  FunctionPassManager FPM;

  if (!O.shouldKeep()) {
    outs() << "InstSimplify\n";
    FPM.addPass(InstSimplifyPass());
  }
  if (!O.shouldKeep()) {
    outs() << "DCE\n";
    FPM.addPass(DCEPass());
  }
  if (!O.shouldKeep()) {
    outs() << "ADCE\n";
    FPM.addPass(ADCEPass());
  }
  if (!O.shouldKeep()) {
    outs() << "BDCE\n";
    FPM.addPass(BDCEPass());
  }
  if (!O.shouldKeep()) {
    outs() << "DSE\n";
    FPM.addPass(DSEPass());
  }
  if (!O.shouldKeep()) {
    outs() << "GVN\n";
    FPM.addPass(GVN());
  }
  if (!O.shouldKeep()) {
    outs() << "NewGVN\n";
    FPM.addPass(NewGVNPass());
  }
  if (!O.shouldKeep()) {
    outs() << "InstCombine\n";
    FPM.addPass(InstCombinePass());
  }
  if (!O.shouldKeep()) {
    outs() << "AggressiveInstCombine\n";
    FPM.addPass(AggressiveInstCombinePass());
  }
  if (!O.shouldKeep()) {
    outs() << "JumpThreading\n";
    FPM.addPass(JumpThreadingPass());
  }
  if (!O.shouldKeep()) {
    outs() << "MemCpyOpt\n";
    FPM.addPass(MemCpyOptPass());
  }
  if (!O.shouldKeep()) {
    outs() << "SROA\n";
    FPM.addPass(SROA());
  }
  if (!O.shouldKeep()) {
    outs() << "SCCP\n";
    FPM.addPass(SCCPPass());
  }
  if (!O.shouldKeep()) {
    outs() << "SimplifyCFG\n";
    FPM.addPass(SimplifyCFGPass());
  }
  if (!O.shouldKeep()) {
    outs() << "EarlyCSE\n";
    FPM.addPass(EarlyCSEPass());
  }
  if (!O.shouldKeep()) {
    outs() << "Reassociate\n";
    FPM.addPass(ReassociatePass());
  }
  if (!O.shouldKeep()) {
    outs() << "CorrelatedValuePropagation\n";
    FPM.addPass(CorrelatedValuePropagationPass());
  }
  if (!O.shouldKeep()) {
    outs() << "TailCallElim\n";
    FPM.addPass(TailCallElimPass());
  }

  ModulePassManager MPM;

  if (!O.shouldKeep()) {
    outs() << "GlobalDCE\n";
    MPM.addPass(GlobalDCEPass());
  }
  if (!O.shouldKeep()) {
    outs() << "DeadArgumentElimination\n";
    MPM.addPass(DeadArgumentEliminationPass());
  }
  if (!O.shouldKeep()) {
    outs() << "GlobalOpt\n";
    MPM.addPass(GlobalOptPass());
  }
  if (!O.shouldKeep()) {
    outs() << "ModuleInliner\n";
    MPM.addPass(ModuleInlinerWrapperPass());
  }
  if (!O.shouldKeep()) {
    outs() << "Internalize\n";
    MPM.addPass(InternalizePass());
  }
  if (!O.shouldKeep()) {
    outs() << "IPSCCP\n";
    MPM.addPass(IPSCCPPass());
  }

  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
  MPM.addPass(createModuleToFunctionPassAdaptor(
      createFunctionToLoopPassAdaptor(std::move(LPM))));
  MPM.run(*Program, MAM);
}

void llvm::reduceUsingPassesDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing with Optimization Passes...\n";
  runDeltaPass(Test, 31, runOptPasses);
}
