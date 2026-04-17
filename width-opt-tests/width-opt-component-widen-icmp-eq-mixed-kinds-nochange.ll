
define i32 @f(i1 %c0, i8 %a, i8 %b) {
entry:
  %v = select i1 %c0, i8 -1, i8 2
  %w = or i8 %a, %b
  %cmp = icmp eq i8 %v, %w
  %cmpz = zext i1 %cmp to i32
  %vx = sext i8 %v to i32
  %wz0 = zext i8 %w to i32
  %wz1 = zext i8 %w to i32
  %sum0 = add i32 %cmpz, %vx
  %sum1 = add i32 %sum0, %wz0
  %r = add i32 %sum1, %wz1
  ret i32 %r
}

