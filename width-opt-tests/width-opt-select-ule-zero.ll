

define i1 @select_zext_ule_zero(i1 %cond, i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %sel = select i1 %cond, i32 %za, i32 %zb
  %c = icmp ule i32 %sel, 0
  ret i1 %c
}


define i1 @select_zext_sle_zero(i1 %cond, i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %sel = select i1 %cond, i32 %za, i32 %zb
  %c = icmp sle i32 %sel, 0
  ret i1 %c
}
