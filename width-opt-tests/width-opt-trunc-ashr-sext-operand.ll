

define i8 @ashr_sext_i8_to_i32(i8 %x) {
  %wide = sext i8 %x to i32
  %shifted = ashr i32 %wide, 3
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}


define <4 x i8> @ashr_sext_i8_to_i32_vec(<4 x i8> %x) {
  %wide = sext <4 x i8> %x to <4 x i32>
  %shifted = ashr <4 x i32> %wide, splat (i32 3)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}



define i8 @ashr_sext_i8_to_i16(i8 %x) {
  %wide = sext i8 %x to i16
  %shifted = ashr i16 %wide, 5
  %narrow = trunc i16 %shifted to i8
  ret i8 %narrow
}


define <4 x i8> @ashr_sext_i8_to_i16_vec(<4 x i8> %x) {
  %wide = sext <4 x i8> %x to <4 x i16>
  %shifted = ashr <4 x i16> %wide, splat (i16 5)
  %narrow = trunc <4 x i16> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}



define i8 @ashr_sext_shift_by_one(i8 %x) {
  %wide = sext i8 %x to i32
  %shifted = ashr i32 %wide, 1
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}


define <4 x i8> @ashr_sext_shift_by_one_vec(<4 x i8> %x) {
  %wide = sext <4 x i8> %x to <4 x i32>
  %shifted = ashr <4 x i32> %wide, splat (i32 1)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}



define i8 @ashr_zext_i8_to_i32(i8 %x) {
  %wide = zext i8 %x to i32
  %shifted = ashr i32 %wide, 3
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}

