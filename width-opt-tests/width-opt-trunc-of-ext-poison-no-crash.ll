
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i4 @trunc_of_zext_poison_retruncate() {
entry:
  %n = trunc i32 poison to i8
  %w = zext i8 %n to i16
  %r = trunc i16 %w to i4
  ret i4 %r
}

define i12 @trunc_of_sext_poison_reextend() {
entry:
  %n = trunc i32 poison to i8
  %w = sext i8 %n to i16
  %r = trunc i16 %w to i12
  ret i12 %r
}
