
define i1 @zext_false_ule_zero() {
entry:
  %ext = zext i1 false to i32
  %cmp = icmp ule i32 %ext, 0
  ret i1 %cmp
}

