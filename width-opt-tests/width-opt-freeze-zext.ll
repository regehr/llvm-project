
define i32 @f(i8 %x) {
entry:
  %zx = zext i8 %x to i32
  %fr = freeze i32 %zx
  ret i32 %fr
}


define <4 x i32> @f_vec(<4 x i8> %x) {
entry:
  %zx = zext <4 x i8> %x to <4 x i32>
  %fr = freeze <4 x i32> %zx
  ret <4 x i32> %fr
}

