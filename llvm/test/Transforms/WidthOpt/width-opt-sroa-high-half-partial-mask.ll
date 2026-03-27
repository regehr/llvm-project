; RUN: opt -passes='width-opt,verify' -S %s | FileCheck %s

; Regression test: tryShrinkHighHalfSROA must NOT fire when the HIGH_MASK does
; not have all ones in the upper N bits.  The original check only verified that
; the lower N bits are zero, but a partial upper mask (e.g. 0x0F00) loses some
; of the high half, making the result hi & 0x0F rather than hi.
;
; alive-tv counterexample: %hi = 0x10 (16), %lo = any
;   Source: masked = combined & 0x0F00 = 0x1000 & 0x0F00 = 0x0000 → result = 0
;   (Incorrect) Target: ret %hi = 16
;
; The fix also checks that HIGH_MASK[N..2N-1] == all-ones before substituting.

; --- Negative case: partial upper mask 0x0F00 → must NOT narrow ---

define i8 @high_half_partial_mask(i8 %hi, i8 %lo) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  ; 3840 = 0x0F00: zeros in bits 0-7, ones only in bits 8-11, zeros in bits 12-15
  %masked = and i16 %combined, 3840
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}

; CHECK-LABEL: define i8 @high_half_partial_mask(
; The full SROA chain must survive; the result must not become %hi directly.
; CHECK: and i16
; CHECK: lshr i16
; CHECK: trunc i16
; CHECK: ret i8
; CHECK-NOT: ret i8 %hi

; --- Positive case: full upper mask 0xFF00 → should narrow ---

define i8 @high_half_full_mask(i8 %hi, i8 %lo) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  ; 65280 = 0xFF00: zeros in bits 0-7, all ones in bits 8-15
  %masked = and i16 %combined, 65280
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}

; CHECK-LABEL: define i8 @high_half_full_mask(
; With a full mask the transformation is correct: result == %hi.
; CHECK: ret i8 %hi
