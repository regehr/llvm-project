; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare void @use8(i8)

define i32 @f(i1 %c, i1 %d) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ 1, %left ], [ 2, %right ]
  %s = select i1 %d, i8 %p, i8 3
  call void @use8(i8 %s)
  %x = zext i8 %s to i32
  %y = zext i8 %s to i32
  %z = zext i8 %s to i32
  %r = add i32 %x, %y
  %t = add i32 %r, %z
  ret i32 %t
}

; CHECK-LABEL: define i32 @f(
; CHECK: merge:
; CHECK: %[[P:.*]] = phi i32 [ 1, %left ], [ 2, %right ]
; CHECK: %[[S:.*]] = select i1 %d, i32 %[[P]], i32 3
; CHECK: %[[N:.*]] = trunc i32 %[[S]] to i8
; CHECK: call void @use8(i8 %[[N]])
; CHECK-NOT: zext i8 %s to i32
; CHECK: %[[R:.*]] = add i32 %[[S]], %[[S]]
; CHECK: %[[T:.*]] = add i32 %[[R]], %[[S]]
; CHECK: ret i32 %[[T]]
