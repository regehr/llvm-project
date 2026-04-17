

define i32 @and_sext_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %r = and i32 %sa, %sb
  ret i32 %r
}


define i32 @or_sext_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %r = or i32 %sa, %sb
  ret i32 %r
}


define i32 @xor_sext_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %r = xor i32 %sa, %sb
  ret i32 %r
}


define <4 x i32> @and_sext_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %r = and <4 x i32> %sa, %sb
  ret <4 x i32> %r
}


define i32 @and_sext_sext_nochange(i8 %a, i8 %b, ptr %p) {
  %sa = sext i8 %a to i32
  store i32 %sa, ptr %p
  %sb = sext i8 %b to i32
  %r = and i32 %sa, %sb
  ret i32 %r
}

