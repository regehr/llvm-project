
define i1 @f(i16 %x, i8 %y) {
  %x32 = sext i16 %x to i32
  %y32 = zext i8 %y to i32
  %c = icmp eq i32 %x32, %y32
  ret i1 %c
}


define <4 x i1> @f_vec(<4 x i16> %x, <4 x i8> %y) {
  %x32 = sext <4 x i16> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %c = icmp eq <4 x i32> %x32, %y32
  ret <4 x i1> %c
}

