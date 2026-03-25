; tryShrinkTruncOfLowBitsBinOp matched trunc(ashr(sext(a:N→W), k), N) and
; called B.CreateAShr(LHSInfo->NarrowValue, NarrowAmt) where NarrowValue was
; the constant i8 0 (the operand of "sext i8 0 to i32").  IRBuilder folded
; ashr(0, 0) to ConstantInt, and the subsequent cast<Instruction> asserted.
; Same latent bug in the lshr/ashr-chain variants.
; RUN: opt -passes='width-opt' -S %s -o /dev/null

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; sext i8 0 → ashr by 0 → trunc back to i8:
; CreateAShr(i8 0, i8 0) folds to ConstantInt; cast<Instruction> asserted.
define i8 @trunc_ashr_sext_of_const() {
entry:
  %s = sext i8 0 to i32
  %a = ashr i32 %s, 0
  %t = trunc i32 %a to i8
  ret i8 %t
}

; zext variant: trunc(ashr(zext(a:N→W), k), N) → lshr(a, k)
define i8 @trunc_ashr_zext_of_const() {
entry:
  %z = zext i8 0 to i32
  %a = ashr i32 %z, 0
  %t = trunc i32 %a to i8
  ret i8 %t
}

; lshr chain variant
define i8 @trunc_lshr_sext_of_const() {
entry:
  %s = sext i8 0 to i32
  %a = lshr i32 %s, 0
  %t = trunc i32 %a to i8
  ret i8 %t
}
