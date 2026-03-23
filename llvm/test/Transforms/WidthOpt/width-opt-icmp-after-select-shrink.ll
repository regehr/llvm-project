; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): NO
; Narrowing a select of extensions can expose a later ext/ext compare shrink.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @f(i1 %cond, i8 %x, i8 %y, i8 %z) {
entry:
  %x32 = zext i8 %x to i32
  %y32 = zext i8 %y to i32
  %s = select i1 %cond, i32 %x32, i32 %y32
  %z32 = zext i8 %z to i32
  %c = icmp eq i32 %s, %z32
  ret i1 %c
}

; CHECK-LABEL: define i1 @f(
; CHECK: %[[S:.*]] = select i1 %cond, i8 %x, i8 %y
; CHECK: %[[C:.*]] = icmp eq i8 %[[S]], %z
; CHECK-NOT: icmp eq i32
; CHECK: ret i1 %[[C]]
