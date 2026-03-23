; Vector division is outside the current scalar udiv narrowing logic.
; The pass should leave these cases alone rather than crashing.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i8> @f(<2 x i8> %x) {
entry:
  %a = udiv <2 x i8> %x, <i8 1, i8 1>
  ret <2 x i8> %a
}

define <2 x i8> @g(<2 x i8> %x) {
entry:
  %a = sdiv <2 x i8> %x, <i8 1, i8 1>
  ret <2 x i8> %a
}

; CHECK-LABEL: define <2 x i8> @f(
; CHECK: %[[A:.*]] = udiv <2 x i8> %x, {{.*}}
; CHECK: ret <2 x i8> %[[A]]

; CHECK-LABEL: define <2 x i8> @g(
; CHECK: %[[B:.*]] = sdiv <2 x i8> %x, {{.*}}
; CHECK: ret <2 x i8> %[[B]]
