
define i4 @f(i8 %n) {
entry:
  %wide = sext i8 %n to i64
  %tr = trunc i64 %wide to i4
  ret i4 %tr
}


define <4 x i4> @f_vec(<4 x i8> %n) {
entry:
  %wide = sext <4 x i8> %n to <4 x i64>
  %tr = trunc <4 x i64> %wide to <4 x i4>
  ret <4 x i4> %tr
}

