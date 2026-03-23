; Fixed-vector zext(trunc(x)) patterns can use the same low-bit mask fold as
; scalars.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i65> @foo(<2 x i64> %t) {
entry:
  %a = trunc <2 x i64> %t to <2 x i32>
  %b = zext <2 x i32> %a to <2 x i65>
  ret <2 x i65> %b
}

; CHECK-LABEL: define <2 x i65> @foo(
; CHECK: %[[MASK:.*]] = and <2 x i64> %t, splat (i64 4294967295)
; CHECK: %[[B:.*]] = zext{{( nneg)?}} <2 x i64> %[[MASK]] to <2 x i65>
; CHECK: ret <2 x i65> %[[B]]
