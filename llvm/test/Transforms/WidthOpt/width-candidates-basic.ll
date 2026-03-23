; RUN: opt -passes='print<width-candidates>' -disable-output %s 2>&1 | FileCheck %s

define i16 @f(i8 %x, i8 %y) {
entry:
  %x16 = zext i8 %x to i16
  %y16 = zext i8 %y to i16
  %cmp = icmp ult i16 %x16, %y16
  %sel = select i1 %cmp, i16 %y16, i16 %x16
  ret i16 %sel
}

; CHECK: Width candidates for function 'f':
; CHECK-DAG: component {{[0-9]+}}: orig=i8, fixed=true, candidates=i8,i16
; CHECK-DAG: component {{[0-9]+}}: orig=i16, fixed=false, candidates=i8,i16
; CHECK-DAG: component {{[0-9]+}}: orig=i1, fixed=true, candidates=i1
; CHECK: edge {{[0-9]+}} -> {{[0-9]+}}
