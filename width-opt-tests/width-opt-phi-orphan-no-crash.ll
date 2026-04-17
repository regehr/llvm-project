
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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
