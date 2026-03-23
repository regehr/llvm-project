; A signed predicate against a zero-extended value CAN be narrowed: since
; zext always produces a non-negative value and the constant 100 is positive,
; icmp slt (zext i8 x to i32), 100 == icmp ult i8 x, 100.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @slt_zext_const(i8 %x) {
  %ext = zext i8 %x to i32
  %cmp = icmp slt i32 %ext, 100
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @slt_zext_const(
; CHECK-NOT: zext
; CHECK: icmp ult i8 %x, 100
