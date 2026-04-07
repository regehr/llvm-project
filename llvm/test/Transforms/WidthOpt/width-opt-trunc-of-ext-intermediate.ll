; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(ext(a:N→W), M) where N < M < W should re-extend a to M.
; This handles the intermediate-width case not covered by the basic
; tryFoldTruncOfExt (which previously only handled M <= N).

; trunc(zext(a:i8→i32), i16) = zext(a:i8→i16)
define i16 @trunc_zext_to_intermediate(i8 %a) {
  %a32 = zext i8 %a to i32
  %t = trunc i32 %a32 to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @trunc_zext_to_intermediate(
; CHECK-NOT: zext i8 {{.*}} to i32
; CHECK-NOT: trunc i32
; CHECK: %[[R:.*]] = zext i8 %a to i16
; CHECK: ret i16 %[[R]]

define <4 x i16> @trunc_zext_to_intermediate_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %t = trunc <4 x i32> %a32 to <4 x i16>
  ret <4 x i16> %t
}

; CHECK-LABEL: define <4 x i16> @trunc_zext_to_intermediate_vec(
; CHECK-NOT: zext <4 x i8> {{.*}} to <4 x i32>
; CHECK-NOT: trunc <4 x i32>
; CHECK: %[[R:.*]] = zext <4 x i8> %a to <4 x i16>
; CHECK: ret <4 x i16> %[[R]]

; trunc(sext(a:i8→i32), i16) = sext(a:i8→i16)
define i16 @trunc_sext_to_intermediate(i8 %a) {
  %a32 = sext i8 %a to i32
  %t = trunc i32 %a32 to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @trunc_sext_to_intermediate(
; CHECK-NOT: sext i8 {{.*}} to i32
; CHECK-NOT: trunc i32
; CHECK: %[[R:.*]] = sext i8 %a to i16
; CHECK: ret i16 %[[R]]

define <4 x i16> @trunc_sext_to_intermediate_vec(<4 x i8> %a) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %t = trunc <4 x i32> %a32 to <4 x i16>
  ret <4 x i16> %t
}

; CHECK-LABEL: define <4 x i16> @trunc_sext_to_intermediate_vec(
; CHECK-NOT: sext <4 x i8> {{.*}} to <4 x i32>
; CHECK-NOT: trunc <4 x i32>
; CHECK: %[[R:.*]] = sext <4 x i8> %a to <4 x i16>
; CHECK: ret <4 x i16> %[[R]]

; trunc(zext(a:i8→i64), i32) = zext(a:i8→i32)
define i32 @trunc_zext_i64_to_i32(i8 %a) {
  %a64 = zext i8 %a to i64
  %t = trunc i64 %a64 to i32
  ret i32 %t
}

; CHECK-LABEL: define i32 @trunc_zext_i64_to_i32(
; CHECK-NOT: zext i8 {{.*}} to i64
; CHECK-NOT: trunc i64
; CHECK: %[[R:.*]] = zext i8 %a to i32
; CHECK: ret i32 %[[R]]

define <4 x i32> @trunc_zext_i64_to_i32_vec(<4 x i8> %a) {
  %a64 = zext <4 x i8> %a to <4 x i64>
  %t = trunc <4 x i64> %a64 to <4 x i32>
  ret <4 x i32> %t
}

; CHECK-LABEL: define <4 x i32> @trunc_zext_i64_to_i32_vec(
; CHECK-NOT: zext <4 x i8> {{.*}} to <4 x i64>
; CHECK-NOT: trunc <4 x i64>
; CHECK: %[[R:.*]] = zext <4 x i8> %a to <4 x i32>
; CHECK: ret <4 x i32> %[[R]]
