; Rebuild a udiv at the narrow operand width when doing so removes existing
; zero-extensions and introduces fewer width changes than it removes.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %d = udiv i32 %x32, %y32
  ret i32 %d
}

; CHECK-LABEL: define i32 @f(
; CHECK-NOT: zext i8 %x to i32
; CHECK-NOT: zext i8 %y to i32
; CHECK: %[[D8:.*]] = udiv i8 %x, %y
; CHECK: %[[D32:.*]] = zext i8 %[[D8]] to i32
; CHECK-NOT: udiv i32
; CHECK: ret i32 %[[D32]]
