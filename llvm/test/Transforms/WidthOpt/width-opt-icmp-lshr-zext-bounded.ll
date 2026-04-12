; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; icmp ult (lshr i32 (zext i8 %a), 25), (zext i8 %y):
; isZeroBoundedAtWidth hits the lshr shift-bounded path (line 2817 of
; WidthOpt.cpp): Shift=25 < SrcBits=32 and SrcBits-Shift=7 <= Width=8,
; so the lshr result is provably bounded at 8 bits without inspecting %a.
; tryShrinkICmpZeroBounded narrows to icmp ult i8 0, %y (the lshr folds to 0
; since any 8-bit value shifted right by 25 is 0).

define i1 @lshr_bounded_icmp(i8 %a, i8 %y) {
  %za = zext i8 %a to i32
  %sh = lshr i32 %za, 25
  %zy = zext i8 %y to i32
  %c = icmp ult i32 %sh, %zy
  ret i1 %c
}
; CHECK-LABEL: define i1 @lshr_bounded_icmp(
; CHECK-NOT:   lshr
; CHECK-NOT:   zext
; CHECK:       icmp ult i8 0, %y
; CHECK:       ret i1
