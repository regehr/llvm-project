; RUN: opt -passes='width-opt,verify' -S %s | FileCheck %s

; Regression test: tryShrinkTruncOfShiftRecurrence must NOT narrow a shift
; recurrence when the shift amount >= target width.
;
; If we narrow  trunc(shl.i32(phi, 8), i8)  to  shl.i8(narrow_phi, 8),
; the result is poison (shift amount == bit width), but the original is always 0
; because shl left by 8 zeroes all 8 low bits before the trunc.
;
; alive-tv counterexample: %x=3
;   Source: trunc(shl.i32(3, 8)) = trunc(768) = 0x00 = 0
;   Target: shl.i8(trunc.i8(3), 8) = shl i8 3, 8 = poison
;
; The guard is: if AmtC >= TargetWidth, return false (don't narrow).

; --- Negative case: shift amount == target width → must NOT narrow ---
; Shift by 8 into an i8 trunc: shl.i8(x, 8) is poison, original is always 0.

define i8 @shl_recurrence_shift_eq_width(i32 %init, i32 %n) {
entry:
  br label %loop
loop:
  %phi = phi i32 [ %init, %entry ], [ %shl, %loop ]
  %ctr = phi i32 [ 0, %entry ], [ %ctr.next, %loop ]
  %shl = shl i32 %phi, 8
  %tr = trunc i32 %shl to i8
  %ctr.next = add i32 %ctr, 1
  %done = icmp eq i32 %ctr.next, %n
  br i1 %done, label %exit, label %loop
exit:
  ret i8 %tr
}

; CHECK-LABEL: define i8 @shl_recurrence_shift_eq_width(
; The wide shl and trunc must survive; no narrow shl i8 ..., 8 must appear.
; CHECK: shl i32
; CHECK: trunc i32
; CHECK-NOT: shl i8 {{.*}}, 8
; CHECK: ret i8

; --- Negative case: shift amount < target width but only breaks even ---
; Narrowing this recurrence would just move the trunc from the exit to the
; entry, with no net instruction reduction.

define i8 @shl_recurrence_shift_lt_width(i32 %init, i32 %n) {
entry:
  br label %loop
loop:
  %phi = phi i32 [ %init, %entry ], [ %shl, %loop ]
  %ctr = phi i32 [ 0, %entry ], [ %ctr.next, %loop ]
  %shl = shl i32 %phi, 3
  %tr = trunc i32 %shl to i8
  %ctr.next = add i32 %ctr, 1
  %done = icmp eq i32 %ctr.next, %n
  br i1 %done, label %exit, label %loop
exit:
  ret i8 %tr
}

; CHECK-LABEL: define i8 @shl_recurrence_shift_lt_width(
; The transform should not fire because it only breaks even on instruction
; count.
; CHECK: shl i32
; CHECK: trunc i32
; CHECK-NOT: shl i8 {{.*}}, 3
; CHECK: ret i8

; --- Positive case: shift amount < target width with a free narrow init ---
; Here the preheader already computes the init as a zext of i8, so shrinking the
; recurrence removes the wide phi/shl/trunc without introducing a replacement
; trunc.

define i8 @shl_recurrence_shift_lt_width_profitable(i8 %init8, i32 %n) {
entry:
  %init = zext i8 %init8 to i32
  br label %loop
loop:
  %phi = phi i32 [ %init, %entry ], [ %shl, %loop ]
  %ctr = phi i32 [ 0, %entry ], [ %ctr.next, %loop ]
  %shl = shl i32 %phi, 3
  %tr = trunc i32 %shl to i8
  %ctr.next = add i32 %ctr, 1
  %done = icmp eq i32 %ctr.next, %n
  br i1 %done, label %exit, label %loop
exit:
  ret i8 %tr
}

; CHECK-LABEL: define i8 @shl_recurrence_shift_lt_width_profitable(
; CHECK: phi i8
; CHECK: shl i8 {{.*}}, 3
; CHECK: ret i8
