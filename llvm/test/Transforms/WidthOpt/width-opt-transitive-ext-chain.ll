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
