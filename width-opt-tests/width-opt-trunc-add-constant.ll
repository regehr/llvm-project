
define i16 @f(i16 %x) {
entry:
  %x32 = zext i16 %x to i32
  %add = add i32 %x32, 5
  %t = trunc i32 %add to i16
  ret i16 %t
}


define <4 x i16> @f_vec(<4 x i16> %x) {
entry:
  %x32 = zext <4 x i16> %x to <4 x i32>
  %add = add <4 x i32> %x32, splat (i32 5)
  %t = trunc <4 x i32> %add to <4 x i16>
  ret <4 x i16> %t
}

