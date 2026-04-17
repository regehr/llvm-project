
define i16 @udiv_mixed_narrow_widths_trunc(i16 %x, i8 %y) {
entry:
  %x32 = zext i16 %x to i32
  %y32 = zext i8 %y to i32
  %d = udiv i32 %x32, %y32
  %t = trunc i32 %d to i16
  ret i16 %t
}


declare void @llvm.assume(i1)

define i16 @udiv_range_and_zext(i16 %x, i32 %y) {
entry:
  %x32 = zext i16 %x to i32
  %cy = icmp ult i32 %y, 65536
  call void @llvm.assume(i1 %cy)
  %d = udiv i32 %x32, %y
  %t = trunc i32 %d to i16
  ret i16 %t
}

