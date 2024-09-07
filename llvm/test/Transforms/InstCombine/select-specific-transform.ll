; RUN: opt -passes=instcombine -S < %s | FileCheck %s

define i32 @src(i32 %x, i32 %y) {
  %xor1 = xor i32 %y, %x
  %cmp = icmp eq i32 -1, %xor1
  %sel = select i1 %cmp, i32 %y, i32 0
  %xor2 = xor i32 %sel, %x
  ret i32 %xor2
}

; CHECK: %xor1 = xor i32 %y, %x
; CHECK-NOT: %xor2 = xor i32 %sel, %x