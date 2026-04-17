
define i8 @f(i8 %x, i32 %y) {
  %ex = zext i8 %x to i32
  %a = add i32 %ex, %y
  %t = trunc i32 %a to i8
  ret i8 %t
}


define <4 x i8> @f_vec(<4 x i8> %x, <4 x i32> %y) {
  %ex = zext <4 x i8> %x to <4 x i32>
  %a = add <4 x i32> %ex, %y
  %t = trunc <4 x i32> %a to <4 x i8>
  ret <4 x i8> %t
}

