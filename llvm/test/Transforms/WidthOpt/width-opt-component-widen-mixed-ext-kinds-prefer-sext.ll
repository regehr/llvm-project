; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c) {
entry:
  %s = select i1 %c, i8 -1, i8 2
  %x = sext i8 %s to i32
  %y = sext i8 %s to i32
  %z = zext i8 %s to i32
  %r = add i32 %x, %y
  %t = add i32 %r, %z
  ret i32 %t
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[S:.*]] = select i1 %c, i32 -1, i32 2
; CHECK-NOT: sext i8
; CHECK: %[[Z:.*]] = and i32 %[[S]], 255
; CHECK-NOT: trunc i32 %[[S]] to i8
; CHECK: %[[R:.*]] = add i32 %[[S]], %[[S]]
; CHECK: %[[T:.*]] = add i32 %[[R]], %[[Z]]
; CHECK: ret i32 %[[T]]
