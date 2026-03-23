; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): NO
; The branch only proves n > -2, which is not enough to replace sext with zext.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare void @use64(i64)

define void @f(i32 %n) {
entry:
  %cmp = icmp sgt i32 %n, -2
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
; CHECK: %cmp = icmp sgt i32 %n, -2
; CHECK: br i1 %cmp, label %bb, label %exit
; CHECK: bb:
; CHECK: %ext.wide = sext i32 %n to i64
; CHECK-NOT: zext
; CHECK: call void @use64(i64 %ext.wide)
; CHECK: br label %exit
