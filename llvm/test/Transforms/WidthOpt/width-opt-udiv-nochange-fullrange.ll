; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i32 %x, i32 %y) {
entry:
  %d = udiv i32 %x, %y
  ret i32 %d
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[D:.*]] = udiv i32 %x, %y
; CHECK: ret i32 %[[D]]
