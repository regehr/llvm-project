

define i16 @src(i16 %x, i8 %y) {
  %x32 = sext i16 %x to i32
  %y32 = zext i8 %y to i32
  %c = icmp slt i32 %x32, %y32
  %y16 = zext i8 %y to i16
  %r = select i1 %c, i16 %y16, i16 %x
  ret i16 %r
}


define <4 x i16> @src_vec(<4 x i16> %x, <4 x i8> %y) {
  %x32 = sext <4 x i16> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %c = icmp slt <4 x i32> %x32, %y32
  %y16 = zext <4 x i8> %y to <4 x i16>
  %r = select <4 x i1> %c, <4 x i16> %y16, <4 x i16> %x
  ret <4 x i16> %r
}

