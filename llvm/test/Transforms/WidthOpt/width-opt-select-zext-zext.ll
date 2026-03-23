; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %x, i8 %y) {
entry:
  %zx = zext i8 %x to i32
  %zy = zext i8 %y to i32
  %r = select i1 %c, i32 %zx, i32 %zy
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[N:.*]] = select i1 %c, i8 %x, i8 %y
; CHECK: %[[W:.*]] = zext i8 %[[N]] to i32
; CHECK: ret i32 %[[W]]
