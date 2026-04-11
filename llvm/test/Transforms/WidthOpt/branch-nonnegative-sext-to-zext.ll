; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; Branch information can justify replacing sext with zext for a wider use, but
; that rewrite is only break-even locally and is now disabled.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare void @use64(i64)

define void @f(i32 %n) {
entry:
  %cmp = icmp sgt i32 %n, -1
  br i1 %cmp, label %bb, label %exit

bb:
  %ext.wide = sext i32 %n to i64
  call void @use64(i64 %ext.wide)
  %ext = trunc i64 %ext.wide to i32
  br label %exit

exit:
  ret void
}

; CHECK-LABEL: define void @f(
; CHECK: bb:
; CHECK: %ext.wide = sext i32 %n to i64
; CHECK: call void @use64(i64 %ext.wide)
