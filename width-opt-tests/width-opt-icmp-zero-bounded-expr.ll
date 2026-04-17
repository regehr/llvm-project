

define i1 @icmp_eq_and_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %and = and i32 %a32, %b32
  %cmp = icmp eq i32 %and, 0
  ret i1 %cmp
}


define i1 @icmp_ne_or_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %or = or i32 %a32, %b32
  %cmp = icmp ne i32 %or, 0
  ret i1 %cmp
}


define i1 @icmp_ult_and_mask(i8 %a) {
  %a32 = zext i8 %a to i32
  %masked = and i32 %a32, 15
  %cmp = icmp ult i32 %masked, 10
  ret i1 %cmp
}


define <4 x i1> @icmp_eq_and_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %and = and <4 x i32> %a32, %b32
  %cmp = icmp eq <4 x i32> %and, zeroinitializer
  ret <4 x i1> %cmp
}


define <4 x i1> @icmp_ne_or_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %or = or <4 x i32> %a32, %b32
  %cmp = icmp ne <4 x i32> %or, zeroinitializer
  ret <4 x i1> %cmp
}


define <4 x i1> @icmp_ult_and_mask_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %masked = and <4 x i32> %a32, <i32 15, i32 15, i32 15, i32 15>
  %cmp = icmp ult <4 x i32> %masked, <i32 10, i32 10, i32 10, i32 10>
  ret <4 x i1> %cmp
}

