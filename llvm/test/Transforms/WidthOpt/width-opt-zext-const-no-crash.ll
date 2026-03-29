; RUN: opt -passes='width-opt' -S %s -o /dev/null
; Verify no crash when tryWidenAddThroughZExt encounters a zext whose source
; is a ConstantData (e.g. i1 true).  findExistingZExtToWidth must guard
; against calling users() on a value without a use list.

define void @test_zext_const_add() {
entry:
  %zext = zext i1 true to i8
  %add = add i8 %zext, 1
  %widezext = zext i8 %add to i32
  ret void
}
