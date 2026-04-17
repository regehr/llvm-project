
define i1 @ult_zext_large_const(i8 %x) {
entry:
  %ext = zext i8 %x to i32
  %cmp = icmp ult i32 %ext, 300
  ret i1 %cmp
}


define i1 @eq_zext_negative_const(i8 %x) {
entry:
  %ext = zext i8 %x to i32
  %cmp = icmp eq i32 %ext, -1
  ret i1 %cmp
}


define i1 @sgt_sext_below_min(i8 %x) {
entry:
  %ext = sext i8 %x to i32
  %cmp = icmp sgt i32 %ext, -129
  ret i1 %cmp
}


define <4 x i1> @ult_zext_large_const_vec(<4 x i8> %x) {
entry:
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp ult <4 x i32> %ext, <i32 300, i32 300, i32 300, i32 300>
  ret <4 x i1> %cmp
}


define <4 x i1> @eq_zext_negative_const_vec(<4 x i8> %x) {
entry:
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp eq <4 x i32> %ext, <i32 -1, i32 -1, i32 -1, i32 -1>
  ret <4 x i1> %cmp
}

