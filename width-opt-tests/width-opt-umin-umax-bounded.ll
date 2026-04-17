

declare i32 @llvm.umin.i32(i32, i32)
declare i32 @llvm.umax.i32(i32, i32)

define i8 @clamp_umax_umin(i8 %a) {
  %a32 = zext i8 %a to i32
  %lo = call i32 @llvm.umax.i32(i32 %a32, i32 10)
  %hi = call i32 @llvm.umin.i32(i32 %lo, i32 200)
  %t = trunc i32 %hi to i8
  ret i8 %t
}


define <4 x i8> @clamp_umax_umin_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %lo = call <4 x i32> @llvm.umax.v4i32(<4 x i32> %a32, <4 x i32> <i32 10, i32 10, i32 10, i32 10>)
  %hi = call <4 x i32> @llvm.umin.v4i32(<4 x i32> %lo, <4 x i32> <i32 200, i32 200, i32 200, i32 200>)
  %t = trunc <4 x i32> %hi to <4 x i8>
  ret <4 x i8> %t
}


declare <4 x i32> @llvm.umin.v4i32(<4 x i32>, <4 x i32>)
declare <4 x i32> @llvm.umax.v4i32(<4 x i32>, <4 x i32>)
