; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Error paths in tryShrinkTruncOfShiftRecurrence.

; trunc(shl(phi[3-way], 3), i8): phi has 3 incoming values (line 4036 → false).

define i8 @trunc_shl_phi3_nochange(i1 %c, i32 %n) {
entry:
  br i1 %c, label %a, label %loop
a:
  br label %loop
loop:
  %phi = phi i32 [ 1, %entry ], [ 2, %a ], [ %shl, %loop ]
  %shl = shl i32 %phi, 3
  %t = trunc i32 %shl to i8
  br i1 %c, label %loop, label %exit
exit:
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_shl_phi3_nochange(
; CHECK:       shl i32
; CHECK:       trunc i32

; trunc(shl(phi[left,right], 3), i8): phi in join block, no backedge (line 4048 → false).

define i8 @trunc_shl_no_backedge_nochange(i1 %c, i32 %a, i32 %b) {
entry:
  br i1 %c, label %left, label %right
left:
  br label %join
right:
  br label %join
join:
  %phi = phi i32 [ %a, %left ], [ %b, %right ]
  %shl = shl i32 %phi, 3
  %t = trunc i32 %shl to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_shl_no_backedge_nochange(
; CHECK:       shl i32
; CHECK:       trunc i32

; trunc(shl(phi, 3), i8) where phi's back-edge value is add, not shl (line 4050 → false).

define i8 @trunc_shl_wrong_backedge_nochange(i1 %c) {
entry:
  br label %loop
loop:
  %phi = phi i32 [ 1, %entry ], [ %add, %loop ]
  %shl = shl i32 %phi, 3
  %add = add i32 %phi, 1
  %t = trunc i32 %shl to i8
  br i1 %c, label %loop, label %exit
exit:
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_shl_wrong_backedge_nochange(
; CHECK:       shl i32
; CHECK:       trunc i32

; trunc(shl(phi, 3), i8) where phi has 2 uses: shl and extra (line 4055 → false).

define i8 @trunc_shl_phi_multi_use_nochange(i1 %c) {
entry:
  br label %loop
loop:
  %phi = phi i32 [ 1, %entry ], [ %shl, %loop ]
  %shl = shl i32 %phi, 3
  %extra = add i32 %phi, 1
  %t = trunc i32 %shl to i8
  br i1 %c, label %loop, label %exit
exit:
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_shl_phi_multi_use_nochange(
; CHECK:       shl i32
; CHECK:       trunc i32

; trunc(shl(phi, 3), i8) where shl has 3 uses: phi, extra, and trunc (line 4059 → false).

define i8 @trunc_shl_shl_multi_use_nochange(i1 %c) {
entry:
  br label %loop
loop:
  %phi = phi i32 [ 1, %entry ], [ %shl, %loop ]
  %shl = shl i32 %phi, 3
  %extra = add i32 %shl, 1
  %t = trunc i32 %shl to i8
  br i1 %c, label %loop, label %exit
exit:
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_shl_shl_multi_use_nochange(
; CHECK:       shl i32
; CHECK:       trunc i32
