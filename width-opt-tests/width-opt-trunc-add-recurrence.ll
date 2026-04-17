

define i8 @add_loop(i8 %init, i8 %step, i32 %n) {
entry:
  %init32 = zext i8 %init to i32
  %step32 = zext i8 %step to i32
  br label %loop
loop:
  %p = phi i32 [ %init32, %entry ], [ %next, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %next = add i32 %p, %step32
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, %n
  br i1 %done, label %exit, label %loop
exit:
  %t = trunc i32 %next to i8
  ret i8 %t
}



define i8 @add_loop_const_step(i8 %init, i32 %n) {
entry:
  %init32 = zext i8 %init to i32
  br label %loop
loop:
  %p = phi i32 [ %init32, %entry ], [ %next, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %next = add i32 %p, 3
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, %n
  br i1 %done, label %exit, label %loop
exit:
  %t = trunc i32 %next to i8
  ret i8 %t
}



define i8 @sub_loop(i8 %init, i8 %step, i32 %n) {
entry:
  %init32 = zext i8 %init to i32
  %step32 = zext i8 %step to i32
  br label %loop
loop:
  %p = phi i32 [ %init32, %entry ], [ %next, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %next = sub i32 %p, %step32
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, %n
  br i1 %done, label %exit, label %loop
exit:
  %t = trunc i32 %next to i8
  ret i8 %t
}



define i8 @xor_loop(i8 %init, i8 %key, i32 %n) {
entry:
  %init32 = zext i8 %init to i32
  %key32 = zext i8 %key to i32
  br label %loop
loop:
  %p = phi i32 [ %init32, %entry ], [ %next, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %next = xor i32 %p, %key32
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, %n
  br i1 %done, label %exit, label %loop
exit:
  %t = trunc i32 %next to i8
  ret i8 %t
}

