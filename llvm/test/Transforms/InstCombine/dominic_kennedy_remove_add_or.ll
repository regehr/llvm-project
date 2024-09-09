; RUN: opt < %s -passes=instcombine -S | FileCheck %s

; positive test
define i1 @test1(i8 %0) {
; CHECK-LABEL: @test1(
; CHECK-NEXT:    ret i1 false
;
  %2 = or i8 %0, 55
  %3 = add i8 %2, 126
  %4 = icmp ult i8 %3, 53
  ret i1 %4
}

; negative test
define i1 @test2(i8 %0) {
; CHECK-LABEL: @test2(
; CHECK-NOT:    ret i1 false
;
  %2 = or i8 %0, 55
  %3 = add i8 %2, 126
  %4 = icmp ult i8 %3, 54
  ret i1 %4
}
