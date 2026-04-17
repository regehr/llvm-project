; RUN: opt -passes=width-opt -S %s | FileCheck %s

; trunc(binop(X, zext Y)) -> binop(trunc X, Y) when binop and zext each have
; one use.  Works for any truncation-commuting opcode and for vectors.

declare i32 @llvm.ctpop.i32(i32)
declare <4 x i16> @llvm.ctpop.v4i16(<4 x i16>)

; CHECK-LABEL: @trunc_and_ctpop_zext_rhs(
; CHECK-NEXT:    %cp = call i32 @llvm.ctpop.i32(i32 %x)
; CHECK-NEXT:    %[[T:.+]] = trunc i32 %cp to i8
; CHECK-NEXT:    %[[R:.+]] = and i8 %[[T]], %a
; CHECK-NEXT:    ret i8 %[[R]]
define i8 @trunc_and_ctpop_zext_rhs(i32 %x, i8 %a) {
  %cp = call i32 @llvm.ctpop.i32(i32 %x)
  %za = zext i8 %a to i32
  %and = and i32 %cp, %za
  %t = trunc i32 %and to i8
  ret i8 %t
}

; CHECK-LABEL: @trunc_and_ctpop_zext_lhs(
; CHECK-NEXT:    %cp = call i32 @llvm.ctpop.i32(i32 %x)
; CHECK-NEXT:    %[[T:.+]] = trunc i32 %cp to i8
; CHECK-NEXT:    %[[R:.+]] = and i8 %a, %[[T]]
; CHECK-NEXT:    ret i8 %[[R]]
define i8 @trunc_and_ctpop_zext_lhs(i32 %x, i8 %a) {
  %cp = call i32 @llvm.ctpop.i32(i32 %x)
  %za = zext i8 %a to i32
  %and = and i32 %za, %cp
  %t = trunc i32 %and to i8
  ret i8 %t
}

; CHECK-LABEL: @trunc_or_ctpop_zext(
; CHECK-NEXT:    %cp = call i32 @llvm.ctpop.i32(i32 %x)
; CHECK-NEXT:    %[[T:.+]] = trunc i32 %cp to i8
; CHECK-NEXT:    %[[R:.+]] = or i8 %[[T]], %a
; CHECK-NEXT:    ret i8 %[[R]]
define i8 @trunc_or_ctpop_zext(i32 %x, i8 %a) {
  %cp = call i32 @llvm.ctpop.i32(i32 %x)
  %za = zext i8 %a to i32
  %or = or i32 %cp, %za
  %t = trunc i32 %or to i8
  ret i8 %t
}

; Preserve operand order for non-commutative binops.
; CHECK-LABEL: @trunc_sub_zext_lhs(
; CHECK-NEXT:    %[[T:.+]] = trunc i32 %x to i8
; CHECK-NEXT:    %[[R:.+]] = sub i8 %a, %[[T]]
; CHECK-NEXT:    ret i8 %[[R]]
define i8 @trunc_sub_zext_lhs(i32 %x, i8 %a) {
  %za = zext i8 %a to i32
  %sub = sub i32 %za, %x
  %t = trunc i32 %sub to i8
  ret i8 %t
}

; Vector: same transformation applies element-wise.
; CHECK-LABEL: @trunc_or_zext_vec(
; CHECK-NEXT:    %cp = call <4 x i16> @llvm.ctpop.v4i16(<4 x i16> %x)
; CHECK-NEXT:    %[[T:.+]] = trunc <4 x i16> %cp to <4 x i8>
; CHECK-NEXT:    %[[R:.+]] = or <4 x i8> %[[T]], %a
; CHECK-NEXT:    ret <4 x i8> %[[R]]
define <4 x i8> @trunc_or_zext_vec(<4 x i16> %x, <4 x i8> %a) {
  %cp = call <4 x i16> @llvm.ctpop.v4i16(<4 x i16> %x)
  %za = zext <4 x i8> %a to <4 x i16>
  %or = or <4 x i16> %cp, %za
  %t = trunc <4 x i16> %or to <4 x i8>
  ret <4 x i8> %t
}

; No transform: binop has more than one use.
; CHECK-LABEL: @no_transform_binop_multiuse(
; CHECK:         %za = zext i8 %a to i32
; CHECK:         %and = and i32 %cp, %za
define i8 @no_transform_binop_multiuse(i32 %x, i8 %a, ptr %p) {
  %cp = call i32 @llvm.ctpop.i32(i32 %x)
  %za = zext i8 %a to i32
  %and = and i32 %cp, %za
  store i32 %and, ptr %p
  %t = trunc i32 %and to i8
  ret i8 %t
}

; No transform: zext has more than one use.
; CHECK-LABEL: @no_transform_zext_multiuse(
; CHECK:         %za = zext i8 %a to i32
; CHECK:         %and = and i32 %cp, %za
define i8 @no_transform_zext_multiuse(i32 %x, i8 %a, ptr %p) {
  %cp = call i32 @llvm.ctpop.i32(i32 %x)
  %za = zext i8 %a to i32
  store i32 %za, ptr %p
  %and = and i32 %cp, %za
  %t = trunc i32 %and to i8
  ret i8 %t
}
