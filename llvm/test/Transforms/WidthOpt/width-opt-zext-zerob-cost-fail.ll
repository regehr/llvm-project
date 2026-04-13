; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; zext i32 (and i16 %x, (zext i8 %a to i16)) to i32:
; tryShrinkZExtOfZeroBounded: getStructuralNarrowWidth = 8 (from zext_i8 side).
; collectTruncRootedValueCost: AddedValues={%x(leaf needing trunc), and_i16},
;   RemovedInstructions={zext_i8_to_i16, and_i16}.
; AddedValues.size()(2) >= RemovedInstructions.size()(2) → cost fails (line 2476).
; No transformation.

define i32 @zext_and_wide_arg_nochange(i8 %a, i16 %x) {
  %za = zext i8 %a to i16
  %and = and i16 %x, %za
  %z = zext i16 %and to i32
  ret i32 %z
}
; CHECK-LABEL: define i32 @zext_and_wide_arg_nochange(
; CHECK:       and i16
; CHECK:       zext {{.*}}i16 %and to i32
; CHECK:       ret i32
