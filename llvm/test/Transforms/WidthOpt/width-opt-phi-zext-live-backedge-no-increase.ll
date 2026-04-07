; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Do not shrink a wide phi when the backedge zext remains live for another use.
; In this loop, replacing %idx.wide with a narrow phi would still need a new
; zext for the GEP while %next.wide stays alive for the loop-exit compare,
; increasing instruction count by one.

define void @no_duplicate_zext(ptr %base, i64 %limit) {
entry:
  br label %loop

loop:
  %idx.wide = phi i64 [ %next.wide, %loop ], [ 0, %entry ]
  %idx = phi i32 [ %next, %loop ], [ 0, %entry ]
  %elt = getelementptr inbounds i8, ptr %base, i64 %idx.wide
  store i8 0, ptr %elt, align 1
  %next = add nuw nsw i32 %idx, 1
  %next.wide = zext i32 %next to i64
  %done = icmp ugt i64 %limit, %next.wide
  br i1 %done, label %exit, label %loop

exit:
  ret void
}

; CHECK-LABEL: define void @no_duplicate_zext(
; CHECK: loop:
; CHECK: %idx.wide = phi i64 [ %next.wide, %loop ], [ 0, %entry ]
; CHECK: %idx = phi i32 [ %next, %loop ], [ 0, %entry ]
; CHECK: getelementptr inbounds i8, ptr %base, i64 %idx.wide
; CHECK: %next.wide = zext i32 %next to i64
; CHECK: %done = icmp ugt i64 %limit, %next.wide
; CHECK-NOT: .narrow
