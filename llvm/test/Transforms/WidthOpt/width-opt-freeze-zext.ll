; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
entry:
  %zx = zext i8 %x to i32
  %fr = freeze i32 %zx
  ret i32 %fr
}

; CHECK-LABEL: define i32 @f(
; CHECK: %zx = zext i8 %x to i32
; CHECK: %fr = freeze i32 %zx
; CHECK: ret i32 %fr

define <4 x i32> @f_vec(<4 x i8> %x) {
entry:
  %zx = zext <4 x i8> %x to <4 x i32>
  %fr = freeze <4 x i32> %zx
  ret <4 x i32> %fr
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %zx = zext <4 x i8> %x to <4 x i32>
; CHECK: %fr = freeze <4 x i32> %zx
; CHECK: ret <4 x i32> %fr
