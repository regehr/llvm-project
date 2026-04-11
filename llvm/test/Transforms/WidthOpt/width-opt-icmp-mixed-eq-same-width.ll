; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i8 %x, i8 %y) {
  %x32 = sext i8 %x to i32
  %y32 = zext i8 %y to i32
  %c = icmp eq i32 %x32, %y32
  ret i1 %c
}

; CHECK-LABEL: define i1 @f(
; CHECK: %x32 = sext i8 %x to i32
; CHECK: %y32 = zext i8 %y to i32
; CHECK: %c = icmp eq i32 %x32, %y32
; CHECK-NOT: icmp eq i9
; CHECK: ret i1 %c

define <4 x i1> @f_vec(<4 x i8> %x, <4 x i8> %y) {
  %x32 = sext <4 x i8> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %c = icmp eq <4 x i32> %x32, %y32
  ret <4 x i1> %c
}

; CHECK-LABEL: define <4 x i1> @f_vec(
; CHECK: %x32 = sext <4 x i8> %x to <4 x i32>
; CHECK: %y32 = zext <4 x i8> %y to <4 x i32>
; CHECK: %c = icmp eq <4 x i32> %x32, %y32
; CHECK-NOT: icmp eq <4 x i9>
; CHECK: ret <4 x i1> %c
