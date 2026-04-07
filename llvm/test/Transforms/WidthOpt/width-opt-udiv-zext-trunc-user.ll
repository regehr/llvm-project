; If both operands already come from removable zexts and the only wide result
; user is a trunc, rebuild the udiv at the narrow width and delete all three
; width changes.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i8 @f(i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %d = udiv i32 %x32, %y32
  %t = trunc i32 %d to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @f(
; CHECK-NOT: zext i8 %x to i32
; CHECK-NOT: zext i8 %y to i32
; CHECK: %[[D8:.*]] = udiv i8 %x, %y
; CHECK-NOT: trunc i32 %d to i8
; CHECK: ret i8 %[[D8]]

define <4 x i8> @f_vec(<4 x i8> %x, <4 x i8> %y) {
entry:
  %x32 = zext <4 x i8> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %d = udiv <4 x i32> %x32, %y32
  %t = trunc <4 x i32> %d to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @f_vec(
; CHECK-NOT: zext <4 x i8> %x to <4 x i32>
; CHECK-NOT: zext <4 x i8> %y to <4 x i32>
; CHECK: %[[D8:.*]] = udiv <4 x i8> %x, %y
; CHECK-NOT: trunc <4 x i32> %d to <4 x i8>
; CHECK: ret <4 x i8> %[[D8]]
