; collectTruncRootedValueCost: and(X, C) where C has no bits in the low
; TargetWidth positions (lines 3744-3756 in WidthOpt.cpp).
; The and contributes 0 to the low bits, enabling identity folds.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(or(and(X, 0xFF00), zext(y: i8→i32)), i8) = y
; and(X, 0xFF00) truncates to 0 in i8, so or is just y.
define i8 @or_and_hi_mask_zext(i32 %x, i8 %y) {
  %masked = and i32 %x, 65280
  %zy = zext i8 %y to i32
  %r = or i32 %masked, %zy
  %t = trunc i32 %r to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @or_and_hi_mask_zext(
; CHECK-NOT:   and i32
; CHECK-NOT:   zext
; CHECK-NOT:   or i32
; CHECK-NOT:   trunc
; CHECK:       ret i8 %y

; trunc(add(and(X, 0xFF00), zext(y: i8→i32)), i8) = y
; Same reasoning: and contributes 0, add identity.
define i8 @add_and_hi_mask_zext(i32 %x, i8 %y) {
  %masked = and i32 %x, 65280
  %zy = zext i8 %y to i32
  %r = add i32 %masked, %zy
  %t = trunc i32 %r to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @add_and_hi_mask_zext(
; CHECK-NOT:   and i32
; CHECK-NOT:   zext
; CHECK-NOT:   add i32
; CHECK-NOT:   trunc
; CHECK:       ret i8 %y
