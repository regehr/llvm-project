

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

