; RUN: opt -passes=instcombine -S < %s | FileCheck %s

; ----------------- POSITIVE TEST CASE -----------------

define i32 @src(i32 %x, i32 %y) {
  %xor1 = xor i32 %y, %x
  %cmp = icmp eq i32 %xor1, -1
  %sel = select i1 %cmp, i32 %y, i32 0
  %xor2 = xor i32 %sel, %x
  ret i32 %xor2
}

; CHECK: %xor1 = xor i32 %y, %x
; CHECK: %cmp = icmp eq i32 %xor1, -1
; CHECK: %xor2 = select i1 %cmp, i32 -1, i32 %x
; CHECK-NOT: %xor2 = xor i32 %sel, %x
; CHECK: ret i32 %xor2

; ----------------- NEGATIVE TEST CASE -----------------

define i32 @src2(i32 %x, i32 %y) {
  %xor1 = xor i32 %y, %x
  %cmp = icmp eq i32 %xor1, -2
  %sel = select i1 %cmp, i32 %y, i32 0
  %xor2 = xor i32 %sel, %x
  ret i32 %xor2
}

; CHECK: %xor1 = xor i32 %y, %x
; CHECK: %cmp = icmp eq i32 %xor1, -2
; CHECK: %sel = select i1 %cmp, i32 %y, i32 0
; CHECK: %xor2 = xor i32 %sel, %x
; CHECK: ret i32 %xor2