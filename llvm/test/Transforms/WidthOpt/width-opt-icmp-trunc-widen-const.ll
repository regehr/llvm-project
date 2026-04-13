; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryWidenTruncZeroExtendedICmp: trunc of high-zero value compared to constant
; (lines 1148-1150): Other is a constant → AddedBoundaryCost=0, zext created for it.
; trunc(and(x, 255), i8) ult 42: high bits of and result are known zero → widen.

define i1 @icmp_trunc_widen_const(i32 %x) {
  %and = and i32 %x, 255
  %tx = trunc i32 %and to i8
  %cmp = icmp ult i8 %tx, 42
  ret i1 %cmp
}
; CHECK-LABEL: define i1 @icmp_trunc_widen_const(
; CHECK-NOT:   trunc i32
; CHECK:       icmp ult i32 %and, 42
; CHECK:       ret i1
