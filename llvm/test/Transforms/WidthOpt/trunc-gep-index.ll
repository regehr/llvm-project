; tryShrinkTruncGEPIndex: when trunc iM X to iN is used only as GEP index
; operands and X is provably zero-bounded at NarrowBits-1 bits (i.e., X < 2^(N-1)),
; replace each GEP's use of the trunc with the wider X and erase the trunc.
; GEP sign-extends indices, and sext(trunc(X)) == X when X < 2^(N-1).
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Basic case: trunc i32 -> i8 used as GEP index.
; %x = and i32 %n, 63  → x ∈ [0,63] ⊂ [0,127), zero-bounded at 7 bits.
; sext(trunc(x, i8), i32) == x, so the GEP can use x directly.
define ptr @trunc_gep_basic(ptr %base, i32 %n) {
  %x = and i32 %n, 63
  %idx = trunc i32 %x to i8
  %p = getelementptr i8, ptr %base, i8 %idx
  ret ptr %p
}

; CHECK-LABEL: define ptr @trunc_gep_basic(
; CHECK:       %x = and i32 %n, 63
; CHECK-NOT:   trunc
; CHECK:       getelementptr i8, ptr %base, i32 %x
; CHECK:       ret ptr

; Multiple GEP uses of the same trunc.
define void @trunc_gep_multi(ptr %a, ptr %b, i32 %n, ptr %out0, ptr %out1) {
  %x = and i32 %n, 63
  %idx = trunc i32 %x to i8
  %p0 = getelementptr i8, ptr %a, i8 %idx
  %p1 = getelementptr i8, ptr %b, i8 %idx
  %v0 = load i8, ptr %p0
  %v1 = load i8, ptr %p1
  store i8 %v0, ptr %out0
  store i8 %v1, ptr %out1
  ret void
}

; CHECK-LABEL: define void @trunc_gep_multi(
; CHECK-NOT:   trunc
; CHECK-DAG:   getelementptr i8, ptr %a, i32 %x
; CHECK-DAG:   getelementptr i8, ptr %b, i32 %x

; No-change: trunc also has a non-GEP use (stored to memory).
define ptr @trunc_gep_nochange_extra_use(ptr %base, i32 %n, ptr %out) {
  %x = and i32 %n, 63
  %idx = trunc i32 %x to i8
  store i8 %idx, ptr %out
  %p = getelementptr i8, ptr %base, i8 %idx
  ret ptr %p
}

; CHECK-LABEL: define ptr @trunc_gep_nochange_extra_use(
; CHECK:       trunc i32 %x to i8
; CHECK:       getelementptr i8, ptr %base, i8 %idx

; No-change: source is not zero-bounded at NarrowBits-1=7 bits (could be >= 128).
define ptr @trunc_gep_nochange_not_bounded(ptr %base, i32 %n) {
  %idx = trunc i32 %n to i8
  %p = getelementptr i8, ptr %base, i8 %idx
  ret ptr %p
}

; CHECK-LABEL: define ptr @trunc_gep_nochange_not_bounded(
; CHECK:       trunc i32 %n to i8
; CHECK:       getelementptr i8, ptr %base, i8 %idx
