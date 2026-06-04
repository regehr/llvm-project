declare i32 @llvm.smul.fix.sat.i32(i32, i32, i32 immarg)
declare i32 @llvm.umul.fix.sat.i32(i32, i32, i32 immarg)

define i32 @fixed_mul_scale(i32 %a, i32 %b) {
  %smul0 = call i32 @llvm.smul.fix.sat.i32(i32 %a, i32 %b, i32 0)
  %umul0 = call i32 @llvm.umul.fix.sat.i32(i32 %b, i32 %a, i32 0)
  %smul1 = call i32 @llvm.smul.fix.sat.i32(i32 %a, i32 %b, i32 1)
  %x0 = add i32 %smul0, %umul0
  %ret = add i32 %x0, %smul1
  ret i32 %ret
}
