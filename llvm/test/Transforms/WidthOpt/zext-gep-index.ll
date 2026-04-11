; tryShrinkZExtGEPIndex: when zext iN x to iM (with nneg flag, or provably
; non-negative) is used only as GEP index operands, replace those uses with
; the narrow x and erase the zext. Valid because GEP sign-extends indices, and
; nneg/zero-bounded guarantees sext(x) == zext(x).
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; With nneg flag: straightforward proof that sext == zext for the index.
define ptr @zext_gep_nneg(i8 %x, ptr %base) {
  %zx = zext nneg i8 %x to i32
  %p = getelementptr i8, ptr %base, i32 %zx
  ret ptr %p
}

; CHECK-LABEL: define ptr @zext_gep_nneg(
; CHECK-NOT:   zext
; CHECK:       getelementptr i8, ptr %base, i8 %x
; CHECK:       ret ptr

; Without nneg but structurally zero-bounded: and with a 6-bit constant
; makes the source fit in 6 bits < 2^7, so zero-bounded at 7 bits (NarrowBits-1).
define ptr @zext_gep_zero_bounded_structural(i8 %a, ptr %base) {
  %y = and i8 %a, 63
  %zx = zext i8 %y to i32
  %p = getelementptr i8, ptr %base, i32 %zx
  ret ptr %p
}

; CHECK-LABEL: define ptr @zext_gep_zero_bounded_structural(
; CHECK-NOT:   zext
; CHECK:       getelementptr i8, ptr %base, i8 %y

; No-change: zext also used outside the GEP.
define ptr @zext_gep_nochange_extra_use(i8 %x, ptr %base, ptr %out) {
  %zx = zext nneg i8 %x to i32
  store i32 %zx, ptr %out
  %p = getelementptr i8, ptr %base, i32 %zx
  ret ptr %p
}

; CHECK-LABEL: define ptr @zext_gep_nochange_extra_use(
; CHECK:       zext nneg i8 %x to i32
; CHECK:       getelementptr i8, ptr %base, i32 %zx
