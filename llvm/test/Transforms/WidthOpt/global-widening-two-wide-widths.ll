; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): NO
; The separate i32 extension path is widened to reuse the existing i64 producer.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i64 @f(i8 %x, i8 %y) {
  %a = xor i8 %x, %y
  %w32 = zext i8 %a to i32
  %u32 = add i32 %w32, 1
  %w64 = zext i8 %a to i64
  %u64 = add i64 %w64, 2
  %u32_64 = zext i32 %u32 to i64
  %r = add i64 %u32_64, %u64
  ret i64 %r
}

; CHECK-LABEL: define i64 @f(
; CHECK: %[[A:.*]] = xor i8 %x, %y
; CHECK: %[[W64:.*]] = zext i8 %[[A]] to i64
; CHECK: %[[U64:.*]] = add i64 %[[W64]], 2
; CHECK: %[[U32W:.*]] = add i64 %[[W64]], 1
; CHECK: %[[R:.*]] = add i64 %[[U32W]], %[[U64]]
; CHECK-NOT: zext i8 %[[A]] to i32
; CHECK-NOT: zext i32
; CHECK: ret i64 %[[R]]
