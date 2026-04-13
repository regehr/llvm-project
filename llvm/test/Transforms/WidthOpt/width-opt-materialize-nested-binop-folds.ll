; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; These tests cover identity-fold shortcuts in materializeTruncRootedValueAtWidth
; when a nested BinaryOperator's operands materialize to constants.
; The outer binop is processed by tryShrinkTruncOfLowBitsBinOp which calls
; materializeTruncRootedValueAtWidth on the inner binop.

; --- And: AllOnes(NarrowRHS) → FoldedResult = NarrowLHS (line 3578) ---
; trunc(or(and(za, 255), zb), i8): inner and(za, 255), NarrowRHS=all-ones → %a.

define i8 @materialize_and_all_ones_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %inner = and i32 %za, 255
  %outer = or i32 %inner, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_and_all_ones_rhs(
; CHECK-NOT:   and i32
; CHECK-NOT:   zext
; CHECK:       or i8 %a, %b
; CHECK:       ret i8

; --- And: AllOnes(NarrowLHS) → FoldedResult = NarrowRHS (line 3579) ---
; trunc(or(and(255, za), zb), i8): constant on LHS, NarrowLHS=all-ones → %a.

define i8 @materialize_and_all_ones_lhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %inner = and i32 255, %za
  %outer = or i32 %inner, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_and_all_ones_lhs(
; CHECK-NOT:   and i32
; CHECK-NOT:   zext
; CHECK:       or i8 %a, %b
; CHECK:       ret i8

; --- Mul: IsZero(NarrowRHS) → FoldedResult = 0 (lines 3600-3601) ---
; trunc(or(mul(za, 0), zb), i8): inner mul, NarrowRHS=0 → 0, outer or folds.

define i8 @materialize_mul_zero_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %mul = mul i32 %za, 0
  %outer = or i32 %mul, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_mul_zero_rhs(
; CHECK-NOT:   mul
; CHECK-NOT:   zext
; CHECK:       ret i8 %b

; --- Mul: IsOne(NarrowRHS) → FoldedResult = NarrowLHS (line 3598) ---
; trunc(or(mul(za, 1), zb), i8): inner mul, NarrowRHS=1 → %a.

define i8 @materialize_mul_one_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %mul = mul i32 %za, 1
  %outer = or i32 %mul, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_mul_one_rhs(
; CHECK-NOT:   mul
; CHECK-NOT:   zext
; CHECK:       or i8 %a, %b
; CHECK:       ret i8

; --- Add: IsZero(NarrowRHS) → FoldedResult = NarrowLHS (line 3593) ---
; trunc(or(add(za, 0), zb), i8): inner add, NarrowRHS=0 → %a.

define i8 @materialize_add_zero_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %add = add i32 %za, 0
  %outer = or i32 %add, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_add_zero_rhs(
; CHECK-NOT:   add i32
; CHECK-NOT:   zext
; CHECK:       or i8 %a, %b
; CHECK:       ret i8

; --- Sub: IsZero(NarrowRHS) → FoldedResult = NarrowLHS (line 3596) ---
; trunc(or(sub(za, 0), zb), i8): inner sub, NarrowRHS=0 → %a.

define i8 @materialize_sub_zero_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %sub = sub i32 %za, 0
  %outer = or i32 %sub, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_sub_zero_rhs(
; CHECK-NOT:   sub i32
; CHECK-NOT:   zext
; CHECK:       or i8 %a, %b
; CHECK:       ret i8

; --- Xor: IsZero(NarrowRHS) → FoldedResult = NarrowLHS (line 3590) ---
; trunc(or(xor(za, 0), zb), i8): inner xor, NarrowRHS=0 → %a.

define i8 @materialize_xor_zero_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %xor = xor i32 %za, 0
  %outer = or i32 %xor, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_xor_zero_rhs(
; CHECK-NOT:   xor i32
; CHECK-NOT:   zext
; CHECK:       or i8 %a, %b
; CHECK:       ret i8

; --- Or: AllOnes(NarrowRHS) → all-ones result (lines 3586-3588) ---
; trunc(and(or(za, 255), zb), i8): inner or(za,255), NarrowRHS=all-ones → all-ones.

define i8 @materialize_or_all_ones_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %inner = or i32 %za, 255
  %outer = and i32 %inner, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @materialize_or_all_ones_rhs(
; CHECK-NOT:   or i32
; CHECK-NOT:   zext
; CHECK-NOT:   and i8
; CHECK:       ret i8 %b
