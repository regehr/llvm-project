
define i1 @icmp_ult_or_same_lshr(i8 %x) {
entry:
  %z = zext i8 %x to i32
  %s = lshr i32 %z, 1
  %o = or i32 %s, %s
  %cmp = icmp ult i32 %o, 10
  ret i1 %cmp
}

