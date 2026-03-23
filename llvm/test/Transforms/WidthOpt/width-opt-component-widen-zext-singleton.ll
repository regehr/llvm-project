; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %a) {
entry:
  %x = zext i8 %a to i16
  %y = zext i16 %x to i32
  %z = zext i16 %x to i32
  %r = add i32 %y, %z
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: entry:
; CHECK: %[[X:.*]] = zext i8 %a to i32
; CHECK-NOT: zext i8 %a to i16
; CHECK-NOT: zext i16
; CHECK: %[[R:.*]] = add i32 %[[X]], %[[X]]
; CHECK: ret i32 %[[R]]
