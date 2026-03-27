; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ poison, %left ], [ 9, %right ]
  %x = zext i8 %p to i32
  %y = zext i8 %p to i32
  %r = add i32 %x, %y
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: merge:
; CHECK: %[[P:.*]] = phi i8 [ poison, %left ], [ 9, %right ]
; CHECK: %[[W:.*]] = zext i8 %[[P]] to i32
; CHECK-NOT: zext i8 %p to i32
; CHECK: %[[R:.*]] = add i32 %[[W]], %[[W]]
; CHECK: ret i32 %[[R]]
