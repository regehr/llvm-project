
define i32 @f(i1 %c1, i1 %c2, i8 %x, i8 %y) {
entry:
  br i1 %c1, label %bb1, label %bb2
bb1:
  %zx = zext i8 %x to i32
  br label %merge
bb2:
  br i1 %c2, label %bb3, label %merge
bb3:
  %zy = zext i8 %y to i32
  br label %merge
merge:
  %p = phi i32 [ %zx, %bb1 ], [ %zy, %bb3 ], [ undef, %bb2 ]
  ret i32 %p
}
