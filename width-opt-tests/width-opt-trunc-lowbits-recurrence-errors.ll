


define i8 @trunc_add_phi3_nochange(i1 %c) {
entry:
  br i1 %c, label %a, label %loop
a:
  br label %loop
loop:
  %phi = phi i32 [ 1, %entry ], [ 2, %a ], [ %add, %loop ]
  %add = add i32 %phi, 1
  %t = trunc i32 %add to i8
  br i1 %c, label %loop, label %exit
exit:
  ret i8 %t
}


define i8 @trunc_add_no_backedge_nochange(i1 %c, i32 %a, i32 %b) {
entry:
  br i1 %c, label %left, label %right
left:
  br label %join
right:
  br label %join
join:
  %phi = phi i32 [ %a, %left ], [ %b, %right ]
  %add = add i32 %phi, 1
  %t = trunc i32 %add to i8
  ret i8 %t
}


define i8 @trunc_add_wrong_backedge_nochange(i1 %c) {
entry:
  br label %loop
loop:
  %phi = phi i32 [ 1, %entry ], [ %xor, %loop ]
  %add = add i32 %phi, 1
  %xor = xor i32 %phi, 1
  %t = trunc i32 %add to i8
  br i1 %c, label %loop, label %exit
exit:
  ret i8 %t
}


define i8 @trunc_add_phi_multi_use_nochange(i1 %c) {
entry:
  br label %loop
loop:
  %phi = phi i32 [ 1, %entry ], [ %add, %loop ]
  %add = add i32 %phi, 1
  %extra = mul i32 %phi, 2
  %t = trunc i32 %add to i8
  br i1 %c, label %loop, label %exit
exit:
  ret i8 %t
}


define i8 @trunc_add_bo_multi_use_nochange(i1 %c) {
entry:
  br label %loop
loop:
  %phi = phi i32 [ 1, %entry ], [ %add, %loop ]
  %add = add i32 %phi, 1
  %extra = xor i32 %add, 1
  %t = trunc i32 %add to i8
  br i1 %c, label %loop, label %exit
exit:
  ret i8 %t
}
