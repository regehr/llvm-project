


define i32 @sub_widen_multi_use(i32 %x, ptr %p) {
  %xb = and i32 %x, 127
  %tx = trunc i32 %xb to i8
  %sub = sub i8 %tx, 5
  store i8 %sub, ptr %p
  %wz = zext nneg i8 %sub to i32
  ret i32 %wz
}


define i32 @sub_widen_neg_const(i32 %x) {
  %xb = and i32 %x, 127
  %tx = trunc i32 %xb to i8
  %sub = sub i8 %tx, -5
  %wz = zext nneg i8 %sub to i32
  ret i32 %wz
}


define i32 @sub_widen_wrong_src_width(i16 %x) {
  %xb = and i16 %x, 127
  %tx = trunc i16 %xb to i8
  %sub = sub i8 %tx, 5
  %wz = zext nneg i8 %sub to i32
  ret i32 %wz
}


define i32 @sub_widen_c_too_large(i32 %x) {
  %xb = and i32 %x, 127
  %tx = trunc i32 %xb to i8
  %sub = sub i8 %tx, 128
  %wz = zext nneg i8 %sub to i32
  ret i32 %wz
}
