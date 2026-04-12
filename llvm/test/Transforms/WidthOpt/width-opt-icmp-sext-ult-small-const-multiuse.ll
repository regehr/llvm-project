; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; icmp ult (sext i8 %x to i32), 5 with the sext having multiple uses.
; tryShrinkICmpExtConst: Kind=SExt, ult, CV=5 < 128 (fits in NarrowWidth-1=7 bits),
; but sext has >1 use, so the loop continues (line 867 of WidthOpt.cpp).
; No transformation results.

define i1 @sext_ult_small_const_multiuse(i8 %x, ptr %p) {
  %s = sext i8 %x to i32
  store i32 %s, ptr %p
  %c = icmp ult i32 %s, 5
  ret i1 %c
}
; CHECK-LABEL: define i1 @sext_ult_small_const_multiuse(
; CHECK:       %s = sext i8 %x to i32
; CHECK:       store i32 %s, ptr %p
; CHECK:       %c = icmp ult i32 %s, 5
; CHECK:       ret i1 %c
