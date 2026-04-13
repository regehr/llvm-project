; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(ashr(sext_i16_to_i32, 4), i8):
; Chain walk: NarrowWidth=16 != TargetWidth=8 → chain block fails.
; Single-ashr case (line 3262): getExtOperandInfo = SExt with NarrowWidth=16.
; NarrowWidth(16) > TargetWidth(8) → return false at line 3264.
; No transformation.

define i8 @trunc_ashr_sext_wide_nochange(i16 %x) {
  %s = sext i16 %x to i32
  %sh = ashr i32 %s, 4
  %t = trunc i32 %sh to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @trunc_ashr_sext_wide_nochange(
; CHECK:       %s = sext i16 %x to i32
; CHECK:       %sh = ashr i32 %s, 4
; CHECK:       %t = trunc i32 %sh to i8
; CHECK:       ret i8 %t

; trunc(ashr(zext_i8_to_i32, 4), i16):
; Chain walk: NarrowWidth=8 != TargetWidth=16 → chain block fails.
; Single-ashr case (line 3262): LHSInfo = ZExt, NarrowWidth=8.
; NarrowWidth(8) <= TargetWidth(16) → passes line 3263.
; ShiftAmt(4) < TargetWidth(16) AND Kind==ZExt BUT NarrowWidth(8) != TargetWidth(16)
; → ZExt block at line 3266 not entered.
; Kind(ZExt) != SExt → return false at line 3285.
; No transformation.

define i16 @trunc_ashr_zext_nochange(i8 %x) {
  %z = zext i8 %x to i32
  %sh = ashr i32 %z, 4
  %t = trunc i32 %sh to i16
  ret i16 %t
}
; CHECK-LABEL: define i16 @trunc_ashr_zext_nochange(
; CHECK:       %z = zext i8 %x to i32
; CHECK:       %sh = ashr i32 %z, 4
; CHECK:       %t = trunc i32 %sh to i16
; CHECK:       ret i16 %t

; trunc(ashr(sext_i8_to_i32, 4), i16):
; Chain walk: NarrowWidth=8 != TargetWidth=16 → chain block fails.
; Single-ashr case (line 3262): LHSInfo = SExt, NarrowWidth=8.
; NarrowWidth(8) <= TargetWidth(16) → passes.
; Kind==SExt → line 3284 (Kind != SExt → false) → falls through to cost check.
; Cost: 1 added (new sext_i8_to_i16), 2 removed (trunc + sext_i8_to_i32) → passes.
; Result: ashr i16 (sext i8 %x to i16), 4.

define i16 @trunc_ashr_sext_narrow(i8 %x) {
  %s = sext i8 %x to i32
  %sh = ashr i32 %s, 4
  %t = trunc i32 %sh to i16
  ret i16 %t
}
; CHECK-LABEL: define i16 @trunc_ashr_sext_narrow(
; CHECK-NOT:   sext i8 {{.*}} to i32
; CHECK-NOT:   ashr i32
; CHECK-NOT:   trunc i32
; CHECK:       sext i8 %x to i16
; CHECK:       ashr i16
; CHECK:       ret i16
