; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %t = trunc i8 %x to i4
  %e = zext i4 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @f(
; CHECK: %t = trunc i8 %x to i4
; CHECK: %e = zext i4 %t to i32
; CHECK: ret i32 %e

define <4 x i32> @f_vec(<4 x i8> %x) {
  %t = trunc <4 x i8> %x to <4 x i4>
  %e = zext <4 x i4> %t to <4 x i32>
  ret <4 x i32> %e
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %t = trunc <4 x i8> %x to <4 x i4>
; CHECK: %e = zext <4 x i4> %t to <4 x i32>
; CHECK: ret <4 x i32> %e
