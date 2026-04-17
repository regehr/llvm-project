


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

