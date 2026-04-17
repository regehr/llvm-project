
define i1 @f(i8 %x, i16 %y) {
  %x32 = zext i8 %x to i32
  %y32 = zext i16 %y to i32
  %fx = freeze i32 %x32
  %fy = freeze i32 %y32
  %c = icmp ult i32 %fx, %fy
  ret i1 %c
}


define <4 x i1> @f_vec(<4 x i8> %x, <4 x i16> %y) {
  %x32 = zext <4 x i8> %x to <4 x i32>
  %y32 = zext <4 x i16> %y to <4 x i32>
  %fx = freeze <4 x i32> %x32
  %fy = freeze <4 x i32> %y32
  %c = icmp ult <4 x i32> %fx, %fy
  ret <4 x i1> %c
}

