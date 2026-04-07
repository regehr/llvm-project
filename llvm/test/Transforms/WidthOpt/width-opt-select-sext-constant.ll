; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r = select i1 %c, i32 %sx, i32 -1
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[N:.*]] = select i1 %c, i8 %x, i8 -1
; CHECK: %[[W:.*]] = sext i8 %[[N]] to i32
; CHECK: ret i32 %[[W]]

define <4 x i32> @f_vec(<4 x i1> %c, <4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r = select <4 x i1> %c, <4 x i32> %sx, <4 x i32> splat (i32 -1)
  ret <4 x i32> %r
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[N:.*]] = select <4 x i1> %c, <4 x i8> %x, <4 x i8> splat (i8 -1)
; CHECK: %[[W:.*]] = sext <4 x i8> %[[N]] to <4 x i32>
; CHECK: ret <4 x i32> %[[W]]
