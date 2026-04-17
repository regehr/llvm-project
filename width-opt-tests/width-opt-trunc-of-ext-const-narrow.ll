

define i4 @trunc_sext_const_narrow() {
  %se = sext i8 5 to i32
  %t = trunc i32 %se to i4
  ret i4 %t
}


define i4 @trunc_zext_const_narrow() {
  %ze = zext i8 200 to i32
  %t = trunc i32 %ze to i4
  ret i4 %t
}
