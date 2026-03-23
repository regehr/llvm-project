; Trunc-rooted add shrinking can recurse through low-bit-preserving arithmetic
; when the removable root trunc pays for rebuilding the expression at the
; target width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @shared_wide_add_operands_narrowed_recursively(i8 %a, i8 %c, i8 %d) {
entry:
  %a32 = zext i8 %a to i32
  %c32 = zext i8 %c to i32
  %d32 = zext i8 %d to i32
  %sub = xor i32 255, %c32
  %mul1 = mul i32 %a32, %sub
  %mul2 = mul i32 %c32, %d32
  %add = add i32 %mul1, %mul2
  %trunc = trunc i32 %add to i16
  %cmp = icmp eq i16 %trunc, 1234
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @shared_wide_add_operands_narrowed_recursively(
; CHECK: %[[A16:.*]] = zext i8 %a to i16
; CHECK: %[[C16:.*]] = zext i8 %c to i16
; CHECK: %sub.narrow = xor i16 255, %[[C16]]
; CHECK: %mul1.narrow = mul i16 %[[A16]], %sub.narrow
; CHECK: %[[D16:.*]] = zext i8 %d to i16
; CHECK: %mul2.narrow = mul i16 %[[C16]], %[[D16]]
; CHECK: %[[ADD:.*]] = add i16 %mul1.narrow, %mul2.narrow
; CHECK: %cmp = icmp eq i16 %[[ADD]], 1234
; CHECK-NOT: trunc i32
