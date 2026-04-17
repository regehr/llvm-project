

declare i32 @llvm.smin.i32(i32, i32)
declare i32 @llvm.smax.i32(i32, i32)

define i8 @smin_and_sext(i8 %a, i8 %b, i8 %c) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %ab = and i32 %sa, %sb
  %r = call i32 @llvm.smin.i32(i32 %ab, i32 %sc)
  %t = trunc i32 %r to i8
  ret i8 %t
}


define i8 @smax_or_sext(i8 %a, i8 %b, i8 %c) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %ab = or i32 %sa, %sb
  %r = call i32 @llvm.smax.i32(i32 %ab, i32 %sc)
  %t = trunc i32 %r to i8
  ret i8 %t
}


define i8 @smin_ashr_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sh = ashr i32 %sa, 3
  %sb = sext i8 %b to i32
  %r = call i32 @llvm.smin.i32(i32 %sh, i32 %sb)
  %t = trunc i32 %r to i8
  ret i8 %t
}



declare <4 x i32> @llvm.smin.v4i32(<4 x i32>, <4 x i32>)
declare <4 x i32> @llvm.smax.v4i32(<4 x i32>, <4 x i32>)

define <4 x i8> @smin_and_sext_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %sc = sext <4 x i8> %c to <4 x i32>
  %ab = and <4 x i32> %sa, %sb
  %r = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %ab, <4 x i32> %sc)
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i8> @smax_or_sext_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %sc = sext <4 x i8> %c to <4 x i32>
  %ab = or <4 x i32> %sa, %sb
  %r = call <4 x i32> @llvm.smax.v4i32(<4 x i32> %ab, <4 x i32> %sc)
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}


define <4 x i8> @smin_ashr_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sh = ashr <4 x i32> %sa, splat (i32 3)
  %sb = sext <4 x i8> %b to <4 x i32>
  %r = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %sh, <4 x i32> %sb)
  %t = trunc <4 x i32> %r to <4 x i8>
  ret <4 x i8> %t
}

