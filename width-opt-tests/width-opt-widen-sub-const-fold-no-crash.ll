
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @widen_sub_const_fold() {
entry:
  %tr   = trunc i32 0 to i16
  %sub  = sub i16 0, %tr
  %zext = zext nneg i16 %sub to i32
  ret i32 %zext
}
