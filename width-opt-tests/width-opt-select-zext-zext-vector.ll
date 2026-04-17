
define <2 x i32> @sel_ext(<2 x i1> %c, <2 x i8> %a, <2 x i8> %b) {
entry:
  %za = zext <2 x i8> %a to <2 x i32>
  %zb = zext <2 x i8> %b to <2 x i32>
  %s = select <2 x i1> %c, <2 x i32> %za, <2 x i32> %zb
  ret <2 x i32> %s
}

