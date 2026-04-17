; RUN: opt -passes=width-opt -S %s | FileCheck %s

declare i32 @llvm.ctpop.i32(i32)
declare <4 x i16> @llvm.ctpop.v4i16(<4 x i16>)

; trunc(trunc(X)) -> trunc(X): fold two consecutive truncations into one.

; CHECK-LABEL: @trunc_trunc_scalar(
; CHECK-NEXT:    %[[R:.+]] = trunc i32 %a to i8
; CHECK-NEXT:    ret i8 %[[R]]
define i8 @trunc_trunc_scalar(i32 %a) {
  %t16 = trunc i32 %a to i16
  %t8 = trunc i16 %t16 to i8
  ret i8 %t8
}

; CHECK-LABEL: @trunc_trunc_vec(
; CHECK-NEXT:    %[[R:.+]] = trunc <4 x i32> %a to <4 x i8>
; CHECK-NEXT:    ret <4 x i8> %[[R]]
define <4 x i8> @trunc_trunc_vec(<4 x i32> %a) {
  %t16 = trunc <4 x i32> %a to <4 x i16>
  %t8 = trunc <4 x i16> %t16 to <4 x i8>
  ret <4 x i8> %t8
}

; No transform: inner trunc has more than one use.
; CHECK-LABEL: @no_transform_inner_multiuse(
; CHECK:         %t16 = trunc i32 %a to i16
; CHECK:         %t8 = trunc i16 %t16 to i8
define i8 @no_transform_inner_multiuse(i32 %a, ptr %p) {
  %t16 = trunc i32 %a to i16
  store i16 %t16, ptr %p
  %t8 = trunc i16 %t16 to i8
  ret i8 %t8
}

; trunc(ctpop(zext(X))) -> ctpop(X): narrow ctpop through a zext.
; The upper bits of zext(X) are zero so they contribute nothing to the count.

; CHECK-LABEL: @ctpop_narrow_scalar(
; CHECK-NEXT:    %[[R:.+]] = call i8 @llvm.ctpop.i8(i8 %a)
; CHECK-NEXT:    ret i8 %[[R]]
define i8 @ctpop_narrow_scalar(i8 %a) {
  %wide = zext i8 %a to i32
  %pop = call i32 @llvm.ctpop.i32(i32 %wide)
  %tr = trunc i32 %pop to i8
  ret i8 %tr
}

; CHECK-LABEL: @ctpop_narrow_vec(
; CHECK-NEXT:    %[[R:.+]] = call <4 x i8> @llvm.ctpop.v4i8(<4 x i8> %a)
; CHECK-NEXT:    ret <4 x i8> %[[R]]
define <4 x i8> @ctpop_narrow_vec(<4 x i8> %a) {
  %wide = zext <4 x i8> %a to <4 x i16>
  %pop = call <4 x i16> @llvm.ctpop.v4i16(<4 x i16> %wide)
  %tr = trunc <4 x i16> %pop to <4 x i8>
  ret <4 x i8> %tr
}

; No transform: ctpop has more than one use.
; CHECK-LABEL: @no_transform_ctpop_multiuse(
; CHECK:         %wide = zext i8 %a to i32
; CHECK:         %pop = call i32 @llvm.ctpop.i32(i32 %wide)
define i8 @no_transform_ctpop_multiuse(i8 %a, ptr %p) {
  %wide = zext i8 %a to i32
  %pop = call i32 @llvm.ctpop.i32(i32 %wide)
  store i32 %pop, ptr %p
  %tr = trunc i32 %pop to i8
  ret i8 %tr
}

; No transform: zext has more than one use.
; CHECK-LABEL: @no_transform_zext_multiuse(
; CHECK:         %wide = zext i8 %a to i32
; CHECK:         %pop = call i32 @llvm.ctpop.i32(i32 %wide)
define i8 @no_transform_zext_multiuse(i8 %a, ptr %p) {
  %wide = zext i8 %a to i32
  store i32 %wide, ptr %p
  %pop = call i32 @llvm.ctpop.i32(i32 %wide)
  %tr = trunc i32 %pop to i8
  ret i8 %tr
}
