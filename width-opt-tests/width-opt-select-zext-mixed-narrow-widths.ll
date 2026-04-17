
define i32 @f(i1 %c, i8 %a, i16 %b) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i16 %b to i32
  %s = select i1 %c, i32 %a32, i32 %b32
  %x = add i32 %s, 1
  %y = xor i32 %s, 42
  %r = add i32 %x, %y
  ret i32 %r
}


define <4 x i32> @f_vec(<4 x i1> %c, <4 x i8> %a, <4 x i16> %b) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i16> %b to <4 x i32>
  %s = select <4 x i1> %c, <4 x i32> %a32, <4 x i32> %b32
  %x = add <4 x i32> %s, <i32 1, i32 1, i32 1, i32 1>
  %y = xor <4 x i32> %s, <i32 42, i32 42, i32 42, i32 42>
  %r = add <4 x i32> %x, %y
  ret <4 x i32> %r
}

