

define i1 @sext_ult_large_const_nochange(i8 %x) {
  %s = sext i8 %x to i32
  %c = icmp ult i32 %s, 200
  ret i1 %c
}

define i1 @sext_ugt_large_const_nochange(i8 %x) {
  %s = sext i8 %x to i32
  %c = icmp ugt i32 %s, 200
  ret i1 %c
}
