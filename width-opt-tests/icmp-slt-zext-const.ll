
define i1 @slt_zext_const(i8 %x) {
  %ext = zext i8 %x to i32
  %cmp = icmp slt i32 %ext, 100
  ret i1 %cmp
}


define <4 x i1> @slt_zext_const_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp slt <4 x i32> %ext, splat (i32 100)
  ret <4 x i1> %cmp
}

