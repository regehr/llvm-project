define i16 @opt13(i16 %x) {
  %a = sub i16 32767, %x
  %b = and i16 %x, %a
  ret i16 %b
}
