
define i32 @f(i8 %x) {
  %sx = sext i8 %x to i32
  %r = and i32 %sx, 255
  ret i32 %r
}


define <4 x i32> @f_vec(<4 x i8> %x) {
  %sx = sext <4 x i8> %x to <4 x i32>
  %r = and <4 x i32> %sx, splat (i32 255)
  ret <4 x i32> %r
}

