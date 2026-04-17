

define i1 @sext_ult_small_const_multiuse(i8 %x, ptr %p) {
  %s = sext i8 %x to i32
  store i32 %s, ptr %p
  %c = icmp ult i32 %s, 5
  ret i1 %c
}
