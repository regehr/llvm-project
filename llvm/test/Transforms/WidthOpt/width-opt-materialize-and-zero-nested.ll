; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; materializeTruncRootedValueAtWidth: And where NarrowLHS is zero (lines 3580-3582).
; The inner and(mul(za,0), zb) materializes via recursion: mul→0, and(0,b)→0.
; The outer or then gets a zero left operand and folds to %c.

define i8 @materialize_and_zero_from_nested_mul(i8 %a, i8 %b, i8 %c) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %zc = zext i8 %c to i32
  %mul = mul i32 %za, 0
  %inner = and i32 %mul, %zb
  %outer = or i32 %inner, %zc
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_and_zero_from_nested_mul(
; CHECK-NOT:   mul
; CHECK-NOT:   and i32
; CHECK-NOT:   or i32
; CHECK:       ret i8 %c
