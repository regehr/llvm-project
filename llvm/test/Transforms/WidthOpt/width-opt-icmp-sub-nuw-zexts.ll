; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; sub nuw: the result is <= the LHS, so it is bounded by the LHS's width.
; An icmp against a constant that fits in i8 can be narrowed from i32 to i8.

define i1 @icmp_ult_sub_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %diff = sub nuw i32 %a32, %b32
  %cmp = icmp ult i32 %diff, 200
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ult_sub_nuw_zexts(
; CHECK-NOT: i32
; CHECK:      %{{.*}} = sub i8 %a, %b
; CHECK:      %{{.*}} = icmp ult i8
; CHECK:      ret i1

; eq/ne also valid for zero-bounded operands.
define i1 @icmp_eq_sub_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %diff = sub nuw i32 %a32, %b32
  %cmp = icmp eq i32 %diff, 50
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_eq_sub_nuw_zexts(
; CHECK-NOT: i32
; CHECK:      %{{.*}} = sub i8 %a, %b
; CHECK:      %{{.*}} = icmp eq i8 {{.*}}, 50
; CHECK:      ret i1
