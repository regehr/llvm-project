; RUN: opt -O2 -S < %s | FileCheck %s


; (0x7FFF - x) ⊕ 0x7FFF → x
; CHECK-LABEL: @test1([[X:%.*]])
define i16 @test1(i16 %x) {
; CHECK-NEXT: ret i16 X
   %a = sub i16 32767, %x
   %b = xor i16 %a, 32767
   ret i16 %b
}


; 0x7FFF ⊕ (0x7FFF - x) → x
; CHECK-LABEL: @test2([[X:%.*]])
define i16 @test2(i16 %x) {
; CHECK-NEXT: ret i16 X
   %a = sub i16 32767, %x
   %b = xor i16 32767, %a
   ret i16 %b
}


; (0x7FFFFFFF - x) ⊕ 0x7FFFFFFF → x
; CHECK-LABEL: @test3([[X:%.*]])
define i32 @test3(i32 %x) {
; CHECK-NEXT: ret i32 X
   %a = sub i32 2147483647, %x
   %b = xor i32 %a, 2147483647
   ret i32 %b
}


; 0x7FFFFFFF ⊕ (0x7FFFFFFF - x) → x
; CHECK-LABEL: @test4([[X:%.*]])
define i32 @test4(i32 %x) {
; CHECK-NEXT: ret i32 X
   %a = sub i32 2147483647, %x
   %b = xor i32 2147483647, %a
   ret i32 %b
}


; (0x7FFFFFFFFFFFFFFF - x) ⊕ 0x7FFFFFFFFFFFFFFF → x
; CHECK-LABEL: @test5([[X:%.*]])
define i64 @test5(i64 %x) {
; CHECK-NEXT: ret i64 X
   %a = sub i64 9223372036854775807, %x
   %b = xor i64 %a, 9223372036854775807
   ret i64 %b
}


; 0x7FFFFFFFFFFFFFFF ⊕ (0x7FFFFFFFFFFFFFFF - x) → x
; CHECK-LABEL: @test6([[X:%.*]])
define i64 @test6(i64 %x) {
; CHECK-NEXT: ret i64 X
   %a = sub i64 9223372036854775807, %x
   %b = xor i64 9223372036854775807, %a
   ret i64 %b
}


; (0x7FFFFFFFFFFFFFFF - x) ⊕ 0x7FFFFFFFFFFFFFFF → x
; CHECK-LABEL: @test7([[X:%.*]])
define i64 @test7(i64 %x) {
; CHECK-NEXT: [[D:%.*]] = tail call i64 @llvm.smax.i64(i64 X, i64 0)
   %a = sub i64 9223372036854775807, %x
   %b = xor i64 %a, 9223372036854775807
   %c = icmp sgt i64 %b, 0
   %d = select i1 %c, i64 %b, i64 0
; CHECK-NEXT: ret i64 D
   ret i64 %d
}


; 0x7FFFFFFFFFFFFFFF ⊕ (0x7FFFFFFFFFFFFFFF - x) → x
; CHECK-LABEL: @test8([[X:%.*]])
define i64 @test8(i64 %x) {
; CHECK-NEXT: [[D:%.*]] = tail call i64 @llvm.smax.i64(i64 X, i64 0)
   %a = sub i64 9223372036854775807, %x
   %b = xor i64 9223372036854775807, %a
   %c = icmp sgt i64 %b, 0
   %d = select i1 %c, i64 %b, i64 0
; CHECK-NEXT: ret i64 D
   ret i64 %d
}
