; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryFoldTruncOfExt: TargetWidth < NarrowWidth and NarrowValue is a constant (line 2630).
; trunc(sext(i8 5 to i32), i4): NarrowValue=5 (constant), TargetWidth=4 < NarrowWidth=8.
; convertConstantToNarrow(i8 5, 4) = i4 5 → ret i4 5.

define i4 @trunc_sext_const_narrow() {
  %se = sext i8 5 to i32
  %t = trunc i32 %se to i4
  ret i4 %t
}
; CHECK-LABEL: define i4 @trunc_sext_const_narrow(
; CHECK-NOT:   sext
; CHECK-NOT:   trunc
; CHECK:       ret i4 5

; trunc(zext(i8 200 to i32), i4): NarrowValue=200, TargetWidth=4 < NarrowWidth=8.
; convertConstantToNarrow(i8 200, 4) = i4 8 (200 & 0xF = 8).

define i4 @trunc_zext_const_narrow() {
  %ze = zext i8 200 to i32
  %t = trunc i32 %ze to i4
  ret i4 %t
}
; CHECK-LABEL: define i4 @trunc_zext_const_narrow(
; CHECK-NOT:   zext
; CHECK-NOT:   trunc
; CHECK:       ret i4
