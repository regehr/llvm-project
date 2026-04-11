; Fixed-vector trunc(select) patterns can shrink through the select just like
; scalar trunc(select), but this case is only break-even locally, so it now
; stays unchanged. The test still checks that WidthOpt handles the pattern
; without crashing.
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
; CHECK: %sel = select <2 x i1> %[[CMP]], <2 x i64> %z, <2 x i64> <i64 42, i64 7>
; CHECK: %r = trunc <2 x i64> %sel to <2 x i32>
; CHECK-NOT: select <2 x i1> %[[CMP]], <2 x i32>
; CHECK: ret <2 x i32> %r
