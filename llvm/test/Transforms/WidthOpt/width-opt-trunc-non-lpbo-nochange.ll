; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(sdiv(sext_a, sext_b), i8): sdiv is not a low-bit-preserving opcode
; and not shl/lshr/ashr → falls through to the final else at line 3286 → return false.
; No transformation.

define i8 @trunc_sdiv_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %div = sdiv i32 %sa, %sb
  %t = trunc i32 %div to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_sdiv_nochange(
; CHECK:       sdiv i32
; CHECK:       trunc i32

; trunc(srem(sext_a, sext_b), i8): same reasoning as sdiv.

define i8 @trunc_srem_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %rem = srem i32 %sa, %sb
  %t = trunc i32 %rem to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_srem_nochange(
; CHECK:       srem i32
; CHECK:       trunc i32
