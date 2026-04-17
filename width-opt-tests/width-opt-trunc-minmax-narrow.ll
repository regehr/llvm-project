

declare i32 @llvm.umin.i32(i32, i32)
declare i32 @llvm.umax.i32(i32, i32)
declare i32 @llvm.smin.i32(i32, i32)
declare i32 @llvm.smax.i32(i32, i32)
declare i32 @llvm.abs.i32(i32, i1)

define i8 @trunc_umin_zext(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %m = call i32 @llvm.umin.i32(i32 %a32, i32 %b32)
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @trunc_umax_zext(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %m = call i32 @llvm.umax.i32(i32 %a32, i32 %b32)
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @trunc_smin_sext(i8 %a, i8 %b) {
  %a32 = sext i8 %a to i32
  %b32 = sext i8 %b to i32
  %m = call i32 @llvm.smin.i32(i32 %a32, i32 %b32)
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @trunc_smax_sext(i8 %a, i8 %b) {
  %a32 = sext i8 %a to i32
  %b32 = sext i8 %b to i32
  %m = call i32 @llvm.smax.i32(i32 %a32, i32 %b32)
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @trunc_abs_sext(i8 %a) {
  %a32 = sext i8 %a to i32
  %abs = call i32 @llvm.abs.i32(i32 %a32, i1 false)
  %t = trunc i32 %abs to i8
  ret i8 %t
}


define i8 @trunc_smin_unbounded_nochange(i32 %x) {
  %m = call i32 @llvm.smin.i32(i32 %x, i32 0)
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @trunc_smax_unbounded_nochange(i32 %x) {
  %m = call i32 @llvm.smax.i32(i32 %x, i32 0)
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @trunc_smin_sext_const(i8 %a) {
  %a32 = sext i8 %a to i32
  %m = call i32 @llvm.smin.i32(i32 %a32, i32 42)
  %t = trunc i32 %m to i8
  ret i8 %t
}


define i8 @trunc_umin_zext_const(i8 %a) {
  %a32 = zext i8 %a to i32
  %m = call i32 @llvm.umin.i32(i32 %a32, i32 200)
  %t = trunc i32 %m to i8
  ret i8 %t
}


define <4 x i8> @trunc_umin_zext_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %m = call <4 x i32> @llvm.umin.v4i32(<4 x i32> %a32, <4 x i32> %b32)
  %t = trunc <4 x i32> %m to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i8> @trunc_smin_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %b32 = sext <4 x i8> %b to <4 x i32>
  %m = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %a32, <4 x i32> %b32)
  %t = trunc <4 x i32> %m to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i8> @trunc_abs_sext_vec(<4 x i8> %a) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %abs = call <4 x i32> @llvm.abs.v4i32(<4 x i32> %a32, i1 false)
  %t = trunc <4 x i32> %abs to <4 x i8>
  ret <4 x i8> %t
}


declare <4 x i32> @llvm.umin.v4i32(<4 x i32>, <4 x i32>)
declare <4 x i32> @llvm.smin.v4i32(<4 x i32>, <4 x i32>)
declare <4 x i32> @llvm.abs.v4i32(<4 x i32>, i1)
