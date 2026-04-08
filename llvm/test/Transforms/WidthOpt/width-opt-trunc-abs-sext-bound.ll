; RUN: opt -passes='width-opt,verify' -S %s | FileCheck %s

; Regression test: trunc(abs(x)) must NOT be narrowed to abs(trunc(x)) unless
; x is sext-bounded at the target width.
;
; abs is not low-bits-preserving in general.  If x overflows the target width,
; the sign of trunc(x) can differ from the sign of x, so
;   abs(trunc(x, N))  !=  trunc(abs(x), N)
;
; Example with N=8, x = sext(34) + 100 = 134:
;   trunc(abs.i32(134), i8) = trunc(134) = -122
;   abs.i8(trunc.i8(134)) = abs.i8(-122) = 122   <- wrong
;
; The corresponding smin/smax cases already require isSextBoundedAtWidth;
; abs must apply the same guard.

; --- Negative case: argument NOT sext-bounded at i8 -------------------------
; add(sext(a), 100) can exceed 127 when a > 27, so it is not sext-bounded.
; width-opt must leave the trunc+abs intact.

define i8 @abs_add_not_sext_bounded(i8 %a) {
  %wa = sext i8 %a to i32
  %add = add i32 %wa, 100
  %abs = call i32 @llvm.abs.i32(i32 %add, i1 false)
  %tr = trunc i32 %abs to i8
  ret i8 %tr
}

; CHECK-LABEL: define i8 @abs_add_not_sext_bounded(
; The sext, add, abs and trunc must all survive unchanged.
; CHECK: sext i8 %a to i32
; CHECK: add i32
; CHECK: @llvm.abs.i32
; CHECK: trunc i32
; CHECK: ret i8

; --- Positive case: argument IS sext-bounded at i8 --------------------------
; sext(a) from i8 is always sext-bounded at 8 bits, so the narrowing is sound:
;   abs(sext(a)) at i8 = abs(a) directly.

define i8 @abs_sext_bounded(i8 %a) {
  %wa = sext i8 %a to i32
  %abs = call i32 @llvm.abs.i32(i32 %wa, i1 false)
  %tr = trunc i32 %abs to i8
  ret i8 %tr
}

; CHECK-LABEL: define i8 @abs_sext_bounded(
; The sext, wide abs, and trunc should all be eliminated.
; CHECK-NOT: sext
; CHECK-NOT: trunc
; CHECK: @llvm.abs.i8(i8 %a
; CHECK: ret i8

declare i32 @llvm.abs.i32(i32, i1 immarg)

define <4 x i8> @abs_add_not_sext_bounded_vec(<4 x i8> %a) {
  %wa = sext <4 x i8> %a to <4 x i32>
  %add = add <4 x i32> %wa, splat (i32 100)
  %abs = call <4 x i32> @llvm.abs.v4i32(<4 x i32> %add, i1 false)
  %tr = trunc <4 x i32> %abs to <4 x i8>
  ret <4 x i8> %tr
}

; CHECK-LABEL: define <4 x i8> @abs_add_not_sext_bounded_vec(
; CHECK: sext <4 x i8> %a to <4 x i32>
; CHECK: add <4 x i32>
; CHECK: @llvm.abs.v4i32
; CHECK: trunc <4 x i32>
; CHECK: ret <4 x i8>

define <4 x i8> @abs_sext_bounded_vec(<4 x i8> %a) {
  %wa = sext <4 x i8> %a to <4 x i32>
  %abs = call <4 x i32> @llvm.abs.v4i32(<4 x i32> %wa, i1 false)
  %tr = trunc <4 x i32> %abs to <4 x i8>
  ret <4 x i8> %tr
}

; CHECK-LABEL: define <4 x i8> @abs_sext_bounded_vec(
; CHECK-NOT: sext
; CHECK-NOT: trunc
; CHECK: @llvm.abs.v4i8(<4 x i8> %a
; CHECK: ret <4 x i8>

declare <4 x i32> @llvm.abs.v4i32(<4 x i32>, i1)
