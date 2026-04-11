; tryFoldAndOfSExtToZExt is disabled under the strict local-profitability
; policy because it is only a break-even canonicalization.
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
; CHECK: sext i8 %x to i32
; CHECK-NOT: zext
; CHECK: and i32 {{.*}}, 255

define <4 x i32> @and_sext_mask_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> %sx, splat (i32 255)
  ret <4 x i32> %r
}
; CHECK-LABEL: @and_sext_mask_no_nneg_vec(
; CHECK: sext <4 x i8> %x to <4 x i32>
; CHECK-NOT: zext
; CHECK: and <4 x i32> {{.*}}, splat (i32 255)

; Mask that does not cover the full source width: still no nneg.
define i32 @and_sext_partial_mask_no_nneg(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r  = and i32 %sx, 173
  ret i32 %r
}
; CHECK-LABEL: @and_sext_partial_mask_no_nneg(
; CHECK: sext i8 %x to i32
; CHECK-NOT: zext

define <4 x i32> @and_sext_partial_mask_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> %sx, splat (i32 173)
  ret <4 x i32> %r
}
; CHECK-LABEL: @and_sext_partial_mask_no_nneg_vec(
; CHECK: sext <4 x i8> %x to <4 x i32>
; CHECK-NOT: zext

; Reversed operand order: and(mask, sext(X)) — same rule.
define i32 @and_sext_reversed_no_nneg(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r  = and i32 255, %sx
  ret i32 %r
}
; CHECK-LABEL: @and_sext_reversed_no_nneg(
; CHECK: sext i8 %x to i32
; CHECK-NOT: zext

define <4 x i32> @and_sext_reversed_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> splat (i32 255), %sx
  ret <4 x i32> %r
}
; CHECK-LABEL: @and_sext_reversed_no_nneg_vec(
; CHECK: sext <4 x i8> %x to <4 x i32>
; CHECK-NOT: zext
