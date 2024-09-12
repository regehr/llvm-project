; RUN: opt < %s -passes=instcombine -S | FileCheck %s

; CHECK-LABEL: @workedfinally(
; CHECK: ret i16 0
define i16 @workedfinally(i16 %x) {
  %x_plus_1 = add i16 %x, 1
  %x_plus_1_sq = mul i16 %x_plus_1, %x_plus_1
  %two_x1 = add i16 %x, 2
  %temp = mul i16 %two_x1, %x
  %B.neg = xor i16 %temp, -1
  %final = add i16 %x_plus_1_sq, %B.neg
  ret i16 %final
}
