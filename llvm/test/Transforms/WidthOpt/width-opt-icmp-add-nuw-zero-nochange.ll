; RUN: opt -passes='width-opt' -S %s | FileCheck %s
;
; add nuw with a literal zero exercises the structural-width logic for
; constant-zero operands. WidthOpt should not claim a narrower width here.

define i1 @icmp_ult_add_nuw_zero(i8 %a) {
entry:
  %a32 = zext i8 %a to i32
  %sum = add nuw i32 %a32, 0
  %cmp = icmp ult i32 %sum, 10
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ult_add_nuw_zero(
; CHECK: %a32 = zext i8 %a to i32
; CHECK: %sum = add nuw i32 %a32, 0
; CHECK: %cmp = icmp ult i32 %sum, 10
; CHECK-NOT: icmp ult i8
; CHECK: ret i1 %cmp
