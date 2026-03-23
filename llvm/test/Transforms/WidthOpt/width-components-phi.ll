; RUN: opt -passes='print<width-components>' -disable-output %s 2>&1 | FileCheck %s

define i32 @g(i1 %c, i32 %a, i32 %b) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i32 [ %a, %left ], [ %b, %right ]
  %f = freeze i32 %p
  %m = and i32 %f, 7
  ret i32 %m
}

; CHECK: Width components for function 'g':
; CHECK-DAG: component {{[0-9]+}}: width=i1, fixed=true
; CHECK-DAG: component {{[0-9]+}}: width=i32, fixed=true
; CHECK-DAG: %p [poly]
; CHECK-DAG: %f [poly]
; CHECK-DAG: %m [poly]
