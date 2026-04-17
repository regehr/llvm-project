

define i32 @sext_of_sext(i8 %x) {
  %s1 = sext i8 %x to i16
  %s2 = sext i16 %s1 to i32
  ret i32 %s2
}


define <4 x i32> @sext_of_sext_vec(<4 x i8> %x) {
  %s1 = sext <4 x i8> %x to <4 x i16>
  %s2 = sext <4 x i16> %s1 to <4 x i32>
  ret <4 x i32> %s2
}


define i32 @sext_of_sext_nochange(i8 %x, ptr %p) {
  %s1 = sext i8 %x to i16
  store i16 %s1, ptr %p
  %s2 = sext i16 %s1 to i32
  ret i32 %s2
}

