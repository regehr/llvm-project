; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c0, i8 %a, i8 %b) {
entry:
  %v = select i1 %c0, i8 -1, i8 2
  %w = or i8 %a, %b
  %cmp = icmp eq i8 %v, %w
  %cmpz = zext i1 %cmp to i32
  %vx = sext i8 %v to i32
  %wz0 = zext i8 %w to i32
  %wz1 = zext i8 %w to i32
  %sum0 = add i32 %cmpz, %vx
  %sum1 = add i32 %sum0, %wz0
  %r = add i32 %sum1, %wz1
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %v = select i1 %c0, i8 -1, i8 2
; CHECK: %w = or i8 %a, %b
; CHECK: %cmp = icmp eq i8 %v, %w
; CHECK: %vx = sext i8 %v to i32
; CHECK: %wz0 = zext i8 %w to i32
; CHECK: %wz1 = zext i8 %w to i32
; CHECK-NOT: trunc i32
; CHECK-NOT: and i32
; CHECK: ret i32 %r
