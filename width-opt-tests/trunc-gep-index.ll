
define ptr @trunc_gep_basic(ptr %base, i32 %n) {
  %x = and i32 %n, 63
  %idx = trunc i32 %x to i8
  %p = getelementptr i8, ptr %base, i8 %idx
  ret ptr %p
}


define void @trunc_gep_multi(ptr %a, ptr %b, i32 %n, ptr %out0, ptr %out1) {
  %x = and i32 %n, 63
  %idx = trunc i32 %x to i8
  %p0 = getelementptr i8, ptr %a, i8 %idx
  %p1 = getelementptr i8, ptr %b, i8 %idx
  %v0 = load i8, ptr %p0
  %v1 = load i8, ptr %p1
  store i8 %v0, ptr %out0
  store i8 %v1, ptr %out1
  ret void
}


define ptr @trunc_gep_nochange_extra_use(ptr %base, i32 %n, ptr %out) {
  %x = and i32 %n, 63
  %idx = trunc i32 %x to i8
  store i8 %idx, ptr %out
  %p = getelementptr i8, ptr %base, i8 %idx
  ret ptr %p
}


define ptr @trunc_gep_nochange_not_bounded(ptr %base, i32 %n) {
  %idx = trunc i32 %n to i8
  %p = getelementptr i8, ptr %base, i8 %idx
  ret ptr %p
}

