; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; materializeTruncRootedValueAtWidth: default case in IntrinsicInst switch (line 3664).
; An unrecognized intrinsic (ctpop) falls through to the fallback-trunc path (line 3669).
; The outer and(ctpop_i32, zext_i8) is narrowed: ctpop stays wide, gets a new trunc.

declare i32 @llvm.ctpop.i32(i32)

define i8 @and_ctpop_zext(i32 %x, i8 %a) {
  %cp = call i32 @llvm.ctpop.i32(i32 %x)
  %za = zext i8 %a to i32
  %and = and i32 %cp, %za
  %t = trunc i32 %and to i8
  ret i8 %t
}
; CHECK-LABEL: define i8 @and_ctpop_zext(
; CHECK: %cp = call i32 @llvm.ctpop.i32(i32 %x)
; CHECK: trunc i32 %cp to i8
; CHECK: and i8
; CHECK-NOT: and i32
; CHECK-NOT: trunc i32 %and
; CHECK: ret i8
