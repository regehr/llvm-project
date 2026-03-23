; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; icmp over zero-bounded bitwise expressions.  The operands are not direct
; extensions (tryShrinkICmp misses them) but their high bits are provably zero
; so the compare can run at the narrow width.

; and(zext(a), zext(b)) == 0  =>  and(a, b) == 0
define i1 @icmp_eq_and_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %and = and i32 %a32, %b32
  %cmp = icmp eq i32 %and, 0
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_eq_and_zexts(
; CHECK-NOT: zext
; CHECK-NOT: and i32
; CHECK: %[[AND:.*]] = and i8 %a, %b
; CHECK: %[[CMP:.*]] = icmp eq i8 %[[AND]], 0
; CHECK: ret i1 %[[CMP]]

; or(zext(a), zext(b)) != 0  =>  or(a, b) != 0
define i1 @icmp_ne_or_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %or = or i32 %a32, %b32
  %cmp = icmp ne i32 %or, 0
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ne_or_zexts(
; CHECK-NOT: zext
; CHECK-NOT: or i32
; CHECK: %[[OR:.*]] = or i8 %a, %b
; CHECK: %[[CMP:.*]] = icmp ne i8 %[[OR]], 0
; CHECK: ret i1 %[[CMP]]

; Unsigned compare: and(zext(a), mask) ult constant -- both sides zero-bounded.
define i1 @icmp_ult_and_mask(i8 %a) {
  %a32 = zext i8 %a to i32
  %masked = and i32 %a32, 15
  %cmp = icmp ult i32 %masked, 10
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ult_and_mask(
; CHECK-NOT: zext
; CHECK-NOT: and i32
; CHECK: %[[AND:.*]] = and i8 %a, 15
; CHECK: %[[CMP:.*]] = icmp ult i8 %[[AND]], 10
; CHECK: ret i1 %[[CMP]]
