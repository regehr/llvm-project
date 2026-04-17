
define i1 @slt_positive_const(i8 %x) {
  %ext = sext i8 %x to i32
  %cmp = icmp slt i32 %ext, 100
  ret i1 %cmp
}


define <4 x i1> @slt_positive_const_vec(<4 x i8> %x) {
  %ext = sext <4 x i8> %x to <4 x i32>
  %cmp = icmp slt <4 x i32> %ext, splat (i32 100)
  ret <4 x i1> %cmp
}


define i1 @sgt_negative_const(i8 %x) {
  %ext = sext i8 %x to i32
  %cmp = icmp sgt i32 %ext, -5
  ret i1 %cmp
}


define <4 x i1> @sgt_negative_const_vec(<4 x i8> %x) {
  %ext = sext <4 x i8> %x to <4 x i32>
  %cmp = icmp sgt <4 x i32> %ext, splat (i32 -5)
  ret <4 x i1> %cmp
}


define i1 @eq_sext_const(i8 %x) {
  %ext = sext i8 %x to i32
  %cmp = icmp eq i32 %ext, -1
  ret i1 %cmp
}


define <4 x i1> @eq_sext_const_vec(<4 x i8> %x) {
  %ext = sext <4 x i8> %x to <4 x i32>
  %cmp = icmp eq <4 x i32> %ext, splat (i32 -1)
  ret <4 x i1> %cmp
}

