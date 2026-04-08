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

; smin/smax with an unextended i32 arg must NOT be narrowed: the i32 sign bit
; does not match the i8 sign bit after truncation.
; e.g. smin.i32(129, 0) = 0 but smin.i8(trunc(129), 0) = smin.i8(-127, 0) = -127.
define i8 @trunc_smin_unbounded_nochange(i32 %x) {
  %m = call i32 @llvm.smin.i32(i32 %x, i32 0)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_smin_unbounded_nochange(
; CHECK: call i32 @llvm.smin.i32(i32 %x, i32 0)
; CHECK: trunc i32
; CHECK: ret i8

define i8 @trunc_smax_unbounded_nochange(i32 %x) {
  %m = call i32 @llvm.smax.i32(i32 %x, i32 0)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_smax_unbounded_nochange(
; CHECK: call i32 @llvm.smax.i32(i32 %x, i32 0)
; CHECK: trunc i32
; CHECK: ret i8

; smin with a constant that fits in i8 signed range and a sext arg: valid to narrow.
define i8 @trunc_smin_sext_const(i8 %a) {
  %a32 = sext i8 %a to i32
  %m = call i32 @llvm.smin.i32(i32 %a32, i32 42)
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_smin_sext_const(
; CHECK-NOT: sext
; CHECK-NOT: smin i32
; CHECK-NOT: trunc
; CHECK: call i8 @llvm.smin.i8(i8 %a, i8 42)
; CHECK: ret i8

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

define <4 x i8> @trunc_umin_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %m = call <4 x i32> @llvm.umin.v4i32(<4 x i32> %a32, <4 x i32> %b32)
  %t = trunc <4 x i32> %m to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @trunc_umin_zext_vec(
; CHECK-NOT: zext
; CHECK-NOT: umin <4 x i32>
; CHECK-NOT: trunc
; CHECK: call <4 x i8> @llvm.umin.v4i8(<4 x i8> %a, <4 x i8> %b)
; CHECK: ret <4 x i8>

define <4 x i8> @trunc_smin_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %b32 = sext <4 x i8> %b to <4 x i32>
  %m = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %a32, <4 x i32> %b32)
  %t = trunc <4 x i32> %m to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @trunc_smin_sext_vec(
; CHECK-NOT: sext
; CHECK-NOT: smin <4 x i32>
; CHECK-NOT: trunc
; CHECK: call <4 x i8> @llvm.smin.v4i8(<4 x i8> %a, <4 x i8> %b)
; CHECK: ret <4 x i8>

define <4 x i8> @trunc_abs_sext_vec(<4 x i8> %a) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %abs = call <4 x i32> @llvm.abs.v4i32(<4 x i32> %a32, i1 false)
  %t = trunc <4 x i32> %abs to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @trunc_abs_sext_vec(
; CHECK-NOT: sext
; CHECK-NOT: abs <4 x i32>
; CHECK-NOT: trunc
; CHECK: call <4 x i8> @llvm.abs.v4i8(<4 x i8> %a, i1 false)
; CHECK: ret <4 x i8>

declare <4 x i32> @llvm.umin.v4i32(<4 x i32>, <4 x i32>)
declare <4 x i32> @llvm.smin.v4i32(<4 x i32>, <4 x i32>)
declare <4 x i32> @llvm.abs.v4i32(<4 x i32>, i1)
