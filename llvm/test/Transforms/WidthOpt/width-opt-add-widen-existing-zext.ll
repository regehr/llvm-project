; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %x16 = zext i8 %x to i16
  %x32 = zext i8 %x to i32
  %add16 = add i16 %x16, 7
  %wide = zext i16 %add16 to i32
  %sum = add i32 %wide, %x32
  ret i32 %sum
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[X32:.*]] = zext i8 %x to i32
; CHECK-NOT: add i16
; CHECK-NOT: zext i16
; CHECK: %[[WIDEADD:.*]] = add i32 %[[X32]], 7
; CHECK: %[[SUM:.*]] = add i32 %[[WIDEADD]], %[[X32]]
; CHECK: ret i32 %[[SUM]]
