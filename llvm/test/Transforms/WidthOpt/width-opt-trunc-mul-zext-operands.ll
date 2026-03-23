; Trunc-rooted low-bit-preserving binops are not limited to add. A root mul
; with removable zext boundaries should rebuild directly at the truncated width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @root_mul_ret(i8 %a, i8 %b) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %mul = mul i32 %a32, %b32
  %tr = trunc i32 %mul to i16
  ret i16 %tr
}

; CHECK-LABEL: define i16 @root_mul_ret(
; CHECK: %[[A16:.*]] = zext i8 %a to i16
; CHECK: %[[B16:.*]] = zext i8 %b to i16
; CHECK: %[[MUL:.*]] = mul i16 %[[A16]], %[[B16]]
; CHECK: ret i16 %[[MUL]]
; CHECK-NOT: trunc i32
