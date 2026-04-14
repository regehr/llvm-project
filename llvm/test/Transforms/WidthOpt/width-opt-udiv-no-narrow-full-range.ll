; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryNarrowUDivWithRange line 1567: TargetWidth >= OrigWidth → return false.
; LHS = i32 -1 (constant with activeBits=32 = OrigWidth), RHS = zext i8 %y (RHSWidth=8).
; TargetWidth = max(32, 8) = 32 = OrigWidth → no narrowing.

define i32 @udiv_no_narrow_full_range(i8 %y) {
  %zy = zext i8 %y to i32
  %r = udiv i32 -1, %zy
  ret i32 %r
}
; CHECK-LABEL: define i32 @udiv_no_narrow_full_range(
; CHECK: udiv i32
; CHECK: ret i32
