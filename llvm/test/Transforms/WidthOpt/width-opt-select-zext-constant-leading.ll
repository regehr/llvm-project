; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %x) {
entry:
  %zx = zext i8 %x to i32
  %r = select i1 %c, i32 7, i32 %zx
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[R8:.*]] = select i1 %c, i8 7, i8 %x
; CHECK: %[[R:.*]] = zext i8 %[[R8]] to i32
; CHECK: ret i32 %[[R]]

define <4 x i32> @f_vec(<4 x i1> %c, <4 x i8> %x) {
entry:
  %zx = zext <4 x i8> %x to <4 x i32>
  %r = select <4 x i1> %c, <4 x i32> splat (i32 7), <4 x i32> %zx
  ret <4 x i32> %r
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[R8:.*]] = select <4 x i1> %c, <4 x i8> splat (i8 7), <4 x i8> %x
; CHECK: %[[R:.*]] = zext <4 x i8> %[[R8]] to <4 x i32>
; CHECK: ret <4 x i32> %[[R]]
