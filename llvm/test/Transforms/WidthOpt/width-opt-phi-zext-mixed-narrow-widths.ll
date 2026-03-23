; Mixed-width zext arms can still be shrunk through a common intermediate
; width. Here the phi should move to i16 instead of requiring both arms to
; have the same exact narrow width.
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
; CHECK: %[[A16:.*]] = zext i8 %a to i16
; CHECK: right:
; CHECK-NOT: %b32 = zext i16 %b to i32
; CHECK: merge:
; CHECK: %[[PN:.*]] = phi i16 [ %[[A16]], %left ], [ %b, %right ]
; CHECK: %[[PW:.*]] = zext i16 %[[PN]] to i32
; CHECK-NOT: phi i32
