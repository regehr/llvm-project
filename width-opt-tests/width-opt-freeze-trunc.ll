
define i8 @f(i16 %x) {
  %x8 = trunc i16 %x to i8
  %fr = freeze i8 %x8
  ret i8 %fr
}


define <4 x i8> @f_vec(<4 x i16> %x) {
  %x8 = trunc <4 x i16> %x to <4 x i8>
  %fr = freeze <4 x i8> %x8
  ret <4 x i8> %fr
}

