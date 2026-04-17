
define i1 @test_trunc_self_eq(i8 %x) {
entry:
  %zext = zext i8 %x to i32
  %t = trunc i32 %zext to i16
  %cmp = icmp eq i16 %t, %t
  ret i1 %cmp
}
