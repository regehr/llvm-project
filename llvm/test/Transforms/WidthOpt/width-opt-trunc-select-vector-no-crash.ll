; Vector trunc(select) patterns are outside the current scalar trunc-rooted
; select shrinking logic. The pass should leave them alone rather than crash.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i32> @trunc_vec(<2 x i64> %x, <2 x i64> %y, <2 x i64> %z) {
entry:
  %cmp = icmp ugt <2 x i64> %x, %y
  %sel = select <2 x i1> %cmp, <2 x i64> %z, <2 x i64> <i64 42, i64 7>
  %r = trunc <2 x i64> %sel to <2 x i32>
  ret <2 x i32> %r
}

; CHECK-LABEL: define <2 x i32> @trunc_vec(
; CHECK: %[[CMP:.*]] = icmp ugt <2 x i64> %x, %y
; CHECK: %[[SEL:.*]] = select <2 x i1> %[[CMP]], <2 x i64> %z, {{.*}}
; CHECK: %[[R:.*]] = trunc <2 x i64> %[[SEL]] to <2 x i32>
; CHECK: ret <2 x i32> %[[R]]
