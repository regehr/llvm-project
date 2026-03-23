; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i32 %a) {
entry:
  %x = trunc i32 %a to i16
  %y = sext i16 %x to i32
  %z = sext i16 %x to i32
  %r = add i32 %y, %z
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: entry:
; CHECK: %[[N:.*]] = trunc i32 %a to i16
; CHECK: %[[W:.*]] = sext i16 %[[N]] to i32
; CHECK-NOT: sext i16 %x to i32
; CHECK: %[[R:.*]] = add i32 %[[W]], %[[W]]
; CHECK: ret i32 %[[R]]
