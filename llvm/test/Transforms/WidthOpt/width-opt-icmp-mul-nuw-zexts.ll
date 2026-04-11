; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; mul nuw: the product of a W0-bit value and a W1-bit value fits in W0+W1 bits.
; Even though the compare could be narrowed to i16, WidthOpt now insists on a
; strict local instruction-count decrease, so these equal-cost rewrites stay
; wide.

define i1 @icmp_ult_mul_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %prod = mul nuw i32 %a32, %b32
  %cmp = icmp ult i32 %prod, 1000
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ult_mul_nuw_zexts(
; CHECK:      %a32 = zext i8 %a to i32
; CHECK:      %b32 = zext i8 %b to i32
; CHECK:      %prod = mul nuw i32 %a32, %b32
; CHECK:      %cmp = icmp ult i32 %prod, 1000
; CHECK-NOT:  i16
; CHECK:      ret i1 %cmp

; eq comparison: also valid.
define i1 @icmp_eq_mul_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %prod = mul nuw i32 %a32, %b32
  %cmp = icmp eq i32 %prod, 100
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_eq_mul_nuw_zexts(
; CHECK:      %a32 = zext i8 %a to i32
; CHECK:      %b32 = zext i8 %b to i32
; CHECK:      %prod = mul nuw i32 %a32, %b32
; CHECK:      %cmp = icmp eq i32 %prod, 100
; CHECK-NOT:  i16
; CHECK:      ret i1 %cmp
