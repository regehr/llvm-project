; RUN: opt -non-global-value-max-name-size=-1 -passes=width-opt -S %s -o - | FileCheck %s
;
; Original test used invoke; replaced with call so Alive2 can verify.
; With strict local profitability the phi of (call-result, 0) is no longer
; narrowed, but WidthOpt must still handle the pattern without crashing.

declare i64 @callee(ptr, i64)

define i32 @invoke_phi_trunc_no_crash() {
entry:
  br i1 false, label %merge, label %call.block

call.block:
  %call = call i64 @callee(ptr null, i64 0)
  br label %merge

merge:
  %wide = phi i64 [ %call, %call.block ], [ 0, %entry ]
  %narrow = trunc i64 %wide to i32
  ret i32 %narrow
}

; CHECK-LABEL: define i32 @invoke_phi_trunc_no_crash()
; CHECK: call.block:
; CHECK: %call = call i64 @callee(ptr null, i64 0)
; CHECK: merge:
; CHECK: %wide = phi i64 [ %call, %call.block ], [ 0, %entry ]
; CHECK: %narrow = trunc i64 %wide to i32
; CHECK-NOT: phi i32
; CHECK: ret i32 %narrow
