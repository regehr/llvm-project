
define i32 @ptrtoint_sub_no_increase(ptr %p1, ptr %p2) {
  %p1i = ptrtoint ptr %p1 to i64
  %p2i = ptrtoint ptr %p2 to i64
  %diff = sub i64 %p1i, %p2i
  %tr = trunc i64 %diff to i32
  ret i32 %tr
}
