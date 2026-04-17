

define i1 @lshr_bounded_icmp(i8 %a, i8 %y) {
  %za = zext i8 %a to i32
  %sh = lshr i32 %za, 25
  %zy = zext i8 %y to i32
  %c = icmp ult i32 %sh, %zy
  ret i1 %c
}
