; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %k) {
entry:
  %s = select i1 %c, i8 -1, i8 2
  %cmp = icmp slt i8 %s, %k
  %x = sext i8 %s to i32
  %y = sext i8 %s to i32
  %z = sext i8 %s to i32
  %r = add i32 %x, %y
  %cmp.z = zext i1 %cmp to i32
  %t = add i32 %r, %z
  %u = add i32 %t, %cmp.z
  ret i32 %u
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[S:.*]] = select i1 %c, i32 -1, i32 2
; CHECK: %[[K32:.*]] = sext i8 %k to i32
; CHECK: %[[CMP:.*]] = icmp slt i32 %[[S]], %[[K32]]
; CHECK-NOT: trunc i32 %[[S]] to i8
; CHECK: %[[R:.*]] = add i32 %[[S]], %[[S]]
; CHECK: %[[CMPZ:.*]] = zext i1 %[[CMP]] to i32
; CHECK: %[[T:.*]] = add i32 %[[R]], %[[S]]
; CHECK: %[[U:.*]] = add i32 %[[T]], %[[CMPZ]]
; CHECK: ret i32 %[[U]]
