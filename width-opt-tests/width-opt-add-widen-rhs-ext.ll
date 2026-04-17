
define i32 @f(i8 %x) {
  %x16 = zext i8 %x to i16
  %add16 = add i16 7, %x16
  %wide = zext i16 %add16 to i32
  ret i32 %wide
}

