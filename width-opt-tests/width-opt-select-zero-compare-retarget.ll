
define i1 @zext_sel_zero_lhs_slt(i1 %c, i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %s = select i1 %c, i32 %x32, i32 %y32
  %cmp = icmp slt i32 0, %s
  ret i1 %cmp
}


define i1 @zext_sel_zero_rhs_sle(i1 %c, i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %s = select i1 %c, i32 %x32, i32 %y32
  %cmp = icmp sle i32 %s, 0
  ret i1 %cmp
}


define i1 @sext_sel_zero_rhs_sle(i1 %c, i8 %x, i8 %y) {
entry:
  %x32 = sext i8 %x to i32
  %y32 = sext i8 %y to i32
  %s = select i1 %c, i32 %x32, i32 %y32
  %cmp = icmp sle i32 %s, 0
  ret i1 %cmp
}


define i1 @zext_sel_zero_rhs_ugt(i1 %c, i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %s = select i1 %c, i32 %x32, i32 %y32
  %cmp = icmp ugt i32 %s, 0
  ret i1 %cmp
}


define i1 @zext_sel_zero_eq(i1 %c, i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %s = select i1 %c, i32 %x32, i32 %y32
  %cmp = icmp eq i32 %s, 0
  ret i1 %cmp
}


define i1 @zext_sel_zero_ne(i1 %c, i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %s = select i1 %c, i32 %x32, i32 %y32
  %cmp = icmp ne i32 %s, 0
  ret i1 %cmp
}


define i1 @sext_sel_zero_ugt_no_retarget(i1 %c, i8 %x, i8 %y) {
entry:
  %x32 = sext i8 %x to i32
  %y32 = sext i8 %y to i32
  %s = select i1 %c, i32 %x32, i32 %y32
  %cmp = icmp ugt i32 %s, 0
  ret i1 %cmp
}


define i1 @zext_sel_zero_ult_no_retarget(i1 %c, i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %s = select i1 %c, i32 %x32, i32 %y32
  %cmp = icmp ult i32 %s, 0
  ret i1 %cmp
}

