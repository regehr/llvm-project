

define i8 @and_0xFF_before_trunc(i32 %x) {
  %m = and i32 %x, 255
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @bitfield_extract_byte1(i32 %a) {
  %s = lshr i32 %a, 8
  %m = and i32 %s, 255
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @and_0xFFFF_before_trunc(i32 %x) {
  %m = and i32 %x, 65535
  %t = trunc i32 %m to i8
  ret i8 %t
}


define <4 x i8> @and_0xFF_before_trunc_vec(<4 x i32> %x) {
  %m = and <4 x i32> %x, <i32 255, i32 255, i32 255, i32 255>
  %t = trunc <4 x i32> %m to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i8> @bitfield_extract_byte1_vec(<4 x i32> %a) {
  %s = lshr <4 x i32> %a, splat (i32 8)
  %m = and <4 x i32> %s, <i32 255, i32 255, i32 255, i32 255>
  %t = trunc <4 x i32> %m to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i8> @and_0xFFFF_before_trunc_vec(<4 x i32> %x) {
  %m = and <4 x i32> %x, <i32 65535, i32 65535, i32 65535, i32 65535>
  %t = trunc <4 x i32> %m to <4 x i8>
  ret <4 x i8> %t
}

