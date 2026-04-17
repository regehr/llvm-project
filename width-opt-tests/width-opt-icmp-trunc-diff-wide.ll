

define i1 @trunc_eq_diff_wide(i32 %x, i64 %y) {
  %tx = trunc i32 %x to i8
  %ty = trunc i64 %y to i8
  %cmp = icmp eq i8 %tx, %ty
  ret i1 %cmp
}
