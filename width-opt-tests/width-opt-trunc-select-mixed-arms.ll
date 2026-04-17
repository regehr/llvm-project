
define i16 @f(i1 %c, i32 %x, i8 %y) {
entry:
  %sy = sext i8 %y to i32
  %sel = select i1 %c, i32 %x, i32 %sy
  %t = trunc i32 %sel to i16
  ret i16 %t
}


define <4 x i16> @f_vec(i1 %c, <4 x i32> %x, <4 x i8> %y) {
entry:
  %sy = sext <4 x i8> %y to <4 x i32>
  %sel = select i1 %c, <4 x i32> %x, <4 x i32> %sy
  %t = trunc <4 x i32> %sel to <4 x i16>
  ret <4 x i16> %t
}

