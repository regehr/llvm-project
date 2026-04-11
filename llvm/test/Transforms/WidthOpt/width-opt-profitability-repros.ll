; RUN: opt -passes='width-opt' -S %s | FileCheck %s
; Strict local-profitability policy: none of these equal-cost or unprofitable
; rewrites should fire.

declare void @use_i8(i8)
declare void @use_i16(i16, i16)
declare void @use_i32(i32)
declare i8 @ranged_byte() #0

define i1 @widen_trunc_eq_break_even(i32 %x, i32 %y) {
entry:
  %x.mask = and i32 %x, 65535
  %y.mask = and i32 %y, 65535
  %x16 = trunc i32 %x.mask to i16
  %y16 = trunc i32 %y.mask to i16
  call void @use_i16(i16 %x16, i16 %y16)
  %cmp = icmp eq i16 %x16, %y16
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @widen_trunc_eq_break_even(
; CHECK: %[[X16:.*]] = trunc i32 %x to i16
; CHECK: %[[Y16:.*]] = trunc i32 %y to i16
; CHECK: call void @use_i16(i16 %[[X16]], i16 %[[Y16]])
; CHECK: %cmp = icmp eq i16 %[[X16]], %[[Y16]]
; CHECK-NOT: icmp eq i32
; CHECK: ret i1 %cmp

define i1 @lshr_to_i1_break_even(i8 %x) {
entry:
  %s = lshr i8 %x, 3
  call void @use_i8(i8 %s)
  %t = trunc i8 %s to i1
  ret i1 %t
}

; CHECK-LABEL: define i1 @lshr_to_i1_break_even(
; CHECK: %s = lshr i8 %x, 3
; CHECK: call void @use_i8(i8 %s)
; CHECK: %t = trunc i8 %s to i1
; CHECK-NOT: and i8 %x, 8
; CHECK: ret i1 %t

define i64 @widen_add_through_zext_break_even(i8 %a) {
entry:
  %a32 = zext i8 %a to i32
  call void @use_i32(i32 %a32)
  %sum = add i32 %a32, 7
  %wide = zext i32 %sum to i64
  ret i64 %wide
}

; CHECK-LABEL: define i64 @widen_add_through_zext_break_even(
; CHECK: %a32 = zext i8 %a to i32
; CHECK: call void @use_i32(i32 %a32)
; CHECK: %sum = add i32 %a32, 7
; CHECK: %wide = zext nneg i32 %sum to i64
; CHECK-NOT: add i64
; CHECK: ret i64 %wide

define i1 @icmp_ext_const_break_even(i8 %x) {
entry:
  %x32 = zext i8 %x to i32
  call void @use_i32(i32 %x32)
  %cmp = icmp ult i32 %x32, 10
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ext_const_break_even(
; CHECK: %x32 = zext i8 %x to i32
; CHECK: call void @use_i32(i32 %x32)
; CHECK: %cmp = icmp ult i32 %x32, 10
; CHECK-NOT: icmp ult i8 %x, 10
; CHECK: ret i1 %cmp

define i32 @zext_of_trunc_break_even(i64 %x, i8 %y) {
entry:
  %tr = trunc i64 %x to i8
  call void @use_i8(i8 %tr)
  %z = zext i8 %tr to i32
  %y32 = zext i8 %y to i32
  %r = add i32 %z, %y32
  ret i32 %r
}

; CHECK-LABEL: define i32 @zext_of_trunc_break_even(
; CHECK: %tr = trunc i64 %x to i8
; CHECK: call void @use_i8(i8 %tr)
; CHECK: %z = zext i8 %tr to i32
; CHECK-NOT: and i32

define i32 @sext_to_nonneg_zext_break_even(i8 %x) {
entry:
  %nonneg = and i8 %x, 127
  %sx = sext i8 %nonneg to i32
  ret i32 %sx
}

; CHECK-LABEL: define i32 @sext_to_nonneg_zext_break_even(
; CHECK: %sx = sext i8 %nonneg to i32
; CHECK-NOT: zext nneg

define i32 @and_of_sext_to_zext_break_even(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r = and i32 %sx, 255
  ret i32 %r
}

; CHECK-LABEL: define i32 @and_of_sext_to_zext_break_even(
; CHECK: %sx = sext i8 %x to i32
; CHECK-NOT: zext i8 %x to i32

define i32 @whole_sext_to_zext_break_even(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %a = and i32 %sx, 255
  %b = and i32 %sx, 127
  %r = or i32 %a, %b
  ret i32 %r
}

; CHECK-LABEL: define i32 @whole_sext_to_zext_break_even(
; CHECK: %sx = sext i8 %x to i32
; CHECK-NOT: zext i8 %x to i32

define i1 @trunc_to_i1_zero_bounded_break_even() {
entry:
  %x = call i8 @ranged_byte(), !range !0
  %t = trunc i8 %x to i1
  ret i1 %t
}

; CHECK-LABEL: define i1 @trunc_to_i1_zero_bounded_break_even(
; CHECK: %x = call i8 @ranged_byte(), !range !0
; CHECK: %t = trunc i8 %x to i1
; CHECK-NOT: icmp ne i8 %x, 0
; CHECK: ret i1 %t

define i1 @trunc_to_i1_nuw_break_even(i8 %x) {
entry:
  %t = trunc nuw i8 %x to i1
  ret i1 %t
}

; CHECK-LABEL: define i1 @trunc_to_i1_nuw_break_even(
; CHECK: %t = trunc nuw i8 %x to i1
; CHECK-NOT: icmp ne i8 %x, 0
; CHECK: ret i1 %t

define i32 @push_freeze_through_ext_break_even(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %fr = freeze i32 %sx
  ret i32 %fr
}

; CHECK-LABEL: define i32 @push_freeze_through_ext_break_even(
; CHECK: %sx = sext i8 %x to i32
; CHECK: %fr = freeze i32 %sx
; CHECK-NOT: freeze i8 %x

attributes #0 = { nounwind }
!0 = !{i8 0, i8 2}
