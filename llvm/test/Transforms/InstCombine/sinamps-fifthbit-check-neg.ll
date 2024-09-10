; RUN: opt < %s -passes=instcombine -S | FileCheck %s

; Should not be optimized as the pattern does not match.
define i1 @test_negative(i16 %t) {
; CHECK-LABEL: @test_negative(
; CHECK-NEXT:    [[A:%.*]] = lshr i16 [[T:%.*]], 3
; CHECK-NEXT:    [[B:%.*]] = and i16 [[A]], 1
; CHECK-NEXT:    [[D:%.*]] = sub nsw i16 0, [[A]]
; CHECK-NEXT:    [[E:%.*]] = and i16 [[B]], [[D]]
; CHECK-NEXT:    [[F:%.*]] = icmp ne i16 [[E]], 0
; CHECK-NEXT:    ret i1 [[F]]
;
  %a = lshr i16 %t, 3
  %b = and i16 %a, 1
  %d = sub nsw i16 0, %a
  %e = and i16 %b, %d
  %f = icmp ne i16 %e, 0
  ret i1 %f
}