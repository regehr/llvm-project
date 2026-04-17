

declare i32 @llvm.ctlz.i32(i32, i1)

define i1 @ctlz_bounded_icmp_nochange(i8 %a, i32 %x) {
  %za = zext i8 %a to i32
  %cl = call i32 @llvm.ctlz.i32(i32 %x, i1 false)
  %c = icmp ult i32 %za, %cl
  ret i1 %c
}

declare i32 @llvm.cttz.i32(i32, i1)

define i1 @cttz_bounded_icmp_nochange(i8 %a, i32 %x) {
  %za = zext i8 %a to i32
  %ct = call i32 @llvm.cttz.i32(i32 %x, i1 false)
  %c = icmp ult i32 %za, %ct
  ret i1 %c
}
