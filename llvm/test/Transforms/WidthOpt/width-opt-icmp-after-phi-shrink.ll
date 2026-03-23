; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): NO
; Narrowing a PHI of extensions can expose a later ext/ext compare shrink.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i1 %cond, i8 %x, i8 %y, i8 %z) {
entry:
  br i1 %cond, label %a, label %b

a:
  %x32 = zext i8 %x to i32
  br label %merge

b:
  %y32 = zext i8 %y to i32
  br label %merge

merge:
  %p = phi i32 [ %x32, %a ], [ %y32, %b ]
  %z32 = zext i8 %z to i32
  %c = icmp eq i32 %p, %z32
  ret i1 %c
}

; CHECK-LABEL: define i1 @f(
; CHECK: merge:
; CHECK: %[[P:.*]] = phi i8 [ %x, %a ], [ %y, %b ]
; CHECK: %[[C:.*]] = icmp eq i8 %[[P]], %z
; CHECK-NOT: icmp eq i32
; CHECK: ret i1 %[[C]]
