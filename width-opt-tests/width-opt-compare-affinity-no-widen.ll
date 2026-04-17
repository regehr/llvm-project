
define i16 @f(i1 %c, i8 %k) {
entry:
  %s = select i1 %c, i8 1, i8 2
  %cmp = icmp eq i8 %s, %k
  %x = zext i8 %s to i16
  %cmp.z = zext i1 %cmp to i16
  %r = add i16 %x, %cmp.z
  ret i16 %r
}

