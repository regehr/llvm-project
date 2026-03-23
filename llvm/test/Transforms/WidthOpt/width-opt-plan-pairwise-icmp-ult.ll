; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c0, i1 %c1) {
entry:
  %s = select i1 %c0, i8 -1, i8 2
  %k = select i1 %c1, i8 5, i8 9
  %cmp = icmp ult i8 %s, %k
  %cmpz = zext i1 %cmp to i32
  %sx0 = sext i8 %s to i32
  %kz0 = zext i8 %k to i32
  %sum0 = add i32 %sx0, %kz0
  %r = add i32 %sum0, %cmpz
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[S:.*]] = select i1 %c0, i32 -1, i32 2
; CHECK: %[[K:.*]] = select i1 %c1, i32 5, i32 9
; CHECK: icmp ult i32
; CHECK-NOT: icmp ult i8
; CHECK-NOT: sext i8
; CHECK-NOT: zext i8
; CHECK: %cmpz = zext i1
; CHECK: %sum0 = add i32 %[[S]], %[[K]]
; CHECK: %r = add i32 %sum0, %cmpz
; CHECK: ret i32 %r
