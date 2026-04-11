; tryShrinkSExtGEPIndex: when sext iN x to iM is used only as GEP index
; operands (not the base pointer), replace those uses with the narrow x and
; erase the sext. GEP sign-extends indices anyway, so the result is identical.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Single GEP use: sext i8 -> i32 used as index into byte array.
define ptr @sext_gep_single(i8 %x, ptr %base) {
  %sx = sext i8 %x to i32
  %p = getelementptr i8, ptr %base, i32 %sx
  ret ptr %p
}

; CHECK-LABEL: define ptr @sext_gep_single(
; CHECK-NOT:   sext
; CHECK:       getelementptr i8, ptr %base, i8 %x
; CHECK:       ret ptr

; Multiple GEP uses of the same sext: both should be updated.
define void @sext_gep_multi(i8 %x, ptr %a, ptr %b, ptr %out0, ptr %out1) {
  %sx = sext i8 %x to i32
  %p0 = getelementptr i8, ptr %a, i32 %sx
  %p1 = getelementptr i8, ptr %b, i32 %sx
  %v0 = load i8, ptr %p0
  %v1 = load i8, ptr %p1
  store i8 %v0, ptr %out0
  store i8 %v1, ptr %out1
  ret void
}

; CHECK-LABEL: define void @sext_gep_multi(
; CHECK-NOT:   sext
; CHECK-DAG:   getelementptr i8, ptr %a, i8 %x
; CHECK-DAG:   getelementptr i8, ptr %b, i8 %x

; No-change: sext also used for something other than a GEP index.
define ptr @sext_gep_nochange_extra_use(i8 %x, ptr %base, ptr %out) {
  %sx = sext i8 %x to i32
  store i32 %sx, ptr %out
  %p = getelementptr i8, ptr %base, i32 %sx
  ret ptr %p
}

; CHECK-LABEL: define ptr @sext_gep_nochange_extra_use(
; CHECK:       sext i8 %x to i32
; CHECK:       getelementptr i8, ptr %base, i32 %sx
