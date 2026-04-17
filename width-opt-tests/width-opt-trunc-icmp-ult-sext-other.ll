
define i1 @f(i8 %x, i8 %y) {
entry:
  %wide = zext i8 %x to i32
  %tx = trunc i32 %wide to i16
  %sy = sext i8 %y to i16
  %cmp = icmp ult i16 %tx, %sy
  ret i1 %cmp
}


define <4 x i1> @f_vec(<4 x i8> %x, <4 x i8> %y) {
entry:
  %wide = zext <4 x i8> %x to <4 x i32>
  %tx = trunc <4 x i32> %wide to <4 x i16>
  %sy = sext <4 x i8> %y to <4 x i16>
  %cmp = icmp ult <4 x i16> %tx, %sy
  ret <4 x i1> %cmp
}

