; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; icmp ult (sext i8 %x to i32), C where C > 2^(N-1)-1 = 127.
; In tryShrinkICmpExtConst: the constant does not fit in NarrowWidth-1 bits
; (200 > 127), so the SExt+unsigned path is skipped. Then
; isCompatiblePredForExtKind(ult, SExt) is false, so the loop continues
; (line 917 of WidthOpt.cpp). No transformation results.

define i1 @sext_ult_large_const_nochange(i8 %x) {
  %s = sext i8 %x to i32
  %c = icmp ult i32 %s, 200
  ret i1 %c
}
; CHECK-LABEL: define i1 @sext_ult_large_const_nochange(
; CHECK:       %s = sext i8 %x to i32
; CHECK:       %c = icmp ult i32 %s, 200
; CHECK:       ret i1 %c

; Also for ugt with a constant exceeding the narrow positive range.
define i1 @sext_ugt_large_const_nochange(i8 %x) {
  %s = sext i8 %x to i32
  %c = icmp ugt i32 %s, 200
  ret i1 %c
}
; CHECK-LABEL: define i1 @sext_ugt_large_const_nochange(
; CHECK:       %s = sext i8 %x to i32
; CHECK:       %c = icmp ugt i32 %s, 200
; CHECK:       ret i1 %c
