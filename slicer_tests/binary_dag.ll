define i32 @binary_dag(i32 %a, i32 %b, i32 %c) {
  %sum = add i32 %a, %b
  %shift = ashr exact i32 %sum, %c
  ret i32 %shift
}
