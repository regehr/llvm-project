; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; lshr of a zero-bounded value is still zero-bounded (lshr shifts zeros in from
; the left).  A chain of lshrs rooted at a zext should narrow fully.

define i8 @lshr_of_lshr(i8 %a) {
  %a32 = zext i8 %a to i32
  %s1 = lshr i32 %a32, 2
  %s2 = lshr i32 %s1, 1
  %t = trunc i32 %s2 to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @lshr_of_lshr(
; CHECK-NOT: zext
; CHECK-NOT: lshr i32
; CHECK-NOT: trunc
; CHECK: lshr i8 %a, 2
; CHECK: lshr i8
; CHECK: ret i8

; lshr then bitwise op, all zero-bounded.

define i8 @lshr_then_or(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %s = lshr i32 %a32, 3
  %v = or i32 %s, %b32
  %t = trunc i32 %v to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @lshr_then_or(
; CHECK-NOT: zext
; CHECK-NOT: i32
; CHECK: lshr i8 %a, 3
; CHECK: or i8
; CHECK: ret i8

define <4 x i8> @lshr_of_lshr_vec(<4 x i8> %a) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %s1 = lshr <4 x i32> %a32, splat (i32 2)
  %s2 = lshr <4 x i32> %s1, splat (i32 1)
  %t = trunc <4 x i32> %s2 to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @lshr_of_lshr_vec(
; CHECK-NOT: zext
; CHECK-NOT: lshr <4 x i32>
; CHECK-NOT: trunc
; CHECK: lshr <4 x i8> %a, splat (i8 2)
; CHECK: lshr <4 x i8>
; CHECK: ret <4 x i8>

define <4 x i8> @lshr_then_or_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %s = lshr <4 x i32> %a32, splat (i32 3)
  %v = or <4 x i32> %s, %b32
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @lshr_then_or_vec(
; CHECK-NOT: zext
; CHECK-NOT: <4 x i32>
; CHECK: lshr <4 x i8> %a, splat (i8 3)
; CHECK: or <4 x i8>
; CHECK: ret <4 x i8>
