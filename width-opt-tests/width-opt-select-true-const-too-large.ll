

define i32 @select_true_const_too_large(i1 %cond, i8 %x) {
  %zx = zext i8 %x to i32
  %sel = select i1 %cond, i32 256, i32 %zx
  ret i32 %sel
}
