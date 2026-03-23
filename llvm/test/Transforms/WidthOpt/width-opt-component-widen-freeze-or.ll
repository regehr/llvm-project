; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %x) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ %x, %left ], [ 7, %right ]
  %fr = freeze i8 %p
  %o = or i8 %fr, 1
  %a = zext i8 %o to i32
  %b = zext i8 %o to i32
  %sum = add i32 %a, %b
  ret i32 %sum
}

; CHECK-LABEL: define i32 @f(
; CHECK: merge:
; CHECK: %[[P:.*]] = phi i8 [ %x, %left ], [ 7, %right ]
; CHECK: %[[FR:.*]] = freeze i8 %[[P]]
; CHECK: %[[W:.*]] = zext i8 %[[FR]] to i32
; CHECK: %[[O:.*]] = or i32 %[[W]], 1
; CHECK-NOT: zext i8 %o to i32
; CHECK: %[[SUM:.*]] = add i32 %[[O]], %[[O]]
; CHECK: ret i32 %[[SUM]]
