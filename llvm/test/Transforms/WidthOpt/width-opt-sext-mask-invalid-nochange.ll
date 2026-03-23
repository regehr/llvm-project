; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %ext = sext i8 %x to i32
  %and = and i32 %ext, 511
  ret i32 %and
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[EXT:.*]] = sext i8 %x to i32
; CHECK: %[[AND:.*]] = and i32 %[[EXT]], 511
; CHECK: ret i32 %[[AND]]
