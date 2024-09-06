; RUN: opt -O2 -S < %s | FileCheck %s


; (0x7FFFFFFF - x) ⊕ 0x7FFFFFFF → x
; CHECK-LABEL: @test1([[X:%.*]])
define i16 @test1(i16 %x) {
; CHECK-NEXT: ret i16 X
   %a = add i16 32767, %x
   %b = xor i16 %a, 32767
   ret i16 %b
}
