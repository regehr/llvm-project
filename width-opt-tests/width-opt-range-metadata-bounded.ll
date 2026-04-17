

define i1 @range_metadata_fits_icmp(ptr %p, i8 %y) {
  %v = load i32, ptr %p, !range !0
  %zy = zext i8 %y to i32
  %c = icmp ult i32 %zy, %v
  ret i1 %c
}


define i1 @range_metadata_too_large_icmp(ptr %p, i8 %y) {
  %v = load i32, ptr %p, !range !1
  %zy = zext i8 %y to i32
  %c = icmp ult i32 %zy, %v
  ret i1 %c
}

!0 = !{i32 0, i32 256}
!1 = !{i32 0, i32 512}
