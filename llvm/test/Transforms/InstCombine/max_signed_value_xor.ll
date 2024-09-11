; RUN: opt -O2 -S < %s | FileCheck %s

; This is a positive test case checking that the optimization of the form
; MaxSignedValue - (x ⊕ c) → x ⊕ (MaxSignedValue - c) is correctly applied for 8-bit integers

define i8 @opt8(i8 %x) {
  ; CHECK-LABEL: @opt8(
  ; CHECK-NEXT: %b = xor i8 %x, 77
  ; CHECK-NEXT: ret i8 %b
  %a = xor i8 %x, 50
  %b = sub i8 127, %a           ; MaxSignedValue for 8-bit is 127
  ret i8 %b
}


; This is a positive test case checking that the optimization of the form
; MaxSignedValue - (x ⊕ c) → x ⊕ (MaxSignedValue - c) is correctly applied for 16-bit integers

define i16 @opt16(i16 %x) {
  ; CHECK-LABEL: @opt16(
  ; CHECK-NEXT: %b = xor i16 %x, 29434
  ; CHECK-NEXT: ret i16 %b
  %a = xor i16 %x, 3333
  %b = sub i16 32767, %a        ; MaxSignedValue for 16-bit is 32767
  ret i16 %b
}


; This is a positive test case checking that the optimization of the form
; MaxSignedValue - (x ⊕ c) → x ⊕ (MaxSignedValue - c) is correctly applied for 32-bit integers

define i32 @opt32(i32 %x) {
  ; CHECK-LABEL: @opt32(
  ; CHECK-NEXT: %b = xor i32 %x, 2147433647
  ; CHECK-NEXT: ret i32 %b
  %a = xor i32 %x, 50000
  %b = sub i32 2147483647, %a    ; MaxSignedValue for 32-bit is 2147483647
  ret i32 %b
}


; This is a positive test case checking that the optimization of the form
; MaxSignedValue - (x ⊕ c) → x ⊕ (MaxSignedValue - c) is correctly applied for 64-bit integers

define i64 @opt64(i64 %x) {
  ; CHECK-LABEL: @opt64(
  ; CHECK-NEXT: %b = xor i64 %x, 9223372036849775807
  ; CHECK-NEXT: ret i64 %b
  %a = xor i64 %x, 5000000
  %b = sub i64 9223372036854775807, %a    ; MaxSignedValue for 64-bit is 9223372036854775807
  ret i64 %b
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