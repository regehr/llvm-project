
define float @f(i1 %c, float %x, float %y) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi float [ %x, %left ], [ %y, %right ]
  ret float %p
}

