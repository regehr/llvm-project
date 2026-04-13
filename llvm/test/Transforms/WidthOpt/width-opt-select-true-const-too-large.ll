; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryShrinkSelectOfExts: TrueArm is a constant too large for NarrowWidth (line 1393).
; select(cond, 256, zext i8 %x to i32): FalseExt NarrowWidth=8, TrueC=256 > 255 → no change.

define i32 @select_true_const_too_large(i1 %cond, i8 %x) {
  %zx = zext i8 %x to i32
  %sel = select i1 %cond, i32 256, i32 %zx
  ret i32 %sel
}
; CHECK-LABEL: define i32 @select_true_const_too_large(
; CHECK:       %zx = zext i8 %x to i32
; CHECK:       %sel = select i1 %cond, i32 256, i32 %zx
; CHECK:       ret i32 %sel
