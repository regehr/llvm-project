


define i8 @trunc_and_zero_rhs(i8 %a) {
  %z = zext i8 %a to i32
  %and = and i32 %z, 256
  %t = trunc i32 %and to i8
  ret i8 %t
}


define i8 @trunc_or_all_ones_rhs(i8 %a) {
  %z = zext i8 %a to i32
  %or = or i32 %z, 255
  %t = trunc i32 %or to i8
  ret i8 %t
}


define i8 @trunc_xor_zero_rhs(i8 %a) {
  %z = zext i8 %a to i32
  %xor = xor i32 %z, 256
  %t = trunc i32 %xor to i8
  ret i8 %t
}


define i8 @trunc_xor_zero_lhs(i8 %a) {
  %z = zext i8 %a to i32
  %xor = xor i32 256, %z
  %t = trunc i32 %xor to i8
  ret i8 %t
}


define i8 @trunc_mul_zero_rhs(i8 %a) {
  %z = zext i8 %a to i32
  %mul = mul i32 %z, 256
  %t = trunc i32 %mul to i8
  ret i8 %t
}
