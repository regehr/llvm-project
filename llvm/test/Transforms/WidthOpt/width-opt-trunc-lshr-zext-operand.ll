; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(lshr(zext(x), k)) -> lshr(x, k): the zext guarantees zero bits above
; the source width, so the logical shift cannot pull nonzero bits into the
; truncated region. Both the zext and trunc are removed.

define i8 @lshr_zext_i8_to_i32(i8 %x) {
  %wide = zext i8 %x to i32
  %shifted = lshr i32 %wide, 3
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}

; CHECK-LABEL: define i8 @lshr_zext_i8_to_i32(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: lshr i8 %x, 3
; CHECK: ret i8

define <4 x i8> @lshr_zext_i8_to_i32_vec(<4 x i8> %x) {
  %wide = zext <4 x i8> %x to <4 x i32>
  %shifted = lshr <4 x i32> %wide, splat (i32 3)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}

; CHECK-LABEL: define <4 x i8> @lshr_zext_i8_to_i32_vec(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: lshr <4 x i8> %x, splat (i8 3)
; CHECK: ret <4 x i8>

; Same pattern through an intermediate width.

define i8 @lshr_zext_i8_to_i16(i8 %x) {
  %wide = zext i8 %x to i16
  %shifted = lshr i16 %wide, 5
  %narrow = trunc i16 %shifted to i8
  ret i8 %narrow
}

; CHECK-LABEL: define i8 @lshr_zext_i8_to_i16(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: lshr i8 %x, 5
; CHECK: ret i8

define <4 x i8> @lshr_zext_i8_to_i16_vec(<4 x i8> %x) {
  %wide = zext <4 x i8> %x to <4 x i16>
  %shifted = lshr <4 x i16> %wide, splat (i16 5)
  %narrow = trunc <4 x i16> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}

; CHECK-LABEL: define <4 x i8> @lshr_zext_i8_to_i16_vec(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: lshr <4 x i8> %x, splat (i8 5)
; CHECK: ret <4 x i8>

; Shift amount of 1 (minimum nontrivial case).

define i8 @lshr_zext_shift_by_one(i8 %x) {
  %wide = zext i8 %x to i32
  %shifted = lshr i32 %wide, 1
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}

; CHECK-LABEL: define i8 @lshr_zext_shift_by_one(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: lshr i8 %x, 1
; CHECK: ret i8

define <4 x i8> @lshr_zext_shift_by_one_vec(<4 x i8> %x) {
  %wide = zext <4 x i8> %x to <4 x i32>
  %shifted = lshr <4 x i32> %wide, splat (i32 1)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}

; CHECK-LABEL: define <4 x i8> @lshr_zext_shift_by_one_vec(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: lshr <4 x i8> %x, splat (i8 1)
; CHECK: ret <4 x i8>
