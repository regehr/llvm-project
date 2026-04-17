

define i8 @trunc_sdiv_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %div = sdiv i32 %sa, %sb
  %t = trunc i32 %div to i8
  ret i8 %t
}


define i8 @trunc_srem_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %rem = srem i32 %sa, %sb
  %t = trunc i32 %rem to i8
  ret i8 %t
}
