; A trunc of a select with constant condition and constant arms triggered a
; cast<SelectInst> assertion when IRBuilder folded B.CreateSelect(false, 0, 0)
; to a ConstantInt rather than creating a new SelectInst.
; RUN: opt -passes='width-opt' -S %s -o /dev/null

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i8 @trunc_of_const_select() {
entry:
  %sel = select i1 false, i32 0, i32 0
  %tr = trunc i32 %sel to i8
  ret i8 %tr
}
