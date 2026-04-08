; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Transitive extension chains: trunc(op(sext(sext(a:N→M)→W), ...), N)
; The inner sext chain is peeled to find the original narrow root.

; trunc(add(sext(sext(a:i8→i16)→i32), sext(sext(b:i8→i16)→i32)), i8) = add(a, b)
define i8 @chain_sext_i8_i16_i32_trunc_i8(i8 %a, i8 %b) {
  %a16 = sext i8 %a to i16
  %b16 = sext i8 %b to i16
  %a32 = sext i16 %a16 to i32
  %b32 = sext i16 %b16 to i32
  %r = add i32 %a32, %b32
  %t = trunc i32 %r to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @chain_sext_i8_i16_i32_trunc_i8(
; CHECK-NOT: sext
; CHECK-NOT: add i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = add i8 %a, %b
; CHECK: ret i8 %[[R]]

; trunc(add(zext(zext(a:i8→i16)→i32), zext(zext(b:i8→i16)→i32)), i8) = add(a, b)
define i8 @chain_zext_i8_i16_i32_trunc_i8(i8 %a, i8 %b) {
  %a16 = zext i8 %a to i16
  %b16 = zext i8 %b to i16
  %a32 = zext i16 %a16 to i32
  %b32 = zext i16 %b16 to i32
  %r = add i32 %a32, %b32
  %t = trunc i32 %r to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @chain_zext_i8_i16_i32_trunc_i8(
; CHECK-NOT: zext
; CHECK-NOT: add i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = add i8 %a, %b
; CHECK: ret i8 %[[R]]

define <4 x i8> @chain_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %a16 = sext <4 x i8> %a to <4 x i16>
  %b16 = sext <4 x i8> %b to <4 x i16>
  %a32 = sext <4 x i16> %a16 to <4 x i32>
  %b32 = sext <4 x i16> %b16 to <4 x i32>
  %r = add <4 x i32> %a32, %b32
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @chain_sext_vec(
; CHECK-NOT: sext
; CHECK-NOT: add <4 x i32>
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = add <4 x i8> %a, %b
; CHECK: ret <4 x i8> %[[R]]

define <4 x i8> @chain_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %a16 = zext <4 x i8> %a to <4 x i16>
  %b16 = zext <4 x i8> %b to <4 x i16>
  %a32 = zext <4 x i16> %a16 to <4 x i32>
  %b32 = zext <4 x i16> %b16 to <4 x i32>
  %r = add <4 x i32> %a32, %b32
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @chain_zext_vec(
; CHECK-NOT: zext
; CHECK-NOT: add <4 x i32>
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = add <4 x i8> %a, %b
; CHECK: ret <4 x i8> %[[R]]
