
define i1 @shared_wide_add_operands_narrowed_recursively(i8 %a, i8 %c, i8 %d) {
entry:
  %a32 = zext i8 %a to i32
  %c32 = zext i8 %c to i32
  %d32 = zext i8 %d to i32
  %sub = xor i32 255, %c32
  %mul1 = mul i32 %a32, %sub
  %mul2 = mul i32 %c32, %d32
  %add = add i32 %mul1, %mul2
  %trunc = trunc i32 %add to i16
  %cmp = icmp eq i16 %trunc, 1234
  ret i1 %cmp
}


define <4 x i1> @shared_wide_add_vec(<4 x i8> %a, <4 x i8> %c, <4 x i8> %d) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %c32 = zext <4 x i8> %c to <4 x i32>
  %d32 = zext <4 x i8> %d to <4 x i32>
  %sub = xor <4 x i32> splat (i32 255), %c32
  %mul1 = mul <4 x i32> %a32, %sub
  %mul2 = mul <4 x i32> %c32, %d32
  %add = add <4 x i32> %mul1, %mul2
  %trunc = trunc <4 x i32> %add to <4 x i16>
  %cmp = icmp eq <4 x i16> %trunc, splat (i16 1234)
  ret <4 x i1> %cmp
}

