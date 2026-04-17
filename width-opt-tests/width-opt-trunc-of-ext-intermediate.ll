

define i16 @trunc_zext_to_intermediate(i8 %a) {
  %a32 = zext i8 %a to i32
  %t = trunc i32 %a32 to i16
  ret i16 %t
}


define <4 x i16> @trunc_zext_to_intermediate_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %t = trunc <4 x i32> %a32 to <4 x i16>
  ret <4 x i16> %t
}


define i16 @trunc_sext_to_intermediate(i8 %a) {
  %a32 = sext i8 %a to i32
  %t = trunc i32 %a32 to i16
  ret i16 %t
}


define <4 x i16> @trunc_sext_to_intermediate_vec(<4 x i8> %a) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %t = trunc <4 x i32> %a32 to <4 x i16>
  ret <4 x i16> %t
}


define i32 @trunc_zext_i64_to_i32(i8 %a) {
  %a64 = zext i8 %a to i64
  %t = trunc i64 %a64 to i32
  ret i32 %t
}


define <4 x i32> @trunc_zext_i64_to_i32_vec(<4 x i8> %a) {
  %a64 = zext <4 x i8> %a to <4 x i64>
  %t = trunc <4 x i64> %a64 to <4 x i32>
  ret <4 x i32> %t
}

