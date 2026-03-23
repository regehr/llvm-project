; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %x) {
entry:
  %zx = zext i8 %x to i32
  %r = select i1 %c, i32 7, i32 %zx
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[R8:.*]] = select i1 %c, i8 7, i8 %x
; CHECK: %[[R:.*]] = zext i8 %[[R8]] to i32
; CHECK: ret i32 %[[R]]
