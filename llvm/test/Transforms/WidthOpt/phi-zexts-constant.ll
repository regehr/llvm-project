; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; InstCombine shrinks this PHI to i8 and re-extends once.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %sel, i8 %a, i8 %b) {
entry:
  switch i8 %sel, label %c [
    i8 0, label %a.bb
    i8 1, label %b.bb
  ]

a.bb:
  %za = zext i8 %a to i32
  br label %merge

b.bb:
  %zb = zext i8 %b to i32
  br label %merge

c:
  br label %merge

merge:
  %p = phi i32 [ %za, %a.bb ], [ %zb, %b.bb ], [ 42, %c ]
  ret i32 %p
}

; CHECK-LABEL: define i32 @f(
; CHECK-NOT: phi i32
; CHECK: merge:
; CHECK: %[[PN:.*]] = phi i8 [ %a, %a.bb ], [ %b, %b.bb ], [ 42, %c ]
; CHECK: %[[W:.*]] = zext i8 %[[PN]] to i32
; CHECK: ret i32 %[[W]]

define <4 x i32> @f_vec(i8 %sel, <4 x i8> %a, <4 x i8> %b) {
entry:
  switch i8 %sel, label %c [
    i8 0, label %a.bb
    i8 1, label %b.bb
  ]

a.bb:
  %za = zext <4 x i8> %a to <4 x i32>
  br label %merge

b.bb:
  %zb = zext <4 x i8> %b to <4 x i32>
  br label %merge

c:
  br label %merge

merge:
  %p = phi <4 x i32> [ %za, %a.bb ], [ %zb, %b.bb ], [ splat (i32 42), %c ]
  ret <4 x i32> %p
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK-NOT: phi <4 x i32>
; CHECK: merge:
; CHECK: %[[PN:.*]] = phi <4 x i8> [ %a, %a.bb ], [ %b, %b.bb ], [ splat (i8 42), %c ]
; CHECK: %[[W:.*]] = zext <4 x i8> %[[PN]] to <4 x i32>
; CHECK: ret <4 x i32> %[[W]]
