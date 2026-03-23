; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; A chain of lshrs rooted at a sext, truncated back to the original narrow
; width, is equivalent to a single ashr by the total shift amount.
; trunc(lshr(lshr(sext(a:N→W), k1), k2), N) = ashr(a, k1+k2).

define i8 @lshr_chain_depth2(i8 %a) {
  %a32 = sext i8 %a to i32
  %s1 = lshr i32 %a32, 2
  %s2 = lshr i32 %s1, 1
  %t = trunc i32 %s2 to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @lshr_chain_depth2(
; CHECK-NOT: sext
; CHECK-NOT: lshr i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = ashr i8 %a, 3
; CHECK: ret i8 %[[R]]

define i8 @lshr_chain_depth3(i8 %a) {
  %a32 = sext i8 %a to i32
  %s1 = lshr i32 %a32, 1
  %s2 = lshr i32 %s1, 1
  %s3 = lshr i32 %s2, 2
  %t = trunc i32 %s3 to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @lshr_chain_depth3(
; CHECK-NOT: sext
; CHECK-NOT: lshr i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = ashr i8 %a, 4
; CHECK: ret i8 %[[R]]
