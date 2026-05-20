declare i32 @llvm.sadd.sat.i32(i32, i32)
declare i32 @llvm.ssub.sat.i32(i32, i32)
declare i32 @llvm.uadd.sat.i32(i32, i32)
declare i32 @llvm.usub.sat.i32(i32, i32)
declare i32 @llvm.sshl.sat.i32(i32, i32)
declare i32 @llvm.ushl.sat.i32(i32, i32)
declare i32 @llvm.smin.i32(i32, i32)
declare i32 @llvm.umin.i32(i32, i32)

define i32 @saturating(i32 %a, i32 %b) {
  %sadd = call i32 @llvm.sadd.sat.i32(i32 %b, i32 %a)
  %ssub = call i32 @llvm.ssub.sat.i32(i32 %a, i32 %b)
  %uadd = call i32 @llvm.uadd.sat.i32(i32 %b, i32 %a)
  %usub = call i32 @llvm.usub.sat.i32(i32 %a, i32 %b)
  %sshl = call i32 @llvm.sshl.sat.i32(i32 %a, i32 %b)
  %ushl = call i32 @llvm.ushl.sat.i32(i32 %a, i32 %b)
  %smin = call i32 @llvm.smin.i32(i32 %b, i32 %a)
  %umin = call i32 @llvm.umin.i32(i32 %b, i32 %a)
  %x0 = add i32 %sadd, %ssub
  %x1 = add i32 %uadd, %usub
  %x2 = add i32 %sshl, %ushl
  %x3 = add i32 %smin, %umin
  %x4 = add i32 %x0, %x1
  %x5 = add i32 %x2, %x3
  %ret = add i32 %x4, %x5
  ret i32 %ret
}
