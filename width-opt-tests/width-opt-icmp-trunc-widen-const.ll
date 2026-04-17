

define i1 @icmp_trunc_widen_const(i32 %x) {
  %and = and i32 %x, 255
  %tx = trunc i32 %and to i8
  %cmp = icmp ult i8 %tx, 42
  ret i1 %cmp
}
