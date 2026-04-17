

define i8 @lshr_zext_i8_to_i32(i8 %x) {
  %wide = zext i8 %x to i32
  %shifted = lshr i32 %wide, 3
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}


define <4 x i8> @lshr_zext_i8_to_i32_vec(<4 x i8> %x) {
  %wide = zext <4 x i8> %x to <4 x i32>
  %shifted = lshr <4 x i32> %wide, splat (i32 3)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}



define i8 @lshr_zext_i8_to_i16(i8 %x) {
  %wide = zext i8 %x to i16
  %shifted = lshr i16 %wide, 5
  %narrow = trunc i16 %shifted to i8
  ret i8 %narrow
}


define <4 x i8> @lshr_zext_i8_to_i16_vec(<4 x i8> %x) {
  %wide = zext <4 x i8> %x to <4 x i16>
  %shifted = lshr <4 x i16> %wide, splat (i16 5)
  %narrow = trunc <4 x i16> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}



define i8 @lshr_zext_shift_by_one(i8 %x) {
  %wide = zext i8 %x to i32
  %shifted = lshr i32 %wide, 1
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}


define <4 x i8> @lshr_zext_shift_by_one_vec(<4 x i8> %x) {
  %wide = zext <4 x i8> %x to <4 x i32>
  %shifted = lshr <4 x i32> %wide, splat (i32 1)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}

