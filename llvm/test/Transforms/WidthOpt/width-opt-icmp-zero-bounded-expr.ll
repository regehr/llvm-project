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

define <4 x i1> @icmp_eq_and_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %and = and <4 x i32> %a32, %b32
  %cmp = icmp eq <4 x i32> %and, zeroinitializer
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @icmp_eq_and_zexts_vec(
; CHECK-NOT: zext
; CHECK-NOT: and <4 x i32>
; CHECK: %[[AND:.*]] = and <4 x i8> %a, %b
; CHECK: %[[CMP:.*]] = icmp eq <4 x i8> %[[AND]], zeroinitializer
; CHECK: ret <4 x i1> %[[CMP]]

define <4 x i1> @icmp_ne_or_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %or = or <4 x i32> %a32, %b32
  %cmp = icmp ne <4 x i32> %or, zeroinitializer
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @icmp_ne_or_zexts_vec(
; CHECK-NOT: zext
; CHECK-NOT: or <4 x i32>
; CHECK: %[[OR:.*]] = or <4 x i8> %a, %b
; CHECK: %[[CMP:.*]] = icmp ne <4 x i8> %[[OR]], zeroinitializer
; CHECK: ret <4 x i1> %[[CMP]]

define <4 x i1> @icmp_ult_and_mask_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %masked = and <4 x i32> %a32, <i32 15, i32 15, i32 15, i32 15>
  %cmp = icmp ult <4 x i32> %masked, <i32 10, i32 10, i32 10, i32 10>
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @icmp_ult_and_mask_vec(
; CHECK-NOT: zext
; CHECK-NOT: and <4 x i32>
; CHECK: %[[AND:.*]] = and <4 x i8> %a, splat (i8 15)
; CHECK: %[[CMP:.*]] = icmp ult <4 x i8> %[[AND]], splat (i8 10)
; CHECK: ret <4 x i1> %[[CMP]]
