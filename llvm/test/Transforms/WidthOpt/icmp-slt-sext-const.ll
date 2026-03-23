; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; Signed relational compare of a sign-extended value against a fitting constant
; is narrowed: the sext is removed and the compare moves to the source width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i1 @slt_positive_const(i8 %x) {
  %ext = sext i8 %x to i32
  %cmp = icmp slt i32 %ext, 100
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @slt_positive_const(
; CHECK-NOT: sext
; CHECK: %[[C:.*]] = icmp slt i8 %x, 100
; CHECK: ret i1 %[[C]]

; Negative constant that fits in i8 signed range.
define i1 @sgt_negative_const(i8 %x) {
  %ext = sext i8 %x to i32
  %cmp = icmp sgt i32 %ext, -5
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @sgt_negative_const(
; CHECK-NOT: sext
; CHECK: %[[C:.*]] = icmp sgt i8 %x, -5
; CHECK: ret i1 %[[C]]

; eq/ne also work with sext.
define i1 @eq_sext_const(i8 %x) {
  %ext = sext i8 %x to i32
  %cmp = icmp eq i32 %ext, -1
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @eq_sext_const(
; CHECK-NOT: sext
; CHECK: %[[C:.*]] = icmp eq i8 %x, -1
; CHECK: ret i1 %[[C]]
