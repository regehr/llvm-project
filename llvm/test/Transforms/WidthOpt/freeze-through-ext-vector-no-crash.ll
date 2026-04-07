; Freeze-through-ext is lane-local for fixed-vector integer casts too.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <4 x i16> @freeze_trunc_vec(<4 x i32> %x) {
  %t = trunc <4 x i32> %x to <4 x i16>
  %f = freeze <4 x i16> %t
  ret <4 x i16> %f
}

; CHECK-LABEL: define <4 x i16> @freeze_trunc_vec(
; CHECK: %x.fr = freeze <4 x i32> %x
; CHECK-NEXT: %f = trunc <4 x i32> %x.fr to <4 x i16>
; CHECK-NEXT: ret <4 x i16> %f

define <4 x i32> @freeze_zext_vec(<4 x i16> %x) {
  %e = zext <4 x i16> %x to <4 x i32>
  %f = freeze <4 x i32> %e
  ret <4 x i32> %f
}

; CHECK-LABEL: define <4 x i32> @freeze_zext_vec(
; CHECK: %x.fr = freeze <4 x i16> %x
; CHECK-NEXT: %f = zext <4 x i16> %x.fr to <4 x i32>
; CHECK-NEXT: ret <4 x i32> %f

define <4 x i32> @freeze_sext_vec(<4 x i16> %x) {
  %e = sext <4 x i16> %x to <4 x i32>
  %f = freeze <4 x i32> %e
  ret <4 x i32> %f
}

; CHECK-LABEL: define <4 x i32> @freeze_sext_vec(
; CHECK: %x.fr = freeze <4 x i16> %x
; CHECK-NEXT: %f = sext <4 x i16> %x.fr to <4 x i32>
; CHECK-NEXT: ret <4 x i32> %f
