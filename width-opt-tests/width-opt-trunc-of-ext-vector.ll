
define <2 x i16> @trunc_ext(<2 x i8> %a) {
entry:
  %z = zext <2 x i8> %a to <2 x i32>
  %t = trunc <2 x i32> %z to <2 x i16>
  ret <2 x i16> %t
}

