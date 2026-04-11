; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i8 @f(i16 %x) {
  %x8 = trunc i16 %x to i8
  %fr = freeze i8 %x8
  ret i8 %fr
}

; CHECK-LABEL: define i8 @f(
; CHECK: %x8 = trunc i16 %x to i8
; CHECK: %fr = freeze i8 %x8
; CHECK: ret i8 %fr

define <4 x i8> @f_vec(<4 x i16> %x) {
  %x8 = trunc <4 x i16> %x to <4 x i8>
  %fr = freeze <4 x i8> %x8
  ret <4 x i8> %fr
}

; CHECK-LABEL: define <4 x i8> @f_vec(
; CHECK: %x8 = trunc <4 x i16> %x to <4 x i8>
; CHECK: %fr = freeze <4 x i8> %x8
; CHECK: ret <4 x i8> %fr
