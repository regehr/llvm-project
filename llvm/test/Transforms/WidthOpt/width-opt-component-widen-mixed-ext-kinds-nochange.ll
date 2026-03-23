; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c) {
entry:
  %s = select i1 %c, i8 -1, i8 2
  %x = sext i8 %s to i32
  %y = zext i8 %s to i32
  %r = add i32 %x, %y
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %s = select i1 %c, i8 -1, i8 2
; CHECK: %x = sext i8 %s to i32
; CHECK: %y = zext i8 %s to i32
; CHECK: %r = add i32 %x, %y
; CHECK-NOT: trunc i32
; CHECK: ret i32 %r
