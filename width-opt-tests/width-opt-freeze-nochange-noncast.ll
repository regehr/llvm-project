
define i16 @f(i16 %x, i16 %y) {
  %sum = add i16 %x, %y
  %fr = freeze i16 %sum
  ret i16 %fr
}

