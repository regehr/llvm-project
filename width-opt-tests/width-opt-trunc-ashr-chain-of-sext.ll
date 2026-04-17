

define i8 @ashr_chain_depth2(i8 %a) {
  %a32 = sext i8 %a to i32
  %s1 = ashr i32 %a32, 2
  %s2 = ashr i32 %s1, 1
  %t = trunc i32 %s2 to i8
  ret i8 %t
}


define i8 @ashr_chain_depth3(i8 %a) {
  %a32 = sext i8 %a to i32
  %s1 = ashr i32 %a32, 1
  %s2 = ashr i32 %s1, 1
  %s3 = ashr i32 %s2, 2
  %t = trunc i32 %s3 to i8
  ret i8 %t
}


define <4 x i8> @ashr_chain_depth2_vec(<4 x i8> %a) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %s1 = ashr <4 x i32> %a32, splat (i32 2)
  %s2 = ashr <4 x i32> %s1, splat (i32 1)
  %t = trunc <4 x i32> %s2 to <4 x i8>
  ret <4 x i8> %t
}

