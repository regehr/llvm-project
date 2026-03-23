; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i1 %c, i32 %x, i32 %y) {
entry:
  %m = mul i32 %x, %y
  %sel = select i1 %c, i32 %m, i32 7
  %t = trunc i32 %sel to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @f(
; CHECK: %m = mul i32 %x, %y
; CHECK: %sel = select i1 %c, i32 %m, i32 7
; CHECK: %t = trunc i32 %sel to i16
; CHECK: ret i16 %t
