; tryFoldAndOfSExtToZExt converts and(sext(X), mask) to and(zext(X), mask).
; The transformation is valid when mask covers only the low SrcWidth bits,
; because sext and zext agree on those bits.  However, this does NOT imply X
; is non-negative, so the new zext must NOT carry nneg.  Setting nneg
; unconditionally was a miscompile: a downstream pass could treat zext nneg as
; a sign-extension (values agree) and propagate incorrect range information to
; other instructions, producing wrong code.
;
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Basic case: and(sext(i8), 255).  X may be negative; no nneg allowed.
define i32 @and_sext_mask_no_nneg(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r  = and i32 %sx, 255
  ret i32 %r
}
; CHECK-LABEL: @and_sext_mask_no_nneg(
; CHECK: zext i8 %x to i32
; CHECK-NOT: zext nneg
; CHECK: and i32 {{.*}}, 255

define <4 x i32> @and_sext_mask_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> %sx, splat (i32 255)
  ret <4 x i32> %r
}
; CHECK-LABEL: @and_sext_mask_no_nneg_vec(
; CHECK: zext <4 x i8> %x to <4 x i32>
; CHECK-NOT: zext nneg
; CHECK: and <4 x i32> {{.*}}, splat (i32 255)

; Mask that does not cover the full source width: still no nneg.
define i32 @and_sext_partial_mask_no_nneg(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r  = and i32 %sx, 173
  ret i32 %r
}
; CHECK-LABEL: @and_sext_partial_mask_no_nneg(
; CHECK: zext i8 %x to i32
; CHECK-NOT: zext nneg

define <4 x i32> @and_sext_partial_mask_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> %sx, splat (i32 173)
  ret <4 x i32> %r
}
; CHECK-LABEL: @and_sext_partial_mask_no_nneg_vec(
; CHECK: zext <4 x i8> %x to <4 x i32>
; CHECK-NOT: zext nneg

; Reversed operand order: and(mask, sext(X)) — same rule.
define i32 @and_sext_reversed_no_nneg(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r  = and i32 255, %sx
  ret i32 %r
}
; CHECK-LABEL: @and_sext_reversed_no_nneg(
; CHECK: zext i8 %x to i32
; CHECK-NOT: zext nneg

define <4 x i32> @and_sext_reversed_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> splat (i32 255), %sx
  ret <4 x i32> %r
}
; CHECK-LABEL: @and_sext_reversed_no_nneg_vec(
; CHECK: zext <4 x i8> %x to <4 x i32>
; CHECK-NOT: zext nneg
