
define i16 @f(i8 %x) {
  %x16 = sext i8 %x to i16
  %fr = freeze i16 %x16
  ret i16 %fr
}


define <4 x i16> @f_vec(<4 x i8> %x) {
  %x16 = sext <4 x i8> %x to <4 x i16>
  %fr = freeze <4 x i16> %x16
  ret <4 x i16> %fr
}

