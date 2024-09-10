; RUN: opt -O2 < %s -S | FileCheck %s


; MAX - (x or MAX) → x and (MAX + 1)
; CHECK-LABEL: @test1(
; CHECK: i32 returned [[X:%.*]])
define i32 @test1(i32 %x) {
   %a = sub i32 2147483647, %x
   %b = xor i32 %a, 2147483647
; CHECK-NEXT: ret i32 [[X]]
   ret i32 %b
}

; MAX - (x or MAX) → x and (MAX + 1)
; CHECK-LABEL: @test2(
; CHECK: i32 returned [[X:%.*]])
define i32 @test2(i16 %x) {
   %a = sub i16 2147483647, %x
   %b = xor i16 %a, 2147483647
; CHECK-NEXT: ret i16 [[X]]
   ret i16 %b
}