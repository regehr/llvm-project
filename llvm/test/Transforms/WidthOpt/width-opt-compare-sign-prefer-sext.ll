; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c) {
entry:
  %s = select i1 %c, i8 -1, i8 2
  %cmp = icmp slt i8 %s, 5
  %x = sext i8 %s to i32
  %y = zext i8 %s to i32
  %cmp.z = zext i1 %cmp to i32
  %r = add i32 %x, %y
  %t = add i32 %r, %cmp.z
  ret i32 %t
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[S:.*]] = select i1 %c, i32 -1, i32 2
; CHECK: %[[CMP:.*]] = icmp slt i32 %[[S]], 5
; CHECK-NOT: trunc i32 %[[S]] to i8
; CHECK: %[[Z:.*]] = and i32 %[[S]], 255
; CHECK: %cmp.z = zext i1 %[[CMP]] to i32
; CHECK: %r = add i32 %[[S]], %[[Z]]
; CHECK: %t = add i32 %r, %cmp.z
; CHECK: ret i32 %t
