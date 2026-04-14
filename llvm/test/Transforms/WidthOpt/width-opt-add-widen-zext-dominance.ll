; tryWidenAddThroughZExt uses findExistingZExtToWidth to locate an already-
; present wider zext of the same value, avoiding a redundant instruction.
; However, an existing zext may live in a block that does not dominate the
; insertion point (immediately before the zext user of the add), which would
; produce a use-before-def dominance violation.
;
; Regression test: %x32_side is a zext i8 → i32 of %x in the `side` block,
; but `side` is not on every path to `main`, so it does not dominate `main`.
; The pass must NOT reuse %x32_side; it must create a fresh zext in `main`.
;
; RUN: opt -passes='width-opt' -S -verify-each %s | FileCheck %s

define i32 @widen_add_non_dominating_zext(i1 %cond, i8 %x) {
entry:
  br i1 %cond, label %side, label %main

side:
  ; This zext has the right type but is in a non-dominating block.
  %x32_side = zext i8 %x to i32
  br label %main

main:
  ; Pattern: zext(add(zext(%x, i16), 3), i32) — should be widened.
  %x16 = zext i8 %x to i16
  %add16 = add i16 %x16, 3
  %wide = zext i16 %add16 to i32
  ret i32 %wide
}

; CHECK-LABEL: @widen_add_non_dominating_zext(
; The pass must create a fresh zext in `main`, not use %x32_side from `side`,
; and the now-dead side-block zext should be deleted immediately.
; CHECK: side:
; CHECK-NEXT: br label %main
; CHECK-LABEL: main:
; CHECK: zext i8 %x to i32
; CHECK: add i32 {{.*}}, 3
; CHECK-NOT: %x32_side
