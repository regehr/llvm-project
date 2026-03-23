; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i8 %x, i8 %y) {
entry:
  %wide = zext i8 %x to i32
  %tx = trunc i32 %wide to i16
  %sy = sext i8 %y to i16
  %cmp = icmp ult i16 %tx, %sy
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @f(
; CHECK: %sy = sext i8 %y to i16
; CHECK: %[[X16:.*]] = zext i8 %x to i16
; CHECK: %[[CMP:.*]] = icmp ult i16 %[[X16]], %sy
; CHECK-NOT: icmp ult i32
; CHECK: ret i1 %[[CMP]]
