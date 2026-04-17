
define i1 @single_use_profitable(i16 %x, i8 %y) {
  %sx = sext i16 %x to i32
  %zy = zext i8 %y to i32
  %c = icmp eq i32 %sx, %zy
  ret i1 %c
}


define i32 @multi_use_sext_no_narrow(i32 %x, i32 %y, ptr %p) {
  %sx = sext i32 %x to i64
  %zy = zext i32 %y to i64
  %c = icmp eq i64 %zy, %sx
  store i64 %sx, ptr %p
  %r = select i1 %c, i32 1, i32 0
  ret i32 %r
}

