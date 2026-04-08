; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; An icmp comparing an `add nuw` of two zexts against a constant can be
; narrowed: the sum of two i8 zero-extensions fits in i9 (max 255+255=510 <
; 512), so a comparison against any constant < 512 can run at i9 width.

define i1 @icmp_ult_add_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %sum = add nuw i32 %a32, %b32
  %cmp = icmp ult i32 %sum, 300
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_ult_add_nuw_zexts(
; CHECK-NOT: i32
; CHECK:      %{{.*}} = zext i8 %a to i9
; CHECK:      %{{.*}} = zext i8 %b to i9
; CHECK:      %{{.*}} = add i9
; CHECK:      %{{.*}} = icmp ult i9 {{.*}}, -212
; CHECK:      ret i1

; A second variant: eq/ne comparison (also valid for zero-bounded operands).
define i1 @icmp_eq_add_nuw_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %sum = add nuw i32 %a32, %b32
  %cmp = icmp eq i32 %sum, 100
  ret i1 %cmp
}

; CHECK-LABEL: define i1 @icmp_eq_add_nuw_zexts(
; CHECK-NOT: i32
; CHECK:      %{{.*}} = zext i8 %a to i9
; CHECK:      %{{.*}} = zext i8 %b to i9
; CHECK:      %{{.*}} = add i9
; CHECK:      %{{.*}} = icmp eq i9 {{.*}}, 100
; CHECK:      ret i1

define <4 x i1> @icmp_ult_add_nuw_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %sum = add nuw <4 x i32> %a32, %b32
  %cmp = icmp ult <4 x i32> %sum, <i32 300, i32 300, i32 300, i32 300>
  ret <4 x i1> %cmp
}

; CHECK-LABEL: define <4 x i1> @icmp_ult_add_nuw_zexts_vec(
; CHECK-NOT: <4 x i32>
; CHECK:      %{{.*}} = zext <4 x i8> %a to <4 x i9>
; CHECK:      %{{.*}} = zext <4 x i8> %b to <4 x i9>
; CHECK:      %{{.*}} = add <4 x i9>
; CHECK:      %{{.*}} = icmp ult <4 x i9>
; CHECK:      ret <4 x i1>
