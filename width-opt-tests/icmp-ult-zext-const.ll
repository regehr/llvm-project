
define i1 @ult_const(i8 %x) {
  %ext = zext i8 %x to i32
  %cmp = icmp ult i32 %ext, 100
  ret i1 %cmp
}


define <4 x i1> @ult_const_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp ult <4 x i32> %ext, splat (i32 100)
  ret <4 x i1> %cmp
}


define i1 @uge_const(i8 %x) {
  %ext = zext i8 %x to i32
  %cmp = icmp uge i32 %ext, 200
  ret i1 %cmp
}


define <4 x i1> @uge_const_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp uge <4 x i32> %ext, splat (i32 200)
  ret <4 x i1> %cmp
}


define i1 @const_on_left(i8 %x) {
  %ext = zext i8 %x to i32
  %cmp = icmp ult i32 50, %ext
  ret i1 %cmp
}


define <4 x i1> @const_on_left_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp ult <4 x i32> splat (i32 50), %ext
  ret <4 x i1> %cmp
}

