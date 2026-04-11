; tryWidenAddOverTruncThroughZExt: zext iN(add iN(trunc iW X, C)) → add iW(X, C_wide)
; when X < 2^(N-1) (zero-bounded at N-1 bits) and C < 2^(N-1).
; These conditions ensure the narrow add doesn't overflow, so widening is exact.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Basic case: trunc i32 -> i8 add, result zext'd back to i32.
; %x = and i32 %n, 63   → x ∈ [0,63] < 128 = 2^7 (zero-bounded at 7 bits)
; C = 5 < 128 (fits in 7 bits)
; add i8(trunc i32 %x, 5) ≤ 68 < 256, no overflow; zext(result) == x + 5
define i32 @widen_add_trunc_zext(i32 %n) {
  %x = and i32 %n, 63
  %tr = trunc i32 %x to i8
  %add = add i8 %tr, 5
  %ze = zext i8 %add to i32
  ret i32 %ze
}

; CHECK-LABEL: define i32 @widen_add_trunc_zext(
; CHECK:       %x = and i32 %n, 63
; CHECK-NOT:   trunc
; CHECK-NOT:   zext
; CHECK:       %add.wide = add i32 %x, 5
; CHECK:       ret i32 %add.wide

; Constant on LHS (commuted): add i8(5, trunc i32 %x) should also work.
define i32 @widen_add_trunc_zext_commuted(i32 %n) {
  %x = and i32 %n, 63
  %tr = trunc i32 %x to i8
  %add = add i8 5, %tr
  %ze = zext i8 %add to i32
  ret i32 %ze
}

; CHECK-LABEL: define i32 @widen_add_trunc_zext_commuted(
; CHECK:       %x = and i32 %n, 63
; CHECK-NOT:   trunc
; CHECK-NOT:   zext
; CHECK:       add i32 %x, 5

; No-change: source not zero-bounded (could be >= 128, so narrow add might overflow).
define i32 @widen_add_trunc_zext_nochange_unbounded(i32 %n) {
  %tr = trunc i32 %n to i8
  %add = add i8 %tr, 5
  %ze = zext i8 %add to i32
  ret i32 %ze
}

; CHECK-LABEL: define i32 @widen_add_trunc_zext_nochange_unbounded(
; CHECK:       trunc i32 %n to i8
; CHECK:       add i8 %tr, 5
; CHECK:       zext i8 %add to i32

; No-change: add has two uses, so its single-use precondition fails.
define i32 @widen_add_trunc_zext_nochange_multiuse(i32 %n, ptr %p) {
  %x = and i32 %n, 63
  %tr = trunc i32 %x to i8
  %add = add i8 %tr, 5
  store i8 %add, ptr %p
  %ze = zext i8 %add to i32
  ret i32 %ze
}

; CHECK-LABEL: define i32 @widen_add_trunc_zext_nochange_multiuse(
; CHECK:       trunc i32 %x to i8
; CHECK:       add i8 %tr, 5
; CHECK:       zext{{( nneg)?}} i8 %add to i32
