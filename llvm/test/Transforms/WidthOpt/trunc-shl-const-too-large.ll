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
