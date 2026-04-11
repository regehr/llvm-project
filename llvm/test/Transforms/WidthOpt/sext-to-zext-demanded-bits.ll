; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; The sign extension could be weakened to zext because the mask only demands
; low bits, but that rewrite is only break-even locally and is now disabled.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %sx = sext i8 %x to i32
  %r = and i32 %sx, 255
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[SX:.*]] = sext i8 %x to i32
; CHECK: %[[R:.*]] = and i32 %[[SX]], 255
; CHECK: ret i32 %[[R]]

define <4 x i32> @f_vec(<4 x i8> %x) {
  %sx = sext <4 x i8> %x to <4 x i32>
  %r = and <4 x i32> %sx, splat (i32 255)
  ret <4 x i32> %r
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[SX:.*]] = sext <4 x i8> %x to <4 x i32>
; CHECK: %[[R:.*]] = and <4 x i32> %[[SX]], splat (i32 255)
; CHECK: ret <4 x i32> %[[R]]
