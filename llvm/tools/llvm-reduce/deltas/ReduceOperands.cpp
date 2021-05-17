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

#include "ReduceOperands.h"
#include "Delta.h"
#include "llvm/ADT/SmallVector.h"
#include <set>
#include <vector>

#include "llvm/Bitcode/BitcodeWriter.h"

using namespace llvm;

static Value *getDefaultValue(Type *T) {
  // FIXME make this a command line option
  if (false)
    return UndefValue::get(T);
  else
    return Constant::getNullValue(T);
}

/// Turn out-of-chunk operands into the default value
static void reduceOperandsInModule(std::vector<Chunk> ChunksToKeep,
                                   Module *Program) {
  Oracle O(ChunksToKeep);

  for (auto &F : *Program) {
    llvm::outs() << "function: " << F.getName() << "\n";
    
    for (auto &BB : F) {
      for (auto &I : BB) {
        int NumOperands = I.getNumOperands();
        for (int OpIndex = 0; OpIndex < NumOperands; ++OpIndex) {
          // If it's already zero, who cares?? we'll just set it to zero again
          auto T = I.getOperand(OpIndex)->getType();
          if (!isa<GetElementPtrInst>(I) && !isa<SwitchInst>(I) && !T->isLabelTy() && !O.shouldKeep())
            I.setOperand(OpIndex, getDefaultValue(T));
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
