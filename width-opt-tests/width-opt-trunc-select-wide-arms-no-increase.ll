
define i8 @wide_select_arms_not_split(i1 %c0, i1 %c1, i32 %x) {
entry:
  %clamp = select i1 %c0, i32 127, i32 -128
  %sel = select i1 %c1, i32 %x, i32 %clamp
  %tr = trunc i32 %sel to i8
  ret i8 %tr
}


define <4 x i8> @wide_select_arms_vec(i1 %c0, i1 %c1, <4 x i32> %x) {
entry:
  %clamp = select i1 %c0, <4 x i32> splat (i32 127), <4 x i32> splat (i32 -128)
  %sel = select i1 %c1, <4 x i32> %x, <4 x i32> %clamp
  %tr = trunc <4 x i32> %sel to <4 x i8>
  ret <4 x i8> %tr
}

