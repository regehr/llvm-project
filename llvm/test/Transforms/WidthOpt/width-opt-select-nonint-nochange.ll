; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define float @f(i1 %c, float %x, float %y) {
entry:
  %s = select i1 %c, float %x, float %y
  ret float %s
}

; CHECK-LABEL: define float @f(
; CHECK: %s = select i1 %c, float %x, float %y
; CHECK: ret float %s
