
define i8 @shl_const_small(i8 %x) {
  %ext = zext i8 %x to i32
  %shl = shl i32 %ext, 2
  %trunc = trunc i32 %shl to i8
  ret i8 %trunc
}


define i8 @add_shl_const(i8 %x, i8 %y) {
  %ex = zext i8 %x to i32
  %ey = zext i8 %y to i32
  %shl = shl i32 %ex, 1
  %add = add i32 %shl, %ey
  %trunc = trunc i32 %add to i8
  ret i8 %trunc
}


define <4 x i8> @shl_const_small_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %shl = shl <4 x i32> %ext, splat (i32 2)
  %trunc = trunc <4 x i32> %shl to <4 x i8>
  ret <4 x i8> %trunc
}

