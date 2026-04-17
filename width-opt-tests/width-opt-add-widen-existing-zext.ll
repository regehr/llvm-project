
define i32 @f(i8 %x) {
  %x16 = zext i8 %x to i16
  %x32 = zext i8 %x to i32
  %add16 = add i16 %x16, 7
  %wide = zext i16 %add16 to i32
  %sum = add i32 %wide, %x32
  ret i32 %sum
}

