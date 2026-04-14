; RUN: opt -passes='width-opt' -S %s | FileCheck %s
; trunc(umin(zext(a), zext(b))) → umin(a, b): exercises tryShrinkTruncOfMinMaxAbs

declare i32 @llvm.umin.i32(i32, i32)
declare i32 @llvm.umax.i32(i32, i32)

define i8 @umin_narrow(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %m = call i32 @llvm.umin.i32(i32 %za, i32 %zb)
  %tr = trunc i32 %m to i8
  ret i8 %tr
}
; CHECK-LABEL: @umin_narrow(
; CHECK-NOT: i32
; CHECK: call i8 @llvm.umin.i8(i8 %a, i8 %b)

define i8 @umax_narrow(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %m = call i32 @llvm.umax.i32(i32 %za, i32 %zb)
  %tr = trunc i32 %m to i8
  ret i8 %tr
}
; CHECK-LABEL: @umax_narrow(
; CHECK-NOT: i32
; CHECK: call i8 @llvm.umax.i8(i8 %a, i8 %b)
