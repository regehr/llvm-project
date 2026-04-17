
declare i32 @llvm.smin.i32(i32, i32)
declare i32 @llvm.smax.i32(i32, i32)
declare i32 @llvm.abs.i32(i32, i1)

define i8 @smin_narrow(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %m = call i32 @llvm.smin.i32(i32 %sa, i32 %sb)
  %tr = trunc i32 %m to i8
  ret i8 %tr
}

define i8 @smax_narrow(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %m = call i32 @llvm.smax.i32(i32 %sa, i32 %sb)
  %tr = trunc i32 %m to i8
  ret i8 %tr
}

define i8 @abs_narrow(i8 %a) {
  %sa = sext i8 %a to i32
  %m = call i32 @llvm.abs.i32(i32 %sa, i1 false)
  %tr = trunc i32 %m to i8
  ret i8 %tr
}
