; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i4 @f(i8 %n) {
entry:
  %wide = sext i8 %n to i64
  %tr = trunc i64 %wide to i4
  ret i4 %tr
}

; CHECK-LABEL: define i4 @f(
; CHECK: trunc i8 %n to i4
; CHECK-NOT: sext i8 %n to i64
; CHECK-NOT: trunc i64
; CHECK: ret i4
