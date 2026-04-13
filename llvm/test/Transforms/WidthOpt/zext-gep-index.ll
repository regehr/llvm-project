; The plain zext-to-GEP-index rewrite is disabled for now: deleting the zext
; here does not reliably produce a strict instruction-count win, because later
; canonical IR can simply materialize a sext instead.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; No-change: even with nneg, width-opt should leave the zext alone.
define ptr @zext_gep_nneg(i8 %x, ptr %base) {
  %zx = zext nneg i8 %x to i32
  %p = getelementptr i8, ptr %base, i32 %zx
  ret ptr %p
}

; CHECK-LABEL: define ptr @zext_gep_nneg(
; CHECK:       zext nneg i8 %x to i32
; CHECK:       getelementptr i8, ptr %base, i32 %zx
; CHECK:       ret ptr

; No-change: same for a structurally zero-bounded source.
define ptr @zext_gep_zero_bounded_structural(i8 %a, ptr %base) {
  %y = and i8 %a, 63
  %zx = zext i8 %y to i32
  %p = getelementptr i8, ptr %base, i32 %zx
  ret ptr %p
}

; CHECK-LABEL: define ptr @zext_gep_zero_bounded_structural(
; CHECK:       %zx = zext nneg i8 %y to i32
; CHECK:       getelementptr i8, ptr %base, i32 %zx

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
