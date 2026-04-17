

define i1 @sext_sext_ult_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %c = icmp ult i32 %sa, %sb
  ret i1 %c
}

define i1 @sext_sext_uge_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %c = icmp uge i32 %sa, %sb
  ret i1 %c
}
