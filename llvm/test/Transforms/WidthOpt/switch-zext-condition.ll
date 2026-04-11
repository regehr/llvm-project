; tryShrinkZExtSwitch: when zext iN x to iM is used only as a switch condition,
; replace the condition with the narrow x, truncate in-range case constants to iN,
; and remove cases > MaxNarrow (unreachable since zext cannot produce them).
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Basic case: zext i8 -> i32, two reachable cases + one out-of-range case.
; Case 300 is unreachable (zext i8 max is 255) and must be removed.
define i32 @zext_switch_basic(i8 %x) {
entry:
  %zx = zext i8 %x to i32
  switch i32 %zx, label %default [
    i32 42,  label %case42
    i32 10,  label %case10
    i32 300, label %case300
  ]
case42:
  ret i32 42
case10:
  ret i32 10
case300:
  ret i32 300
default:
  ret i32 0
}

; CHECK-LABEL: define i32 @zext_switch_basic(
; CHECK-NOT:   zext i8 %x to i32
; CHECK:       switch i8 %x, label %default [
; CHECK-NOT:   i32 300
; CHECK-DAG:   i8 42, label %case42
; CHECK-DAG:   i8 10, label %case10
; CHECK:       ]

; No-change: zext also has a non-switch use, so the transformation must not fire.
define i32 @zext_switch_nochange_extra_use(i8 %x, ptr %p) {
entry:
  %zx = zext i8 %x to i32
  store i32 %zx, ptr %p
  switch i32 %zx, label %default [
    i32 42, label %case42
  ]
case42:
  ret i32 42
default:
  ret i32 0
}

; CHECK-LABEL: define i32 @zext_switch_nochange_extra_use(
; CHECK:       zext i8 %x to i32
; CHECK:       switch i32 %zx, label %default [
