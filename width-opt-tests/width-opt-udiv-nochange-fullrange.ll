
define i32 @f(i32 %x, i32 %y) {
entry:
  %d = udiv i32 %x, %y
  ret i32 %d
}


define <4 x i32> @f_vec(<4 x i32> %x, <4 x i32> %y) {
entry:
  %d = udiv <4 x i32> %x, %y
  ret <4 x i32> %d
}

