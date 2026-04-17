
define i8 @or_and_hi_mask_zext(i32 %x, i8 %y) {
  %masked = and i32 %x, 65280
  %zy = zext i8 %y to i32
  %r = or i32 %masked, %zy
  %t = trunc i32 %r to i8
  ret i8 %t
}


define i8 @add_and_hi_mask_zext(i32 %x, i8 %y) {
  %masked = and i32 %x, 65280
  %zy = zext i8 %y to i32
  %r = add i32 %masked, %zy
  %t = trunc i32 %r to i8
  ret i8 %t
}

