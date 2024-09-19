; RUN: opt < %s -passes='instcombine' -S | FileCheck %s

; Positive test cases

define i16 @src48(i16 %x) {
    ; CHECK-LABEL: define i16 @src48(
    ; CHECK:       [[result:%.*]] = add i16 [[x:%.*]], 32767
    ; CHECK-NEXT:  ret i16 [[result]]
    %x2 = sub i16 0, %x
    %result = xor i16 %x2, 32767
    ret i16 %result
}


; Negative test cases
define i16 @src49(i16 %x) {
    ; CHECK-LABEL: define i16 @src49(
    ; CHECK:       [[x2:%.*]] = sub i16 1, [[x:%.*]]
    ; CHECK:       [[result:%.*]] = xor i16 [[x2:%.*]], 32767
    ; CHECK-NEXT:  ret i16 [[result]]
    %x2 = sub i16 1, %x
    %result = xor i16 %x2, 32767
    ret i16 %result
}
