
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i64 @zext_chain_i24_i32_i64(ptr %p) {
entry:
  %load = load i24, ptr %p, align 1
  %ext1 = zext i24 %load to i32
  %ext2 = zext i32 %ext1 to i64
  ret i64 %ext2
}
