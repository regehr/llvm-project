; Range facts alone are not enough: if narrowing a udiv would only add
; trunc/zext boundaries around anchored wide values, keep the original udiv.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i32 %x, i32 %y) {
entry:
  %cx = icmp ult i32 %x, 256
  call void @llvm.assume(i1 %cx)
  %cy = icmp ult i32 %y, 256
  call void @llvm.assume(i1 %cy)
  %d = udiv i32 %x, %y
  ret i32 %d
}

declare void @llvm.assume(i1)

; CHECK-LABEL: define i32 @f(
; CHECK: %cx = icmp ult i32 %x, 256
; CHECK: call void @llvm.assume(i1 %cx)
; CHECK: %cy = icmp ult i32 %y, 256
; CHECK: call void @llvm.assume(i1 %cy)
; CHECK: %d = udiv i32 %x, %y
; CHECK-NOT: trunc i32 %x
; CHECK-NOT: trunc i32 %y
; CHECK-NOT: zext i8
; CHECK: ret i32 %d
