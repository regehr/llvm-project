; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %ext = sext i8 %x to i32
  %and = and i32 255, %ext
  ret i32 %and
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[Z:.*]] = zext i8 %x to i32
; CHECK: %[[A:.*]] = and i32 255, %[[Z]]
; CHECK: ret i32 %[[A]]

define <4 x i32> @f_vec(<4 x i8> %x) {
  %ext = sext <4 x i8> %x to <4 x i32>
  %and = and <4 x i32> splat (i32 255), %ext
  ret <4 x i32> %and
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[Z:.*]] = zext <4 x i8> %x to <4 x i32>
; CHECK: %[[A:.*]] = and <4 x i32> splat (i32 255), %[[Z]]
; CHECK: ret <4 x i32> %[[A]]
