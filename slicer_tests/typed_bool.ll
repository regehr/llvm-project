define i32 @typed_bool(i32 %a, i32 %b, i32 %c) {
  %slt = icmp slt i32 %a, %b
  %sgt = icmp sgt i32 %a, %b
  %eq = icmp eq i32 %a, %b
  %sel0 = select i1 %slt, i32 %a, i32 %c
  %sel1 = select i1 %sgt, i32 %b, i32 %c
  %zext = zext i1 %eq to i32
  %trunc = trunc i32 %c to i1
  %sext = sext i1 %trunc to i32
  %bool_and = and i1 %slt, %eq
  %ignored = zext i1 %bool_and to i32
  %sum0 = add i32 %sel0, %sel1
  %sum1 = add i32 %sum0, %zext
  %sum2 = add i32 %sum1, %sext
  %ret = add i32 %sum2, %ignored
  ret i32 %ret
}
