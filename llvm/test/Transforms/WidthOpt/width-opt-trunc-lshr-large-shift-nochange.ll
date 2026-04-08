; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; lshr of a zero-bounded value (zext i8→i32) by a shift amount >= 8 shifts
; out all value bits and yields 0.  We fold to the constant directly rather
; than emitting a narrow lshr (which would be poison for shift >= bitwidth).

define i8 @lshr_shift_too_large(i8 %x) {
  %wide = zext i8 %x to i32
  %shifted = lshr i32 %wide, 8
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}

; CHECK-LABEL: define i8 @lshr_shift_too_large(
; CHECK-NOT: zext
; CHECK-NOT: lshr
; CHECK-NOT: trunc
; CHECK: ret i8 0

define <4 x i8> @lshr_shift_too_large_vec(<4 x i8> %x) {
  %wide = zext <4 x i8> %x to <4 x i32>
  %shifted = lshr <4 x i32> %wide, splat (i32 8)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}

; CHECK-LABEL: define <4 x i8> @lshr_shift_too_large_vec(
; CHECK-NOT: zext
; CHECK-NOT: lshr
; CHECK-NOT: trunc
; CHECK: ret <4 x i8> zeroinitializer
