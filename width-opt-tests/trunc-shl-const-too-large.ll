
define i8 @shl_const_too_large(i8 %x) {
  %ext = zext i8 %x to i32
  %shl = shl i32 %ext, 8
  %trunc = trunc i32 %shl to i8
  ret i8 %trunc
}


define <4 x i8> @shl_const_too_large_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %shl = shl <4 x i32> %ext, splat (i32 8)
  %trunc = trunc <4 x i32> %shl to <4 x i8>
  ret <4 x i8> %trunc
}

