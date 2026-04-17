
define i32 @f(i8 %sel, i8 %a, i8 %b) {
entry:
  switch i8 %sel, label %c [
    i8 0, label %a.bb
    i8 1, label %b.bb
  ]

a.bb:
  %za = zext i8 %a to i32
  br label %merge

b.bb:
  %zb = zext i8 %b to i32
  br label %merge

c:
  br label %merge

merge:
  %p = phi i32 [ 7, %c ], [ %za, %a.bb ], [ %zb, %b.bb ]
  ret i32 %p
}


define <4 x i32> @f_vec(i8 %sel, <4 x i8> %a, <4 x i8> %b) {
entry:
  switch i8 %sel, label %c [
    i8 0, label %a.bb
    i8 1, label %b.bb
  ]

a.bb:
  %za = zext <4 x i8> %a to <4 x i32>
  br label %merge

b.bb:
  %zb = zext <4 x i8> %b to <4 x i32>
  br label %merge

c:
  br label %merge

merge:
  %p = phi <4 x i32> [ splat (i32 7), %c ], [ %za, %a.bb ], [ %zb, %b.bb ]
  ret <4 x i32> %p
}

