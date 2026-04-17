

define i8 @shared_sext_producer(i1 %cond, i8 %a) {
  %ext = sext i8 %a to i32
  ; Both arms use the same ext — shared producer.
  %sel = select i1 %cond, i32 %ext, i32 %ext
  %tr = trunc i32 %sel to i8
  ret i8 %tr
}


define i8 @shared_zext_producer(i1 %cond, i8 %a) {
  %ext = zext i8 %a to i32
  %sel = select i1 %cond, i32 %ext, i32 %ext
  %tr = trunc i32 %sel to i8
  ret i8 %tr
}

