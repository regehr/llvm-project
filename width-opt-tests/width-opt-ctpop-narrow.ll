

declare i32 @llvm.ctpop.i32(i32)

define i8 @ctpop_zext_trunc(i8 %a) {
  %a32 = zext i8 %a to i32
  %p = call i32 @llvm.ctpop.i32(i32 %a32)
  %t = trunc i32 %p to i8
  ret i8 %t
}


define <4 x i8> @ctpop_zext_trunc_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %p = call <4 x i32> @llvm.ctpop.v4i32(<4 x i32> %a32)
  %t = trunc <4 x i32> %p to <4 x i8>
  ret <4 x i8> %t
}


declare <4 x i32> @llvm.ctpop.v4i32(<4 x i32>)
