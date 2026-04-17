
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @sext_chain_undef(i8 %x) {
entry:
  %conv1 = sext i8 undef to i16
  %conv2 = sext i16 %conv1 to i32
  ret i32 %conv2
}
