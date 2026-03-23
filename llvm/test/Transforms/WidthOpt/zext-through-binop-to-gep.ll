; When a zext feeds only lshr/and operations that themselves feed only GEP
; indices, we can narrow the entire chain back to the source type.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

@hex_asc = external global [0 x i8]

; bin2hex style: zext i8 -> i32, used by lshr and and, both going to GEPs
define void @bin2hex(i8 %x, ptr %out0, ptr %out1) {
  %z = zext i8 %x to i32
  %hi = lshr i32 %z, 4
  %lo = and i32 %z, 15
  %p0 = getelementptr [0 x i8], ptr @hex_asc, i64 0, i32 %hi
  %p1 = getelementptr [0 x i8], ptr @hex_asc, i64 0, i32 %lo
  %c0 = load i8, ptr %p0
  %c1 = load i8, ptr %p1
  store i8 %c0, ptr %out0
  store i8 %c1, ptr %out1
  ret void
}

; CHECK-LABEL: define void @bin2hex(
; CHECK-NOT: zext
; CHECK: lshr i8 %x, 4
; CHECK: and i8 %x, 15

; Single lshr use only
define ptr @lshr_only(i8 %x) {
  %z = zext i8 %x to i32
  %s = lshr i32 %z, 1
  %p = getelementptr [0 x i8], ptr @hex_asc, i64 0, i32 %s
  ret ptr %p
}

; CHECK-LABEL: define ptr @lshr_only(
; CHECK-NOT: zext
; CHECK: lshr i8 %x, 1

; Single and use only
define ptr @and_only(i8 %x) {
  %z = zext i8 %x to i32
  %s = and i32 %z, 63
  %p = getelementptr [0 x i8], ptr @hex_asc, i64 0, i32 %s
  ret ptr %p
}

; CHECK-LABEL: define ptr @and_only(
; CHECK-NOT: zext
; CHECK: and i8 %x, 63
