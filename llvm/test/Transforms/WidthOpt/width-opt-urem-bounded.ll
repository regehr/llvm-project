; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(urem(zext(a), zext(b)), i8) → urem(a, b):
; isZeroBoundedAtWidth(urem, 8) checks operand(0) first (line 2851):
; isZeroBoundedAtWidth(zext i8 %a, 8) → true → returns true immediately.
; The urem is narrowable because both operands are zero-bounded at 8 bits.

define i8 @urem_both_zext(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %r = urem i32 %za, %zb
  %t = trunc i32 %r to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @urem_both_zext(
; CHECK-NOT:   zext
; CHECK-NOT:   urem i32
; CHECK:       urem i8 %a, %b
; CHECK:       ret i8

; trunc(lshr(urem(x, zext(b)), 25), i8) → 0:
; tryShrinkTruncOfLowBitsBinOp: lshr AmtC=25 >= TargetWidth=8, so checks
; isZeroBoundedAtWidth(urem i32 %x, zext i8 %b, 8):
; operand(0)=%x is unconstrained → false; operand(1)=zext i8 %b → true
; → returns true via line 2852 (short-circuit OR, second operand).
; Result: lshr of 8-bit bounded value by 25 bits is always 0.

define i8 @urem_bounded_by_divisor_lshr(i32 %x, i8 %b) {
  %zb = zext i8 %b to i32
  %r = urem i32 %x, %zb
  %sh = lshr i32 %r, 25
  %t = trunc i32 %sh to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @urem_bounded_by_divisor_lshr(
; CHECK:       ret i8 0
