; The generic component widener called B.CreateSelect(i1 false, WideTV, WideFV),
; which IRBuilder folded to WideFV (not a SelectInst).  The subsequent
; cast<SelectInst> then asserted.  Also covered: tryRetargetExternalUnsignedCompareViaSelect
; called B.CreateSelect on a select with a constant condition.
; RUN: opt -passes='width-opt' -S %s -o /dev/null

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; %sel has condition i1 false; the generic widener creates a wide select with
; the same constant condition, which IRBuilder folds to the false arm — a
; ZExtInst, not a SelectInst — causing cast<SelectInst> to assert.
define i64 @select_const_false_cond_in_widened_component(i8 %x) {
entry:
  %sel = select i1 false, i8 0, i8 %x
  %ext = zext nneg i8 %sel to i64
  ret i64 %ext
}

; Analogous pattern with i1 true condition.
define i64 @select_const_true_cond_in_widened_component(i8 %x) {
entry:
  %sel = select i1 true, i8 %x, i8 0
  %ext = zext nneg i8 %sel to i64
  ret i64 %ext
}
