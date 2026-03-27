; RUN: opt -passes='width-opt,verify' -S %s | FileCheck %s

; Regression test: zext nneg of a narrow subtraction must not be widened just
; from the result's nneg bit.  Wrapped narrow subtractions can still end in the
; non-negative half of the narrow type.
;
; alive-tv counterexample for @dont_widen_wrapped_sub:
;   %x = 253
;   narrow:  trunc(253) = 253, sub i8 0, 253 = 3, zext nneg -> 3
;   widened: sub i16 0, 253 = -253

define i16 @dont_widen_wrapped_sub(i16 %x) {
entry:
  %xb = and i16 %x, 255
  %t = trunc i16 %xb to i8
  %sub = sub i8 0, %t
  %z = zext nneg i8 %sub to i16
  ret i16 %z
}

; CHECK-LABEL: define i16 @dont_widen_wrapped_sub(
; CHECK: trunc i16 %x to i8
; CHECK: sub i8 0, %t1
; CHECK: zext nneg i8 %sub to i16
; CHECK-NOT: sub i16 0,
; CHECK: ret i16 %z

define i16 @widen_nonneg_sub(i16 %x) {
entry:
  %xb = and i16 %x, 127
  %t = trunc i16 %xb to i8
  %sub = sub i8 %t, 5
  %z = zext nneg i8 %sub to i16
  ret i16 %z
}

; CHECK-LABEL: define i16 @widen_nonneg_sub(
; CHECK: and i16 %x, 127
; CHECK: sub i16 %xb, 5
; CHECK-NOT: trunc i16 %xb to i8
; CHECK-NOT: sub i8 %t, 5
; CHECK-NOT: zext nneg i8 %sub to i16
; CHECK: ret i16
