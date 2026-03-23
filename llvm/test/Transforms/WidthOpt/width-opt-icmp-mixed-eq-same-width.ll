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
