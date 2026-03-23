; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %k) {
entry:
  %s = select i1 %c, i8 -1, i8 2
  %cmp = icmp ult i8 %s, %k
  %x = sext i8 %s to i32
  %y = sext i8 %s to i32
  %sum = add i32 %x, %y
  %cmp.z = zext i1 %cmp to i32
  %r = add i32 %sum, %cmp.z
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[S32:.*]] = select i1 %c, i32 -1, i32 2
; CHECK: %[[CMP:.*]] = icmp ult i8 2, %k
; CHECK: %[[SPLIT:.*]] = select i1 %c, i1 false, i1 %[[CMP]]
; CHECK-NOT: trunc i32 %[[S32]] to i8
; CHECK-NOT: sext i8 %s to i32
; CHECK: %sum = add i32 %[[S32]], %[[S32]]
; CHECK: %cmp.z = zext i1 %[[SPLIT]] to i32
; CHECK: %r = add i32 %sum, %cmp.z
