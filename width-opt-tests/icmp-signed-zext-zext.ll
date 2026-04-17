
define i1 @slt_zext_zext(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %cmp = icmp slt i32 %za, %zb
  ret i1 %cmp
}


define <4 x i1> @slt_zext_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %za = zext <4 x i8> %a to <4 x i32>
  %zb = zext <4 x i8> %b to <4 x i32>
  %cmp = icmp slt <4 x i32> %za, %zb
  ret <4 x i1> %cmp
}


define i1 @sle_zext_zext(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %cmp = icmp sle i32 %za, %zb
  ret i1 %cmp
}


define <4 x i1> @sle_zext_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %za = zext <4 x i8> %a to <4 x i32>
  %zb = zext <4 x i8> %b to <4 x i32>
  %cmp = icmp sle <4 x i32> %za, %zb
  ret <4 x i1> %cmp
}


define i1 @sgt_zext_zext(i8 %a, i8 %b) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %cmp = icmp sgt i32 %za, %zb
  ret i1 %cmp
}


define <4 x i1> @sgt_zext_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %za = zext <4 x i8> %a to <4 x i32>
  %zb = zext <4 x i8> %b to <4 x i32>
  %cmp = icmp sgt <4 x i32> %za, %zb
  ret <4 x i1> %cmp
}

