
declare void @use8(i8)

define i32 @g(i1 %c) {
entry:
  %s = select i1 %c, i8 -1, i8 2
  call void @use8(i8 %s)
  %x = sext i8 %s to i32
  %y = zext i8 %s to i32
  %r = add i32 %x, %y
  ret i32 %r
}

