; Narrowing a trunc-rooted select is only profitable when removable
; instructions pay for any new arm materialization. If both select arms are
; still wide, leave the root trunc in place.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i8 @wide_select_arms_not_split(i1 %c0, i1 %c1, i32 %x) {
entry:
  %clamp = select i1 %c0, i32 127, i32 -128
  %sel = select i1 %c1, i32 %x, i32 %clamp
  %tr = trunc i32 %sel to i8
  ret i8 %tr
}

; CHECK-LABEL: define i8 @wide_select_arms_not_split(
; CHECK: %clamp = select i1 %c0, i32 127, i32 -128
; CHECK: %sel = select i1 %c1, i32 %x, i32 %clamp
; CHECK: %tr = trunc i32 %sel to i8
; CHECK-NOT: trunc i32 %x to i8
; CHECK-NOT: trunc i32 %clamp to i8
