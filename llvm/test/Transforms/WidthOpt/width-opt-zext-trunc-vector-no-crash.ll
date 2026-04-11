; Fixed-vector zext(trunc(x)) patterns are handled without crashing. Under the
; strict local-profitability policy this equal-cost mask fold no longer fires.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i65> @foo(<2 x i64> %t) {
entry:
  %a = trunc <2 x i64> %t to <2 x i32>
  %b = zext <2 x i32> %a to <2 x i65>
  ret <2 x i65> %b
}

; CHECK-LABEL: define <2 x i65> @foo(
; CHECK: %a = trunc <2 x i64> %t to <2 x i32>
; CHECK: %b = zext <2 x i32> %a to <2 x i65>
; CHECK: ret <2 x i65> %b
