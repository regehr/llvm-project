


define i1 @zext_ule_non_max_const(i8 %x) {
  %z = zext i8 %x to i32
  %c = icmp ule i32 %z, 100
  ret i1 %c
}


define i1 @sext_sge_non_min_const(i8 %x) {
  %s = sext i8 %x to i32
  %c = icmp sge i32 %s, -100
  ret i1 %c
}


define i1 @sext_sle_non_max_const(i8 %x) {
  %s = sext i8 %x to i32
  %c = icmp sle i32 %s, 100
  ret i1 %c
}


define i1 @zext_ugt_out_of_range(i8 %x) {
  %z = zext i8 %x to i32
  %c = icmp ugt i32 %z, 300
  ret i1 %c
}
