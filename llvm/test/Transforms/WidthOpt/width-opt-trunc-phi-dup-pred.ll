; RUN: opt -passes='width-opt,verify' -S %s | FileCheck %s

; Regression test: when a PHI has duplicate predecessor edges (e.g. both arms
; of a conditional branch target the same block, or multiple switch cases go to
; the same destination), tryShrinkTruncOfPhi must reuse the same materialized
; narrow value for all entries from that predecessor.  Before the fix, each
; duplicate entry got its own trunc instruction, producing a PHI with multiple
; entries for the same block but different Value*s, which the verifier rejects:
;   "PHI node has multiple entries for the same basic block with different
;    incoming values!"

; Both arms of the branch go to %bb, creating two predecessor edges from
; %entry. Under the strict profitability rule this narrowing no longer fires,
; but the pass must still leave valid IR that satisfies the verifier.
define i8 @dup_pred_phi_trunc(i1 %cond, i32 %x) {
entry:
  br i1 %cond, label %bb, label %bb
bb:
  %phi = phi i32 [ %x, %entry ], [ %x, %entry ]
  %tr = trunc i32 %phi to i8
  ret i8 %tr
}

; CHECK-LABEL: define i8 @dup_pred_phi_trunc(
; CHECK:         %phi = phi i32 [ %x, %entry ], [ %x, %entry ]
; CHECK:         %tr = trunc i32 %phi to i8
; CHECK:         ret i8 %tr

define <4 x i8> @dup_pred_phi_trunc_vec(i1 %cond, <4 x i32> %x) {
entry:
  br i1 %cond, label %bb, label %bb
bb:
  %phi = phi <4 x i32> [ %x, %entry ], [ %x, %entry ]
  %tr = trunc <4 x i32> %phi to <4 x i8>
  ret <4 x i8> %tr
}

; CHECK-LABEL: define <4 x i8> @dup_pred_phi_trunc_vec(
; CHECK:         %phi = phi <4 x i32> [ %x, %entry ], [ %x, %entry ]
; CHECK:         %tr = trunc <4 x i32> %phi to <4 x i8>
; CHECK:         ret <4 x i8> %tr
