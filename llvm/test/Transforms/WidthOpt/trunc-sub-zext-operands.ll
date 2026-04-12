; tryShrinkTruncOfLowBitsBinOp: sub opcode variants.
; These exercise the sub code path (lines 3347-3348 in WidthOpt.cpp) which
; was previously untested.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Basic case: trunc(sub(zext(x: i8→i32), zext(y: i8→i32)), i8) = sub(x, y)
define i8 @sub_zext_operands(i8 %x, i8 %y) {
  %zx = zext i8 %x to i32
  %zy = zext i8 %y to i32
  %s = sub i32 %zx, %zy
  %t = trunc i32 %s to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @sub_zext_operands(
; CHECK-NOT:   zext
; CHECK-NOT:   sub i32
; CHECK-NOT:   trunc
; CHECK:       %{{.*}} = sub i8 %x, %y
; CHECK:       ret i8

; Identity fold: trunc(sub(zext(x), C_hi), i8) where C_hi has no low 8 bits.
; 65280 = 0xFF00; in i8 that truncates to 0 → sub identity → result is x.
define i8 @sub_const_hi_fold(i8 %x) {
  %zx = zext i8 %x to i32
  %s = sub i32 %zx, 65280
  %t = trunc i32 %s to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @sub_const_hi_fold(
; CHECK-NOT:   zext
; CHECK-NOT:   sub
; CHECK-NOT:   trunc
; CHECK:       ret i8 %x
