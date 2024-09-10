; RUN: opt -O2 -S < %s | FileCheck %s


; (0x7FFF + x) ⊕ 0x7FFF → (0x7FFF + x) ⊕ 0x7FFF
; CHECK-LABEL: @test1(
; CHECK: i16 [[X:%.*]])
define i16 @test1(i16 %x) {
   %a = add i16 32767, %x
   %b = xor i16 %a, 32767
; CHECK-NOT: ret i16 X
   ret i16 %b
}