; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i8 %x, i16 %y) {
  %x32 = zext i8 %x to i32
  %y32 = sext i16 %y to i32
  %c = icmp eq i32 %x32, %y32
  ret i1 %c
}

; CHECK-LABEL: define i1 @f(
; CHECK: %[[X16:.*]] = zext i8 %x to i16
; CHECK: %[[C:.*]] = icmp eq i16 %[[X16]], %y
; CHECK: ret i1 %[[C]]
