; RUN: opt -instcombine -S < %s | FileCheck %s

; This is a positive test case checking that the optimization of the form
; MaxSignedValue - (x ⊕ c) → x ⊕ (MaxSignedValue - c) is correctly applied

define i16 @opt16(i16 %x) {
  ; CHECK-LABEL: @opt16(
  ; CHECK-NEXT: %b = xor i16 %x, 29434
  ; CHECK-NEXT: ret i16 %b
  %a = xor i16 %x, 3333
  %b = sub i16 32767, %a
  ret i16 %b
}

; This is a negative test case where the optimization should NOT be applied
; because 20000 is not the max signed value for i16 (32767).

define i16 @neg_opt16(i16 %x) {
  ; CHECK-LABEL: @neg_opt16(
  ; CHECK-NOT: %b = xor
  ; CHECK: %b = sub i16 20000, %a
  %a = xor i16 %x, 3333
  %b = sub i16 20000, %a
  ret i16 %b
}