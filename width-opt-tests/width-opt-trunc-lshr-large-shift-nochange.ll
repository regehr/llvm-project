

define i8 @lshr_shift_too_large(i8 %x) {
  %wide = zext i8 %x to i32
  %shifted = lshr i32 %wide, 8
  %narrow = trunc i32 %shifted to i8
  ret i8 %narrow
}


define <4 x i8> @lshr_shift_too_large_vec(<4 x i8> %x) {
  %wide = zext <4 x i8> %x to <4 x i32>
  %shifted = lshr <4 x i32> %wide, splat (i32 8)
  %narrow = trunc <4 x i32> %shifted to <4 x i8>
  ret <4 x i8> %narrow
}

