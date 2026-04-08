; Out-of-range constants can still make ext-vs-constant compares constant even
; when the constant does not round-trip through the source width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @ult_zext_large_const(i8 %x) {
entry:
  %ext = zext i8 %x to i32
  %cmp = icmp ult i32 %ext, 300
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @ult_zext_large_const(
; CHECK: ret i1 true

define i1 @eq_zext_negative_const(i8 %x) {
entry:
  %ext = zext i8 %x to i32
  %cmp = icmp eq i32 %ext, -1
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @eq_zext_negative_const(
; CHECK: ret i1 false

define i1 @sgt_sext_below_min(i8 %x) {
entry:
  %ext = sext i8 %x to i32
  %cmp = icmp sgt i32 %ext, -129
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sgt_sext_below_min(
; CHECK: ret i1 true

define <4 x i1> @ult_zext_large_const_vec(<4 x i8> %x) {
entry:
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp ult <4 x i32> %ext, <i32 300, i32 300, i32 300, i32 300>
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @ult_zext_large_const_vec(
; CHECK: ret <4 x i1> splat (i1 true)

define <4 x i1> @eq_zext_negative_const_vec(<4 x i8> %x) {
entry:
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp eq <4 x i32> %ext, <i32 -1, i32 -1, i32 -1, i32 -1>
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @eq_zext_negative_const_vec(
; CHECK: ret <4 x i1> zeroinitializer
