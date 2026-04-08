; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; tryShrinkICmpSignBounded: narrow signed icmp when both operands are
; structurally sext-bounded (and/or/xor trees of sext, ashr of sext).

; ---- basic signed predicates on sext trees ----

define i1 @icmp_slt_and_sext(i8 %a, i8 %b, i8 %c) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %ab = and i32 %sa, %sb
  %r = icmp slt i32 %ab, %sc
  ret i1 %r
}

; CHECK-LABEL: define i1 @icmp_slt_and_sext(
; CHECK:      %[[AB:.*]] = and i8 %a, %b
; CHECK:      %[[R:.*]] = icmp slt i8 %[[AB]], %c
; CHECK:      ret i1 %[[R]]

define i1 @icmp_sle_xor_sext(i8 %a, i8 %b, i8 %c) {
  %sa = sext i8 %a to i32
  %sb = sext i8 %b to i32
  %sc = sext i8 %c to i32
  %ab = xor i32 %sa, %sb
  %r = icmp sle i32 %ab, %sc
  ret i1 %r
}

; CHECK-LABEL: define i1 @icmp_sle_xor_sext(
; CHECK:      %[[AB:.*]] = xor i8 %a, %b
; CHECK:      %[[R:.*]] = icmp sle i8 %[[AB]], %c
; CHECK:      ret i1 %[[R]]

define i1 @icmp_eq_ashr_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sh = ashr i32 %sa, 2
  %sb = sext i8 %b to i32
  %r = icmp eq i32 %sh, %sb
  ret i1 %r
}

; CHECK-LABEL: define i1 @icmp_eq_ashr_sext(
; CHECK:      %[[SH:.*]] = ashr i8 %a, 2
; CHECK:      %[[R:.*]] = icmp eq i8 %[[SH]], %b
; CHECK:      ret i1 %[[R]]

define i1 @icmp_sgt_ashr_sext(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sh = ashr i32 %sa, 3
  %sb = sext i8 %b to i32
  %r = icmp sgt i32 %sh, %sb
  ret i1 %r
}

; CHECK-LABEL: define i1 @icmp_sgt_ashr_sext(
; CHECK:      %[[SH:.*]] = ashr i8 %a, 3
; CHECK:      %[[R:.*]] = icmp sgt i8 %[[SH]], %b
; CHECK:      ret i1 %[[R]]

; ---- vector variants ----

define <4 x i1> @icmp_slt_and_sext_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %sc = sext <4 x i8> %c to <4 x i32>
  %ab = and <4 x i32> %sa, %sb
  %r = icmp slt <4 x i32> %ab, %sc
  ret <4 x i1> %r
}

; CHECK-LABEL: define <4 x i1> @icmp_slt_and_sext_vec(
; CHECK:      %[[AB:.*]] = and <4 x i8> %a, %b
; CHECK:      %[[R:.*]] = icmp slt <4 x i8> %[[AB]], %c
; CHECK:      ret <4 x i1> %[[R]]

define <4 x i1> @icmp_sle_xor_sext_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sb = sext <4 x i8> %b to <4 x i32>
  %sc = sext <4 x i8> %c to <4 x i32>
  %ab = xor <4 x i32> %sa, %sb
  %r = icmp sle <4 x i32> %ab, %sc
  ret <4 x i1> %r
}

; CHECK-LABEL: define <4 x i1> @icmp_sle_xor_sext_vec(
; CHECK:      %[[AB:.*]] = xor <4 x i8> %a, %b
; CHECK:      %[[R:.*]] = icmp sle <4 x i8> %[[AB]], %c
; CHECK:      ret <4 x i1> %[[R]]

define <4 x i1> @icmp_eq_ashr_sext_vec(<4 x i8> %a, <4 x i8> %b) {
  %sa = sext <4 x i8> %a to <4 x i32>
  %sh = ashr <4 x i32> %sa, splat (i32 2)
  %sb = sext <4 x i8> %b to <4 x i32>
  %r = icmp eq <4 x i32> %sh, %sb
  ret <4 x i1> %r
}

; CHECK-LABEL: define <4 x i1> @icmp_eq_ashr_sext_vec(
; CHECK:      %[[SH:.*]] = ashr <4 x i8> %a, splat (i8 2)
; CHECK:      %[[R:.*]] = icmp eq <4 x i8> %[[SH]], %b
; CHECK:      ret <4 x i1> %[[R]]

; ---- no-change: ashr amount >= TargetWidth does not narrow ----

define i1 @icmp_ashr_too_large_nochange(i8 %a, i8 %b) {
  %sa = sext i8 %a to i32
  %sh = ashr i32 %sa, 8
  %sb = sext i8 %b to i32
  %r = icmp eq i32 %sh, %sb
  ret i1 %r
}

; CHECK-LABEL: define i1 @icmp_ashr_too_large_nochange(
; CHECK-NOT:   icmp eq i8
; CHECK:       ret i1
