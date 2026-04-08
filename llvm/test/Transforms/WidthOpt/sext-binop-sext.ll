; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Sink sext through bitwise binops when both operands are sext from the same
; source type and each has exactly one use:
;   and/or/xor(sext(a), sext(b)) → sext(and/or/xor(a, b))

define i32 @and_sext_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %r = and i32 %sa, %sb
  ret i32 %r
}

; CHECK-LABEL: define i32 @and_sext_sext(
; CHECK:      %[[R:.*]] = and i8 %a, %b
; CHECK:      %[[S:.*]] = sext i8 %[[R]] to i32
; CHECK:      ret i32 %[[S]]

define i32 @or_sext_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %r = or i32 %sa, %sb
  ret i32 %r
}

; CHECK-LABEL: define i32 @or_sext_sext(
; CHECK:      %[[R:.*]] = or i8 %a, %b
; CHECK:      %[[S:.*]] = sext i8 %[[R]] to i32
; CHECK:      ret i32 %[[S]]

define i32 @xor_sext_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %r = xor i32 %sa, %sb
  ret i32 %r
}

; CHECK-LABEL: define i32 @xor_sext_sext(
; CHECK:      %[[R:.*]] = xor i8 %a, %b
; CHECK:      %[[S:.*]] = sext i8 %[[R]] to i32
; CHECK:      ret i32 %[[S]]

define <4 x i32> @and_sext_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %r = and <4 x i32> %sa, %sb
  ret <4 x i32> %r
}

; CHECK-LABEL: define <4 x i32> @and_sext_sext_vec(
; CHECK:      %[[R:.*]] = and <4 x i8> %a, %b
; CHECK:      %[[S:.*]] = sext <4 x i8> %[[R]] to <4 x i32>
; CHECK:      ret <4 x i32> %[[S]]

; No-change: one sext has two uses, so removing it would not be profitable.
define i32 @and_sext_sext_nochange(i8 %a, i8 %b, ptr %p) {
  %sa = sext i8 %a to i32
  store i32 %sa, ptr %p
  %sb = sext i8 %b to i32
  %r = and i32 %sa, %sb
  ret i32 %r
}

; CHECK-LABEL: define i32 @and_sext_sext_nochange(
; CHECK:      %sa = sext i8 %a to i32
; CHECK:      %sb = sext i8 %b to i32
; CHECK:      %r = and i32 %sa, %sb
; CHECK:      ret i32 %r
