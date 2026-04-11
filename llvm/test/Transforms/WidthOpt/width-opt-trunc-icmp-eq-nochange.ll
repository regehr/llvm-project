; The old widening step was only break-even and is now rejected, so the
; trunc-based compare remains unchanged.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare void @llvm.assume(i1)

define i1 @f(i32 %x, i32 %y) {
entry:
  %x.in.range = icmp ult i32 %x, 65537
  %y.in.range = icmp ult i32 %y, 65536
  call void @llvm.assume(i1 %x.in.range)
  call void @llvm.assume(i1 %y.in.range)
  %x16 = trunc i32 %x to i16
  %y16 = trunc i32 %y to i16
  %cmp = icmp eq i16 %x16, %y16
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @f(
; CHECK: %x16 = trunc i32 %x to i16
; CHECK: %y16 = trunc i32 %y to i16
; CHECK: %cmp = icmp eq i16 %x16, %y16
; CHECK-NOT: and i32 %x, 65535
; CHECK: ret i1 %cmp
