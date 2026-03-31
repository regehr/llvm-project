; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; mul nuw: the product of a W0-bit value and a W1-bit value fits in W0+W1 bits.
; Two i8 zero-extensions multiplied (nuw) fit in i16; the icmp can run at i16.

define i1 @icmp_ult_mul_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %prod = mul nuw i32 %a32, %b32
  %cmp = icmp ult i32 %prod, 1000
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ult_mul_nuw_zexts(
; CHECK-NOT: i32
; CHECK:      %{{.*}} = zext i8 %a to i16
; CHECK:      %{{.*}} = zext i8 %b to i16
; CHECK:      %{{.*}} = mul i16
; CHECK:      %{{.*}} = icmp ult i16
; CHECK:      ret i1

; eq comparison: also valid.
define i1 @icmp_eq_mul_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %prod = mul nuw i32 %a32, %b32
  %cmp = icmp eq i32 %prod, 100
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_eq_mul_nuw_zexts(
; CHECK-NOT: i32
; CHECK:      %{{.*}} = zext i8 %a to i16
; CHECK:      %{{.*}} = zext i8 %b to i16
; CHECK:      %{{.*}} = mul i16
; CHECK:      %{{.*}} = icmp eq i16 {{.*}}, 100
; CHECK:      ret i1
