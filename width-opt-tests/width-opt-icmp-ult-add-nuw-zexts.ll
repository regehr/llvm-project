

define i1 @icmp_ult_add_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %sum = add nuw i32 %a32, %b32
  %cmp = icmp ult i32 %sum, 300
  ret i1 %cmp
}


define i1 @icmp_eq_add_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %sum = add nuw i32 %a32, %b32
  %cmp = icmp eq i32 %sum, 100
  ret i1 %cmp
}


define <4 x i1> @icmp_ult_add_nuw_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %sum = add nuw <4 x i32> %a32, %b32
  %cmp = icmp ult <4 x i32> %sum, <i32 300, i32 300, i32 300, i32 300>
  ret <4 x i1> %cmp
}

