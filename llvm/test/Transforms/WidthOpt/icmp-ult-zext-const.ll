; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; Unsigned relational compare of a zero-extended value against a fitting
; constant is narrowed: the zext is removed and the compare moves to the
; source width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @ult_const(i8 %x) {
  %ext = zext i8 %x to i32
  %cmp = icmp ult i32 %ext, 100
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @ult_const(
; CHECK-NOT: zext
; CHECK: %[[C:.*]] = icmp ult i8 %x, 100
; CHECK: ret i1 %[[C]]

define <4 x i1> @ult_const_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp ult <4 x i32> %ext, splat (i32 100)
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @ult_const_vec(
; CHECK-NOT: zext
; CHECK: %[[C:.*]] = icmp ult <4 x i8> %x, splat (i8 100)
; CHECK: ret <4 x i1> %[[C]]

define i1 @uge_const(i8 %x) {
  %ext = zext i8 %x to i32
  %cmp = icmp uge i32 %ext, 200
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @uge_const(
; CHECK-NOT: zext
; CHECK: %[[C:.*]] = icmp uge i8 %x, -56
; CHECK: ret i1 %[[C]]

define <4 x i1> @uge_const_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp uge <4 x i32> %ext, splat (i32 200)
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @uge_const_vec(
; CHECK-NOT: zext
; CHECK: %[[C:.*]] = icmp uge <4 x i8> %x, splat (i8 -56)
; CHECK: ret <4 x i1> %[[C]]

; Constant on the left: predicate is swapped so the narrow value stays on LHS.
define i1 @const_on_left(i8 %x) {
  %ext = zext i8 %x to i32
  %cmp = icmp ult i32 50, %ext
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @const_on_left(
; CHECK-NOT: zext
; CHECK: %[[C:.*]] = icmp ugt i8 %x, 50
; CHECK: ret i1 %[[C]]

define <4 x i1> @const_on_left_vec(<4 x i8> %x) {
  %ext = zext <4 x i8> %x to <4 x i32>
  %cmp = icmp ult <4 x i32> splat (i32 50), %ext
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @const_on_left_vec(
; CHECK-NOT: zext
; CHECK: %[[C:.*]] = icmp ugt <4 x i8> %x, splat (i8 50)
; CHECK: ret <4 x i1> %[[C]]
