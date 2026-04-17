
define i16 @f(i8 %a, i1 %cond) {
entry:
  %conv = sext i8 %a to i32
  %sub = sub nsw i32 0, %conv
  %sel = select i1 %cond, i32 %sub, i32 %conv
  %t = trunc i32 %sel to i16
  ret i16 %t
}


define <4 x i16> @f_vec(<4 x i8> %a, <4 x i1> %cond) {
entry:
  %conv = sext <4 x i8> %a to <4 x i32>
  %sub = sub nsw <4 x i32> zeroinitializer, %conv
  %sel = select <4 x i1> %cond, <4 x i32> %sub, <4 x i32> %conv
  %t = trunc <4 x i32> %sel to <4 x i16>
  ret <4 x i16> %t
}

