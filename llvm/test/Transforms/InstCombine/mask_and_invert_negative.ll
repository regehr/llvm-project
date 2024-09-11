; RUN: opt --O2 -S %s | FileCheck %s

; NOTMAX - (x or NOTMAX) → no optimization
; CHECK-LABEL: @test1(
; CHECK: i16 [[X:%.*]])
; CHECK-NEXT: sub i16 32333, [[X]]
; CHECK-NEXT: or i16 {{%.*}}, 32333
; CHECK-NEXT: ret i16 {{%.*}}
define i16 @test1(i16 %x) {
   %a = sub i16 32333, %x
   %b = or i16 %a, 32333
   ret i16 %b
}