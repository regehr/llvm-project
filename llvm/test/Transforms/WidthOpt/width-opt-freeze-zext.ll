; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
entry:
  %zx = zext i8 %x to i32
  %fr = freeze i32 %zx
  ret i32 %fr
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[FR:.*]] = freeze i8 %x
; CHECK: %[[ZX:.*]] = zext i8 %[[FR]] to i32
; CHECK: ret i32 %[[ZX]]
