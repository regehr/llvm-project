; RUN: opt -O2 -S %s | FileCheck %s

; CHECK-LABEL: @func(
; CHECK-Not: ret i32 %x
define i32 @func(i32 %x) {
  %a = sub i32 2147483632, %x
  %b = xor i32 %a, 2147483632 
  ret i32 %b
}
