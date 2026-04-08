; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Collapse sext(sext(x: iN→iM): iM→iW) → sext(x: iN→iW).

define i32 @sext_of_sext(i8 %x) {
  %s1 = sext i8 %x to i16
  %s2 = sext i16 %s1 to i32
  ret i32 %s2
}

; CHECK-LABEL: define i32 @sext_of_sext(
; CHECK:      %[[S:.*]] = sext i8 %x to i32
; CHECK-NOT:  sext i8 {{.*}} to i16
; CHECK-NOT:  sext i16
; CHECK:      ret i32 %[[S]]

define <4 x i32> @sext_of_sext_vec(<4 x i8> %x) {
  %s1 = sext <4 x i8> %x to <4 x i16>
  %s2 = sext <4 x i16> %s1 to <4 x i32>
  ret <4 x i32> %s2
}

; CHECK-LABEL: define <4 x i32> @sext_of_sext_vec(
; CHECK:      %[[S:.*]] = sext <4 x i8> %x to <4 x i32>
; CHECK-NOT:  sext <4 x i8> {{.*}} to <4 x i16>
; CHECK-NOT:  sext <4 x i16>
; CHECK:      ret <4 x i32> %[[S]]

; No-change: inner sext has two uses, so it cannot be removed.
define i32 @sext_of_sext_nochange(i8 %x, ptr %p) {
  %s1 = sext i8 %x to i16
  store i16 %s1, ptr %p
  %s2 = sext i16 %s1 to i32
  ret i32 %s2
}

; CHECK-LABEL: define i32 @sext_of_sext_nochange(
; CHECK:      %s1 = sext i8 %x to i16
; CHECK:      %s2 = sext i16 %s1 to i32
; CHECK:      ret i32 %s2
