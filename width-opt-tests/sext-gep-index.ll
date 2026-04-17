
define ptr @sext_gep_single(i8 %x, ptr %base) {
  %sx = sext i8 %x to i32
  %p = getelementptr i8, ptr %base, i32 %sx
  ret ptr %p
}


define void @sext_gep_multi(i8 %x, ptr %a, ptr %b, ptr %out0, ptr %out1) {
  %sx = sext i8 %x to i32
  %p0 = getelementptr i8, ptr %a, i32 %sx
  %p1 = getelementptr i8, ptr %b, i32 %sx
  %v0 = load i8, ptr %p0
  %v1 = load i8, ptr %p1
  store i8 %v0, ptr %out0
  store i8 %v1, ptr %out1
  ret void
}


define ptr @sext_gep_nochange_extra_use(i8 %x, ptr %base, ptr %out) {
  %sx = sext i8 %x to i32
  store i32 %sx, ptr %out
  %p = getelementptr i8, ptr %base, i32 %sx
  ret ptr %p
}

