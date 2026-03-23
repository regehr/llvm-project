; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(umin/umax(zext(a:i8→i32), zext(b:i8→i32)), i8) = umin/umax(a, b)
; trunc(smin/smax(sext(a:i8→i32), sext(b:i8→i32)), i8) = smin/smax(a, b)
; trunc(abs(sext(a:i8→i32), false), i8) = abs(a, false)

declare i32 @llvm.umin.i32(i32, i32)
declare i32 @llvm.umax.i32(i32, i32)
declare i32 @llvm.smin.i32(i32, i32)
declare i32 @llvm.smax.i32(i32, i32)
declare i32 @llvm.abs.i32(i32, i1)

define i8 @trunc_umin_zext(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %m = call i32 @llvm.umin.i32(i32 %a32, i32 %b32)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_umin_zext(
; CHECK-NOT: zext
; CHECK-NOT: umin i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = call i8 @llvm.umin.i8(i8 %a, i8 %b)
; CHECK: ret i8 %[[R]]

define i8 @trunc_umax_zext(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %m = call i32 @llvm.umax.i32(i32 %a32, i32 %b32)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_umax_zext(
; CHECK-NOT: zext
; CHECK-NOT: umax i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = call i8 @llvm.umax.i8(i8 %a, i8 %b)
; CHECK: ret i8 %[[R]]

define i8 @trunc_smin_sext(i8 %a, i8 %b) {
  %a32 = sext i8 %a to i32
  %b32 = sext i8 %b to i32
  %m = call i32 @llvm.smin.i32(i32 %a32, i32 %b32)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_smin_sext(
; CHECK-NOT: sext
; CHECK-NOT: smin i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = call i8 @llvm.smin.i8(i8 %a, i8 %b)
; CHECK: ret i8 %[[R]]

define i8 @trunc_smax_sext(i8 %a, i8 %b) {
  %a32 = sext i8 %a to i32
  %b32 = sext i8 %b to i32
  %m = call i32 @llvm.smax.i32(i32 %a32, i32 %b32)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_smax_sext(
; CHECK-NOT: sext
; CHECK-NOT: smax i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = call i8 @llvm.smax.i8(i8 %a, i8 %b)
; CHECK: ret i8 %[[R]]

define i8 @trunc_abs_sext(i8 %a) {
  %a32 = sext i8 %a to i32
  %abs = call i32 @llvm.abs.i32(i32 %a32, i1 false)
  %t = trunc i32 %abs to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_abs_sext(
; CHECK-NOT: sext
; CHECK-NOT: abs i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = call i8 @llvm.abs.i8(i8 %a, i1 false)
; CHECK: ret i8 %[[R]]

; umin with constant that fits in i8
define i8 @trunc_umin_zext_const(i8 %a) {
  %a32 = zext i8 %a to i32
  %m = call i32 @llvm.umin.i32(i32 %a32, i32 200)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_umin_zext_const(
; CHECK-NOT: zext
; CHECK-NOT: umin i32
; CHECK-NOT: trunc
; CHECK: call i8 @llvm.umin.i8(i8 %a, i8
; CHECK: ret i8
