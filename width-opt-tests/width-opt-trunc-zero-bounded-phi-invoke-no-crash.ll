
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

