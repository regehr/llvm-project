

define i8 @trunc_lshr_sext(i8 %a) {
  %a32 = sext i8 %a to i32
  %s = lshr i32 %a32, 3
  %t = trunc i32 %s to i8
  ret i8 %t
}


define i8 @trunc_lshr_sext_by_one(i8 %a) {
  %a32 = sext i8 %a to i32
  %s = lshr i32 %a32, 1
  %t = trunc i32 %s to i8
  ret i8 %t
}


define i16 @trunc_lshr_sext_i16(i16 %a) {
  %a32 = sext i16 %a to i32
  %s = lshr i32 %a32, 5
  %t = trunc i32 %s to i16
  ret i16 %t
}


define <4 x i8> @trunc_lshr_sext_vec(<4 x i8> %a) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %s = lshr <4 x i32> %a32, splat (i32 3)
  %t = trunc <4 x i32> %s to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i16> @trunc_lshr_sext_i16_vec(<4 x i16> %a) {
  %a32 = sext <4 x i16> %a to <4 x i32>
  %s = lshr <4 x i32> %a32, splat (i32 5)
  %t = trunc <4 x i32> %s to <4 x i16>
  ret <4 x i16> %t
}

