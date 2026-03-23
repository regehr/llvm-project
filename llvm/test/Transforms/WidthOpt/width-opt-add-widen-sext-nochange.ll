; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
entry:
  %sx = sext i8 %x to i16
  %add16 = add i16 %sx, 7
  %wide = zext i16 %add16 to i32
  ret i32 %wide
}

; CHECK-LABEL: define i32 @f(
; CHECK: %sx = sext i8 %x to i16
; CHECK: %add16 = add i16 %sx, 7
; CHECK: %wide = zext i16 %add16 to i32
; CHECK: ret i32 %wide
