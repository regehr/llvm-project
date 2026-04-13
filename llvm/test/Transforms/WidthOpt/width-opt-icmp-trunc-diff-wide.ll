; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryWidenTruncEqualityICmp: LHS from i32, RHS from i64 → WideWidth mismatch (line 1047).
; icmp eq (trunc i32 %x to i8), (trunc i64 %y to i8): getValueWidth(i32)≠getValueWidth(i64) → no change.

define i1 @trunc_eq_diff_wide(i32 %x, i64 %y) {
  %tx = trunc i32 %x to i8
  %ty = trunc i64 %y to i8
  %cmp = icmp eq i8 %tx, %ty
  ret i1 %cmp
}
; CHECK-LABEL: define i1 @trunc_eq_diff_wide(
; CHECK:       trunc i32
; CHECK:       trunc i64
; CHECK:       icmp eq i8
; CHECK:       ret i1
