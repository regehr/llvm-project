; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %x16 = zext i8 %x to i16
  %add16 = add i16 %x16, 65535
  %wide = zext i16 %add16 to i32
  ret i32 %wide
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[X16:.*]] = zext i8 %x to i16
; CHECK: %[[ADD:.*]] = add i16 %[[X16]], -1
; CHECK: %[[WIDE:.*]] = zext i16 %[[ADD]] to i32
; CHECK: ret i32 %[[WIDE]]
