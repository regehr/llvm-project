; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; A trunc-rooted shl self-recurrence is currently left at the wide type.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s
; ALIVE2-EXTRA-ARGS: --src-unroll=16 --tgt-unroll=16

define i16 @f(i8 %x) {
entry:
  %zext = zext i8 %x to i32
  br label %loop

loop:
  %p = phi i32 [ %zext, %entry ], [ %shl, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %shl = shl i32 %p, 1
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, 10
  br i1 %done, label %exit, label %loop

exit:
  %t = trunc i32 %shl to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @f(
; CHECK: %zext = zext i8 %x to i32
; CHECK: %p = phi i32 [ %zext, %entry ], [ %shl, %loop ]
; CHECK: %shl = shl i32 %p, 1
; CHECK: %t = trunc i32 %shl to i16
; CHECK: ret i16 %t
