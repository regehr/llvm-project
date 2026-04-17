

define i32 @udiv_no_narrow_full_range(i8 %y) {
  %zy = zext i8 %y to i32
  %r = udiv i32 -1, %zy
  ret i32 %r
}
