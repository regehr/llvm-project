; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; trunc(shl(zext %x, k)) where k < TargetWidth is narrowed: shl is low-bit
; preserving so the truncated result equals shl(%x, k) at the narrow width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i8 @shl_const_small(i8 %x) {
  %ext = zext i8 %x to i32
  %shl = shl i32 %ext, 2
  %trunc = trunc i32 %shl to i8
  ret i8 %trunc
}

; CHECK-LABEL: define i8 @shl_const_small(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: %[[S:.*]] = shl i8 %x, 2
; CHECK: ret i8 %[[S]]

; The shl can appear as an operand inside a larger low-bits expression.
define i8 @add_shl_const(i8 %x, i8 %y) {
  %ex = zext i8 %x to i32
  %ey = zext i8 %y to i32
  %shl = shl i32 %ex, 1
  %add = add i32 %shl, %ey
  %trunc = trunc i32 %add to i8
  ret i8 %trunc
}

; CHECK-LABEL: define i8 @add_shl_const(
; CHECK-NOT: zext
; CHECK-NOT: trunc i32
; CHECK: %[[S:.*]] = shl i8 %x, 1
; CHECK: %[[A:.*]] = add i8 %[[S]], %y
; CHECK: ret i8 %[[A]]
