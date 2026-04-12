; collectTruncRootedValueCost transitive chain path (lines 3696-3705):
; When an outer extension's NarrowWidth > TargetWidth, recurse into the
; extension's source. This fires when the INNER sext has multiple uses,
; preventing tryShrinkSExtOfSExt from collapsing the chain first.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(add(sext(sext(a: i8→i16): i16→i32), sext(b: i8→i32)), i8) = add(a, b)
; %a16 has two uses (store + %a32) so tryShrinkSExtOfSExt won't collapse %a32.
; collectTruncRootedValueCost must handle the transitive chain.
define i8 @transitive_sext_inner_multiuse(i8 %a, i8 %b, ptr %p) {
  %a16 = sext i8 %a to i16
  store i16 %a16, ptr %p
  %a32 = sext i16 %a16 to i32
  %b32 = sext i8 %b to i32
  %add = add i32 %a32, %b32
  %t = trunc i32 %add to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @transitive_sext_inner_multiuse(
; CHECK:       %a16 = sext i8 %a to i16
; CHECK:       store i16 %a16, ptr %p
; CHECK-NOT:   sext i16
; CHECK-NOT:   add i32
; CHECK-NOT:   trunc
; CHECK:       %{{.*}} = add i8 %a, %b
; CHECK:       ret i8
