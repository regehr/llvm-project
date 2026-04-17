
define <2 x i8> @f(<2 x i8> %x) {
entry:
  %a = udiv <2 x i8> %x, <i8 1, i8 1>
  ret <2 x i8> %a
}

define <2 x i8> @g(<2 x i8> %x) {
entry:
  %a = sdiv <2 x i8> %x, <i8 1, i8 1>
  ret <2 x i8> %a
}


