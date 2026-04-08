; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(phi(v0, v1, ...)) where arms are a mix of sext-bounded and
; zero-bounded values.  collectTruncRootedValueCost handles both kinds,
; so the phi can be narrowed regardless of which extension kind each arm uses.

; One arm sext(a), one arm zext(b) — truncated back to i8.
define i8 @phi_mixed_sext_zext(i8 %a, i8 %b, i1 %p) {
entry:
  %a32 = sext i8 %a to i32
  %b32 = zext i8 %b to i32
  br i1 %p, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi i32 [ %a32, %left ], [ %b32, %right ]
  %t = trunc i32 %v to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @phi_mixed_sext_zext(
; CHECK-NOT: sext
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: phi i8 [ %a, %left ], [ %b, %right ]
; CHECK: ret i8

; One arm is add(sext(a), sext(b)) — a low-bit-preserving binop of sexts.
define i8 @phi_sext_binop_arm(i8 %a, i8 %b, i8 %c, i1 %p) {
entry:
  %a32 = sext i8 %a to i32
  %b32 = sext i8 %b to i32
  %c32 = sext i8 %c to i32
  %add = add i32 %a32, %b32
  br i1 %p, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi i32 [ %add, %left ], [ %c32, %right ]
  %t = trunc i32 %v to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @phi_sext_binop_arm(
; CHECK-NOT: sext
; CHECK-NOT: trunc
; CHECK: add i8 %a, %b
; CHECK: phi i8
; CHECK: ret i8

define <4 x i8> @phi_mixed_sext_zext_vec(<4 x i8> %a, <4 x i8> %b, i1 %p) {
entry:
  %a32 = sext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  br i1 %p, label %left, label %right
left:
  br label %merge
right:
  br label %merge
merge:
  %v = phi <4 x i32> [ %a32, %left ], [ %b32, %right ]
  %t = trunc <4 x i32> %v to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @phi_mixed_sext_zext_vec(
; CHECK-NOT: sext
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: phi <4 x i8> [ %a, %left ], [ %b, %right ]
; CHECK: ret <4 x i8>
