; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %t = trunc i8 %x to i4
  %e = zext i4 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[M:.*]] = and i8 %x, 15
; CHECK: %[[E:.*]] = zext{{( nneg)?}} i8 %[[M]] to i32
; CHECK: ret i32 %[[E]]

define <4 x i32> @f_vec(<4 x i8> %x) {
  %t = trunc <4 x i8> %x to <4 x i4>
  %e = zext <4 x i4> %t to <4 x i32>
  ret <4 x i32> %e
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[M:.*]] = and <4 x i8> %x, splat (i8 15)
; CHECK: %[[E:.*]] = zext{{( nneg)?}} <4 x i8> %[[M]] to <4 x i32>
; CHECK: ret <4 x i32> %[[E]]
