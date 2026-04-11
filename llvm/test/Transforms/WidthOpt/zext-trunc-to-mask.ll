; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; InstCombine turns zext(trunc(x)) into a low-bit mask at the destination
; width, but WidthOpt now rejects this equal-cost rewrite.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i64 %x) {
entry:
  %t = trunc i64 %x to i16
  %e = zext i16 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @f(
; CHECK: %t = trunc i64 %x to i16
; CHECK: %e = zext i16 %t to i32
; CHECK: ret i32 %e

define <4 x i32> @f_vec(<4 x i64> %x) {
entry:
  %t = trunc <4 x i64> %x to <4 x i16>
  %e = zext <4 x i16> %t to <4 x i32>
  ret <4 x i32> %e
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %t = trunc <4 x i64> %x to <4 x i16>
; CHECK: %e = zext <4 x i16> %t to <4 x i32>
; CHECK: ret <4 x i32> %e
