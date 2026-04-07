; When both operands of a signed icmp are zext from the same width, zext always
; produces a non-negative value so signed ordering == unsigned ordering at the
; narrow type.  E.g. icmp slt i32 (zext i8 a), (zext i8 b) ≡ icmp ult i8 a, b.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @slt_zext_zext(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %cmp = icmp slt i32 %za, %zb
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @slt_zext_zext(
; CHECK-NOT: zext
; CHECK: icmp ult i8 %a, %b

define <4 x i1> @slt_zext_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %za = zext <4 x i8> %a to <4 x i32>
  %zb = zext <4 x i8> %b to <4 x i32>
  %cmp = icmp slt <4 x i32> %za, %zb
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @slt_zext_zext_vec(
; CHECK-NOT: zext
; CHECK: icmp ult <4 x i8> %a, %b

define i1 @sle_zext_zext(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %cmp = icmp sle i32 %za, %zb
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sle_zext_zext(
; CHECK-NOT: zext
; CHECK: icmp ule i8 %a, %b

define <4 x i1> @sle_zext_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %za = zext <4 x i8> %a to <4 x i32>
  %zb = zext <4 x i8> %b to <4 x i32>
  %cmp = icmp sle <4 x i32> %za, %zb
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @sle_zext_zext_vec(
; CHECK-NOT: zext
; CHECK: icmp ule <4 x i8> %a, %b

define i1 @sgt_zext_zext(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %cmp = icmp sgt i32 %za, %zb
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sgt_zext_zext(
; CHECK-NOT: zext
; CHECK: icmp ugt i8 %a, %b

define <4 x i1> @sgt_zext_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %za = zext <4 x i8> %a to <4 x i32>
  %zb = zext <4 x i8> %b to <4 x i32>
  %cmp = icmp sgt <4 x i32> %za, %zb
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @sgt_zext_zext_vec(
; CHECK-NOT: zext
; CHECK: icmp ugt <4 x i8> %a, %b
