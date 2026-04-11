; RUN: opt -passes='width-opt' -S %s | FileCheck %s
;
; Cover edge folds in tryShrinkICmpExtConst()/buildConstantAwareICmp() and the
; out-of-range constant reasoning helpers.

define i1 @ugt_min_false() {
entry:
  %ext = zext i1 false to i32
  %cmp = icmp ugt i32 %ext, 0
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @ugt_min_false(
; CHECK: ret i1 false

define i1 @uge_max_true() {
entry:
  %ext = zext i1 true to i32
  %cmp = icmp uge i32 %ext, 1
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @uge_max_true(
; CHECK: ret i1 true

define i1 @ult_max_false() {
entry:
  %ext = zext i1 true to i32
  %cmp = icmp ult i32 %ext, 1
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @ult_max_false(
; CHECK: ret i1 false

define i1 @slt_max_false() {
entry:
  %ext = sext i1 false to i32
  %cmp = icmp slt i32 %ext, 0
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @slt_max_false(
; CHECK: ret i1 false

define i1 @sge_max_true() {
entry:
  %ext = sext i1 false to i32
  %cmp = icmp sge i32 %ext, 0
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sge_max_true(
; CHECK: ret i1 true

define i1 @sle_min_true() {
entry:
  %ext = sext i1 true to i32
  %cmp = icmp sle i32 %ext, -1
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sle_min_true(
; CHECK: ret i1 true

define i1 @sgt_min_false() {
entry:
  %ext = sext i1 true to i32
  %cmp = icmp sgt i32 %ext, -1
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sgt_min_false(
; CHECK: ret i1 false

define i1 @zext_uge_large_false(i8 %x) {
entry:
  %ext = zext i8 %x to i32
  %cmp = icmp uge i32 %ext, 300
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @zext_uge_large_false(
; CHECK: ret i1 false

define i1 @zext_ule_large_true(i8 %x) {
entry:
  %ext = zext i8 %x to i32
  %cmp = icmp ule i32 %ext, 300
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @zext_ule_large_true(
; CHECK: ret i1 true

define i1 @sext_slt_above_max_true(i8 %x) {
entry:
  %ext = sext i8 %x to i32
  %cmp = icmp slt i32 %ext, 128
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sext_slt_above_max_true(
; CHECK: ret i1 true

define i1 @sext_sle_above_max_true(i8 %x) {
entry:
  %ext = sext i8 %x to i32
  %cmp = icmp sle i32 %ext, 128
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sext_sle_above_max_true(
; CHECK: ret i1 true

define i1 @sext_eq_above_max_false(i8 %x) {
entry:
  %ext = sext i8 %x to i32
  %cmp = icmp eq i32 %ext, 128
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sext_eq_above_max_false(
; CHECK: ret i1 false

define i1 @sext_unsigned_fit(i8 %x) {
entry:
  %ext = sext i8 %x to i32
  %cmp = icmp ult i32 %ext, 100
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sext_unsigned_fit(
; CHECK: %[[CMP:.*]] = icmp ult i8 %x, 100
; CHECK-NOT: icmp ult i32
; CHECK: ret i1 %[[CMP]]

define i1 @zext_signed_gt_negative_true(i8 %x) {
entry:
  %ext = zext i8 %x to i32
  %cmp = icmp sgt i32 %ext, -1
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @zext_signed_gt_negative_true(
; CHECK: ret i1 true

define i1 @zext_signed_le_negative_false(i8 %x) {
entry:
  %ext = zext i8 %x to i32
  %cmp = icmp sle i32 %ext, -1
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @zext_signed_le_negative_false(
; CHECK: ret i1 false
