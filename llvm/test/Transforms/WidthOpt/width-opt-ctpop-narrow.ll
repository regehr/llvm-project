; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; trunc(ctpop(zext(a:i8→i32)), i8) = ctpop(a:i8)
; ctpop of a zero-extended value equals ctpop of the original value,
; and ctpop(a:i8) <= 8, which fits in i8.

declare i32 @llvm.ctpop.i32(i32)

define i8 @ctpop_zext_trunc(i8 %a) {
  %a32 = zext i8 %a to i32
  %p = call i32 @llvm.ctpop.i32(i32 %a32)
  %t = trunc i32 %p to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @ctpop_zext_trunc(
; CHECK-NOT: zext
; CHECK-NOT: ctpop i32
; CHECK: %[[R:.*]] = call i8 @llvm.ctpop.i8(i8 %a)
; CHECK: ret i8 %[[R]]
