; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(lshr(sext(a:N→W), k), N) = ashr(a, k).
; At every bit position p of the result:
;   p < N-k : contains bit p+k of a (shifted bits, same for sext and ashr)
;   p >= N-k: contains the sign bit of a (sext fills above N; ashr sign-extends)
; So the wide lshr-of-sext truncated back to N bits equals ashr at width N.

define i8 @trunc_lshr_sext(i8 %a) {
  %a32 = sext i8 %a to i32
  %s = lshr i32 %a32, 3
  %t = trunc i32 %s to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_lshr_sext(
; CHECK-NOT: sext
; CHECK-NOT: lshr i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = ashr i8 %a, 3
; CHECK: ret i8 %[[R]]

; Shift by 1.
define i8 @trunc_lshr_sext_by_one(i8 %a) {
  %a32 = sext i8 %a to i32
  %s = lshr i32 %a32, 1
  %t = trunc i32 %s to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @trunc_lshr_sext_by_one(
; CHECK-NOT: sext
; CHECK-NOT: lshr i32
; CHECK: ashr i8 %a, 1
; CHECK: ret i8

; i16 source width.
define i16 @trunc_lshr_sext_i16(i16 %a) {
  %a32 = sext i16 %a to i32
  %s = lshr i32 %a32, 5
  %t = trunc i32 %s to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @trunc_lshr_sext_i16(
; CHECK-NOT: sext
; CHECK-NOT: lshr i32
; CHECK: ashr i16 %a, 5
; CHECK: ret i16
