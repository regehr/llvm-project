; tryShrinkPhiOfExts must not call users() on a ConstantData (e.g. ConstantInt)
; operand of a sext.  ConstantData values have no use list; the assertion
; 'hasUseList()' fires when the materialization check tries to scan them.
;
; The crash fires when a phi has all same-kind sext incomings, at least one
; incoming is a sext of a constant (NarrowWidth=8), and another has a wider
; source (NarrowWidth=16), making Info.NarrowWidth=16.  The constant incoming
; then needs materialization to i16 and the users() scan on i8 -1 (ConstantData)
; is reached without the fix.  A non-negative constant such as i8 0 does not
; reproduce the crash because width-opt rewrites sext i8 0 to zext nneg i8 0
; first, giving the phi mixed extension kinds and causing an early exit.
;
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @phi_sext_constant_no_crash(i1 %cond, i16 %x) {
entry:
  br i1 %cond, label %then, label %else

then:
  ; sext of a negative ConstantInt — stays as SExt, NarrowValue is ConstantData
  ; (i8 -1) which has no use list.
  %ext_const = sext i8 -1 to i32
  br label %merge

else:
  ; sext of a variable with a wider source; drives Info.NarrowWidth to 16,
  ; so the constant incoming needs materialization to i16, triggering the
  ; users() scan on i8 -1 (ConstantData) without the fix.
  %ext_val = sext i16 %x to i32
  br label %merge

merge:
  %phi = phi i32 [ %ext_const, %then ], [ %ext_val, %else ]
  ret i32 %phi
}

; CHECK-LABEL: define i32 @phi_sext_constant_no_crash(
; CHECK: merge:
; CHECK: %[[PN:.*]] = phi i16 [ -1, %then ], [ %x, %else ]
; CHECK: %[[W:.*]] = sext i16 %[[PN]] to i32
; CHECK: ret i32 %[[W]]
