

define i8 @materialize_and_zero_from_nested_mul(i8 %a, i8 %b, i8 %c) {
  %za = zext i8 %a to i32
  %zb = zext i8 %b to i32
  %zc = zext i8 %c to i32
  %mul = mul i32 %za, 0
  %inner = and i32 %mul, %zb
  %outer = or i32 %inner, %zc
  %t = trunc i32 %outer to i8
  ret i8 %t
}
