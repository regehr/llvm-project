
define i64 @f(i8 %x, i8 %y) {
  %a = xor i8 %x, %y
  %w32 = zext i8 %a to i32
  %u32 = add i32 %w32, 1
  %w64 = zext i8 %a to i64
  %u64 = add i64 %w64, 2
  %u32_64 = zext i32 %u32 to i64
  %r = add i64 %u32_64, %u64
  ret i64 %r
}

