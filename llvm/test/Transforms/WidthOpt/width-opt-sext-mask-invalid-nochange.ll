; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %ext = sext i8 %x to i32
  %and = and i32 %ext, 511
  ret i32 %and
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[EXT:.*]] = sext i8 %x to i32
; CHECK: %[[AND:.*]] = and i32 %[[EXT]], 511
; CHECK: ret i32 %[[AND]]

define <4 x i32> @f_vec(<4 x i8> %x) {
  %ext = sext <4 x i8> %x to <4 x i32>
  %and = and <4 x i32> %ext, splat (i32 511)
  ret <4 x i32> %and
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[EXT:.*]] = sext <4 x i8> %x to <4 x i32>
; CHECK: %[[AND:.*]] = and <4 x i32> %[[EXT]], splat (i32 511)
; CHECK: ret <4 x i32> %[[AND]]
