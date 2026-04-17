
define i32 @f(i8 %x, i8 %y) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %d = udiv i32 %x32, %y32
  ret i32 %d
}


define <4 x i32> @f_vec(<4 x i8> %x, <4 x i8> %y) {
entry:
  %x32 = zext <4 x i8> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %d = udiv <4 x i32> %x32, %y32
  ret <4 x i32> %d
}

