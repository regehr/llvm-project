


define i32 @add_widen_i1(i1 %a, i1 %b) {
  %add = add i1 %a, %b
  %wz = zext i1 %add to i32
  ret i32 %wz
}


define i32 @add_widen_non_const(i32 %x, i8 %b) {
  %tx = trunc i32 %x to i8
  %add = add i8 %tx, %b
  %wz = zext i8 %add to i32
  ret i32 %wz
}


define i32 @add_widen_wrong_src_width(i16 %x) {
  %tx = trunc i16 %x to i8
  %add = add i8 %tx, 5
  %wz = zext i8 %add to i32
  ret i32 %wz
}


define i32 @add_widen_c_too_large(i32 %x) {
  %tx = trunc i32 %x to i8
  %add = add i8 %tx, 128
  %wz = zext i8 %add to i32
  ret i32 %wz
}
