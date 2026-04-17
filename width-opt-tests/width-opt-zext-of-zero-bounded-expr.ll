

define i32 @zext_and_zexts(i8 %a, i8 %b) {
  %a16 = zext i8 %a to i16
  %b16 = zext i8 %b to i16
  %and = and i16 %a16, %b16
  %ext = zext i16 %and to i32
  ret i32 %ext
}


define <4 x i32> @zext_and_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a16 = zext <4 x i8> %a to <4 x i16>
  %b16 = zext <4 x i8> %b to <4 x i16>
  %and = and <4 x i16> %a16, %b16
  %ext = zext <4 x i16> %and to <4 x i32>
  ret <4 x i32> %ext
}


define i32 @zext_or_zexts(i8 %a, i8 %b) {
  %a16 = zext i8 %a to i16
  %b16 = zext i8 %b to i16
  %or = or i16 %a16, %b16
  %ext = zext i16 %or to i32
  ret i32 %ext
}


define <4 x i32> @zext_or_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a16 = zext <4 x i8> %a to <4 x i16>
  %b16 = zext <4 x i8> %b to <4 x i16>
  %or = or <4 x i16> %a16, %b16
  %ext = zext <4 x i16> %or to <4 x i32>
  ret <4 x i32> %ext
}


define i32 @zext_of_zext(i8 %a) {
  %a16 = zext i8 %a to i16
  %a32 = zext i16 %a16 to i32
  ret i32 %a32
}


define <4 x i32> @zext_of_zext_vec(<4 x i8> %a) {
  %a16 = zext <4 x i8> %a to <4 x i16>
  %a32 = zext <4 x i16> %a16 to <4 x i32>
  ret <4 x i32> %a32
}

