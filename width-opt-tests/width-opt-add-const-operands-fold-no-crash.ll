
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @add_over_trunc_of_const_zext(i32 %unused) {
entry:
  %t    = trunc i32 0 to i8
  %add  = add i8 %t, 0
  %ext  = zext i8 %add to i32
  ret i32 %ext
}
