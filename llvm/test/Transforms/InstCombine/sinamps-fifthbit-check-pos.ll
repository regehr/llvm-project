; RUN: opt < %s -passes=instcombine -S | FileCheck %s

; Should be optimized to only an and and icmp ne.
define i1 @test_positive(i16 %t) {
; CHECK-LABEL: @test_positive(
; CHECK-NEXT:    [[A2:%.*]] = and i16 [[T:%.*]], 16
; CHECK-NEXT:    [[F:%.*]] = icmp ne i16 [[A2]], 0
; CHECK-NEXT:    ret i1 [[F]]
;
  %a = lshr i16 %t, 4
  %b = and i16 %a, 1
  %d = sub nsw i16 0, %a
  %e = and i16 %b, %d
  %f = icmp ne i16 %e, 0
  ret i1 %f
}