; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; When multiple truncs share the same wide binop source, narrow the binop
; once and reuse the result for all truncs.

define void @multi_trunc_same_src(i8 %a, i8 %b, ptr %p, ptr %q) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %r = add i32 %a32, %b32
  %t1 = trunc i32 %r to i8
  %t2 = trunc i32 %r to i8
  store i8 %t1, ptr %p
  store i8 %t2, ptr %q
  ret void
}

; CHECK-LABEL: define void @multi_trunc_same_src(
; CHECK-NOT: zext i8
; CHECK-NOT: add i32
; CHECK: %[[R:.*]] = add i8 %a, %b
; CHECK-NOT: trunc i32
; CHECK: store i8 %[[R]], ptr %p
; CHECK: store i8 %[[R]], ptr %q
