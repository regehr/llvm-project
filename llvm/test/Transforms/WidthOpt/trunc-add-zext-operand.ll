; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; A trunc of an add can be pulled through when the narrow operand is already
; represented exactly at the destination width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i8 @f(i8 %x, i32 %y) {
  %ex = zext i8 %x to i32
  %a = add i32 %ex, %y
  %t = trunc i32 %a to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @f(
; CHECK: %[[Y8:.*]] = trunc i32 %y to i8
; CHECK: %[[ADD:.*]] = add i8 %x, %[[Y8]]
; CHECK-NOT: zext i8 %x to i32
; CHECK-NOT: trunc i32 %a to i8
; CHECK: ret i8 %[[ADD]]

define <4 x i8> @f_vec(<4 x i8> %x, <4 x i32> %y) {
  %ex = zext <4 x i8> %x to <4 x i32>
  %a = add <4 x i32> %ex, %y
  %t = trunc <4 x i32> %a to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @f_vec(
; CHECK: %[[Y8:.*]] = trunc <4 x i32> %y to <4 x i8>
; CHECK: %[[ADD:.*]] = add <4 x i8> %x, %[[Y8]]
; CHECK-NOT: zext <4 x i8> %x to <4 x i32>
; CHECK-NOT: trunc <4 x i32> %a to <4 x i8>
; CHECK: ret <4 x i8> %[[ADD]]
