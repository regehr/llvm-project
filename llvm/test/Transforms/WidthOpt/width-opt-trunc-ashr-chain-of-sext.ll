; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; A chain of ashrs rooted at a sext, truncated back to the original narrow
; width, is equivalent to a single ashr by the total shift amount.
; trunc(ashr(ashr(sext(a:N→W), k1), k2), N) = ashr(a, k1+k2).

define i8 @ashr_chain_depth2(i8 %a) {
  %a32 = sext i8 %a to i32
  %s1 = ashr i32 %a32, 2
  %s2 = ashr i32 %s1, 1
  %t = trunc i32 %s2 to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @ashr_chain_depth2(
; CHECK-NOT: sext
; CHECK-NOT: ashr i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = ashr i8 %a, 3
; CHECK: ret i8 %[[R]]

define i8 @ashr_chain_depth3(i8 %a) {
  %a32 = sext i8 %a to i32
  %s1 = ashr i32 %a32, 1
  %s2 = ashr i32 %s1, 1
  %s3 = ashr i32 %s2, 2
  %t = trunc i32 %s3 to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @ashr_chain_depth3(
; CHECK-NOT: sext
; CHECK-NOT: ashr i32
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = ashr i8 %a, 4
; CHECK: ret i8 %[[R]]

define <4 x i8> @ashr_chain_depth2_vec(<4 x i8> %a) {
  %a32 = sext <4 x i8> %a to <4 x i32>
  %s1 = ashr <4 x i32> %a32, splat (i32 2)
  %s2 = ashr <4 x i32> %s1, splat (i32 1)
  %t = trunc <4 x i32> %s2 to <4 x i8>
  ret <4 x i8> %t
}

; CHECK-LABEL: define <4 x i8> @ashr_chain_depth2_vec(
; CHECK-NOT: sext
; CHECK-NOT: ashr <4 x i32>
; CHECK-NOT: trunc
; CHECK: %[[R:.*]] = ashr <4 x i8> %a, splat (i8 3)
; CHECK: ret <4 x i8> %[[R]]
