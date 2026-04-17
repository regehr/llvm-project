
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i64 @select_const_false_cond_in_widened_component(i8 %x) {
entry:
  %sel = select i1 false, i8 0, i8 %x
  %ext = zext nneg i8 %sel to i64
  ret i64 %ext
}

define i64 @select_const_true_cond_in_widened_component(i8 %x) {
entry:
  %sel = select i1 true, i8 %x, i8 0
  %ext = zext nneg i8 %sel to i64
  ret i64 %ext
}
