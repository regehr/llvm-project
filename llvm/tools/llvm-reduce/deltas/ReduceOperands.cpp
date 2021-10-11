//===- ReduceOperands.cpp - Specialized Delta Pass ------------------------===//
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

#include "ReduceOperands.h"
#include "Delta.h"
#include "llvm/ADT/SmallVector.h"
#include <set>
#include <vector>

using namespace llvm;

/// Returns if the given operand is undef.
static bool operandIsUndefValue(Use &Op) {
  if (auto *C = dyn_cast<Constant>(Op)) {
    return isa<UndefValue>(C);
  }
  return false;
}

/// Returns if an operand can be reduced to undef.
/// TODO: make this logic check what types are reducible rather than
/// check what types that are not reducible.
static bool canReduceOperand(Use &Op) {
  auto *Ty = Op->getType();
  // Can't reduce labels to undef
  return !Ty->isLabelTy() && !operandIsUndefValue(Op);
}

/// Sets Operands to undef.
static void extractOperandsFromModule(Oracle &O, Module &Program) {
  // Extract Operands from the module.
  for (auto &F : Program.functions()) {
    for (auto &I : instructions(&F)) {
      for (auto &Op : I.operands()) {
        // Filter Operands then set to undef.
        if (canReduceOperand(Op) && !O.shouldKeep()) {
          auto *Ty = Op->getType();
          Op.set(UndefValue::get(Ty));
        }
      }
    }
  }
}

/// Counts the amount of operands in the module that can be reduced.
static int countOperands(Module &Program) {
  int Count = 0;
  for (auto &F : Program.functions()) {
    for (auto &I : instructions(&F)) {
      for (auto &Op : I.operands()) {
        if (canReduceOperand(Op)) {
          Count++;
        }
      }
    }
  }
}

/// Counts the amount of basic blocks and prints their name & respective index
static unsigned countOperands(Module *Program) {
  // TODO: Silence index with --quiet flag
  outs() << "----------------------------\n";
  int OperandCount = 0;
  for (auto &F : *Program)
    for (auto &BB : F)
      for (auto &I : BB)
        OperandCount += I.getNumOperands();
  outs() << "Number of operands: " << OperandCount << "\n";

  return OperandCount;
}

void llvm::reduceOperandsDeltaPass(TestRunner &Test) {
  outs() << "*** Reducing Operands...\n";
  unsigned OperandCount = countOperands(Test.getProgram());
  runDeltaPass(Test, OperandCount, reduceOperandsInModule);
}
