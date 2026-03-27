; `sext -> zext nneg` is only valid when the source is both non-negative and
; not undef at the use.  If one phi arm is undef, replacing the sexts below
; with `zext nneg` makes negative undef choices turn into poison.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @phi_undef_shared_sext(i1 %c) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ undef, %left ], [ 9, %right ]
  %x = sext i8 %p to i32
  %y = sext i8 %p to i32
  %r = add i32 %x, %y
  ret i32 %r
}

; CHECK-LABEL: define i32 @phi_undef_shared_sext(
; CHECK: %[[P:.*]] = phi i8 [ undef, %left ], [ 9, %right ]
; CHECK: %[[EXT:.*]] = sext i8 %[[P]] to i32
; CHECK-NOT: zext nneg
; CHECK: %[[R:.*]] = add i32 %[[EXT]], %[[EXT]]
; CHECK: ret i32 %[[R]]
