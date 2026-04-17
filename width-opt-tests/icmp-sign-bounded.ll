


define i1 @icmp_slt_and_sext(i8 %a, i8 %b, i8 %c) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %ab = and i32 %sa, %sb
  %r = icmp slt i32 %ab, %sc
  ret i1 %r
}


define i1 @icmp_sle_xor_sext(i8 %a, i8 %b, i8 %c) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %ab = xor i32 %sa, %sb
  %r = icmp sle i32 %ab, %sc
  ret i1 %r
}


define i1 @icmp_eq_ashr_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sh = ashr i32 %sa, 2
  %sb = sext i8 %b to i32
  %r = icmp eq i32 %sh, %sb
  ret i1 %r
}


define i1 @icmp_sgt_ashr_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sh = ashr i32 %sa, 3
  %sb = sext i8 %b to i32
  %r = icmp sgt i32 %sh, %sb
  ret i1 %r
}



define <4 x i1> @icmp_slt_and_sext_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %sc = sext <4 x i8> %c to <4 x i32>
  %ab = and <4 x i32> %sa, %sb
  %r = icmp slt <4 x i32> %ab, %sc
  ret <4 x i1> %r
}


define <4 x i1> @icmp_sle_xor_sext_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %sc = sext <4 x i8> %c to <4 x i32>
  %ab = xor <4 x i32> %sa, %sb
  %r = icmp sle <4 x i32> %ab, %sc
  ret <4 x i1> %r
}


define <4 x i1> @icmp_eq_ashr_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sh = ashr <4 x i32> %sa, splat (i32 2)
  %sb = sext <4 x i8> %b to <4 x i32>
  %r = icmp eq <4 x i32> %sh, %sb
  ret <4 x i1> %r
}



define i1 @icmp_ashr_too_large_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sh = ashr i32 %sa, 8
  %sb = sext i8 %b to i32
  %r = icmp eq i32 %sh, %sb
  ret i1 %r
}

