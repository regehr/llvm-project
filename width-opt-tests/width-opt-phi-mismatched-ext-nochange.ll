
define i32 @f(i1 %c, i8 %x, i8 %y) {
entry:
  br i1 %c, label %left, label %right

left:
  %sx = sext i8 %x to i32
  br label %merge

right:
  %zy = zext i8 %y to i32
  br label %merge

merge:
  %p = phi i32 [ %sx, %left ], [ %zy, %right ]
  ret i32 %p
}


define <4 x i32> @f_vec(i1 %c, <4 x i8> %x, <4 x i8> %y) {
entry:
  br i1 %c, label %left, label %right

left:
  %sx = sext <4 x i8> %x to <4 x i32>
  br label %merge

right:
  %zy = zext <4 x i8> %y to <4 x i32>
  br label %merge

merge:
  %p = phi <4 x i32> [ %sx, %left ], [ %zy, %right ]
  ret <4 x i32> %p
}

