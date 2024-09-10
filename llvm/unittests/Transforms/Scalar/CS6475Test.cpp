//===- llvm/unittest/Analysis/LoopPassManagerTest.cpp - LPM tests ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm-c/Transforms/PassBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/MemorySSA.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/SourceMgr.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using namespace llvm;

struct CS6475InstCombineTest : public testing::Test {
  PassBuilder PB;
  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;
  ModulePassManager MPM;
  LLVMContext Context;
  std::unique_ptr<Module> M;

  CS6475InstCombineTest() {
    // Register all the basic analyses with the managers.
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
  }

  const Instruction *getInstructionByOpcode(const BasicBlock &BB, unsigned Opcode,
                                    unsigned Number) const {
    unsigned CurrNumber = 0;
    for (const Instruction &I : BB) {
      if (I.getOpcode() == Opcode && CurrNumber == Number) {
        return &I;
      }
      ++CurrNumber;
    }
    return nullptr;
  }

  const BasicBlock *getBasicBlockByName(const Function &F, StringRef Name) const {
    for (const BasicBlock &BB : F) {
      if (BB.getName() == Name)
        return &BB;
    }
    return nullptr;
  }

  void ParseAssembly(const char *IR) {
    SMDiagnostic Error;
    M = parseAssemblyString(IR, Error, Context);
    std::string errMsg;
    raw_string_ostream os(errMsg);
    Error.print("", os);

    // A failure here means that the test itself is buggy.
    if (!M)
      report_fatal_error(os.str().c_str());
  }
};

TEST_F(CS6475InstCombineTest, efficient_induction_of_sums) {
  ParseAssembly(
"define dso_local noundef i32 @src(i32 noundef %num) local_unnamed_addr #0 {\n"
"entry:\n"
"  %0 = add i32 %num, -1001\n"
"  %or.cond = icmp ult i32 %0, -1000\n"
"  br i1 %or.cond, label %cleanup, label %for.body.preheader\n"
"\n"
"for.body.preheader:                               ; preds = %entry\n"
"  %1 = shl i32 %num, 1\n"
"  %2 = add i32 %num, -1\n"
"  %3 = zext i32 %2 to i33\n"
"  %4 = add i32 %num, -2\n"
"  %5 = zext i32 %4 to i33\n"
"  %6 = mul i33 %3, %5\n"
"  %7 = lshr i33 %6, 1\n"
"  %8 = trunc nuw i33 %7 to i32\n"
"  %9 = add i32 %1, %8\n"
"  %10 = add i32 %9, -1\n"
"  br label %cleanup\n"
"\n"
"cleanup:                                          ; preds = %for.body.preheader, %entry\n"
"  %retval.0 = phi i32 [ 0, %entry ], [ %10, %for.body.preheader ]\n"
"  ret i32 %retval.0\n"
"}\n");

  errs() << *M << "\n";

  FunctionPassManager FPM;
  FPM.addPass(InstCombinePass());
  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));

  MPM.run(*M, MAM);

  errs() << *M << "\n";

  EXPECT_TRUE(M->size() == 1);
  for(const auto& F : *M) {
    const auto *const BB = getBasicBlockByName(F, "for.body.preheader");
    EXPECT_TRUE(getInstructionByOpcode(*BB, Instruction::Add, 0));
    EXPECT_TRUE(getInstructionByOpcode(*BB, Instruction::Mul, 1));
    EXPECT_TRUE(getInstructionByOpcode(*BB, Instruction::LShr, 2));
  }
}

TEST_F(CS6475InstCombineTest, efficient_induction_of_sums_negative) {
  ParseAssembly(
"define dso_local noundef i32 @src(i32 noundef %num) local_unnamed_addr #0 {\n"
"entry:\n"
"  %0 = add i32 %num, -100000\n"
"  %or.cond = icmp ult i32 %0, -100001\n"
"  br i1 %or.cond, label %cleanup, label %for.body.preheader\n"
"\n"
"for.body.preheader:                               ; preds = %entry\n"
"  %1 = shl i32 %num, 1\n"
"  %2 = add i32 %num, -1\n"
"  %3 = zext i32 %2 to i33\n"
"  %4 = add i32 %num, -2\n"
"  %5 = zext i32 %4 to i33\n"
"  %6 = mul i33 %3, %5\n"
"  %7 = lshr i33 %6, 1\n"
"  %8 = trunc nuw i33 %7 to i32\n"
"  %9 = add i32 %1, %8\n"
"  %10 = add i32 %9, -1\n"
"  br label %cleanup\n"
"\n"
"cleanup:                                          ; preds = %for.body.preheader, %entry\n"
"  %retval.0 = phi i32 [ 0, %entry ], [ %10, %for.body.preheader ]\n"
"  ret i32 %retval.0\n"
"}\n");

  errs() << *M << "\n";

  FunctionPassManager FPM;
  FPM.addPass(InstCombinePass());
  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));

  MPM.run(*M, MAM);

  errs() << *M << "\n";

  EXPECT_TRUE(M->size() == 1);
  for(const auto& F : *M) {
    const auto *const BB = getBasicBlockByName(F, "for.body.preheader");
    EXPECT_FALSE(getInstructionByOpcode(*BB, Instruction::Add, 0));
    EXPECT_FALSE(getInstructionByOpcode(*BB, Instruction::Mul, 1));
    EXPECT_FALSE(getInstructionByOpcode(*BB, Instruction::LShr, 2));
  }
}