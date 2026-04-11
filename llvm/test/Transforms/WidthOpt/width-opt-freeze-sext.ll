; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i8 %x) {
  %x16 = sext i8 %x to i16
  %fr = freeze i16 %x16
  ret i16 %fr
}

; CHECK-LABEL: define i16 @f(
; CHECK: %x16 = sext i8 %x to i16
; CHECK: %fr = freeze i16 %x16
; CHECK: ret i16 %fr

define <4 x i16> @f_vec(<4 x i8> %x) {
  %x16 = sext <4 x i8> %x to <4 x i16>
  %fr = freeze <4 x i16> %x16
  ret <4 x i16> %fr
}

; CHECK-LABEL: define <4 x i16> @f_vec(
; CHECK: %x16 = sext <4 x i8> %x to <4 x i16>
; CHECK: %fr = freeze <4 x i16> %x16
; CHECK: ret <4 x i16> %fr
