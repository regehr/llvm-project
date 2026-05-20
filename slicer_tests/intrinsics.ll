declare i32 @llvm.ctlz.i32(i32, i1 immarg)
declare i32 @llvm.cttz.i32(i32, i1 immarg)
declare i32 @llvm.ctpop.i32(i32)
declare i32 @llvm.abs.i32(i32, i1 immarg)
declare i32 @llvm.smax.i32(i32, i32)

define i32 @intrinsics(i32 %a, i32 %b) {
  %ctlz = call i32 @llvm.ctlz.i32(i32 %a, i1 false)
  %ctlz_undef = call i32 @llvm.ctlz.i32(i32 %a, i1 true)
  %cttz = call i32 @llvm.cttz.i32(i32 %a, i1 false)
  %cttz_undef = call i32 @llvm.cttz.i32(i32 %a, i1 true)
  %pop = call i32 @llvm.ctpop.i32(i32 %a)
  %abs = call i32 @llvm.abs.i32(i32 %a, i1 false)
  %abs_undef = call i32 @llvm.abs.i32(i32 %a, i1 true)
  %max = call i32 @llvm.smax.i32(i32 %b, i32 %a)
  %x0 = add i32 %ctlz, %ctlz_undef
  %x1 = add i32 %cttz, %cttz_undef
  %x2 = add i32 %pop, %abs
  %x3 = add i32 %abs_undef, %max
  %x4 = add i32 %x0, %x1
  %x5 = add i32 %x2, %x3
  %ret = add i32 %x4, %x5
  ret i32 %ret
}
