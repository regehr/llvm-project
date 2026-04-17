
define i1 @f(i1 %cond, i8 %x, i8 %y, i8 %z) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %s = select i1 %cond, i32 %x32, i32 %y32
  %z32 = zext i8 %z to i32
  %c = icmp eq i32 %s, %z32
  ret i1 %c
}


define <4 x i1> @f_vec(i1 %cond, <4 x i8> %x, <4 x i8> %y, <4 x i8> %z) {
entry:
  %x32 = zext <4 x i8> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %s = select i1 %cond, <4 x i32> %x32, <4 x i32> %y32
  %z32 = zext <4 x i8> %z to <4 x i32>
  %c = icmp eq <4 x i32> %s, %z32
  ret <4 x i1> %c
}

