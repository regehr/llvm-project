; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; Known-zero high bits from llvm.assume let the trunc-vs-zext compare move back
; to the original wide type.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare void @llvm.assume(i1)

define i1 @f(i32 %x, i8 %y) {
entry:
  %x_lb_only = icmp ult i32 %x, 65536
  call void @llvm.assume(i1 %x_lb_only)
  %x16 = trunc i32 %x to i16
  %y16 = zext i8 %y to i16
  %r = icmp ugt i16 %x16, %y16
  ret i1 %r
}

; CHECK-LABEL: define i1 @f(
; CHECK: %[[ASSUME:.*]] = icmp ult i32 %x, 65536
; CHECK: call void @llvm.assume(i1 %[[ASSUME]])
; CHECK: %[[Y32:.*]] = zext i8 %y to i32
; CHECK: %[[CMP:.*]] = icmp ugt i32 %x, %[[Y32]]
; CHECK-NOT: trunc i32 %x to i16
; CHECK-NOT: zext i8 %y to i16
; CHECK: ret i1 %[[CMP]]
