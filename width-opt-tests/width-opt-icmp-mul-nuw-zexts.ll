

define i1 @icmp_ult_mul_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %prod = mul nuw i32 %a32, %b32
  %cmp = icmp ult i32 %prod, 1000
  ret i1 %cmp
}


define i1 @icmp_eq_mul_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %prod = mul nuw i32 %a32, %b32
  %cmp = icmp eq i32 %prod, 100
  ret i1 %cmp
}

