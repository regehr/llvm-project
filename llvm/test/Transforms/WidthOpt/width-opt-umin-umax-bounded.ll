; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; umax then umin with constants that fit in i8 form a clamp, all narrowable.

declare i32 @llvm.umin.i32(i32, i32)
declare i32 @llvm.umax.i32(i32, i32)

define i8 @clamp_umax_umin(i8 %a) {
  %a32 = zext i8 %a to i32
  %lo = call i32 @llvm.umax.i32(i32 %a32, i32 10)
  %hi = call i32 @llvm.umin.i32(i32 %lo, i32 200)
  %t = trunc i32 %hi to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @clamp_umax_umin(
; CHECK-NOT: zext i8
; CHECK-NOT: umax i32
; CHECK-NOT: umin i32
; CHECK: call i8 @llvm.umax.i8
; CHECK: call i8 @llvm.umin.i8
; CHECK: ret i8

define <4 x i8> @clamp_umax_umin_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %lo = call <4 x i32> @llvm.umax.v4i32(<4 x i32> %a32, <4 x i32> <i32 10, i32 10, i32 10, i32 10>)
  %hi = call <4 x i32> @llvm.umin.v4i32(<4 x i32> %lo, <4 x i32> <i32 200, i32 200, i32 200, i32 200>)
  %t = trunc <4 x i32> %hi to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @clamp_umax_umin_vec(
; CHECK-NOT: zext <4 x i8>
; CHECK-NOT: umax <4 x i32>
; CHECK-NOT: umin <4 x i32>
; CHECK: call <4 x i8> @llvm.umax.v4i8
; CHECK: call <4 x i8> @llvm.umin.v4i8
; CHECK: ret <4 x i8>

declare <4 x i32> @llvm.umin.v4i32(<4 x i32>, <4 x i32>)
declare <4 x i32> @llvm.umax.v4i32(<4 x i32>, <4 x i32>)
