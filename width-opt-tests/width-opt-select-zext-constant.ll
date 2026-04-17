
define i32 @f(i1 %c, i8 %x) {
entry:
  %zx = zext i8 %x to i32
  %r = select i1 %c, i32 %zx, i32 42
  ret i32 %r
}


define <4 x i32> @f_vec(<4 x i1> %c, <4 x i8> %x) {
entry:
  %zx = zext <4 x i8> %x to <4 x i32>
  %r = select <4 x i1> %c, <4 x i32> %zx, <4 x i32> splat (i32 42)
  ret <4 x i32> %r
}

