; RUN: opt -passes='print<width-plan>' -disable-output %s 2>&1 | FileCheck %s

define i32 @f(i1 %c) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ 1, %left ], [ 2, %right ]
  %x = zext i8 %p to i32
  %y = zext i8 %p to i32
  %z = zext i8 %p to i32
  %r = add i32 %x, %y
  %s = add i32 %r, %z
  ret i32 %s
}

; CHECK: Width plan for function 'f':
; CHECK-DAG: component {{[0-9]+}}: orig=i8, fixed=false, chosen=i32, candidates=i8,i32
; CHECK-DAG: component {{[0-9]+}}: orig=i32, fixed=false, chosen=i32, candidates=i8,i32
; CHECK-DAG: component {{[0-9]+}}: orig=i32, fixed=true, chosen=i32, candidates=i32
