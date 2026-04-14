; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryShrinkSROAI128Destruct: continue paths in the user-scan loop.
; All functions have a qualifying LowTrunc so the function proceeds to transform.

; lines 3036, 3040, 3043: various non-qualifying users that hit continue.
; The function still replaces %lo_result with %lo.

define i64 @sroa_destruct_extra_users(i64 %lo, i128 %wide, ptr %p) {
  %masked = and i128 %wide, -18446744073709551616
  %lo_ext = zext i64 %lo to i128
  %combined = or i128 %masked, %lo_ext
  ; Qualifying LowTrunc (TargetWidth=64)
  %lo_result = trunc i128 %combined to i64
  ; line 3036: TruncInst with different width (32 ≠ 64) → continue
  %bad_trunc = trunc i128 %combined to i32
  store i32 %bad_trunc, ptr %p
  ; line 3040: non-LShr BinaryOperator user (And) → continue
  %and_use = and i128 %combined, 255
  store i128 %and_use, ptr %p
  ; line 3043: LShr with wrong shift amount (32 ≠ 64) → continue
  %bad_shift = lshr i128 %combined, 32
  store i128 %bad_shift, ptr %p
  ret i64 %lo_result
}
; CHECK-LABEL: define i64 @sroa_destruct_extra_users(
; CHECK: and i128
; CHECK: or i128
; CHECK: trunc i128 %combined to i32
; CHECK: and i128 %combined, 255
; CHECK: lshr i128 %combined, 32
; CHECK: ret i64 %lo

; line 3045: LShr with multiple uses → !hasOneUse() → continue.
; Function still transforms via LowTrunc.

define i64 @sroa_destruct_lshr_multi_use(i64 %lo, i128 %wide, ptr %p) {
  %masked = and i128 %wide, -18446744073709551616
  %lo_ext = zext i64 %lo to i128
  %combined = or i128 %masked, %lo_ext
  %lo_result = trunc i128 %combined to i64
  ; LShr with two TruncInst users → multi-use → line 3045 continue
  %multi_lshr = lshr i128 %combined, 64
  %use1 = trunc i128 %multi_lshr to i64
  %use2 = trunc i128 %multi_lshr to i32
  store i64 %use1, ptr %p
  store i32 %use2, ptr %p
  ret i64 %lo_result
}
; CHECK-LABEL: define i64 @sroa_destruct_lshr_multi_use(
; CHECK: lshr i128
; CHECK: ret i64 %lo

; line 3048: LShr with one use but user is not a TruncInst → HighTrunc=null → continue.

define i64 @sroa_destruct_lshr_no_trunc(i64 %lo, i128 %wide, ptr %p) {
  %masked = and i128 %wide, -18446744073709551616
  %lo_ext = zext i64 %lo to i128
  %combined = or i128 %masked, %lo_ext
  %lo_result = trunc i128 %combined to i64
  ; LShr with single use = store (not a TruncInst) → line 3048 continue
  %lshr64 = lshr i128 %combined, 64
  store i128 %lshr64, ptr %p
  ret i64 %lo_result
}
; CHECK-LABEL: define i64 @sroa_destruct_lshr_no_trunc(
; CHECK: lshr i128 %combined, 64
; CHECK: ret i64 %lo
