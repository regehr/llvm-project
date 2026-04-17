


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
