
define i32 @f(i1 %c, i8 %x) {
entry:
  br i1 %c, label %left, label %right

left:
  %zx = zext i8 %x to i32
  br label %merge

right:
  br label %merge

merge:
  %p = phi i32 [ %zx, %left ], [ 256, %right ]
  ret i32 %p
}


define <4 x i32> @f_vec(i1 %c, <4 x i8> %x) {
entry:
  br i1 %c, label %left, label %right

left:
  %zx = zext <4 x i8> %x to <4 x i32>
  br label %merge

right:
  br label %merge

merge:
  %p = phi <4 x i32> [ %zx, %left ], [ splat (i32 256), %right ]
  ret <4 x i32> %p
}

