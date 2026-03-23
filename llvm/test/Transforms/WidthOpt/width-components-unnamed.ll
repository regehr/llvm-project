; RUN: opt -passes='print<width-components>' -disable-output %s 2>&1 | FileCheck %s

define i16 @f(i16 %x, i16 %y) {
  %1 = and i16 %x, %y
  %2 = or i16 %1, %x
  ret i16 %2
}

; CHECK: Width components for function 'f':
; CHECK-DAG: %1 [poly]
; CHECK-DAG: %2 [poly]
