; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; InstCombine turns zext(trunc(x)) into a low-bit mask at the destination width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i64 %x) {
entry:
  %t = trunc i64 %x to i16
  %e = zext i16 %t to i32
  ret i32 %e
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[TR:.*]] = trunc i64 %x to i32
; CHECK: %[[MASK:.*]] = and i32 %[[TR]], 65535
; CHECK-NOT: trunc i64 %x to i16
; CHECK-NOT: zext i16
; CHECK: ret i32 %[[MASK]]
