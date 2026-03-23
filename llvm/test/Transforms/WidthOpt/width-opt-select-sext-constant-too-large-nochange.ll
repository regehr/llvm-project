; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r = select i1 %c, i32 %sx, i32 128
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %sx = sext i8 %x to i32
; CHECK: %r = select i1 %c, i32 %sx, i32 128
; CHECK: ret i32 %r
