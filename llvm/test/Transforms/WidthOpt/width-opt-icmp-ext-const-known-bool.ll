; RUN: opt -passes='width-opt' -S %s | FileCheck %s
;
; Exercise the constant-aware narrow compare builder. After shrinking the icmp
; to the i1 source type, the compare becomes `icmp ule i1 false, 0`, which is
; known true and is materialized via getSameShapeBoolConstant().

define i1 @zext_false_ule_zero() {
entry:
  %ext = zext i1 false to i32
  %cmp = icmp ule i32 %ext, 0
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @zext_false_ule_zero(
; CHECK: ret i1 true
