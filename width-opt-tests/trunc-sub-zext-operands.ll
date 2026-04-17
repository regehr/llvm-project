
define i8 @sub_zext_operands(i8 %x, i8 %y) {
  %zx = zext i8 %x to i32
  %zy = zext i8 %y to i32
  %s = sub i32 %zx, %zy
  %t = trunc i32 %s to i8
  ret i8 %t
}


define i8 @sub_const_hi_fold(i8 %x) {
  %zx = zext i8 %x to i32
  %s = sub i32 %zx, 65280
  %t = trunc i32 %s to i8
  ret i8 %t
}

