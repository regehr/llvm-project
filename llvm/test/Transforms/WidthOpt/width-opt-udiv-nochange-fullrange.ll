; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i32 %x, i32 %y) {
entry:
  %d = udiv i32 %x, %y
  ret i32 %d
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[D:.*]] = udiv i32 %x, %y
; CHECK: ret i32 %[[D]]

define <4 x i32> @f_vec(<4 x i32> %x, <4 x i32> %y) {
entry:
  %d = udiv <4 x i32> %x, %y
  ret <4 x i32> %d
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[D:.*]] = udiv <4 x i32> %x, %y
; CHECK: ret <4 x i32> %[[D]]
