; Freeze-through-ext is lane-local for fixed-vector integer casts too, but the
; rewrite is only break-even locally and is now disabled.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <4 x i16> @freeze_trunc_vec(<4 x i32> %x) {
  %t = trunc <4 x i32> %x to <4 x i16>
  %f = freeze <4 x i16> %t
  ret <4 x i16> %f
}

; CHECK-LABEL: define <4 x i16> @freeze_trunc_vec(
; CHECK: %t = trunc <4 x i32> %x to <4 x i16>
; CHECK-NEXT: %f = freeze <4 x i16> %t
; CHECK-NEXT: ret <4 x i16> %f

define <4 x i32> @freeze_zext_vec(<4 x i16> %x) {
  %e = zext <4 x i16> %x to <4 x i32>
  %f = freeze <4 x i32> %e
  ret <4 x i32> %f
}

; CHECK-LABEL: define <4 x i32> @freeze_zext_vec(
; CHECK: %e = zext <4 x i16> %x to <4 x i32>
; CHECK-NEXT: %f = freeze <4 x i32> %e
; CHECK-NEXT: ret <4 x i32> %f

define <4 x i32> @freeze_sext_vec(<4 x i16> %x) {
  %e = sext <4 x i16> %x to <4 x i32>
  %f = freeze <4 x i32> %e
  ret <4 x i32> %f
}

; CHECK-LABEL: define <4 x i32> @freeze_sext_vec(
; CHECK: %e = sext <4 x i16> %x to <4 x i32>
; CHECK-NEXT: %f = freeze <4 x i32> %e
; CHECK-NEXT: ret <4 x i32> %f
