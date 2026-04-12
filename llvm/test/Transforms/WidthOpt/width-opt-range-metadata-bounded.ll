; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; isZeroBoundedAtWidth: !range metadata path (lines 2789-2804 of WidthOpt.cpp).
; A load with !range [0, 256) is proven zero-bounded at 8 bits because all
; range upper bounds are <= 2^8 = 256.
; tryShrinkICmpZeroBounded calls isZeroBoundedAtWidth(load, 8) → true (line 2804).
; However collectTruncRootedValueCost cannot handle loads, so the cost check
; fails and the comparison is unchanged.

define i1 @range_metadata_fits_icmp(ptr %p, i8 %y) {
  %v = load i32, ptr %p, !range !0
  %zy = zext i8 %y to i32
  %c = icmp ult i32 %zy, %v
  ret i1 %c
}
; CHECK-LABEL: define i1 @range_metadata_fits_icmp(
; CHECK:       %v = load i32, ptr %p
; CHECK:       %zy = zext i8 %y to i32
; CHECK:       %c = icmp ult i32 %zy, %v
; CHECK:       ret i1 %c

; !range [0, 512) has upper bound 512 > 256 = 2^8, so AllFit=false (line 2798-2800)
; and isZeroBoundedAtWidth returns false. No transformation.

define i1 @range_metadata_too_large_icmp(ptr %p, i8 %y) {
  %v = load i32, ptr %p, !range !1
  %zy = zext i8 %y to i32
  %c = icmp ult i32 %zy, %v
  ret i1 %c
}
; CHECK-LABEL: define i1 @range_metadata_too_large_icmp(
; CHECK:       %v = load i32, ptr %p
; CHECK:       %zy = zext i8 %y to i32
; CHECK:       %c = icmp ult i32 %zy, %v
; CHECK:       ret i1 %c

!0 = !{i32 0, i32 256}
!1 = !{i32 0, i32 512}
