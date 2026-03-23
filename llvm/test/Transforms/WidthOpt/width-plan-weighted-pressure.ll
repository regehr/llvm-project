; RUN: opt -passes='print<width-plan>' -disable-output %s 2>&1 | FileCheck %s

declare i32 @sink3(i32, i32, i32)

define i32 @f(i1 %c, i8 %k) {
entry:
  br i1 %c, label %left, label %right

left:
  br label %merge

right:
  br label %merge

merge:
  %p = phi i8 [ 1, %left ], [ 2, %right ]
  %s = select i1 %c, i8 %p, i8 3
  %cmp = icmp eq i8 %s, %k
  %x = zext i8 %p to i32
  %y = zext i8 %s to i32
  %mix = xor i32 %x, %y
  %call = call i32 @sink3(i32 %mix, i32 %x, i32 %y)
  %cmp.z = zext i1 %cmp to i32
  %r = add i32 %call, %cmp.z
  ret i32 %r
}

; CHECK: Width plan for function 'f':
; CHECK-DAG: component {{[0-9]+}}: orig=i8, fixed=true, chosen=i8, candidates=i8
; CHECK-DAG: component {{[0-9]+}}: orig=i8, fixed=false, chosen=i32, candidates=i8,i32
; CHECK-DAG: component {{[0-9]+}}: orig=i32, fixed=false, chosen=i32, candidates=i8,i32
; CHECK: compare-affinity {{[0-9]+}} <-> {{[0-9]+}} weight=1
