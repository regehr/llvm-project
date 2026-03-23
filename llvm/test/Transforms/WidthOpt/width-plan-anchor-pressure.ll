; RUN: opt -passes='print<width-plan>' -disable-output %s 2>&1 | FileCheck %s

declare void @use8(i8)

define i32 @f(i1 %c) {
entry:
  %s = select i1 %c, i8 -1, i8 2
  call void @use8(i8 %s)
  %x = sext i8 %s to i32
  %y = zext i8 %s to i32
  %r = add i32 %x, %y
  ret i32 %r
}

; CHECK: Width plan for function 'f':
; CHECK-DAG: component {{[0-9]+}}: orig=i8, fixed=false, chosen=i8, candidates=i8,i32
; CHECK: anchor-pressure component {{[0-9]+}} -> i8 weight=1
