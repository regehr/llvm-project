

define i8 @dup_pred_phi_trunc(i1 %cond, i32 %x) {
entry:
  br i1 %cond, label %bb, label %bb
bb:
  %phi = phi i32 [ %x, %entry ], [ %x, %entry ]
  %tr = trunc i32 %phi to i8
  ret i8 %tr
}


define <4 x i8> @dup_pred_phi_trunc_vec(i1 %cond, <4 x i32> %x) {
entry:
  br i1 %cond, label %bb, label %bb
bb:
  %phi = phi <4 x i32> [ %x, %entry ], [ %x, %entry ]
  %tr = trunc <4 x i32> %phi to <4 x i8>
  ret <4 x i8> %tr
}

