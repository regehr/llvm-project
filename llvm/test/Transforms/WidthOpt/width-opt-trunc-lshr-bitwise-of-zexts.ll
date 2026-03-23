; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(lshr(or(zext(a), zext(b)), k)) -- the LHS of lshr is a bitwise or of
; two zero-extensions, not a direct zext, but all bits above the target width
; are provably zero so the shift and truncation can be sunk.

define i8 @lshr_of_or(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %or = or i32 %a32, %b32
  %shr = lshr i32 %or, 3
  %result = trunc i32 %shr to i8
  ret i8 %result
}

; CHECK-LABEL: define i8 @lshr_of_or(
; CHECK-NOT: zext
; CHECK-NOT: lshr i32
; CHECK-NOT: trunc
; CHECK: %[[OR:.*]] = or i8 %a, %b
; CHECK: %[[SHR:.*]] = lshr i8 %[[OR]], 3
; CHECK: ret i8 %[[SHR]]

; Same pattern with and.

define i8 @lshr_of_and(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %and = and i32 %a32, %b32
  %shr = lshr i32 %and, 2
  %result = trunc i32 %shr to i8
  ret i8 %result
}

; CHECK-LABEL: define i8 @lshr_of_and(
; CHECK-NOT: zext
; CHECK-NOT: lshr i32
; CHECK-NOT: trunc
; CHECK: %[[AND:.*]] = and i8 %a, %b
; CHECK: %[[SHR:.*]] = lshr i8 %[[AND]], 2
; CHECK: ret i8 %[[SHR]]

; and(x, mask) is zero-bounded when the mask fits in TargetWidth bits, even
; if x is an unconstrained wide value.

define i8 @lshr_of_and_mask(i32 %x) {
  %masked = and i32 %x, 255
  %shr = lshr i32 %masked, 3
  %result = trunc i32 %shr to i8
  ret i8 %result
}

; CHECK-LABEL: define i8 @lshr_of_and_mask(
; CHECK-NOT: lshr i32
; CHECK-NOT: and i32
; CHECK: trunc i32 %x to i8
; CHECK: lshr i8
; CHECK: ret i8

; Nested: lshr(xor(or(zext(a), zext(b)), zext(c)), k).

define i8 @lshr_of_nested_bitwise(i8 %a, i8 %b, i8 %c) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %c32 = zext i8 %c to i32
  %or = or i32 %a32, %b32
  %xor = xor i32 %or, %c32
  %shr = lshr i32 %xor, 1
  %result = trunc i32 %shr to i8
  ret i8 %result
}

; CHECK-LABEL: define i8 @lshr_of_nested_bitwise(
; CHECK-NOT: zext
; CHECK-NOT: lshr i32
; CHECK-NOT: trunc
; CHECK: or i8
; CHECK: xor i8
; CHECK: lshr i8
; CHECK: ret i8
