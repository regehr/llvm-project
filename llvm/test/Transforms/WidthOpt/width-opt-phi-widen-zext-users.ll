; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ 1, %left ], [ 2, %right ]
  %x = zext i8 %p to i32
  %y = zext i8 %p to i32
  %z = zext i8 %p to i32
  %r = add i32 %x, %y
  %s = add i32 %r, %z
  ret i32 %s
}

; CHECK-LABEL: define i32 @f(
; CHECK: merge:
; CHECK: %[[P:.*]] = phi i32 [ 1, %left ], [ 2, %right ]
; CHECK-NOT: zext i8 %p to i32
; CHECK: %[[R:.*]] = add i32 %[[P]], %[[P]]
; CHECK: %[[S:.*]] = add i32 %[[R]], %[[P]]
; CHECK: ret i32 %[[S]]
