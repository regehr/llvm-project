

define i8 @chain_sext_i8_i16_i32_trunc_i8(i8 %a, i8 %b) {
  %a16 = sext i8 %a to i16
  %b16 = sext i8 %b to i16
  %a32 = sext i16 %a16 to i32
  %b32 = sext i16 %b16 to i32
  %r = add i32 %a32, %b32
  %t = trunc i32 %r to i8
  ret i8 %t
}


define i8 @chain_zext_i8_i16_i32_trunc_i8(i8 %a, i8 %b) {
  %a16 = zext i8 %a to i16
  %b16 = zext i8 %b to i16
  %a32 = zext i16 %a16 to i32
  %b32 = zext i16 %b16 to i32
  %r = add i32 %a32, %b32
  %t = trunc i32 %r to i8
  ret i8 %t
}


define <4 x i8> @chain_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %a16 = sext <4 x i8> %a to <4 x i16>
  %b16 = sext <4 x i8> %b to <4 x i16>
  %a32 = sext <4 x i16> %a16 to <4 x i32>
  %b32 = sext <4 x i16> %b16 to <4 x i32>
  %r = add <4 x i32> %a32, %b32
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i8> @chain_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %a16 = zext <4 x i8> %a to <4 x i16>
  %b16 = zext <4 x i8> %b to <4 x i16>
  %a32 = zext <4 x i16> %a16 to <4 x i32>
  %b32 = zext <4 x i16> %b16 to <4 x i32>
  %r = add <4 x i32> %a32, %b32
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}

