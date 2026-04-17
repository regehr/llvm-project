

define i16 @dont_widen_wrapped_sub(i16 %x) {
entry:
  %xb = and i16 %x, 255
  %t = trunc i16 %xb to i8
  %sub = sub i8 0, %t
  %z = zext nneg i8 %sub to i16
  ret i16 %z
}


define i16 @widen_nonneg_sub(i16 %x) {
entry:
  %xb = and i16 %x, 127
  %t = trunc i16 %xb to i8
  %sub = sub i8 %t, 5
  %z = zext nneg i8 %sub to i16
  ret i16 %z
}

