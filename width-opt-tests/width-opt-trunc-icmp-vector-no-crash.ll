
define <2 x i1> @test36vec(<2 x i32> %a) {
entry:
  %b = lshr <2 x i32> %a, <i32 31, i32 31>
  %c = trunc <2 x i32> %b to <2 x i8>
  %d = icmp eq <2 x i8> %c, zeroinitializer
  ret <2 x i1> %d
}

