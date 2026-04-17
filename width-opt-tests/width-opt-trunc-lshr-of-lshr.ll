

define i8 @lshr_of_lshr(i8 %a) {
  %a32 = zext i8 %a to i32
  %s1 = lshr i32 %a32, 2
  %s2 = lshr i32 %s1, 1
  %t = trunc i32 %s2 to i8
  ret i8 %t
}



define i8 @lshr_then_or(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %s = lshr i32 %a32, 3
  %v = or i32 %s, %b32
  %t = trunc i32 %v to i8
  ret i8 %t
}


define <4 x i8> @lshr_of_lshr_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %s1 = lshr <4 x i32> %a32, splat (i32 2)
  %s2 = lshr <4 x i32> %s1, splat (i32 1)
  %t = trunc <4 x i32> %s2 to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i8> @lshr_then_or_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %s = lshr <4 x i32> %a32, splat (i32 3)
  %v = or <4 x i32> %s, %b32
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}

