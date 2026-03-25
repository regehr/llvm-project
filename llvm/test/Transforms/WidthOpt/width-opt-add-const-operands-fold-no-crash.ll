; tryWidenAddOverTruncThroughZExt called B.CreateAdd(X, WideC) where X was a
; ConstantInt (trunc-of-zero folded to zero).  IRBuilder folded the add to a
; ConstantInt rather than an AddInst, and the subsequent cast<Instruction>
; asserted.  Same latent bug in tryWidenAddThroughZExt with constant operands.
; RUN: opt -passes='width-opt' -S %s -o /dev/null

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; %t = trunc i32 0 to i8: X is the constant i32 0.
; tryWidenAddOverTruncThroughZExt builds add iW(i32 0, i32 0) which folds to
; ConstantInt; cast<Instruction> then asserted.
define i32 @add_over_trunc_of_const_zext(i32 %unused) {
entry:
  %t    = trunc i32 0 to i8
  %add  = add i8 %t, 0
  %ext  = zext i8 %add to i32
  ret i32 %ext
}
