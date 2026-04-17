
define i64 @no_dead_cast(i1 %flag, i8 %val, i64 %extra) {
entry:
  ; zext i1 → i64 has two uses (phi + add), so its producer cannot be removed.
  %w1 = zext i1 %flag to i64
  %w2 = zext i8 %val to i64
  %use = add i64 %w1, %extra
  br i1 %flag, label %a, label %b

a:
  br label %merge

b:
  br label %merge

merge:
  %p = phi i64 [ %w1, %a ], [ %w2, %b ]
  %r = add i64 %p, %use
  ret i64 %r
}


define <4 x i64> @no_dead_cast_vec(<4 x i1> %flag, <4 x i8> %val, <4 x i64> %extra, i1 %cond) {
entry:
  %w1 = zext <4 x i1> %flag to <4 x i64>
  %w2 = zext <4 x i8> %val to <4 x i64>
  %use = add <4 x i64> %w1, %extra
  br i1 %cond, label %a, label %b

a:
  br label %merge

b:
  br label %merge

merge:
  %p = phi <4 x i64> [ %w1, %a ], [ %w2, %b ]
  %r = add <4 x i64> %p, %use
  ret <4 x i64> %r
}

