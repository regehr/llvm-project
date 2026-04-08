; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): NO
; trunc(shl(zext %x, k)) is NOT narrowed when k >= TargetWidth: narrowing would
; produce a shl with an amount >= the type width, which is undefined behaviour.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i8 @shl_const_too_large(i8 %x) {
  %ext = zext i8 %x to i32
  %shl = shl i32 %ext, 8
  %trunc = trunc i32 %shl to i8
  ret i8 %trunc
}

; CHECK-LABEL: define i8 @shl_const_too_large(
; CHECK: %ext = zext i8 %x to i32
; CHECK: %shl = shl i32 %ext, 8
; CHECK: %trunc = trunc i32 %shl to i8

define <4 x i8> @shl_const_too_large_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %shl = shl <4 x i32> %ext, splat (i32 8)
  %trunc = trunc <4 x i32> %shl to <4 x i8>
  ret <4 x i8> %trunc
}

; CHECK-LABEL: define <4 x i8> @shl_const_too_large_vec(
; CHECK: zext <4 x i8> %x to <4 x i32>
; CHECK: shl <4 x i32>
; CHECK: trunc <4 x i32>
