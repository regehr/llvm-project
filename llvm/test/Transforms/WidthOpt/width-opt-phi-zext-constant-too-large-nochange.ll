; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %x) {
entry:
  br i1 %c, label %left, label %right

left:
  %zx = zext i8 %x to i32
  br label %merge

right:
  br label %merge

merge:
  %p = phi i32 [ %zx, %left ], [ 256, %right ]
  ret i32 %p
}

; CHECK-LABEL: define i32 @f(
; CHECK: %zx = zext i8 %x to i32
; CHECK: %p = phi i32 [ %zx, %left ], [ 256, %right ]
; CHECK: ret i32 %p
