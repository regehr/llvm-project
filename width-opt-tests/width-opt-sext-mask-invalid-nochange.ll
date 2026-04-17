
define i32 @f(i8 %x) {
  %ext = sext i8 %x to i32
  %and = and i32 %ext, 511
  ret i32 %and
}


define <4 x i32> @f_vec(<4 x i8> %x) {
  %ext = sext <4 x i8> %x to <4 x i32>
  %and = and <4 x i32> %ext, splat (i32 511)
  ret <4 x i32> %and
}

