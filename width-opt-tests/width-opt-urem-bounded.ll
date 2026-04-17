

define i8 @urem_both_zext(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %r = urem i32 %za, %zb
  %t = trunc i32 %r to i8
  ret i8 %t
}


define i8 @urem_bounded_by_divisor_lshr(i32 %x, i8 %b) {
  %zb = zext i8 %b to i32
  %r = urem i32 %x, %zb
  %sh = lshr i32 %r, 25
  %t = trunc i32 %sh to i8
  ret i8 %t
}
