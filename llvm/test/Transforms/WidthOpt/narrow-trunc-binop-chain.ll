; RUN: opt -passes=width-opt -S %s | FileCheck %s

; Narrow a chain: trunc(binop*(zext i8, C)) -> binop*(i8, i8).
; The InstSimplifyFolder collapses identities (and x,-1 -> x, mul x,0 -> 0,
; etc.) so the wide zexts and trunc all disappear.

; CHECK-LABEL: @materialize_and_all_ones_rhs(
; CHECK-NEXT:    %{{.*}} = or i8 %a, %b
; CHECK-NEXT:    ret i8
define i8 @materialize_and_all_ones_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %inner = and i32 %za, 255
  %outer = or i32 %inner, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}

; CHECK-LABEL: @materialize_and_all_ones_lhs(
; CHECK-NEXT:    %{{.*}} = or i8 %a, %b
; CHECK-NEXT:    ret i8
define i8 @materialize_and_all_ones_lhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %inner = and i32 255, %za
  %outer = or i32 %inner, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}

; mul x,0 -> 0; or 0,b -> b: whole chain collapses to ret %b.
; CHECK-LABEL: @materialize_mul_zero_rhs(
; CHECK-NEXT:    ret i8 %b
define i8 @materialize_mul_zero_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %mul = mul i32 %za, 0
  %outer = or i32 %mul, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}

; CHECK-LABEL: @materialize_mul_one_rhs(
; CHECK-NEXT:    %{{.*}} = or i8 %a, %b
; CHECK-NEXT:    ret i8
define i8 @materialize_mul_one_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %mul = mul i32 %za, 1
  %outer = or i32 %mul, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}

; CHECK-LABEL: @materialize_add_zero_rhs(
; CHECK-NEXT:    %{{.*}} = or i8 %a, %b
; CHECK-NEXT:    ret i8
define i8 @materialize_add_zero_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %add = add i32 %za, 0
  %outer = or i32 %add, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}

; CHECK-LABEL: @materialize_sub_zero_rhs(
; CHECK-NEXT:    %{{.*}} = or i8 %a, %b
; CHECK-NEXT:    ret i8
define i8 @materialize_sub_zero_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %sub = sub i32 %za, 0
  %outer = or i32 %sub, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}

; CHECK-LABEL: @materialize_xor_zero_rhs(
; CHECK-NEXT:    %{{.*}} = or i8 %a, %b
; CHECK-NEXT:    ret i8
define i8 @materialize_xor_zero_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %xor = xor i32 %za, 0
  %outer = or i32 %xor, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}

; or x,-1 -> -1; and -1,b -> b: whole chain collapses to ret %b.
; CHECK-LABEL: @materialize_or_all_ones_rhs(
; CHECK-NEXT:    ret i8 %b
define i8 @materialize_or_all_ones_rhs(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %inner = or i32 %za, 255
  %outer = and i32 %inner, %zb
  %t = trunc i32 %outer to i8
  ret i8 %t
}
