; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i1 %d, i8 %x) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ 1, %left ], [ 2, %right ]
  %s = select i1 %d, i8 %p, i8 %x
  %a = zext i8 %s to i32
  %b = zext i8 %s to i32
  %r = add i32 %a, %b
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: merge:
; CHECK: %[[P:.*]] = phi i32 [ 1, %left ], [ 2, %right ]
; CHECK: %[[X32:.*]] = zext i8 %x to i32
; CHECK: %[[S:.*]] = select i1 %d, i32 %[[P]], i32 %[[X32]]
; CHECK-NOT: zext i8 %s to i32
; CHECK: %[[R:.*]] = add i32 %[[S]], %[[S]]
; CHECK: ret i32 %[[R]]
