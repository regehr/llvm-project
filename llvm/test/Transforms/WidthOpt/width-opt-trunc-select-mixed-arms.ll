; This trunc(select) rewrite is only break-even locally, so WidthOpt keeps the
; original wide select and trailing trunc.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i1 %c, i32 %x, i8 %y) {
entry:
  %sy = sext i8 %y to i32
  %sel = select i1 %c, i32 %x, i32 %sy
  %t = trunc i32 %sel to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @f(
; CHECK: %sy = sext i8 %y to i32
; CHECK: %sel = select i1 %c, i32 %x, i32 %sy
; CHECK: %t = trunc i32 %sel to i16
; CHECK-NOT: select i1 %c, i16
; CHECK: ret i16 %t

define <4 x i16> @f_vec(i1 %c, <4 x i32> %x, <4 x i8> %y) {
entry:
  %sy = sext <4 x i8> %y to <4 x i32>
  %sel = select i1 %c, <4 x i32> %x, <4 x i32> %sy
  %t = trunc <4 x i32> %sel to <4 x i16>
  ret <4 x i16> %t
}

; CHECK-LABEL: define <4 x i16> @f_vec(
; CHECK: %sy = sext <4 x i8> %y to <4 x i32>
; CHECK: %sel = select i1 %c, <4 x i32> %x, <4 x i32> %sy
; CHECK: %t = trunc <4 x i32> %sel to <4 x i16>
; CHECK-NOT: select i1 %c, <4 x i16>
; CHECK: ret <4 x i16> %t
