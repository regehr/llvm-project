; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(phi(...)) where every incoming value is zero-bounded at the target
; width.  This goes beyond tryShrinkPhiOfExts which requires direct extensions.

; One arm is and(zext(a), zext(b)), the other is zext(c).
define i8 @phi_mixed_arms(i8 %a, i8 %b, i8 %c, i1 %p) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %c32 = zext i8 %c to i32
  %and = and i32 %a32, %b32
  br i1 %p, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi i32 [ %and, %left ], [ %c32, %right ]
  %t = trunc i32 %v to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @phi_mixed_arms(
; CHECK-NOT: zext
; CHECK-NOT: trunc i32
; CHECK: and i8 %a, %b
; CHECK: phi i8
; CHECK: ret i8

define <4 x i8> @phi_mixed_arms_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c, i1 %p) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %c32 = zext <4 x i8> %c to <4 x i32>
  %and = and <4 x i32> %a32, %b32
  br i1 %p, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi <4 x i32> [ %and, %left ], [ %c32, %right ]
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @phi_mixed_arms_vec(
; CHECK-NOT: zext
; CHECK-NOT: trunc <4 x i32>
; CHECK: and <4 x i8> %a, %b
; CHECK: phi <4 x i8>
; CHECK: ret <4 x i8>

; Three-predecessor phi, all arms zero-bounded.
define i8 @phi_three_arms(i8 %a, i8 %b, i8 %c, i2 %sel) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %c32 = zext i8 %c to i32
  %or = or i32 %a32, %b32
  %s0 = icmp eq i2 %sel, 0
  %s1 = icmp eq i2 %sel, 1
  br i1 %s0, label %arm0, label %check1
check1:
  br i1 %s1, label %arm1, label %arm2
arm0:
  br label %merge
arm1:
  br label %merge
arm2:
  br label %merge
merge:
  %v = phi i32 [ %or, %arm0 ], [ %c32, %arm1 ], [ %a32, %arm2 ]
  %t = trunc i32 %v to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @phi_three_arms(
; CHECK-NOT: zext
; CHECK-NOT: trunc i32
; CHECK: phi i8
; CHECK: ret i8

define <4 x i8> @phi_three_arms_vec(<4 x i8> %a, <4 x i8> %b, <4 x i8> %c, i2 %sel) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %c32 = zext <4 x i8> %c to <4 x i32>
  %or = or <4 x i32> %a32, %b32
  %s0 = icmp eq i2 %sel, 0
  %s1 = icmp eq i2 %sel, 1
  br i1 %s0, label %arm0, label %check1
check1:
  br i1 %s1, label %arm1, label %arm2
arm0:
  br label %merge
arm1:
  br label %merge
arm2:
  br label %merge
merge:
  %v = phi <4 x i32> [ %or, %arm0 ], [ %c32, %arm1 ], [ %a32, %arm2 ]
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @phi_three_arms_vec(
; CHECK-NOT: zext
; CHECK-NOT: trunc <4 x i32>
; CHECK: phi <4 x i8>
; CHECK: ret <4 x i8>
