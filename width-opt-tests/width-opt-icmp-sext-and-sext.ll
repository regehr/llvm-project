

define i1 @sext_and_sext_slt_multiuse(i8 %a, i8 %b, i8 %c, ptr %p) {
  %sa = sext i8 %a to i32
  store i32 %sa, ptr %p            ; extra use prevents tryShrinkSExtBitwiseBinop
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %and = and i32 %sa, %sb
  %cmp = icmp slt i32 %and, %sc
  ret i1 %cmp
}

define i1 @sext_or_sext_sgt_multiuse(i8 %a, i8 %b, i8 %c, ptr %p) {
  %sa = sext i8 %a to i32
  store i32 %sa, ptr %p
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %or = or i32 %sa, %sb
  %cmp = icmp sgt i32 %or, %sc
  ret i1 %cmp
}
