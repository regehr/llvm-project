
declare void @use64(i64)

define void @f(i32 %n) {
entry:
  %cmp = icmp sgt i32 %n, -1
  br i1 %cmp, label %bb, label %exit

bb:
  %ext.wide = sext i32 %n to i64
  call void @use64(i64 %ext.wide)
  %ext = trunc i64 %ext.wide to i32
  br label %exit

exit:
  ret void
}

