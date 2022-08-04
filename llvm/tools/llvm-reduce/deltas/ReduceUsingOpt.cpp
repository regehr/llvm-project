//===- ReduceUsingOpt.h - Specialized Delta Pass --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a function which calls the Generic Delta pass
// in order to call various optimization passes in order to simplify
// and canonicalize the code being reduced.
//
//===----------------------------------------------------------------------===//

#include "ReduceUsingOpt.h"
#include "Delta.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/AggressiveInstCombine/AggressiveInstCombine.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/IPO/DeadArgumentElimination.h"
#include "llvm/Transforms/IPO/GlobalDCE.h"
#include "llvm/Transforms/IPO/GlobalOpt.h"
#include "llvm/Transforms/IPO/Internalize.h"
#include "llvm/Transforms/IPO/SCCP.h"
#include "llvm/Transforms/IPO/StripSymbols.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/ADCE.h"
#include "llvm/Transforms/Scalar/BDCE.h"
#include "llvm/Transforms/Scalar/CorrelatedValuePropagation.h"
#include "llvm/Transforms/Scalar/DCE.h"
#include "llvm/Transforms/Scalar/DeadStoreElimination.h"
#include "llvm/Transforms/Scalar/EarlyCSE.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/IndVarSimplify.h"
#include "llvm/Transforms/Scalar/InstSimplifyPass.h"
#include "llvm/Transforms/Scalar/JumpThreading.h"
#include "llvm/Transforms/Scalar/LICM.h"
#include "llvm/Transforms/Scalar/LoopDeletion.h"
#include "llvm/Transforms/Scalar/LoopIdiomRecognize.h"
#include "llvm/Transforms/Scalar/LoopInstSimplify.h"
#include "llvm/Transforms/Scalar/LoopSimplifyCFG.h"
#include "llvm/Transforms/Scalar/MemCpyOptimizer.h"
#include "llvm/Transforms/Scalar/NewGVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SCCP.h"
#include "llvm/Transforms/Scalar/SROA.h"
#include "llvm/Transforms/Scalar/SimpleLoopUnswitch.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Scalar/TailRecursionElimination.h"

using namespace llvm;

static std::vector<std::function<void(FunctionPassManager &)>> FunctionPasses =
    {
        [](FunctionPassManager &FPM) -> void {
          FPM.addPass(InstSimplifyPass());
        },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(DCEPass()); },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(ADCEPass()); },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(BDCEPass()); },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(DSEPass()); },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(GVNPass()); },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(NewGVNPass()); },
        [](FunctionPassManager &FPM) -> void {
          FPM.addPass(InstCombinePass());
        },
        [](FunctionPassManager &FPM) -> void {
          FPM.addPass(AggressiveInstCombinePass());
        },
        [](FunctionPassManager &FPM) -> void {
          FPM.addPass(JumpThreadingPass());
        },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(MemCpyOptPass()); },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(SROAPass()); },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(SCCPPass()); },
        [](FunctionPassManager &FPM) -> void {
          FPM.addPass(SimplifyCFGPass());
        },
        [](FunctionPassManager &FPM) -> void { FPM.addPass(EarlyCSEPass()); },
        [](FunctionPassManager &FPM) -> void {
          FPM.addPass(ReassociatePass());
        },
        [](FunctionPassManager &FPM) -> void {
          FPM.addPass(CorrelatedValuePropagationPass());
        },
        [](FunctionPassManager &FPM) -> void {
          FPM.addPass(TailCallElimPass());
        },
};

static std::vector<std::function<void(LoopPassManager &)>> LoopPasses = {
    // MssaOptCap = 100 and MssaNoAccForPromotionCap = 250 are the
    // default values found in LICM.cpp
    [](LoopPassManager &LPM) -> void { LPM.addPass(LICMPass(100, 250, true)); },
    [](LoopPassManager &LPM) -> void { LPM.addPass(LoopDeletionPass()); },
    [](LoopPassManager &LPM) -> void { LPM.addPass(LoopInstSimplifyPass()); },
    [](LoopPassManager &LPM) -> void { LPM.addPass(LoopSimplifyCFGPass()); },
    [](LoopPassManager &LPM) -> void { LPM.addPass(SimpleLoopUnswitchPass()); },
    [](LoopPassManager &LPM) -> void { LPM.addPass(LoopIdiomRecognizePass()); },
    [](LoopPassManager &LPM) -> void { LPM.addPass(IndVarSimplifyPass()); },
};

static std::vector<std::function<void(ModulePassManager &)>> ModulePasses = {
    [](ModulePassManager &MPM) -> void { MPM.addPass(GlobalDCEPass()); },
    [](ModulePassManager &MPM) -> void { MPM.addPass(StripSymbolsPass()); },
    [](ModulePassManager &MPM) -> void {
      MPM.addPass(DeadArgumentEliminationPass());
    },
    [](ModulePassManager &MPM) -> void { MPM.addPass(GlobalOptPass()); },
    [](ModulePassManager &MPM) -> void {
      MPM.addPass(ModuleInlinerWrapperPass());
    },
    [](ModulePassManager &MPM) -> void { MPM.addPass(InternalizePass()); },
    [](ModulePassManager &MPM) -> void { MPM.addPass(IPSCCPPass()); },
};

static void runOptPasses(Oracle &O, Module &Program) {
  PassBuilder PB;

  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModulePassManager MPM;
  ModuleAnalysisManager MAM;

  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  FunctionPassManager FPM;
  for (auto FP : FunctionPasses)
    if (!O.shouldKeep())
      FP(FPM);
  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));

  LoopPassManager LPM;
  for (auto LP : LoopPasses)
    if (!O.shouldKeep())
      LP(LPM);
  MPM.addPass(createModuleToFunctionPassAdaptor(
      createFunctionToLoopPassAdaptor(std::move(LPM))));

  for (auto MP : ModulePasses)
    if (!O.shouldKeep())
      MP(MPM);

  MPM.run(Program, MAM);
}

void llvm::reduceUsingOptDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing with Optimization Passes...\n";
  runDeltaPass(Test, runOptPasses);
}
