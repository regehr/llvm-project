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

define <4 x i1> @shared_wide_add_vec(<4 x i8> %a, <4 x i8> %c, <4 x i8> %d) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %c32 = zext <4 x i8> %c to <4 x i32>
  %d32 = zext <4 x i8> %d to <4 x i32>
  %sub = xor <4 x i32> splat (i32 255), %c32
  %mul1 = mul <4 x i32> %a32, %sub
  %mul2 = mul <4 x i32> %c32, %d32
  %add = add <4 x i32> %mul1, %mul2
  %trunc = trunc <4 x i32> %add to <4 x i16>
  %cmp = icmp eq <4 x i16> %trunc, splat (i16 1234)
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @shared_wide_add_vec(
; CHECK: %[[A16:.*]] = zext <4 x i8> %a to <4 x i16>
; CHECK: %[[C16:.*]] = zext <4 x i8> %c to <4 x i16>
; CHECK: %sub.narrow = xor <4 x i16> splat (i16 255), %[[C16]]
; CHECK: %mul1.narrow = mul <4 x i16> %[[A16]], %sub.narrow
; CHECK: %[[D16:.*]] = zext <4 x i8> %d to <4 x i16>
; CHECK: %mul2.narrow = mul <4 x i16> %[[C16]], %[[D16]]
; CHECK: %[[ADD:.*]] = add <4 x i16> %mul1.narrow, %mul2.narrow
; CHECK: %cmp = icmp eq <4 x i16> %[[ADD]], splat (i16 1234)
; CHECK-NOT: trunc <4 x i32>
