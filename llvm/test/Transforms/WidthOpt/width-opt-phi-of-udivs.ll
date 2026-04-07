; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; phi of two udiv results, each with zero-bounded operands:
; the phi and both udivs should all be narrowed to i8.

define i8 @phi_of_udivs(i8 %a, i8 %b, i8 %c, i1 %cond) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %c32 = zext i8 %c to i32
  %d1 = udiv i32 %a32, %c32
  %d2 = udiv i32 %b32, %c32
  br i1 %cond, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi i32 [ %d1, %left ], [ %d2, %right ]
  %t = trunc i32 %v to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @phi_of_udivs(
; CHECK-NOT: zext i8
; CHECK-NOT: udiv i32
; CHECK: udiv i8
; CHECK: udiv i8
; CHECK: phi i8
; CHECK-NOT: trunc i32
; CHECK: ret i8

define <4 x i8> @phi_of_udivs_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c, i1 %cond) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %c32 = zext <4 x i8> %c to <4 x i32>
  %d1 = udiv <4 x i32> %a32, %c32
  %d2 = udiv <4 x i32> %b32, %c32
  br i1 %cond, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi <4 x i32> [ %d1, %left ], [ %d2, %right ]
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @phi_of_udivs_vec(
; CHECK-NOT: zext <4 x i8>
; CHECK-NOT: udiv <4 x i32>
; CHECK: udiv <4 x i8>
; CHECK: udiv <4 x i8>
; CHECK: phi <4 x i8>
; CHECK-NOT: trunc <4 x i32>
; CHECK: ret <4 x i8>

; phi of two urem results -- same idea.

define i8 @phi_of_urems(i8 %a, i8 %b, i8 %c, i1 %cond) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %c32 = zext i8 %c to i32
  %r1 = urem i32 %a32, %c32
  %r2 = urem i32 %b32, %c32
  br i1 %cond, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi i32 [ %r1, %left ], [ %r2, %right ]
  %t = trunc i32 %v to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @phi_of_urems(
; CHECK-NOT: zext i8
; CHECK-NOT: urem i32
; CHECK: urem i8
; CHECK: urem i8
; CHECK: phi i8
; CHECK-NOT: trunc i32
; CHECK: ret i8

define <4 x i8> @phi_of_urems_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c, i1 %cond) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %c32 = zext <4 x i8> %c to <4 x i32>
  %r1 = urem <4 x i32> %a32, %c32
  %r2 = urem <4 x i32> %b32, %c32
  br i1 %cond, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi <4 x i32> [ %r1, %left ], [ %r2, %right ]
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @phi_of_urems_vec(
; CHECK-NOT: zext <4 x i8>
; CHECK-NOT: urem <4 x i32>
; CHECK: urem <4 x i8>
; CHECK: urem <4 x i8>
; CHECK: phi <4 x i8>
; CHECK-NOT: trunc <4 x i32>
; CHECK: ret <4 x i8>
