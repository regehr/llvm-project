; RUN: opt < %s -passes=instcombine -S | FileCheck %s

; Test 1: Simple Case (Optimization Applied)
define i1 @test1(i16 %x) {
  %mul1 = mul nsw i16 %x, 3 ; Positive constant
  %mul2 = mul nsw i16 %mul1, %x
  %cmp = icmp sge i16 %mul2, 0
  ret i1 %cmp
}

; CHECK-LABEL: @test1(
; CHECK-NEXT:    ret i1 true

; Test 2: No `nsw` Flag (Optimization Not Applied)
define i1 @test2(i16 %x) {
  %mul1 = mul i16 %x, 3 ; No 'nsw' flag
  %mul2 = mul i16 %mul1, %x
  %cmp = icmp sge i16 %mul2, 0
  ret i1 %cmp
}

; CHECK-LABEL: @test2(
; CHECK-NOT:    ret i1 true 
; CHECK: ret i1 %cmp

; Test 3: Non-Positive Constant (Optimization Not Applied)
define i1 @test3(i16 %x) {
  %mul1 = mul nsw i16 %x, -5 ; Non-positive constant
  %mul2 = mul nsw i16 %mul1, %x
  %cmp = icmp sge i16 %mul2, 0
  ret i1 %cmp
}

; CHECK-LABEL: @test3(
; CHECK-NOT:    ret i1 true
; CHECK: ret i1 %cmp
