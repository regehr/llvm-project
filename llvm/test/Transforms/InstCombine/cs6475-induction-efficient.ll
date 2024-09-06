; RUN: opt < %s -passes=instcombine -S | FileCheck %s

define dso_local noundef i32 @src(i32 noundef %num) local_unnamed_addr {
entry:
  %0 = add i32 %num, -65535
  %or.cond = icmp ult i32 %0, -65534
  br i1 %or.cond, label %cleanup, label %for.body.preheader

for.body.preheader:                               ; preds = %entry
; CHECK-LABEL: for.body.preheader:
; CHECK-NEXT:  [[C1:%.*]] = add nuw nsw i32 [[NUM:%.*]], 1
; CHECK-NEXT:  [[C2:%.*]] = mul i32 [[NUM]], [[C1]]
; CHECK-NEXT:  [[C3:%.*]] = lshr i32 [[C2]], 1

  %1 = shl i32 %num, 1
  %2 = add i32 %num, -1
  %3 = zext i32 %2 to i33
  %4 = add i32 %num, -2
  %5 = zext i32 %4 to i33
  %6 = mul i33 %3, %5
  %7 = lshr i33 %6, 1
  %8 = trunc nuw i33 %7 to i32
  %9 = add i32 %1, %8
  %10 = add i32 %9, -1
  br label %cleanup

cleanup:                                          ; preds = %for.body.preheader, %entry
  %retval.0 = phi i32 [ 0, %entry ], [ %10, %for.body.preheader ]
  ret i32 %retval.0
}

define dso_local noundef i32 @src2(i32 noundef %num) local_unnamed_addr {
entry:
  %0 = add i32 %num, -100001
  %or.cond = icmp ult i32 %0, -100000
  br i1 %or.cond, label %cleanup, label %for.body.preheader

for.body.preheader:                               ; preds = %entry
; CHECK-LABEL: for.body.preheader:
; CHECK:  [[C1:%.*]] = lshr i33 
; CHECK: br label
; i33 value means that my opt did not fire

  %1 = shl i32 %num, 1
  %2 = add i32 %num, -1
  %3 = zext i32 %2 to i33
  %4 = add i32 %num, -2
  %5 = zext i32 %4 to i33
  %6 = mul i33 %3, %5
  %7 = lshr i33 %6, 1
  %8 = trunc nuw i33 %7 to i32
  %9 = add i32 %1, %8
  %10 = add i32 %9, -1
  br label %cleanup

cleanup:                                          ; preds = %for.body.preheader, %entry
  %retval.0 = phi i32 [ 0, %entry ], [ %10, %for.body.preheader ]
  ret i32 %retval.0
}
