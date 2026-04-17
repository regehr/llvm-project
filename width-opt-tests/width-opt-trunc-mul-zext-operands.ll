
define i16 @root_mul_ret(i8 %a, i8 %b) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %mul = mul i32 %a32, %b32
  %tr = trunc i32 %mul to i16
  ret i16 %tr
}


define <4 x i16> @root_mul_ret_vec(<4 x i8> %a, <4 x i8> %b) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %mul = mul <4 x i32> %a32, %b32
  %tr = trunc <4 x i32> %mul to <4 x i16>
  ret <4 x i16> %tr
}

