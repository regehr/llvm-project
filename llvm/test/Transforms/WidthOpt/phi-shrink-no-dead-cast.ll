; tryShrinkPhiOfExts must not leave dead cast instructions behind when the
; profitability check blocks the phi shrink.
;
; The bug: the cost-accounting loop called materializeAtWidth to emit new casts
; before the AddedInstructions > RemovedInstructions check.  When the check
; fired (added cost exceeded removed cost), those casts had already been emitted
; but were never used, leaving dead instructions in the IR.
;
; The reproducer has a phi i64 with two incoming zexts of different source
; widths (i1 and i8).  Shrinking to i8 would need to rematerialize the i1
; incoming as "zext i1 → i8", but the wide zext i1 → i64 has multiple uses
; (so its producer cannot be removed), making the transform unprofitable.
; The dead "zext i1 → i8" must not appear in the output.
;
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i64 @no_dead_cast(i1 %flag, i8 %val, i64 %extra) {
entry:
  ; zext i1 → i64 has two uses (phi + add), so its producer cannot be removed.
  %w1 = zext i1 %flag to i64
  %w2 = zext i8 %val to i64
  %use = add i64 %w1, %extra
  br i1 %flag, label %a, label %b

a:
  br label %merge

b:
  br label %merge

merge:
  %p = phi i64 [ %w1, %a ], [ %w2, %b ]
  %r = add i64 %p, %use
  ret i64 %r
}

; CHECK-LABEL: define i64 @no_dead_cast(
; CHECK-NOT: zext i1 {{.*}} to i8
; CHECK: ret i64
