
; RUN: opt -O2 < %s -S | FileCheck %s

; Positive test
; Reverse 8 bits
define i8 @expected_optimization(i8 %0) {
; CHECK-LABEL: @expected_optimization(
; CHECK-NEXT:    [[TMP2:%.*]] = zext i8 [[TMP0:%.*]] to i64
; CHECK-NEXT:    [[TMP3:%.*]] = mul nuw nsw i64 [[TMP2]], 2149582850
; CHECK-NEXT:    [[TMP4:%.*]] = and i64 [[TMP3]], 36578664720
; CHECK-NEXT:    [[TMP5:%.*]] = mul i64 [[TMP4]], 4311810305
; CHECK-NEXT:    [[TMP9:%.*]] = lshr i64 [[TMP5]], 32
; CHECK-NEXT:    [[TMP10:%.*]] = trunc i64 [[TMP9]] to i8
; CHECK-NEXT:    ret i8 [[TMP10]]
;
  %2 = zext i8 %0 to i64
  %3 = mul nuw nsw i64 %2, 2050
  %4 = and i64 %3, 139536
  %5 = mul nuw nsw i64 %2, 32800
  %6 = and i64 %5, 558144
  %7 = or i64 %4, %6
  %8 = mul nuw nsw i64 %7, 65793
  %9 = lshr i64 %8, 16
  %10 = trunc i64 %9 to i8
  ret i8 %10
}

; Negative test
; Make sure it doesn't use the longer optimization when extension is i64
; ie we dont want: bitreverse8(x) = (((x * 0x0802LU) & 0x22110LU) | ((x * 0x8020LU) & 0x88440LU)) * 0x10101LU >> 16
define i8 @rev8_mul_and_lshr(i8 %0) {
; CHECK-LABEL: @rev8_mul_and_lshr(
; CHECK-NEXT:    [[TMP2:%.*]] = zext i8 [[TMP0:%.*]] to i64
; CHECK-NOT:    [[TMP3:%.*]] = mul nuw nsw i64 [[TMP2]], 2050
; CHECK-NOT:    [[TMP4:%.*]] = and i64 [[TMP3]], 139536
; CHECK-NOT:    [[TMP5:%.*]] = mul nuw nsw i64 [[TMP2]], 32800
; CHECK-NOT:    [[TMP6:%.*]] = and i64 [[TMP5]], 558144
;
  %2 = zext i8 %0 to i64
  %3 = mul nuw nsw i64 %2, 2050
  %4 = and i64 %3, 139536
  %5 = mul nuw nsw i64 %2, 32800
  %6 = and i64 %5, 558144
  %7 = or i64 %4, %6
  %8 = mul nuw nsw i64 %7, 65793
  %9 = lshr i64 %8, 16
  %10 = trunc i64 %9 to i8
  ret i8 %10
}

; Negative test
; Enforce our optimization only gets triggered when extension is i64
define i8 @wrong_bit_extension(i8 %0) {
; CHECK-LABEL: @wrong_bit_extension(
; CHECK-NEXT:    [[TMP2:%.*]] = zext i8 [[TMP0:%.*]] to i32
; CHECK-NOT:    [[TMP3:%.*]] = mul nuw nsw i32 [[TMP2]], 2149582850
; CHECK-NOT:    [[TMP4:%.*]] = and i32 [[TMP3]], 36578664720
; CHECK-NOT:    [[TMP5:%.*]] = mul i32 [[TMP4]], 4311810305
; CHECK-NOT:    [[TMP9:%.*]] = lshr i32 [[TMP5]], 32
; CHECK-NOT:    [[TMP10:%.*]] = trunc i32 [[TMP9]] to i8
; CHECK-NOT:    ret i8 [[TMP10]]
;
  %2 = zext i8 %0 to i32
  %3 = mul nuw nsw i32 %2, 2050
  %4 = and i32 %3, 139536
  %5 = mul nuw nsw i32 %2, 32800
  %6 = and i32 %5, 558144
  %7 = or i32 %4, %6
  %8 = mul nuw nsw i32 %7, 65793
  %9 = lshr i32 %8, 16
  %10 = trunc i32 %9 to i8
  ret i8 %10
}
