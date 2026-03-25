; Singleton trunc widening called B.CreateTrunc(poison, i8) which IRBuilder
; folded to PoisonValue.  The subsequent cast<Instruction> then asserted.
; RUN: opt -passes='width-opt' -S %s -o /dev/null

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; %conv996 = trunc i32 poison to i8: CreateTrunc(poison) folds to PoisonValue,
; so CreateSExt(PoisonValue) also folds; cast<Instruction> on either result asserted.
define i16 @trunc_singleton_poison_operand() {
entry:
  %conv996 = trunc i32 poison to i8
  %sext = sext i8 %conv996 to i16
  ret i16 %sext
}
