; tryFoldZExtOfTruncToMask positive case: zext iN(trunc iW X to iN) → and iW(X, mask)
; when SrcWidth == WideWidth (i.e., the trunc narrows and the zext restores to the same
; width) and the trunc has exactly one use. Net effect: -1 instruction (trunc + zext → and).
;
; Contrast with zext-trunc-to-mask.ll which tests the no-change case (SrcWidth > WideWidth,
; so the added and+trunc cost equals the removed zext+trunc, giving no net gain).
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; SrcWidth == WideWidth == 32, NarrowWidth == 8, trunc has one use.
; Removes trunc + zext (2 instructions), adds one and: net -1.
define i32 @zext_trunc_to_mask_i8(i32 %x) {
  %t = trunc i32 %x to i8
  %e = zext i8 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @zext_trunc_to_mask_i8(
; CHECK-NOT:   trunc
; CHECK-NOT:   zext
; CHECK:       and i32 %x, 255
; CHECK:       ret i32

; SrcWidth == WideWidth == 32, NarrowWidth == 16.
define i32 @zext_trunc_to_mask_i16(i32 %x) {
  %t = trunc i32 %x to i16
  %e = zext i16 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @zext_trunc_to_mask_i16(
; CHECK-NOT:   trunc
; CHECK-NOT:   zext
; CHECK:       and i32 %x, 65535
; CHECK:       ret i32

; No-change: trunc has two uses, so removing it does not save an instruction.
define i32 @zext_trunc_to_mask_nochange_multiuse(i32 %x, ptr %p) {
  %t = trunc i32 %x to i8
  store i8 %t, ptr %p
  %e = zext i8 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @zext_trunc_to_mask_nochange_multiuse(
; CHECK:       trunc i32 %x to i8
; CHECK:       zext i8 %t to i32
