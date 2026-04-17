


define i8 @trunc_of_trunc_i32_to_i16_to_i8(i32 %a) {
  %t16 = trunc i32 %a to i16
  %t8 = trunc i16 %t16 to i8
  ret i8 %t8
}


define i1 @trunc_lshr_to_i1(i32 %x) {
  %sh = lshr i32 %x, 3
  %tr = trunc i32 %sh to i1
  ret i1 %tr
}


define i1 @trunc_to_i1_zero_bounded(i1 %flag) {
  %wide = zext i1 %flag to i32
  %tr = trunc i32 %wide to i1
  ret i1 %tr
}


define i1 @trunc_nuw_to_i1(i32 %x) {
  %tr = trunc nuw i32 %x to i1
  ret i1 %tr
}


define i8 @narrow_udiv_zext_bounded(i8 %a, i8 %b) {
  %wa = zext i8 %a to i32
  %wb = zext i8 %b to i32
  %div = udiv i32 %wa, %wb
  %tr = trunc i32 %div to i8
  ret i8 %tr
}


define i8 @trunc_ctpop_zext(i8 %a) {
  %wide = zext i8 %a to i32
  %pop = call i32 @llvm.ctpop.i32(i32 %wide)
  %tr = trunc i32 %pop to i8
  ret i8 %tr
}

declare i32 @llvm.ctpop.i32(i32)
