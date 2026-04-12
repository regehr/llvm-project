; Constant folding of trunc(binop(C1, C2)):
; When a trunc's source is a binop with both operands as constants, width-opt
; folds the binop to a constant then folds the trunc.
; Opcodes in the fold switch: LShr, AShr, Shl, And, Or.
; Opcodes not in the switch (e.g., mul) fall through to tryShrinkTruncOfLowBitsBinOp.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; lshr: 256 >> 1 = 128, trunc i32 128 to i8 = -128
; width-opt folds the trunc to a constant; the dead binop is left for DCE.
define i8 @trunc_lshr_consts() {
  %r = lshr i32 256, 1
  %t = trunc i32 %r to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_lshr_consts(
; CHECK-NOT:   trunc
; CHECK:       ret i8 -128

; ashr: -256 >> 1 = -128, trunc i32 -128 to i8 = -128
define i8 @trunc_ashr_consts() {
  %r = ashr i32 -256, 1
  %t = trunc i32 %r to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_ashr_consts(
; CHECK-NOT:   trunc
; CHECK:       ret i8 -128

; shl: 1 << 4 = 16, trunc i32 16 to i8 = 16
define i8 @trunc_shl_consts() {
  %r = shl i32 1, 4
  %t = trunc i32 %r to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_shl_consts(
; CHECK-NOT:   trunc
; CHECK:       ret i8 16

; and: 511 & 255 = 255, trunc i32 255 to i8 = -1
define i8 @trunc_and_consts() {
  %r = and i32 511, 255
  %t = trunc i32 %r to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_and_consts(
; CHECK-NOT:   trunc
; CHECK:       ret i8 -1

; or: 256 | 15 = 271, trunc i32 271 to i8 = 15
define i8 @trunc_or_consts() {
  %r = or i32 256, 15
  %t = trunc i32 %r to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_or_consts(
; CHECK-NOT:   trunc
; CHECK:       ret i8 15

; mul is not in the fold switch (hits "default: goto done_const_fold"), but
; tryShrinkTruncOfLowBitsBinOp handles it: 300*3=900, trunc to i8 = -124
define i8 @trunc_mul_consts() {
  %r = mul i32 300, 3
  %t = trunc i32 %r to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_mul_consts(
; CHECK-NOT:   trunc
; CHECK:       ret i8 -124
