; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; The sign extension is weakened to zext because the mask only demands low bits.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %x) {
  %sx = sext i8 %x to i32
  %r = and i32 %sx, 255
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[ZX:.*]] = zext i8 %x to i32
; CHECK-NOT: sext i8 %x to i32
; CHECK: %[[R:.*]] = and i32 %[[ZX]], 255
; CHECK: ret i32 %[[R]]
