; RUN: opt -passes='width-opt' -S %s | FileCheck %s
;
; WidthOpt should ignore pointer compares; this covers the non-integer guards in
; the zero-bounded and sign-bounded icmp shrinkers.

define i1 @ptr_icmp_eq(ptr %p, ptr %q) {
entry:
  %cmp = icmp eq ptr %p, %q
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @ptr_icmp_eq(
; CHECK: %cmp = icmp eq ptr %p, %q
; CHECK: ret i1 %cmp
