; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c) {
entry:
  %s = select i1 %c, i8 -1, i8 2
  %x = sext i8 %s to i32
  %y = zext i8 %s to i32
  %z = zext i8 %s to i32
  %r = add i32 %x, %y
  %t = add i32 %r, %z
  ret i32 %t
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[S:.*]] = select i1 %c, i32 255, i32 2
; CHECK: %[[N:.*]] = trunc i32 %[[S]] to i8
; CHECK: %[[X:.*]] = sext i8 %[[N]] to i32
; CHECK-NOT: zext i8 %s to i32
; CHECK: %[[R:.*]] = add i32 %[[X]], %[[S]]
; CHECK: %[[T:.*]] = add i32 %[[R]], %[[S]]
; CHECK: ret i32 %[[T]]
