
define i8 @transitive_sext_inner_multiuse(i8 %a, i8 %b, ptr %p) {
  %a16 = sext i8 %a to i16
  store i16 %a16, ptr %p
  %a32 = sext i16 %a16 to i32
  %b32 = sext i8 %b to i32
  %add = add i32 %a32, %b32
  %t = trunc i32 %add to i8
  ret i8 %t
}

