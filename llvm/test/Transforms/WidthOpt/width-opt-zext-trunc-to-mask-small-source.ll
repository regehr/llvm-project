; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %t = trunc i8 %x to i4
  %e = zext i4 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[M:.*]] = and i8 %x, 15
; CHECK: %[[E:.*]] = zext{{( nneg)?}} i8 %[[M]] to i32
; CHECK: ret i32 %[[E]]
