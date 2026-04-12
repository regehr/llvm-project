; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; icmp ult (sext i8 %a to i32), (sext i8 %b to i32): unsigned comparison of two
; sext values. computeShrinkWidth returns 0 for sext/sext with unsigned predicate
; (line 217 of WidthOpt.cpp), so tryShrinkICmp cannot narrow this comparison.

define i1 @sext_sext_ult_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %c = icmp ult i32 %sa, %sb
  ret i1 %c
}
; CHECK-LABEL: define i1 @sext_sext_ult_nochange(
; CHECK:       %sa = sext i8 %a to i32
; CHECK:       %sb = sext i8 %b to i32
; CHECK:       %c = icmp ult i32 %sa, %sb
; CHECK:       ret i1 %c

; Same for uge.
define i1 @sext_sext_uge_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %c = icmp uge i32 %sa, %sb
  ret i1 %c
}
; CHECK-LABEL: define i1 @sext_sext_uge_nochange(
; CHECK:       %sa = sext i8 %a to i32
; CHECK:       %sb = sext i8 %b to i32
; CHECK:       %c = icmp uge i32 %sa, %sb
; CHECK:       ret i1 %c
