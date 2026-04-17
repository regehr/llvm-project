
define i32 @widen_add_trunc_zext(i32 %n) {
  %x = and i32 %n, 63
  %tr = trunc i32 %x to i8
  %add = add i8 %tr, 5
  %ze = zext i8 %add to i32
  ret i32 %ze
}


define i32 @widen_add_trunc_zext_commuted(i32 %n) {
  %x = and i32 %n, 63
  %tr = trunc i32 %x to i8
  %add = add i8 5, %tr
  %ze = zext i8 %add to i32
  ret i32 %ze
}


define i32 @widen_add_trunc_zext_nochange_unbounded(i32 %n) {
  %tr = trunc i32 %n to i8
  %add = add i8 %tr, 5
  %ze = zext i8 %add to i32
  ret i32 %ze
}


define i32 @widen_add_trunc_zext_nochange_multiuse(i32 %n, ptr %p) {
  %x = and i32 %n, 63
  %tr = trunc i32 %x to i8
  %add = add i8 %tr, 5
  store i8 %add, ptr %p
  %ze = zext i8 %add to i32
  ret i32 %ze
}

