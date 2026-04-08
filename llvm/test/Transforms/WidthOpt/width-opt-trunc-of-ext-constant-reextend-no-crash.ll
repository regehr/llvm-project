; WidthOpt may need to rebuild a narrower zext/sext while folding
; trunc(ext(x), M). When the source is constant-rooted, it must use constant
; folding instead of constructing an illegal ConstantExpr zext/sext.
; RUN: opt -passes='width-opt' -disable-output %s

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i16 @func_1() {
entry:
  %conv3717 = sext i32 0 to i64
  %cmp3718 = icmp slt i64 0, %conv3717
  %conv3719 = zext i1 %cmp3718 to i32
  %conv3720 = sext i32 %conv3719 to i64
  %and3721 = and i64 %conv3720, -1
  %conv3722 = trunc i64 %and3721 to i16
  ret i16 %conv3722
}
