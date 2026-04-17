
define i32 @second_round_lvi(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %cmp = icmp sgt i32 %sx, 0
  br i1 %cmp, label %then, label %else

then:
  %zx = zext i8 %x to i32
  ret i32 %zx

else:
  ret i32 0
}
