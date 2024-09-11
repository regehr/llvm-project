; RUN: opt --O2 -S %s | FileCheck %s


; MAX - (x or MAX) → no optimization
; CHECK-LABEL: @test1(
; CHECK: i16 [[X:%.*]])
define i16 @test1(i16 %x) {
   ; CHECH-NEXT: and i16 [[X]], i16 -32768
   %a = sub i16 32767, %x
   %b = or i16 %a, 32767
   ret i16 %b
}

; MAX - (x or MAX) → no optimization
; CHECK-LABEL: @test2(
; CHECK: i32 [[X:%.*]])
define i32 @test2(i32 %x) {
   ; CHECH-NEXT: and i32 [[X]], i32 -2147483648
   %a = sub i32 2147483647, %x
   %b = or i32 %a, 2147483647
   ret i32 %b
}