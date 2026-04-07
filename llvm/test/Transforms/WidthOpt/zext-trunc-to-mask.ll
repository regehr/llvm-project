; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; InstCombine turns zext(trunc(x)) into a low-bit mask at the destination width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i64 %x) {
entry:
  %t = trunc i64 %x to i16
  %e = zext i16 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[TR:.*]] = trunc i64 %x to i32
; CHECK: %[[MASK:.*]] = and i32 %[[TR]], 65535
; CHECK-NOT: trunc i64 %x to i16
; CHECK-NOT: zext i16
; CHECK: ret i32 %[[MASK]]

define <4 x i32> @f_vec(<4 x i64> %x) {
entry:
  %t = trunc <4 x i64> %x to <4 x i16>
  %e = zext <4 x i16> %t to <4 x i32>
  ret <4 x i32> %e
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[TR:.*]] = trunc <4 x i64> %x to <4 x i32>
; CHECK: %[[MASK:.*]] = and <4 x i32> %[[TR]], splat (i32 65535)
; CHECK-NOT: trunc <4 x i64> %x to <4 x i16>
; CHECK-NOT: zext <4 x i16>
; CHECK: ret <4 x i32> %[[MASK]]
