; Mixed-width zext arms can still be shrunk through a common intermediate
; width in principle, but this phi rewrite is only break-even locally, so
; WidthOpt now leaves it unchanged.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %a, i16 %b) {
entry:
  br i1 %c, label %left, label %right

left:
  %a32 = zext i8 %a to i32
  br label %merge

right:
  %b32 = zext i16 %b to i32
  br label %merge

merge:
  %p = phi i32 [ %a32, %left ], [ %b32, %right ]
  %x = add i32 %p, 1
  %y = xor i32 %p, 42
  %r = add i32 %x, %y
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: left:
; CHECK: %a32 = zext i8 %a to i32
; CHECK: right:
; CHECK: %b32 = zext i16 %b to i32
; CHECK: merge:
; CHECK: %p = phi i32 [ %a32, %left ], [ %b32, %right ]
; CHECK-NOT: phi i16

define <4 x i32> @f_vec(i1 %c, <4 x i8> %a, <4 x i16> %b) {
entry:
  br i1 %c, label %left, label %right

left:
  %a32 = zext <4 x i8> %a to <4 x i32>
  br label %merge

right:
  %b32 = zext <4 x i16> %b to <4 x i32>
  br label %merge

merge:
  %p = phi <4 x i32> [ %a32, %left ], [ %b32, %right ]
  %x = add <4 x i32> %p, <i32 1, i32 1, i32 1, i32 1>
  %y = xor <4 x i32> %p, <i32 42, i32 42, i32 42, i32 42>
  %r = add <4 x i32> %x, %y
  ret <4 x i32> %r
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: left:
; CHECK: %a32 = zext <4 x i8> %a to <4 x i32>
; CHECK: right:
; CHECK: %b32 = zext <4 x i16> %b to <4 x i32>
; CHECK: merge:
; CHECK: %p = phi <4 x i32> [ %a32, %left ], [ %b32, %right ]
; CHECK-NOT: phi <4 x i16>
