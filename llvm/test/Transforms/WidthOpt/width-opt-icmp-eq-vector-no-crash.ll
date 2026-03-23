; Vector trunc-based equality patterns are outside the current scalar equality
; widening logic. The pass should leave them alone rather than crashing.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i1> @eq_21_vector(<2 x i32> %x, <2 x i32> %y) {
entry:
  %x.321 = lshr <2 x i32> %x, <i32 8, i32 8>
  %x.1 = trunc <2 x i32> %x.321 to <2 x i8>
  %x.32 = lshr <2 x i32> %x, <i32 16, i32 16>
  %x.2 = trunc <2 x i32> %x.32 to <2 x i8>
  %y.321 = lshr <2 x i32> %y, <i32 8, i32 8>
  %y.1 = trunc <2 x i32> %y.321 to <2 x i8>
  %y.32 = lshr <2 x i32> %y, <i32 16, i32 16>
  %y.2 = trunc <2 x i32> %y.32 to <2 x i8>
  %c.1 = icmp eq <2 x i8> %x.1, %y.1
  %c.2 = icmp eq <2 x i8> %x.2, %y.2
  %c.210 = and <2 x i1> %c.2, %c.1
  ret <2 x i1> %c.210
}

; CHECK-LABEL: define <2 x i1> @eq_21_vector(
; CHECK: %[[X1:.*]] = lshr <2 x i32> %x, {{.*}}
; CHECK: %[[TX1:.*]] = trunc <2 x i32> %[[X1]] to <2 x i8>
; CHECK: %[[X2:.*]] = lshr <2 x i32> %x, {{.*}}
; CHECK: %[[TX2:.*]] = trunc <2 x i32> %[[X2]] to <2 x i8>
; CHECK: %[[Y1:.*]] = lshr <2 x i32> %y, {{.*}}
; CHECK: %[[TY1:.*]] = trunc <2 x i32> %[[Y1]] to <2 x i8>
; CHECK: %[[Y2:.*]] = lshr <2 x i32> %y, {{.*}}
; CHECK: %[[TY2:.*]] = trunc <2 x i32> %[[Y2]] to <2 x i8>
; CHECK: %[[C1:.*]] = icmp eq <2 x i8> %[[TX1]], %[[TY1]]
; CHECK: %[[C2:.*]] = icmp eq <2 x i8> %[[TX2]], %[[TY2]]
; CHECK: %[[CA:.*]] = and <2 x i1> %[[C2]], %[[C1]]
; CHECK: ret <2 x i1> %[[CA]]
