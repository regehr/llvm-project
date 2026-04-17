
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i8 @trunc_ashr_sext_of_const() {
entry:
  %s = sext i8 0 to i32
  %a = ashr i32 %s, 0
  %t = trunc i32 %a to i8
  ret i8 %t
}

define i8 @trunc_ashr_zext_of_const() {
entry:
  %z = zext i8 0 to i32
  %a = ashr i32 %z, 0
  %t = trunc i32 %a to i8
  ret i8 %t
}

define i8 @trunc_lshr_sext_of_const() {
entry:
  %s = sext i8 0 to i32
  %a = lshr i32 %s, 0
  %t = trunc i32 %a to i8
  ret i8 %t
}
