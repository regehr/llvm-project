
define i1 @icmp_ult_add_nuw_zero(i8 %a) {
entry:
  %a32 = zext i8 %a to i32
  %sum = add nuw i32 %a32, 0
  %cmp = icmp ult i32 %sum, 10
  ret i1 %cmp
}

