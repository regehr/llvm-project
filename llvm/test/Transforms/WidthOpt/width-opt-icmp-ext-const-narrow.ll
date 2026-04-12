; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryShrinkICmpExtConst narrows icmp of extensions against constants.
; buildConstantAwareICmp is called with the narrow constant as RHS; it calls
; getKnownCompareResultWithConstantLHS with the swapped predicate.

; icmp ule (zext i8 %x), 100: fits in i8, ule is ZExt-compatible.
; Swapped pred = uge; getKnownCompareResultWithConstantLHS(uge, 100):
; UGE case: 100 != MaxValue(i8=255) → break (line 382), returns nullopt.
; No fold in buildConstantAwareICmp → emits icmp ule i8 %x, 100.

define i1 @zext_ule_non_max_const(i8 %x) {
  %z = zext i8 %x to i32
  %c = icmp ule i32 %z, 100
  ret i1 %c
}
; CHECK-LABEL: define i1 @zext_ule_non_max_const(
; CHECK-NOT:   zext
; CHECK:       icmp ule i8 %x, 100
; CHECK:       ret i1

; icmp sge (sext i8 %x), -100: -100 fits in i8, sge is SExt-compatible.
; Swapped pred = sle; getKnownCompareResultWithConstantLHS(sle, -100):
; SLE case: -100 != MinSignedValue(i8=-128) → break (line 398), returns nullopt.
; No fold → emits icmp sge i8 %x, -100.

define i1 @sext_sge_non_min_const(i8 %x) {
  %s = sext i8 %x to i32
  %c = icmp sge i32 %s, -100
  ret i1 %c
}
; CHECK-LABEL: define i1 @sext_sge_non_min_const(
; CHECK-NOT:   sext
; CHECK:       icmp sge i8 %x, -100
; CHECK:       ret i1

; icmp sle (sext i8 %x), 100: 100 fits in i8, sle is SExt-compatible.
; Swapped pred = sge; getKnownCompareResultWithConstantLHS(sge, 100):
; SGE case: 100 != MaxSignedValue(i8=127) → break (line 406), returns nullopt.
; No fold → emits icmp sle i8 %x, 100.

define i1 @sext_sle_non_max_const(i8 %x) {
  %s = sext i8 %x to i32
  %c = icmp sle i32 %s, 100
  ret i1 %c
}
; CHECK-LABEL: define i1 @sext_sle_non_max_const(
; CHECK-NOT:   sext
; CHECK:       icmp sle i8 %x, 100
; CHECK:       ret i1

; icmp ugt (zext i8 %x), 300: 300 > 255, out of range for i8.
; canRepresentConstant returns false; getKnownCompareResultWithExtAndConstant
; hits the UGT case (line 435): zext(x) <= 255 < 300, so ugt is always false.
; The comparison folds to false (i1 0).

define i1 @zext_ugt_out_of_range(i8 %x) {
  %z = zext i8 %x to i32
  %c = icmp ugt i32 %z, 300
  ret i1 %c
}
; CHECK-LABEL: define i1 @zext_ugt_out_of_range(
; CHECK-NOT:   zext
; CHECK-NOT:   icmp
; CHECK:       ret i1 false
