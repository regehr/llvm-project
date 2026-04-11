; tryShrinkSExtSwitch: when sext iN x to iM is used only as a switch condition,
; replace the condition with the narrow x, truncate in-range case constants to iN,
; and remove cases outside the signed iN range (unreachable via sext).
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Basic case: sext i8 -> i32, two reachable cases + one out-of-range case.
; sext i8 produces values in [-128, 127].
; Case 200 has trunc(200 to i8) = -56 and sext(-56 to i32) = -56 ≠ 200 → unreachable.
define i32 @sext_switch_basic(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  switch i32 %sx, label %default [
    i32 42,  label %case42
    i32 -10, label %caseneg
    i32 200, label %case200
  ]
case42:
  ret i32 42
caseneg:
  ret i32 -10
case200:
  ret i32 200
default:
  ret i32 0
}

; CHECK-LABEL: define i32 @sext_switch_basic(
; CHECK-NOT:   sext i8 %x to i32
; CHECK:       switch i8 %x, label %default [
; CHECK-NOT:   i32 200
; CHECK-DAG:   i8 42, label %case42
; CHECK-DAG:   i8 -10, label %caseneg
; CHECK:       ]

; No-change: sext also has a non-switch use, so the transformation must not fire.
define i32 @sext_switch_nochange_extra_use(i8 %x, ptr %p) {
entry:
  %sx = sext i8 %x to i32
  store i32 %sx, ptr %p
  switch i32 %sx, label %default [
    i32 42, label %case42
  ]
case42:
  ret i32 42
default:
  ret i32 0
}

; CHECK-LABEL: define i32 @sext_switch_nochange_extra_use(
; CHECK:       sext i8 %x to i32
; CHECK:       switch i32 %sx, label %default [
