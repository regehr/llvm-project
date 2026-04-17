
define <2 x i32> @phi_ext(i1 %c, <2 x i8> %a, <2 x i8> %b) {
entry:
  br i1 %c, label %t, label %f

t:
  %za = zext <2 x i8> %a to <2 x i32>
  br label %m

f:
  %zb = zext <2 x i8> %b to <2 x i32>
  br label %m

m:
  %p = phi <2 x i32> [ %za, %t ], [ %zb, %f ]
  ret <2 x i32> %p
}

