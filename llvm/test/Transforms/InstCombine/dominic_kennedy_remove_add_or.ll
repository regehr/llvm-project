; RUN: opt < %s -passes=instcombine -S | FileCheck %s

; positive test
define i1 @test1(i8 %0) {
; CHECK-LABEL: @test1(
; CHECK-NEXT:    ret i1 false
;
  %2 = or i8 %0, 32
  %3 = add i8 %2, -65
  %4 = icmp ult i8 %3, 31
  ret i1 %4
}

; negative test
define i1 @test2(i8 %0) {
; CHECK-LABEL: @test2(
; CHECK-NOT:    ret i1 false
;
  %2 = or i8 %0, 32
  %3 = add i8 %2, -65
  %4 = icmp ult i8 %3, 32
  ret i1 %4
}
