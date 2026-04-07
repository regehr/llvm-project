; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; Freeze does not block narrowing here; both operands are frozen at their
; narrow widths before the compare is shrunk.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i8 %x, i16 %y) {
  %x32 = zext i8 %x to i32
  %y32 = zext i16 %y to i32
  %fx = freeze i32 %x32
  %fy = freeze i32 %y32
  %c = icmp ult i32 %fx, %fy
  ret i1 %c
}

; CHECK-LABEL: define i1 @f(
; CHECK: %[[XF:.*]] = freeze i8 %x
; CHECK: %[[YF:.*]] = freeze i16 %y
; CHECK: %[[ZX:.*]] = zext i8 %[[XF]] to i16
; CHECK: %[[CMP:.*]] = icmp ult i16 %[[ZX]], %[[YF]]
; CHECK-NOT: freeze i32
; CHECK-NOT: icmp ult i32
; CHECK: ret i1 %[[CMP]]

define <4 x i1> @f_vec(<4 x i8> %x, <4 x i16> %y) {
  %x32 = zext <4 x i8> %x to <4 x i32>
  %y32 = zext <4 x i16> %y to <4 x i32>
  %fx = freeze <4 x i32> %x32
  %fy = freeze <4 x i32> %y32
  %c = icmp ult <4 x i32> %fx, %fy
  ret <4 x i1> %c
}

; CHECK-LABEL: define <4 x i1> @f_vec(
; CHECK: %[[XF:.*]] = freeze <4 x i8> %x
; CHECK: %[[YF:.*]] = freeze <4 x i16> %y
; CHECK: %[[ZX:.*]] = zext <4 x i8> %[[XF]] to <4 x i16>
; CHECK: %[[CMP:.*]] = icmp ult <4 x i16> %[[ZX]], %[[YF]]
; CHECK-NOT: freeze <4 x i32>
; CHECK-NOT: icmp ult <4 x i32>
; CHECK: ret <4 x i1> %[[CMP]]
