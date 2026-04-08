; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; A shift amount >= the target width would make the narrow ashr poison, so
; this must not be transformed.

define i8 @ashr_shift_too_large(i8 %x) {
  %wide = sext i8 %x to i32
  %shifted = ashr i32 %wide, 8
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}

; CHECK-LABEL: define i8 @ashr_shift_too_large(
; CHECK: sext
; CHECK: ashr
; CHECK: trunc

define <4 x i8> @ashr_shift_too_large_vec(<4 x i8> %x) {
  %wide = sext <4 x i8> %x to <4 x i32>
  %shifted = ashr <4 x i32> %wide, splat (i32 8)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}

; CHECK-LABEL: define <4 x i8> @ashr_shift_too_large_vec(
; CHECK: sext
; CHECK: ashr
; CHECK: trunc
