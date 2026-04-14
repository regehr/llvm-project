; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryWidenSubOverTruncThroughZExtNneg: various negative paths.

; line 1902: Sub has multiple uses → return false.

define i32 @sub_widen_multi_use(i32 %x, ptr %p) {
  %xb = and i32 %x, 127
  %tx = trunc i32 %xb to i8
  %sub = sub i8 %tx, 5
  store i8 %sub, ptr %p
  %wz = zext nneg i8 %sub to i32
  ret i32 %wz
}
; CHECK-LABEL: define i32 @sub_widen_multi_use(
; CHECK: sub i8
; CHECK: zext nneg i8
; CHECK: ret i32

; line 1917: C is negative → C->isNegative() → return false.

define i32 @sub_widen_neg_const(i32 %x) {
  %xb = and i32 %x, 127
  %tx = trunc i32 %xb to i8
  %sub = sub i8 %tx, -5
  %wz = zext nneg i8 %sub to i32
  ret i32 %wz
}
; CHECK-LABEL: define i32 @sub_widen_neg_const(
; CHECK: sub i8
; CHECK: zext nneg i8
; CHECK: ret i32

; line 1921: X (Trunc source) width ≠ WideWidth (16 ≠ 32) → return false.

define i32 @sub_widen_wrong_src_width(i16 %x) {
  %xb = and i16 %x, 127
  %tx = trunc i16 %xb to i8
  %sub = sub i8 %tx, 5
  %wz = zext nneg i8 %sub to i32
  ret i32 %wz
}
; CHECK-LABEL: define i32 @sub_widen_wrong_src_width(
; CHECK: sub i8
; CHECK: zext nneg i8
; CHECK: ret i32

; line 1928: C=128 doesn't fit in MidWidth-1=7 bits → !isIntN(7) → return false.
; X is zero-bounded at 7 bits (passes line 1926), but C fails line 1928.

define i32 @sub_widen_c_too_large(i32 %x) {
  %xb = and i32 %x, 127
  %tx = trunc i32 %xb to i8
  %sub = sub i8 %tx, 128
  %wz = zext nneg i8 %sub to i32
  ret i32 %wz
}
; CHECK-LABEL: define i32 @sub_widen_c_too_large(
; CHECK: sub i8
; CHECK: zext nneg i8
; CHECK: ret i32
