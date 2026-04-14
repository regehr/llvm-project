; RUN: opt -passes='width-opt' -S %s | FileCheck %s
; Phi with two sext arms and a poison arm: canRepresentConstant(poison, SExt, N)
; returns true (line 162 in WidthOpt.cpp).

define i32 @f(i1 %c1, i1 %c2, i8 %x, i8 %y) {
entry:
  br i1 %c1, label %bb1, label %bb2
bb1:
  %sx = sext i8 %x to i32
  br label %merge
bb2:
  br i1 %c2, label %bb3, label %merge
bb3:
  %sy = sext i8 %y to i32
  br label %merge
merge:
  %p = phi i32 [ %sx, %bb1 ], [ %sy, %bb3 ], [ poison, %bb2 ]
  ret i32 %p
}
; CHECK-LABEL: @f(
; CHECK: phi i8
