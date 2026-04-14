; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryShrinkTruncOfZeroBoundedPhi: various negative paths.

; line 4226: cost check fails for one incoming value (shl with variable shift amount).
; collectTruncRootedValueCost returns false for shl(b, s) when s is non-constant.

define i8 @phi_trunc_shl_var_cost_fail(i8 %a, i32 %b, i32 %s, i1 %c) {
entry:
  br i1 %c, label %t, label %f
t:
  %shl = shl i32 %b, %s
  br label %exit
f:
  %za = zext i8 %a to i32
  br label %exit
exit:
  %phi = phi i32 [ %shl, %t ], [ %za, %f ]
  %tr = trunc i32 %phi to i8
  ret i8 %tr
}
; CHECK-LABEL: define i8 @phi_trunc_shl_var_cost_fail(
; CHECK: phi i32
; CHECK: trunc i32
; CHECK: ret i8

; lines 4252-4255: incoming value is an invoke result — cannot insert before terminator.
; The invoke %r is the terminator of invoke_bb; inserting before it is illegal.
; The cost check passes (zext %za is removable), but the invoke check bails out.

declare i32 @may_throw()

define i8 @phi_of_invoke_nochange(i8 %a, i1 %c) personality ptr null {
entry:
  %za = zext i8 %a to i32
  br i1 %c, label %invoke_bb, label %normal
invoke_bb:
  %r = invoke i32 @may_throw() to label %normal unwind label %lpad
normal:
  %phi = phi i32 [ %r, %invoke_bb ], [ %za, %entry ]
  %tr = trunc i32 %phi to i8
  ret i8 %tr
lpad:
  %lp = landingpad { ptr, i32 } cleanup
  ret i8 0
}
; CHECK-LABEL: define i8 @phi_of_invoke_nochange(
; CHECK: phi i32
; CHECK: trunc i32
; CHECK: ret i8
