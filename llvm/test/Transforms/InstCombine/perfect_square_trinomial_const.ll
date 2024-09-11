; RUN: opt < %s -O2 -S | FileCheck %s

; positive tests

define i16 @test1(i16 %x) {
        %tmp1 = mul i16 %x, %x
        %tmp2 = mul i16 %x, 2
        %tmp3 = add i16 %tmp1, %tmp2
        %tmp4 = add i16 %tmp3, 1
        ret i16 %tmp4
; CHECK-LABEL: @test1(
; CHECK-NEXT: add i16 %x, 1
; CHECK-NEXT: mul i16 %1, %1
; CHECK-NEXT: ret i16
}

define i32 @test2(i32 %x) {
        %tmp1 = mul i32 %x, %x
        %tmp2 = mul i32 %x, 2
        %tmp3 = add i32 %tmp1, %tmp2
        %tmp4 = add i32 %tmp3, 1
        ret i32 %tmp4
; CHECK-LABEL: @test2(
; CHECK-NEXT: add i32 %x, 1
; CHECK-NEXT: mul i32 %1, %1
; CHECK-NEXT: ret i32
}

define i64 @test3(i64 %x) {
        %tmp1 = mul i64 %x, %x
        %tmp2 = mul i64 %x, 2
        %tmp3 = add i64 %tmp1, %tmp2
        %tmp4 = add i64 %tmp3, 1
        ret i64 %tmp4
; CHECK-LABEL: @test3(
; CHECK-NEXT: add i64 %x, 1
; CHECK-NEXT: mul i64 %1, %1
; CHECK-NEXT: ret i64
}

; replace mul x, 2 with shl x, 1
define i64 @test4(i64 %x) {
        %tmp1 = mul i64 %x, %x
        %tmp2 = shl i64 %x, 1
        %tmp3 = add i64 %tmp1, %tmp2
        %tmp4 = add i64 %tmp3, 1
        ret i64 %tmp4
; CHECK-LABEL: @test4(
; CHECK-NEXT: add i64 %x, 1
; CHECK-NEXT: mul i64 %1, %1
; CHECK-NEXT: ret i64
}

; exchange order of terms
define i32 @test6(i32 %x) {
        %tmp1 = mul i32 %x, %x
        %tmp2 = add i32 %tmp1, 1
        %tmp3 = shl i32 %x, 1
        %tmp4 = add i32 %tmp2, %tmp3
        ret i32 %tmp4
; CHECK-LABEL: @test6(
; CHECK-NEXT: add i32 %x, 1
; CHECK-NEXT: mul i32 %1, %1
; CHECK-NEXT: ret i32 
}

; negative tests

define i16 @test7(i16 %x) {
        %tmp1 = mul i16 %x, %x
        %tmp2 = mul i16 %x, 2
        %tmp3 = add i16 %tmp1, %tmp2
        %tmp4 = add i16 %tmp3, 2
        ret i16 %tmp4
; CHECK-LABEL: @test7(
; CHECK-NEXT: add i16 %x, 2
; CHECK-NEXT: mul i16 %tmp21, %x
; CHECK-NEXT: add i16 %tmp3, 2
; CHECK-NEXT: ret i16
}

define i32 @test8(i32 %x) {
        %tmp1 = mul i32 %x, %x
        %tmp2 = mul i32 %x, 3
        %tmp3 = add i32 %tmp1, %tmp2
        %tmp4 = add i32 %tmp3, 1
        ret i32 %tmp4
; CHECK-LABEL: @test8(
; CHECK-NEXT: add i32 %x, 3
; CHECK-NEXT: mul i32 %tmp21, %x
; CHECK-NEXT: add i32 %tmp3, 1
; CHECK-NEXT: ret i32
}

define i64 @test9(i64 %x) {
        %tmp1 = mul i64 %x, %x
        %tmp2 = mul i64 %x, 3
        %tmp3 = add i64 %tmp1, %tmp2
        %tmp4 = add i64 %tmp3, 4
        ret i64 %tmp4
; CHECK-LABEL: @test9(
; CHECK-NEXT: add i64 %x, 3
; CHECK-NEXT: mul i64 %tmp21, %x
; CHECK-NEXT: add i64 %tmp3, 4
; CHECK-NEXT: ret i64
}


