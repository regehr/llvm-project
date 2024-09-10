; RUN: opt < %s -passes=instcombine -S | FileCheck %s

define i8 @test-i8-pos1(i8 %x) {
    %a = or i8 %x, 127
    %b = sub i8 -2, %a
    ret i8 %b
    ; CHECK-LABEL: @test-i8-pos1(
    ; CHECK-NEXT:  [[p1:%.*]] = or i8 %x, 127
    ; CHECK-NEXT:  ret i8 [[p1]]
}

define i8 @test-i8-pos2(i8 %x) {
    %a = or i8 127, %x
    %b = sub i8 -2, %a
    ret i8 %b
    ; CHECK-LABEL: @test-i8-pos2(
    ; CHECK-NEXT:  [[p1:%.*]] = or i8 %x, 127
    ; CHECK-NEXT:  ret i8 [[p1]]
}

define i8 @test-i8-neg1(i8 %x) {
    %a = or i8 %x, 126
    %b = sub i8 -2, %a
    ret i8 %b
    ; CHECK-LABEL: @test-i8-neg1(
    ; CHECK-NEXT:  [[p1:%.*]] = or i8 %x, 126
    ; CHECK-NEXT:  [[p2:%.*]] = sub i8 -2, [[p1]]
    ; CHECK-NEXT:  ret i8 [[p2]]
}

define i8 @test-i8-neg2(i8 %x) {
    %a = or i8 126, %x
    %b = sub i8 -2, %a
    ret i8 %b
    ; CHECK-LABEL: @test-i8-neg2(
    ; CHECK-NEXT:  [[p1:%.*]] = or i8 %x, 126
    ; CHECK-NEXT:  [[p2:%.*]] = sub i8 -2, [[p1]]
    ; CHECK-NEXT:  ret i8 [[p2]]
}

define i8 @test-i8-neg3(i8 %x) {
    %a = or i8 %x, 127
    %b = sub i8 0, %a
    ret i8 %b
    ; CHECK-LABEL: @test-i8-neg3(
    ; CHECK-NEXT:  [[p1:%.*]] = or i8 %x, 127
    ; CHECK-NEXT:  [[p2:%.*]] = sub nsw i8 0, [[p1]]
    ; CHECK-NEXT:  ret i8 [[p2]]
}

define i16 @test-i16-pos1(i16 %x) {
    %a = or i16 %x, 32767
    %b = sub i16 -2, %a
    ret i16 %b
    ; CHECK-LABEL: @test-i16-pos1(
    ; CHECK-NEXT:  [[p1:%.*]] = or i16 %x, 32767
    ; CHECK-NEXT:  ret i16 [[p1]]
}

define i16 @test-i16-pos2(i16 %x) {
    %a = or i16 32767, %x
    %b = sub i16 -2, %a
    ret i16 %b
    ; CHECK-LABEL: @test-i16-pos2(
    ; CHECK-NEXT:  [[p1:%.*]] = or i16 %x, 32767
    ; CHECK-NEXT:  ret i16 [[p1]]
}

define i16 @test-i16-neg1(i16 %x) {
    %a = or i16 %x, 32766
    %b = sub i16 -2, %a
    ret i16 %b
    ; CHECK-LABEL: @test-i16-neg1(
    ; CHECK-NEXT:  [[p1:%.*]] = or i16 %x, 32766
    ; CHECK-NEXT:  [[p2:%.*]] = sub i16 -2, [[p1]]
    ; CHECK-NEXT:  ret i16 [[p2]]
}

define i16 @test-i16-neg2(i16 %x) {
    %a = or i16 32766, %x
    %b = sub i16 -2, %a
    ret i16 %b
    ; CHECK-LABEL: @test-i16-neg2(
    ; CHECK-NEXT:  [[p1:%.*]] = or i16 %x, 32766
    ; CHECK-NEXT:  [[p2:%.*]] = sub i16 -2, [[p1]]
    ; CHECK-NEXT:  ret i16 [[p2]]
}

define i16 @test-i16-neg3(i16 %x) {
    %a = or i16 %x, 32767
    %b = sub i16 0, %a
    ret i16 %b
    ; CHECK-LABEL: @test-i16-neg3(
    ; CHECK-NEXT:  [[p1:%.*]] = or i16 %x, 32767
    ; CHECK-NEXT:  [[p2:%.*]] = sub nsw i16 0, [[p1]]
    ; CHECK-NEXT:  ret i16 [[p2]]
}

define i32 @test-i32-pos1(i32 %x) {
    %a = or i32 %x, 2147483647
    %b = sub i32 -2, %a
    ret i32 %b
    ; CHECK-LABEL: @test-i32-pos1(
    ; CHECK-NEXT:  [[p1:%.*]] = or i32 %x, 2147483647
    ; CHECK-NEXT:  ret i32 [[p1]]
}

define i32 @test-i32-pos2(i32 %x) {
    %a = or i32 2147483647, %x
    %b = sub i32 -2, %a
    ret i32 %b
    ; CHECK-LABEL: @test-i32-pos2(
    ; CHECK-NEXT:  [[p1:%.*]] = or i32 %x, 2147483647
    ; CHECK-NEXT:  ret i32 [[p1]]
}

define i32 @test-i32-neg1(i32 %x) {
    %a = or i32 %x, 2147483646
    %b = sub i32 -2, %a
    ret i32 %b
    ; CHECK-LABEL: @test-i32-neg1(
    ; CHECK-NEXT:  [[p1:%.*]] = or i32 %x, 2147483646
    ; CHECK-NEXT:  [[p2:%.*]] = sub i32 -2, [[p1]]
    ; CHECK-NEXT:  ret i32 [[p2]]
}

define i32 @test-i32-neg2(i32 %x) {
    %a = or i32 2147483646, %x
    %b = sub i32 -2, %a
    ret i32 %b
    ; CHECK-LABEL: @test-i32-neg2(
    ; CHECK-NEXT:  [[p1:%.*]] = or i32 %x, 2147483646
    ; CHECK-NEXT:  [[p2:%.*]] = sub i32 -2, [[p1]]
    ; CHECK-NEXT:  ret i32 [[p2]]
}

define i32 @test-i32-neg3(i32 %x) {
    %a = or i32 %x, 2147483647
    %b = sub i32 0, %a
    ret i32 %b
    ; CHECK-LABEL: @test-i32-neg3(
    ; CHECK-NEXT:  [[p1:%.*]] = or i32 %x, 2147483647
    ; CHECK-NEXT:  [[p2:%.*]] = sub nsw i32 0, [[p1]]
    ; CHECK-NEXT:  ret i32 [[p2]]
}

define i64 @test-i64-pos1(i64 %x) {
    %a = or i64 %x, 9223372036854775807
    %b = sub i64 -2, %a
    ret i64 %b
    ; CHECK-LABEL: @test-i64-pos1(
    ; CHECK-NEXT:  [[p1:%.*]] = or i64 %x, 9223372036854775807
    ; CHECK-NEXT:  ret i64 [[p1]]
}

define i64 @test-i64-pos2(i64 %x) {
    %a = or i64 9223372036854775807, %x
    %b = sub i64 -2, %a
    ret i64 %b
    ; CHECK-LABEL: @test-i64-pos2(
    ; CHECK-NEXT:  [[p1:%.*]] = or i64 %x, 9223372036854775807
    ; CHECK-NEXT:  ret i64 [[p1]]
}

define i64 @test-i64-neg1(i64 %x) {
    %a = or i64 %x, 9223372036854775806
    %b = sub i64 -2, %a
    ret i64 %b
    ; CHECK-LABEL: @test-i64-neg1(
    ; CHECK-NEXT:  [[p1:%.*]] = or i64 %x, 9223372036854775806
    ; CHECK-NEXT:  [[p2:%.*]] = sub i64 -2, [[p1]]
    ; CHECK-NEXT:  ret i64 [[p2]]
}

define i64 @test-i64-neg2(i64 %x) {
    %a = or i64 9223372036854775806, %x
    %b = sub i64 -2, %a
    ret i64 %b
    ; CHECK-LABEL: @test-i64-neg2(
    ; CHECK-NEXT:  [[p1:%.*]] = or i64 %x, 9223372036854775806
    ; CHECK-NEXT:  [[p2:%.*]] = sub i64 -2, [[p1]]
    ; CHECK-NEXT:  ret i64 [[p2]]
}

define i64 @test-i64-neg3(i64 %x) {
    %a = or i64 %x, 9223372036854775807
    %b = sub i64 0, %a
    ret i64 %b
    ; CHECK-LABEL: @test-i64-neg3(
    ; CHECK-NEXT:  [[p1:%.*]] = or i64 %x, 9223372036854775807
    ; CHECK-NEXT:  [[p2:%.*]] = sub nsw i64 0, [[p1]]
    ; CHECK-NEXT:  ret i64 [[p2]]
}
