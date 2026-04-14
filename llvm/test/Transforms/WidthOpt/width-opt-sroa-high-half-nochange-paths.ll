; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryShrinkHighHalfSROA: various negative paths.

; line 2956: lshr source is not an And BinaryOperator — MaskBO=nullptr → return false.

define i8 @sroa_high_half_no_and(i16 %x) {
  %shr = lshr i16 %x, 8
  %t = trunc i16 %shr to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @sroa_high_half_no_and(
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8

; lines 2963+2971: And mask low N bits non-zero → continue both Idx → V=null → return false.
; mask = -255 = 0xFF01 in i16: MaskVal.trunc(8) = 0x01 ≠ 0 → fail.

define i8 @sroa_high_half_bad_low_mask(i8 %hi, i8 %lo) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -255
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
; CHECK-LABEL: define i8 @sroa_high_half_bad_low_mask(
; CHECK: and i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8

; line 2976: mask passes (0xFF00) but V (the other And operand) is not an Or → return false.

define i8 @sroa_high_half_v_not_or(i16 %x) {
  %masked = and i16 %x, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
; CHECK-LABEL: define i8 @sroa_high_half_v_not_or(
; CHECK: and i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8

; lines 2984+3000: Or has no BinaryOperator arm → dyn_cast<BinaryOperator> fails both → HiVal=null.

define i8 @sroa_high_half_no_shl_in_or(i16 %x, i16 %y) {
  %or_val = or i16 %x, %y
  %masked = and i16 %or_val, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
; CHECK-LABEL: define i8 @sroa_high_half_no_shl_in_or(
; CHECK: or i16
; CHECK: and i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8

; line 2984 (hasOneUse): Shl has two uses → !ShlBO->hasOneUse() → continue.

define i8 @sroa_shl_multi_use(i8 %hi, i8 %lo, ptr %p) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 8
  store i16 %hi_shifted, ptr %p
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
; CHECK-LABEL: define i8 @sroa_shl_multi_use(
; CHECK: shl i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8

; line 2987: Shl exists but ShlAmt != TargetWidth → continue.

define i8 @sroa_shl_wrong_amount(i8 %hi, i8 %lo) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 7
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
; CHECK-LABEL: define i8 @sroa_shl_wrong_amount(
; CHECK: shl i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8

; line 2991: ZE src width ≠ TargetWidth → continue.
; zext i4 to i16 as hi: SrcTy width = 4 ≠ 8 = TargetWidth.

define i8 @sroa_ze_wrong_src_width(i4 %hi, i8 %lo) {
  %lo_wide = zext i8 %lo to i16
  %hi_wide = zext i4 %hi to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
; CHECK-LABEL: define i8 @sroa_ze_wrong_src_width(
; CHECK: shl i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8

; line 2991 (!ZE->hasOneUse()): ZExt has two uses → continue.

define i8 @sroa_ze_multi_use(i8 %hi, i8 %lo, ptr %p) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  store i16 %hi_wide, ptr %p
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
; CHECK-LABEL: define i8 @sroa_ze_multi_use(
; CHECK: shl i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8

; line 2995: LowPart not zero-bounded at TargetWidth → continue.
; %lo is i16 (can have bits 8-15 set) → isZeroBoundedAtWidth(%lo, 8) = false.

define i8 @sroa_low_part_not_bounded(i8 %hi, i16 %lo) {
  %hi_wide = zext i8 %hi to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
; CHECK-LABEL: define i8 @sroa_low_part_not_bounded(
; CHECK: shl i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8
