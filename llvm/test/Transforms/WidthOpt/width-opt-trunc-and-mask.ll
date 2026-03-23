; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(and(x, mask), N) = trunc(x, N) when mask has all N low bits set.
; The AND cannot change any bits that survive the truncation.

define i8 @and_0xFF_before_trunc(i32 %x) {
  %m = and i32 %x, 255
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @and_0xFF_before_trunc(
; CHECK-NOT: and i32
; CHECK: trunc i32 %x to i8
; CHECK: ret i8

; Bitfield extraction: extract byte 1 of a 32-bit value
define i8 @bitfield_extract_byte1(i32 %a) {
  %s = lshr i32 %a, 8
  %m = and i32 %s, 255
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @bitfield_extract_byte1(
; CHECK-NOT: and i32
; CHECK: lshr i32 %a, 8
; CHECK: trunc i32

; Mask that covers more than the low 8 bits also qualifies (e.g., 0xFFFF)
define i8 @and_0xFFFF_before_trunc(i32 %x) {
  %m = and i32 %x, 65535
  %t = trunc i32 %m to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @and_0xFFFF_before_trunc(
; CHECK-NOT: and i32
; CHECK: trunc i32 %x to i8
; CHECK: ret i8
