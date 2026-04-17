


define i8 @abs_add_not_sext_bounded(i8 %a) {
  %wa = sext i8 %a to i32
  %add = add i32 %wa, 100
  %abs = call i32 @llvm.abs.i32(i32 %add, i1 false)
  %tr = trunc i32 %abs to i8
  ret i8 %tr
}



define i8 @abs_sext_bounded(i8 %a) {
  %wa = sext i8 %a to i32
  %abs = call i32 @llvm.abs.i32(i32 %wa, i1 false)
  %tr = trunc i32 %abs to i8
  ret i8 %tr
}


declare i32 @llvm.abs.i32(i32, i1 immarg)

define <4 x i8> @abs_add_not_sext_bounded_vec(<4 x i8> %a) {
  %wa = sext <4 x i8> %a to <4 x i32>
  %add = add <4 x i32> %wa, splat (i32 100)
  %abs = call <4 x i32> @llvm.abs.v4i32(<4 x i32> %add, i1 false)
  %tr = trunc <4 x i32> %abs to <4 x i8>
  ret <4 x i8> %tr
}


define <4 x i8> @abs_sext_bounded_vec(<4 x i8> %a) {
  %wa = sext <4 x i8> %a to <4 x i32>
  %abs = call <4 x i32> @llvm.abs.v4i32(<4 x i32> %wa, i1 false)
  %tr = trunc <4 x i32> %abs to <4 x i8>
  ret <4 x i8> %tr
}


declare <4 x i32> @llvm.abs.v4i32(<4 x i32>, i1)
