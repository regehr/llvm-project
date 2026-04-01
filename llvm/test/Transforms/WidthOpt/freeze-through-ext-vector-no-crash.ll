; Freeze of a cast on a vector type should not crash. The tryPushFreezeThroughExt
; path only applies to scalar integers; vector casts should be left alone.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <4 x i16> @freeze_trunc_vec(<4 x i32> %x) {
  %t = trunc <4 x i32> %x to <4 x i16>
  %f = freeze <4 x i16> %t
  ret <4 x i16> %f
}

; CHECK-LABEL: define <4 x i16> @freeze_trunc_vec(
; CHECK: %t = trunc <4 x i32> %x to <4 x i16>
; CHECK: %f = freeze <4 x i16> %t
; CHECK: ret <4 x i16> %f

define <4 x i32> @freeze_zext_vec(<4 x i16> %x) {
  %e = zext <4 x i16> %x to <4 x i32>
  %f = freeze <4 x i32> %e
  ret <4 x i32> %f
}

; CHECK-LABEL: define <4 x i32> @freeze_zext_vec(
; CHECK: %e = zext <4 x i16> %x to <4 x i32>
; CHECK: %f = freeze <4 x i32> %e
; CHECK: ret <4 x i32> %f

define <4 x i32> @freeze_sext_vec(<4 x i16> %x) {
  %e = sext <4 x i16> %x to <4 x i32>
  %f = freeze <4 x i32> %e
  ret <4 x i32> %f
}

; CHECK-LABEL: define <4 x i32> @freeze_sext_vec(
; CHECK: %e = sext <4 x i16> %x to <4 x i32>
; CHECK: %f = freeze <4 x i32> %e
; CHECK: ret <4 x i32> %f
