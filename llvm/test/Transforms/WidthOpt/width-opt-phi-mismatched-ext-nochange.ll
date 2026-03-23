; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %x, i8 %y) {
entry:
  br i1 %c, label %left, label %right

left:
  %sx = sext i8 %x to i32
  br label %merge

right:
  %zy = zext i8 %y to i32
  br label %merge

merge:
  %p = phi i32 [ %sx, %left ], [ %zy, %right ]
  ret i32 %p
}

; CHECK-LABEL: define i32 @f(
; CHECK: left:
; CHECK: %sx = sext i8 %x to i32
; CHECK: right:
; CHECK: %zy = zext i8 %y to i32
; CHECK: merge:
; CHECK: %p = phi i32 [ %sx, %left ], [ %zy, %right ]
; CHECK: ret i32 %p
