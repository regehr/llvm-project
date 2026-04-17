

define i8 @lshr_of_or(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %or = or i32 %a32, %b32
  %shr = lshr i32 %or, 3
  %result = trunc i32 %shr to i8
  ret i8 %result
}



define i8 @lshr_of_and(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %and = and i32 %a32, %b32
  %shr = lshr i32 %and, 2
  %result = trunc i32 %shr to i8
  ret i8 %result
}



define i8 @lshr_of_and_mask(i32 %x) {
  %masked = and i32 %x, 255
  %shr = lshr i32 %masked, 3
  %result = trunc i32 %shr to i8
  ret i8 %result
}



define i8 @lshr_of_nested_bitwise(i8 %a, i8 %b, i8 %c) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %c32 = zext i8 %c to i32
  %or = or i32 %a32, %b32
  %xor = xor i32 %or, %c32
  %shr = lshr i32 %xor, 1
  %result = trunc i32 %shr to i8
  ret i8 %result
}


define <4 x i8> @lshr_of_or_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %or = or <4 x i32> %a32, %b32
  %shr = lshr <4 x i32> %or, splat (i32 3)
  %result = trunc <4 x i32> %shr to <4 x i8>
  ret <4 x i8> %result
}


define <4 x i8> @lshr_of_and_mask_vec(<4 x i32> %x) {
  %masked = and <4 x i32> %x, <i32 255, i32 255, i32 255, i32 255>
  %shr = lshr <4 x i32> %masked, splat (i32 3)
  %result = trunc <4 x i32> %shr to <4 x i8>
  ret <4 x i8> %result
}

