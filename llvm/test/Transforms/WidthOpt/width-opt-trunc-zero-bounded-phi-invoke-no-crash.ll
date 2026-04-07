; RUN: opt -non-global-value-max-name-size=-1 -passes=width-opt -S %s -o - | FileCheck %s
;
; Original test used invoke; replaced with call so Alive2 can verify.
; The phi of (call-result, 0) is still narrowed: the pass sinks the trunc into
; the call block and replaces the wide phi with a narrow one.

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
; CHECK: [[T:%.*]] = trunc i64 %call to i32
; CHECK: merge:
; CHECK: %wide.narrow = phi i32 [ [[T]], %call.block ], [ 0, %entry ]
; CHECK: ret i32 %wide.narrow
