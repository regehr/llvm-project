; RUN: opt -passes='print<width-candidates>' -disable-output %s 2>&1 | FileCheck %s

define i8 @f(i1 %c) {
entry:
  %x = zext i1 %c to i8
  ret i8 %x
}

; CHECK: Width candidates for function 'f':
; CHECK-DAG: orig=i1, fixed=true, candidates=i1
; CHECK-DAG: orig=i8, fixed=false, candidates=i8
