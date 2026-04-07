; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): NO
; Mixed signed/unsigned extension feeding a signed compare is not narrowed.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i16 %x, i8 %y) {
  %x32 = sext i16 %x to i32
  %y32 = zext i8 %y to i32
  %c = icmp slt i32 %x32, %y32
  ret i1 %c
}

; CHECK-LABEL: define i1 @f(
; CHECK: %[[Y16:.*]] = zext i8 %y to i16
; CHECK: %[[C:.*]] = icmp slt i16 %x, %[[Y16]]
; CHECK: ret i1 %[[C]]

define <4 x i1> @f_vec(<4 x i16> %x, <4 x i8> %y) {
  %x32 = sext <4 x i16> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %c = icmp slt <4 x i32> %x32, %y32
  ret <4 x i1> %c
}

; CHECK-LABEL: define <4 x i1> @f_vec(
; CHECK: %[[Y16:.*]] = zext <4 x i8> %y to <4 x i16>
; CHECK: %[[C:.*]] = icmp slt <4 x i16> %x, %[[Y16]]
; CHECK: ret <4 x i1> %[[C]]
