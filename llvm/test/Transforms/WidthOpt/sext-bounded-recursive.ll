; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Recursive isSextBoundedAtWidth enables trunc(smin/smax/abs) narrowing when
; the intrinsic arguments are and/or/xor/ashr trees of sext instructions.

declare i32 @llvm.smin.i32(i32, i32)
declare i32 @llvm.smax.i32(i32, i32)

define i8 @smin_and_sext(i8 %a, i8 %b, i8 %c) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %ab = and i32 %sa, %sb
  %r = call i32 @llvm.smin.i32(i32 %ab, i32 %sc)
  %t = trunc i32 %r to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @smin_and_sext(
; CHECK:      %[[AB:.*]] = and i8 %a, %b
; CHECK:      %[[R:.*]] = call i8 @llvm.smin.i8(i8 %[[AB]], i8 %c)
; CHECK:      ret i8 %[[R]]

define i8 @smax_or_sext(i8 %a, i8 %b, i8 %c) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %ab = or i32 %sa, %sb
  %r = call i32 @llvm.smax.i32(i32 %ab, i32 %sc)
  %t = trunc i32 %r to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @smax_or_sext(
; CHECK:      %[[AB:.*]] = or i8 %a, %b
; CHECK:      %[[R:.*]] = call i8 @llvm.smax.i8(i8 %[[AB]], i8 %c)
; CHECK:      ret i8 %[[R]]

define i8 @smin_ashr_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sh = ashr i32 %sa, 3
  %sb = sext i8 %b to i32
  %r = call i32 @llvm.smin.i32(i32 %sh, i32 %sb)
  %t = trunc i32 %r to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @smin_ashr_sext(
; CHECK:      %[[SH:.*]] = ashr i8 %a, 3
; CHECK:      %[[R:.*]] = call i8 @llvm.smin.i8(i8 %[[SH]], i8 %b)
; CHECK:      ret i8 %[[R]]

; ---- vector variants ----

declare <4 x i32> @llvm.smin.v4i32(<4 x i32>, <4 x i32>)
declare <4 x i32> @llvm.smax.v4i32(<4 x i32>, <4 x i32>)

define <4 x i8> @smin_and_sext_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %sc = sext <4 x i8> %c to <4 x i32>
  %ab = and <4 x i32> %sa, %sb
  %r = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %ab, <4 x i32> %sc)
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @smin_and_sext_vec(
; CHECK:      %[[AB:.*]] = and <4 x i8> %a, %b
; CHECK:      %[[R:.*]] = call <4 x i8> @llvm.smin.v4i8(<4 x i8> %[[AB]], <4 x i8> %c)
; CHECK:      ret <4 x i8> %[[R]]

define <4 x i8> @smax_or_sext_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %sc = sext <4 x i8> %c to <4 x i32>
  %ab = or <4 x i32> %sa, %sb
  %r = call <4 x i32> @llvm.smax.v4i32(<4 x i32> %ab, <4 x i32> %sc)
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @smax_or_sext_vec(
; CHECK:      %[[AB:.*]] = or <4 x i8> %a, %b
; CHECK:      %[[R:.*]] = call <4 x i8> @llvm.smax.v4i8(<4 x i8> %[[AB]], <4 x i8> %c)
; CHECK:      ret <4 x i8> %[[R]]

define <4 x i8> @smin_ashr_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sh = ashr <4 x i32> %sa, splat (i32 3)
  %sb = sext <4 x i8> %b to <4 x i32>
  %r = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %sh, <4 x i32> %sb)
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @smin_ashr_sext_vec(
; CHECK:      %[[SH:.*]] = ashr <4 x i8> %a, splat (i8 3)
; CHECK:      %[[R:.*]] = call <4 x i8> @llvm.smin.v4i8(<4 x i8> %[[SH]], <4 x i8> %b)
; CHECK:      ret <4 x i8> %[[R]]
