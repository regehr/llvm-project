

define i8 @trunc_ashr_sext_wide_nochange(i16 %x) {
  %s = sext i16 %x to i32
  %sh = ashr i32 %s, 4
  %t = trunc i32 %sh to i8
  ret i8 %t
}


define i16 @trunc_ashr_zext_nochange(i8 %x) {
  %z = zext i8 %x to i32
  %sh = ashr i32 %z, 4
  %t = trunc i32 %sh to i16
  ret i16 %t
}


define i16 @trunc_ashr_sext_narrow(i8 %x) {
  %s = sext i8 %x to i32
  %sh = ashr i32 %s, 4
  %t = trunc i32 %sh to i16
  ret i16 %t
}
