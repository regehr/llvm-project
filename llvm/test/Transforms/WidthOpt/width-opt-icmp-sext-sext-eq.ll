; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i8 %x, i16 %y) {
  %x32 = sext i8 %x to i32
  %y32 = sext i16 %y to i32
  %c = icmp eq i32 %x32, %y32
  ret i1 %c
}

; CHECK-LABEL: define i1 @f(
; CHECK: %[[X16:.*]] = sext i8 %x to i16
; CHECK: %[[C:.*]] = icmp eq i16 %[[X16]], %y
; CHECK: ret i1 %[[C]]

define <4 x i1> @f_vec(<4 x i8> %x, <4 x i16> %y) {
  %x32 = sext <4 x i8> %x to <4 x i32>
  %y32 = sext <4 x i16> %y to <4 x i32>
  %c = icmp eq <4 x i32> %x32, %y32
  ret <4 x i1> %c
}

; CHECK-LABEL: define <4 x i1> @f_vec(
; CHECK: %[[X16:.*]] = sext <4 x i8> %x to <4 x i16>
; CHECK: %[[C:.*]] = icmp eq <4 x i16> %[[X16]], %y
; CHECK: ret <4 x i1> %[[C]]
