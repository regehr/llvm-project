; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i8 %x, i8 %y) {
  %x32 = sext i8 %x to i32
  %y32 = zext i8 %y to i32
  %c = icmp eq i32 %x32, %y32
  ret i1 %c
}

; CHECK-LABEL: define i1 @f(
; CHECK: %[[X9:.*]] = sext i8 %x to i9
; CHECK: %[[Y9:.*]] = zext i8 %y to i9
; CHECK: %[[C:.*]] = icmp eq i9 %[[X9]], %[[Y9]]
; CHECK-NOT: icmp eq i8
; CHECK: ret i1 %[[C]]

define <4 x i1> @f_vec(<4 x i8> %x, <4 x i8> %y) {
  %x32 = sext <4 x i8> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %c = icmp eq <4 x i32> %x32, %y32
  ret <4 x i1> %c
}

; CHECK-LABEL: define <4 x i1> @f_vec(
; CHECK: %[[X9:.*]] = sext <4 x i8> %x to <4 x i9>
; CHECK: %[[Y9:.*]] = zext <4 x i8> %y to <4 x i9>
; CHECK: %[[C:.*]] = icmp eq <4 x i9> %[[X9]], %[[Y9]]
; CHECK-NOT: icmp eq <4 x i8>
; CHECK: ret <4 x i1> %[[C]]
