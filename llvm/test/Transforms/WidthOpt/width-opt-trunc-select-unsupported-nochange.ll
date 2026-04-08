; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i1 %c, i32 %x, i32 %y) {
entry:
  %m = mul i32 %x, %y
  %sel = select i1 %c, i32 %m, i32 7
  %t = trunc i32 %sel to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @f(
; CHECK: %m = mul i32 %x, %y
; CHECK: %sel = select i1 %c, i32 %m, i32 7
; CHECK: %t = trunc i32 %sel to i16
; CHECK: ret i16 %t

define <4 x i16> @f_vec(i1 %c, <4 x i32> %x, <4 x i32> %y) {
entry:
  %m = mul <4 x i32> %x, %y
  %sel = select i1 %c, <4 x i32> %m, <4 x i32> splat (i32 7)
  %t = trunc <4 x i32> %sel to <4 x i16>
  ret <4 x i16> %t
}

; CHECK-LABEL: define <4 x i16> @f_vec(
; CHECK: %m = mul <4 x i32> %x, %y
; CHECK: %sel = select i1 %c, <4 x i32> %m, <4 x i32> splat (i32 7)
; CHECK: %t = trunc <4 x i32> %sel to <4 x i16>
; CHECK: ret <4 x i16> %t
