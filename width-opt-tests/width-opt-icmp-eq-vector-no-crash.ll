
define <2 x i1> @eq_21_vector(<2 x i32> %x, <2 x i32> %y) {
entry:
  %x.321 = lshr <2 x i32> %x, <i32 8, i32 8>
  %x.1 = trunc <2 x i32> %x.321 to <2 x i8>
  %x.32 = lshr <2 x i32> %x, <i32 16, i32 16>
  %x.2 = trunc <2 x i32> %x.32 to <2 x i8>
  %y.321 = lshr <2 x i32> %y, <i32 8, i32 8>
  %y.1 = trunc <2 x i32> %y.321 to <2 x i8>
  %y.32 = lshr <2 x i32> %y, <i32 16, i32 16>
  %y.2 = trunc <2 x i32> %y.32 to <2 x i8>
  %c.1 = icmp eq <2 x i8> %x.1, %y.1
  %c.2 = icmp eq <2 x i8> %x.2, %y.2
  %c.210 = and <2 x i1> %c.2, %c.1
  ret <2 x i1> %c.210
}

