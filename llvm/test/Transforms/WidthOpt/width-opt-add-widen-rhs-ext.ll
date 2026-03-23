; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %x16 = zext i8 %x to i16
  %add16 = add i16 7, %x16
  %wide = zext i16 %add16 to i32
  ret i32 %wide
}

; CHECK-LABEL: define i32 @f(
; CHECK-NOT: add i16
; CHECK-NOT: zext i16
; CHECK: %[[X32:.*]] = zext i8 %x to i32
; CHECK: %[[ADD:.*]] = add i32 %[[X32]], 7
; CHECK: ret i32 %[[ADD]]
