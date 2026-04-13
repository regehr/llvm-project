; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; These tests cover the identity-fold shortcuts in tryShrinkTruncOfLowBitsBinOp
; (lines 3334, 3340, 3342, 3343, 3353) where a narrowed constant operand is
; 0 or all-ones, allowing the binop to be replaced by a simpler value.

; trunc(and(zext_i8, 256), i8): 256 truncated to i8 = 0 → and(X,0)=0. Line 3334.

define i8 @trunc_and_zero_rhs(i8 %a) {
  %z = zext i8 %a to i32
  %and = and i32 %z, 256
  %t = trunc i32 %and to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_and_zero_rhs(
; CHECK-NOT:   zext
; CHECK-NOT:   and
; CHECK:       ret i8 0

; trunc(or(zext_i8, 255), i8): 255 is all-ones for i8 → or(X,all-ones)=all-ones. Line 3340.

define i8 @trunc_or_all_ones_rhs(i8 %a) {
  %z = zext i8 %a to i32
  %or = or i32 %z, 255
  %t = trunc i32 %or to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_or_all_ones_rhs(
; CHECK-NOT:   zext
; CHECK-NOT:   or
; CHECK:       ret i8 -1

; trunc(xor(zext_i8, 256), i8): 256 truncated to i8 = 0 → xor(X,0)=X. Line 3342.

define i8 @trunc_xor_zero_rhs(i8 %a) {
  %z = zext i8 %a to i32
  %xor = xor i32 %z, 256
  %t = trunc i32 %xor to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_xor_zero_rhs(
; CHECK-NOT:   zext
; CHECK-NOT:   xor
; CHECK:       ret i8 %a

; trunc(xor(256, zext_i8), i8): constant on LHS, 256→0 → xor(0,X)=X. Line 3343.

define i8 @trunc_xor_zero_lhs(i8 %a) {
  %z = zext i8 %a to i32
  %xor = xor i32 256, %z
  %t = trunc i32 %xor to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_xor_zero_lhs(
; CHECK-NOT:   zext
; CHECK-NOT:   xor
; CHECK:       ret i8 %a

; trunc(mul(zext_i8, 256), i8): 256 truncated to i8 = 0 → mul(X,0)=0. Line 3353.

define i8 @trunc_mul_zero_rhs(i8 %a) {
  %z = zext i8 %a to i32
  %mul = mul i32 %z, 256
  %t = trunc i32 %mul to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_mul_zero_rhs(
; CHECK-NOT:   zext
; CHECK-NOT:   mul
; CHECK:       ret i8 0
