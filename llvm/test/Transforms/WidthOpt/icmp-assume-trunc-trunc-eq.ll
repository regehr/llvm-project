; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; Assumptions let InstCombine eliminate both truncs and compare at the wider type.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare void @llvm.assume(i1)

define i1 @f(i32 %x, i32 %y) {
entry:
  %ax = icmp ult i32 %x, 65536
  %ay = icmp ult i32 %y, 65536
  call void @llvm.assume(i1 %ax)
  call void @llvm.assume(i1 %ay)
  %x16 = trunc i32 %x to i16
  %y16 = trunc i32 %y to i16
  %r = icmp eq i16 %x16, %y16
  ret i1 %r
}

; CHECK-LABEL: define i1 @f(
; CHECK-NOT: trunc i32
; CHECK: icmp eq i32 %x, %y
; CHECK: ret i1
