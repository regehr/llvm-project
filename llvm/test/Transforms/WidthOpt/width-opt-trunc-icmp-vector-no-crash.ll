; Vector trunc/icmp patterns are outside the current scalar compare-widening
; logic. The pass should leave them alone rather than crashing.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i1> @test36vec(<2 x i32> %a) {
entry:
  %b = lshr <2 x i32> %a, <i32 31, i32 31>
  %c = trunc <2 x i32> %b to <2 x i8>
  %d = icmp eq <2 x i8> %c, zeroinitializer
  ret <2 x i1> %d
}

; CHECK-LABEL: define <2 x i1> @test36vec(
; CHECK: %[[B:.*]] = lshr <2 x i32> %a, {{.*}}
; CHECK: %[[C:.*]] = trunc <2 x i32> %[[B]] to <2 x i8>
; CHECK: %[[D:.*]] = icmp eq <2 x i8> %[[C]], zeroinitializer
; CHECK: ret <2 x i1> %[[D]]
