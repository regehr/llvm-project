; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; A break-even widen is not profitable here. Without width-opt, later cleanup can
; delete the wide mask and keep the narrow compare, so preserving the mask just
; swaps one instruction for another.

define i1 @icmp_trunc_widen_const(i32 %x) {
  %and = and i32 %x, 255
  %tx = trunc i32 %and to i8
  %cmp = icmp ult i8 %tx, 42
  ret i1 %cmp
}
; CHECK-LABEL: define i1 @icmp_trunc_widen_const(
; CHECK-NOT:   and i32
; CHECK:       %[[TX:.+]] = trunc i32 %x to i8
; CHECK:       icmp ult i8 %[[TX]], 42
; CHECK:       ret i1
