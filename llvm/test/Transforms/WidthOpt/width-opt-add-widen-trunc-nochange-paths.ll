; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryWidenAddOverTruncThroughZExt: various negative paths.

; line 1838: MidWidth < 2 (i1 add) → return false before checking trySide.

define i32 @add_widen_i1(i1 %a, i1 %b) {
  %add = add i1 %a, %b
  %wz = zext i1 %add to i32
  ret i32 %wz
}
; CHECK-LABEL: define i32 @add_widen_i1(
; CHECK: add i1
; CHECK: zext i1
; CHECK: ret i32

; line 1846: Trunc found but other operand is non-constant → !C → return false.

define i32 @add_widen_non_const(i32 %x, i8 %b) {
  %tx = trunc i32 %x to i8
  %add = add i8 %tx, %b
  %wz = zext i8 %add to i32
  ret i32 %wz
}
; CHECK-LABEL: define i32 @add_widen_non_const(
; CHECK: trunc i32
; CHECK: add i8
; CHECK: zext i8
; CHECK: ret i32

; line 1850: X (source of Trunc) has wrong width — width ≠ WideWidth → return false.
; X = i16 %x, WideWidth = 32 (from zext to i32) → 16 ≠ 32.

define i32 @add_widen_wrong_src_width(i16 %x) {
  %tx = trunc i16 %x to i8
  %add = add i8 %tx, 5
  %wz = zext i8 %add to i32
  ret i32 %wz
}
; CHECK-LABEL: define i32 @add_widen_wrong_src_width(
; CHECK: trunc i16
; CHECK: add i8
; CHECK: zext i8
; CHECK: ret i32

; line 1855: C value doesn't fit in MidWidth-1 bits → !isIntN(7) → return false.
; C = 128 for MidWidth=8: needs 8 bits, isIntN(7) = false.

define i32 @add_widen_c_too_large(i32 %x) {
  %tx = trunc i32 %x to i8
  %add = add i8 %tx, 128
  %wz = zext i8 %add to i32
  ret i32 %wz
}
; CHECK-LABEL: define i32 @add_widen_c_too_large(
; CHECK: trunc i32
; CHECK: add i8
; CHECK: zext i8
; CHECK: ret i32
