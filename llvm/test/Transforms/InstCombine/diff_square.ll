; RUN: opt < %s -passes='instcombine' -S | FileCheck %s

; Positive test cases

define i16 @diff_square_i16(i16 %x) {
    ; CHECK-LABEL: define i16 @diff_square_i16(
    ; CHECK:       [[x2:%.*]] = mul i16 [[x:%.*]], [[x]]
    ; CHECK-NEXT:  [[x4:%.*]] = mul i16 [[x2]], [[x2]]
    ; CHECK-NEXT:  [[result:%.*]] = add i16 [[x4]], -1
    ; CHECK-NEXT:  ret i16 [[result]]
    %x2 = mul i16 %x, %x
    %x2_plus_1 = add i16 %x2, 1
    %x2_minus_1 = sub i16 %x2, 1
    %result = mul i16 %x2_plus_1, %x2_minus_1
    ret i16 %result
}

define i32 @diff_square_i32(i32 %x) {
    ; CHECK-LABEL: define i32 @diff_square_i32(
    ; CHECK:       [[x2:%.*]] = mul i32 [[x:%.*]], [[x]]
    ; CHECK-NEXT:  [[x4:%.*]] = mul i32 [[x2]], [[x2]]
    ; CHECK-NEXT:  [[result:%.*]] = add i32 [[x4]], -1
    ; CHECK-NEXT:  ret i32 [[result]]
    %x2 = mul i32 %x, %x 
    %x2_plus_1 = add i32 %x2, 1
    %x2_minus_1 = sub i32 %x2, 1
    %result = mul i32 %x2_plus_1, %x2_minus_1
    ret i32 %result
}

define i64 @diff_square_i64(i64 %x) {
    ; CHECK-LABEL: define i64 @diff_square_i64(
    ; CHECK:       [[x2:%.*]] = mul i64 [[x:%.*]], [[x]]
    ; CHECK-NEXT:  [[x4:%.*]] = mul i64 [[x2]], [[x2]]
    ; CHECK-NEXT:  [[result:%.*]] = add i64 [[x4]], -1
    ; CHECK-NEXT:  ret i64 [[result]]
    %x2 = mul i64 %x, %x
    %x2_plus_1 = add i64 %x2, 1
    %x2_minus_1 = sub i64 %x2, 1
    %result = mul i64 %x2_plus_1, %x2_minus_1
    ret i64 %result
}

; Negative test case

define float @diff_square_f32(float %x) {
; CHECK-LABEL: define float @diff_square_f32(
; CHECK:       [[x2:%.*]] = fmul float [[x:%.*]], [[x]]
; CHECK-NEXT:  [[x2_plus_1:%.*]] = fadd float [[x2]], 1.0
; CHECK-NEXT:  [[x2_minus_1:%.*]] = fadd float [[x2]], -1.0
; CHECK-NEXT:  [[result:%.*]] = fmul float [[x2_plus_1]], [[x2_minus_1]]
; CHECK-NEXT:  ret float [[result]]

  %x2 = fmul float %x, %x          ; x^2
  %x2_plus_1 = fadd float %x2, 1.0 ; x^2 + 1
  %x2_minus_1 = fsub float %x2, 1.0; x^2 - 1
  %result = fmul float %x2_plus_1, %x2_minus_1 ; (x^2 + 1)(x^2 - 1)
  ret float %result
}
