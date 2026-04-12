; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; icmp slt (and (sext i8 %a), (sext i8 %b)), (sext i8 %c)
; where sext %a has a second use (a store), so tryShrinkSExtBitwiseBinop
; cannot fire (requires one-use sexts). Instead tryShrinkICmpSignBounded:
;   getStructuralSignedNarrowWidth(and(sext_a, sext_b)) = 8.
;   isSextBoundedAtWidth(and(sext_a, sext_b), 8) hits the and/or/xor path
;   (lines 2898-2899 of WidthOpt.cpp): both sexts are sext-bounded at 8.
; The comparison is narrowed to i8; the wide and is replaced by a narrow one.

define i1 @sext_and_sext_slt_multiuse(i8 %a, i8 %b, i8 %c, ptr %p) {
  %sa = sext i8 %a to i32
  store i32 %sa, ptr %p            ; extra use prevents tryShrinkSExtBitwiseBinop
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %and = and i32 %sa, %sb
  %cmp = icmp slt i32 %and, %sc
  ret i1 %cmp
}
; CHECK-LABEL: define i1 @sext_and_sext_slt_multiuse(
; CHECK:       %sa = sext i8 %a to i32
; CHECK:       store i32 %sa, ptr %p
; CHECK-NOT:   and i32
; CHECK:       and i8 %a, %b
; CHECK:       icmp slt i8
; CHECK:       ret i1

; Same with or.
define i1 @sext_or_sext_sgt_multiuse(i8 %a, i8 %b, i8 %c, ptr %p) {
  %sa = sext i8 %a to i32
  store i32 %sa, ptr %p
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %or = or i32 %sa, %sb
  %cmp = icmp sgt i32 %or, %sc
  ret i1 %cmp
}
; CHECK-LABEL: define i1 @sext_or_sext_sgt_multiuse(
; CHECK:       %sa = sext i8 %a to i32
; CHECK:       store i32 %sa, ptr %p
; CHECK-NOT:   or i32
; CHECK:       or i8 %a, %b
; CHECK:       icmp sgt i8
; CHECK:       ret i1
