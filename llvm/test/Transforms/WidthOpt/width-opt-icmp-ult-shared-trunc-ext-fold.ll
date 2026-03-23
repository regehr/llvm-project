; Shared trunc users should not block compare cleanup when the trunc is just
; undoing an extension and can be folded away directly.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @shared_trunc_ext_folded(i8 %n) {
entry:
  %wide = zext i8 %n to i64
  %tr = trunc i64 %wide to i8
  %div = sdiv i8 -1, %tr
  %rem = srem i8 -1, %div
  %cmp = icmp ult i8 %tr, %rem
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @shared_trunc_ext_folded(
; CHECK-NOT: zext i8 %n to i64
; CHECK-NOT: trunc i64
; CHECK: %div = sdiv i8 -1, %n
; CHECK: %rem = srem i8 -1, %div
; CHECK: %cmp = icmp ult i8 %n, %rem
; CHECK-NOT: icmp ult i64
; CHECK-NOT: zext i8 %rem to i64
