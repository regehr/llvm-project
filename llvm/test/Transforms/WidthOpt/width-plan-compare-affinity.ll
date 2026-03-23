; RUN: opt -passes='print<width-plan>' -disable-output %s 2>&1 | FileCheck %s

define i16 @f(i1 %c, i8 %k) {
entry:
  %s = select i1 %c, i8 1, i8 2
  %cmp = icmp eq i8 %s, %k
  %x = zext i8 %s to i16
  %cmp.z = zext i1 %cmp to i16
  %r = add i16 %x, %cmp.z
  ret i16 %r
}

; CHECK: Width plan for function 'f':
; CHECK-DAG: component {{[0-9]+}}: orig=i8, fixed=true, chosen=i8, candidates=i8
; CHECK-DAG: component {{[0-9]+}}: orig=i8, fixed=false, chosen=i8, candidates=i8,i16
; CHECK-DAG: component {{[0-9]+}}: orig=i16, fixed=false, chosen=i16, candidates=i8,i16
; CHECK-DAG: component {{[0-9]+}}: orig=i1, fixed=true, chosen=i1, candidates=i1
; CHECK: compare-affinity
