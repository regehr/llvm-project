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
  %x = xor i8 %p, 7
  %a = zext i8 %x to i32
  %b = zext i8 %x to i32
  %d = zext i8 %x to i32
  %r = add i32 %a, %b
  %t = add i32 %r, %d
  ret i32 %t
}

; CHECK-LABEL: define i32 @f(
; CHECK: merge:
; CHECK: %[[P:.*]] = phi i32 [ 1, %left ], [ 2, %right ]
; CHECK: %[[X:.*]] = xor i32 %[[P]], 7
; CHECK-NOT: zext i8 %x to i32
; CHECK: %[[R:.*]] = add i32 %[[X]], %[[X]]
; CHECK: %[[T:.*]] = add i32 %[[R]], %[[X]]
; CHECK: ret i32 %[[T]]
