; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare i8 @llvm.smax.i8(i8, i8)

define i16 @f(i8 %a, i8 %b) {
entry:
  %m = call i8 @llvm.smax.i8(i8 %a, i8 %b)
  %x = sext i8 %m to i16
  %y = sext i8 %m to i16
  %z = sext i8 %m to i16
  %r = add i16 %x, %y
  %s = add i16 %r, %z
  ret i16 %s
}

; CHECK-LABEL: define i16 @f(
; CHECK: entry:
; CHECK: %[[A16:.*]] = sext i8 %a to i16
; CHECK: %[[B16:.*]] = sext i8 %b to i16
; CHECK: %[[M:.*]] = call i16 @llvm.smax.i16(i16 %[[A16]], i16 %[[B16]])
; CHECK-NOT: call i8 @llvm.smax.i8
; CHECK-NOT: sext i8 %m to i16
; CHECK: %[[R:.*]] = add i16 %[[M]], %[[M]]
; CHECK: %[[S:.*]] = add i16 %[[R]], %[[M]]
; CHECK: ret i16 %[[S]]
