


define i8 @phi_trunc_shl_var_cost_fail(i8 %a, i32 %b, i32 %s, i1 %c) {
entry:
  br i1 %c, label %t, label %f
t:
  %shl = shl i32 %b, %s
  br label %exit
f:
  %za = zext i8 %a to i32
  br label %exit
exit:
  %phi = phi i32 [ %shl, %t ], [ %za, %f ]
  %tr = trunc i32 %phi to i8
  ret i8 %tr
}


declare i32 @may_throw()

define i8 @phi_of_two_calls_nochange(i1 %c) {
entry:
  br i1 %c, label %bb1, label %bb2
bb1:
  %r1 = call i32 @may_throw()
  br label %exit
bb2:
  %r2 = call i32 @may_throw()
  br label %exit
exit:
  %phi = phi i32 [ %r1, %bb1 ], [ %r2, %bb2 ]
  %tr = trunc i32 %phi to i8
  ret i8 %tr
}
