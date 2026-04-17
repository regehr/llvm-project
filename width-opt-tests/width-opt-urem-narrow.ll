

define i8 @urem_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %r = urem i32 %a32, %b32
  %t = trunc i32 %r to i8
  ret i8 %t
}


define <4 x i8> @urem_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %r = urem <4 x i32> %a32, %b32
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}


define i8 @urem_zext_const(i8 %a) {
  %a32 = zext i8 %a to i32
  %r = urem i32 %a32, 17
  %t = trunc i32 %r to i8
  ret i8 %t
}


define <4 x i8> @urem_zext_const_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %r = urem <4 x i32> %a32, <i32 17, i32 17, i32 17, i32 17>
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}

