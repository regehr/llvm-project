

define i8 @phi_of_udivs(i8 %a, i8 %b, i8 %c, i1 %cond) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %c32 = zext i8 %c to i32
  %d1 = udiv i32 %a32, %c32
  %d2 = udiv i32 %b32, %c32
  br i1 %cond, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi i32 [ %d1, %left ], [ %d2, %right ]
  %t = trunc i32 %v to i8
  ret i8 %t
}


define <4 x i8> @phi_of_udivs_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c, i1 %cond) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %c32 = zext <4 x i8> %c to <4 x i32>
  %d1 = udiv <4 x i32> %a32, %c32
  %d2 = udiv <4 x i32> %b32, %c32
  br i1 %cond, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi <4 x i32> [ %d1, %left ], [ %d2, %right ]
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}



define i8 @phi_of_urems(i8 %a, i8 %b, i8 %c, i1 %cond) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %c32 = zext i8 %c to i32
  %r1 = urem i32 %a32, %c32
  %r2 = urem i32 %b32, %c32
  br i1 %cond, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi i32 [ %r1, %left ], [ %r2, %right ]
  %t = trunc i32 %v to i8
  ret i8 %t
}


define <4 x i8> @phi_of_urems_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c, i1 %cond) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %c32 = zext <4 x i8> %c to <4 x i32>
  %r1 = urem <4 x i32> %a32, %c32
  %r2 = urem <4 x i32> %b32, %c32
  br i1 %cond, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi <4 x i32> [ %r1, %left ], [ %r2, %right ]
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}

