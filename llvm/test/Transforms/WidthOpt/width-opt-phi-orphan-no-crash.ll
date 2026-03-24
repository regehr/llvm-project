; tryWidenComponentFromPlan created a new wide PHI node and called takeName
; before checking whether all incoming values could be materialized.  When a
; phi had a non-phi component member as an incoming value (e.g. a TruncInst in
; a phi cycle through an unreachable block), the fill loop returned false and
; left an orphan PHI with an incomplete incoming-value list in the IR.  The
; verifier then fired: "PHINode should have one entry for each predecessor".
; RUN: opt -passes='width-opt' -S %s -o /dev/null

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; The unreachable block forms a phi cycle: %l feeds into the dead block which
; produces %trunc which feeds back into %l.  tryWidenComponentFromPlan tried
; to widen the {%l, %trunc} i32 component but bailed out after creating the
; new wide PHI because %trunc (a non-phi component member) had no NewValues
; entry yet, leaving an orphan phi with only one incoming value.
define i32 @phi_cycle_through_unreachable(i32 %arg) {
entry:
  br label %end

dead:                                             ; No predecessors!
  %sext  = sext i32 %l to i64
  %and   = and i64 %sext, 1
  %trunc = trunc i64 %and to i32
  br label %end

end:                                              ; preds = %dead, %entry
  %l = phi i32 [ %trunc, %dead ], [ 0, %entry ]
  %ext = sext i32 %l to i64
  ret i32 0
}
