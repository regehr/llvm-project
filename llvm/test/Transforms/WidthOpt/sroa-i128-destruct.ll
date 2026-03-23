; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Low-half SROA: trunc(or(and(X, HIGH_MASK), zext(lo)), i64) = lo
define i64 @low_half(i64 %lo, i128 %hi_src) {
  %mask = and i128 %hi_src, -18446744073709551616
  %lo_ext = zext i64 %lo to i128
  %combined = or i128 %mask, %lo_ext
  %lo_result = trunc i128 %combined to i64
  ret i64 %lo_result
}
; CHECK-LABEL: define i64 @low_half(
; CHECK-NEXT: ret i64 %lo

; High-half SROA: trunc(lshr(and(or(and(Y, LOW_MASK), shl(zext(hi), 64)), HIGH_MASK), 64), i64) = hi
define i64 @high_half(i64 %hi) {
  %hi_ext = zext i64 %hi to i128
  %hi_shl = shl i128 %hi_ext, 64
  %lo_mask = and i128 undef, 18446744073709551615
  %combined = or i128 %lo_mask, %hi_shl
  %high_masked = and i128 %combined, -18446744073709551616
  %shifted = lshr i128 %high_masked, 64
  %result = trunc i128 %shifted to i64
  ret i64 %result
}
; CHECK-LABEL: define i64 @high_half(
; CHECK-NEXT: ret i64 %hi
