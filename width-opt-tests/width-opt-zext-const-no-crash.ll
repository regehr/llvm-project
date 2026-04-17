
define void @test_zext_const_add() {
entry:
  %zext = zext i1 true to i8
  %add = add i8 %zext, 1
  %widezext = zext i8 %add to i32
  ret void
}
