; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; select of two zexts compared ule 0: getNarrowPredicateForZeroCompare ICMP_ULE case (line 333).
; ule 0 is equivalent to eq 0 when the value is zero-extended (always >= 0).

define i1 @select_zext_ule_zero(i1 %cond, i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %sel = select i1 %cond, i32 %za, i32 %zb
  %c = icmp ule i32 %sel, 0
  ret i1 %c
}
; CHECK-LABEL: define i1 @select_zext_ule_zero(
; CHECK-NOT:   zext
; CHECK:       %sel.narrow = select i1 %cond, i8 %a, i8 %b
; CHECK:       icmp eq i8 %sel.narrow, 0
; CHECK:       ret i1

; select of two zexts compared sle 0 (ZExt+SLE → line 334 fallthrough → EQ).

define i1 @select_zext_sle_zero(i1 %cond, i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %sel = select i1 %cond, i32 %za, i32 %zb
  %c = icmp sle i32 %sel, 0
  ret i1 %c
}
; CHECK-LABEL: define i1 @select_zext_sle_zero(
; CHECK-NOT:   zext
; CHECK:       %sel.narrow = select i1 %cond, i8 %a, i8 %b
; CHECK:       icmp eq i8 %sel.narrow, 0
; CHECK:       ret i1
