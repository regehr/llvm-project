; RUN: opt -passes='width-opt' -S %s | FileCheck %s
;
; Exercise the structural-width cache and the lshr-preserves-zero-boundedness
; path without triggering a rewrite.

define i1 @icmp_ult_or_same_lshr(i8 %x) {
entry:
  %z = zext i8 %x to i32
  %s = lshr i32 %z, 1
  %o = or i32 %s, %s
  %cmp = icmp ult i32 %o, 10
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ult_or_same_lshr(
; CHECK: %z = zext i8 %x to i32
; CHECK: %s = lshr i32 %z, 1
; CHECK: %o = or i32 %s, %s
; CHECK: %cmp = icmp ult i32 %o, 10
; CHECK-NOT: icmp ult i8
; CHECK: ret i1 %cmp
