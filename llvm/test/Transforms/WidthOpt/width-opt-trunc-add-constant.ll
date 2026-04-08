; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i16 %x) {
entry:
  %x32 = zext i16 %x to i32
  %add = add i32 %x32, 5
  %t = trunc i32 %add to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @f(
; CHECK-NOT: add i32
; CHECK-NOT: trunc i32
; CHECK: %[[ADD:.*]] = add i16 %x, 5
; CHECK: ret i16 %[[ADD]]

define <4 x i16> @f_vec(<4 x i16> %x) {
entry:
  %x32 = zext <4 x i16> %x to <4 x i32>
  %add = add <4 x i32> %x32, splat (i32 5)
  %t = trunc <4 x i32> %add to <4 x i16>
  ret <4 x i16> %t
}

; CHECK-LABEL: define <4 x i16> @f_vec(
; CHECK-NOT: add <4 x i32>
; CHECK-NOT: trunc <4 x i32>
; CHECK: %[[ADD:.*]] = add <4 x i16> %x, splat (i16 5)
; CHECK: ret <4 x i16> %[[ADD]]
