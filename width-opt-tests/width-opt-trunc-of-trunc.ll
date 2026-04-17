

define i8 @trunc_i32_to_i16_to_i8(i32 %a) {
  %t16 = trunc i32 %a to i16
  %t8 = trunc i16 %t16 to i8
  ret i8 %t8
}


define <4 x i8> @trunc_i32_to_i16_to_i8_vec(<4 x i32> %a) {
  %t16 = trunc <4 x i32> %a to <4 x i16>
  %t8 = trunc <4 x i16> %t16 to <4 x i8>
  ret <4 x i8> %t8
}


define i8 @trunc_i64_to_i32_to_i8(i64 %a) {
  %t32 = trunc i64 %a to i32
  %t8 = trunc i32 %t32 to i8
  ret i8 %t8
}


define <4 x i8> @trunc_i64_to_i32_to_i8_vec(<4 x i64> %a) {
  %t32 = trunc <4 x i64> %a to <4 x i32>
  %t8 = trunc <4 x i32> %t32 to <4 x i8>
  ret <4 x i8> %t8
}

