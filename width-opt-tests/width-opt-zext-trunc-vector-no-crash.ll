
define <2 x i65> @foo(<2 x i64> %t) {
entry:
  %a = trunc <2 x i64> %t to <2 x i32>
  %b = zext <2 x i32> %a to <2 x i65>
  ret <2 x i65> %b
}

