; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(trunc(a:W→M), N) where N < M < W = trunc(a:W→N)

define i8 @trunc_i32_to_i16_to_i8(i32 %a) {
  %t16 = trunc i32 %a to i16
  %t8 = trunc i16 %t16 to i8
  ret i8 %t8
}

; CHECK-LABEL: define i8 @trunc_i32_to_i16_to_i8(
; CHECK-NOT: trunc i32 {{.*}} to i16
; CHECK: %[[R:.*]] = trunc i32 %a to i8
; CHECK: ret i8 %[[R]]

define <4 x i8> @trunc_i32_to_i16_to_i8_vec(<4 x i32> %a) {
  %t16 = trunc <4 x i32> %a to <4 x i16>
  %t8 = trunc <4 x i16> %t16 to <4 x i8>
  ret <4 x i8> %t8
}

; CHECK-LABEL: define <4 x i8> @trunc_i32_to_i16_to_i8_vec(
; CHECK-NOT: trunc <4 x i32> {{.*}} to <4 x i16>
; CHECK: %[[R:.*]] = trunc <4 x i32> %a to <4 x i8>
; CHECK: ret <4 x i8> %[[R]]

define i8 @trunc_i64_to_i32_to_i8(i64 %a) {
  %t32 = trunc i64 %a to i32
  %t8 = trunc i32 %t32 to i8
  ret i8 %t8
}

; CHECK-LABEL: define i8 @trunc_i64_to_i32_to_i8(
; CHECK-NOT: trunc i64 {{.*}} to i32
; CHECK: %[[R:.*]] = trunc i64 %a to i8
; CHECK: ret i8 %[[R]]

define <4 x i8> @trunc_i64_to_i32_to_i8_vec(<4 x i64> %a) {
  %t32 = trunc <4 x i64> %a to <4 x i32>
  %t8 = trunc <4 x i32> %t32 to <4 x i8>
  ret <4 x i8> %t8
}

; CHECK-LABEL: define <4 x i8> @trunc_i64_to_i32_to_i8_vec(
; CHECK-NOT: trunc <4 x i64> {{.*}} to <4 x i32>
; CHECK: %[[R:.*]] = trunc <4 x i64> %a to <4 x i8>
; CHECK: ret <4 x i8> %[[R]]
