; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare i32 @sink3(i32, i32, i32)

define i32 @f(i1 %c, i8 %k) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ 1, %left ], [ 2, %right ]
  %s = select i1 %c, i8 %p, i8 3
  %cmp = icmp eq i8 %s, %k
  %x = zext i8 %p to i32
  %y = zext i8 %s to i32
  %mix = xor i32 %x, %y
  %call = call i32 @sink3(i32 %mix, i32 %x, i32 %y)
  %cmp.z = zext i1 %cmp to i32
  %r = add i32 %call, %cmp.z
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[P32:.*]] = phi i32 [ 1, %left ], [ 2, %right ]
; CHECK: %[[S32:.*]] = select i1 %c, i32 %[[P32]], i32 3
; CHECK: %[[K32:.*]] = zext i8 %k to i32
; CHECK: %cmp = icmp eq i32 %[[S32]], %[[K32]]
; CHECK: %mix = xor i32 %[[P32]], %[[S32]]
; CHECK: %[[CALL:.*]] = call i32 @sink3(i32 %mix, i32 %[[P32]], i32 %[[S32]])
; CHECK: %r = add i32 %[[CALL]], %cmp.z
; CHECK-NOT: zext i8 %p to i32
; CHECK-NOT: zext i8 %s to i32
