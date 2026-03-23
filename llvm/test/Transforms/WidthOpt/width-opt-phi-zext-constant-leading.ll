; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; A leading constant incoming should not block shrinking a PHI of zexts.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %sel, i8 %a, i8 %b) {
entry:
  switch i8 %sel, label %c [
    i8 0, label %a.bb
    i8 1, label %b.bb
  ]

a.bb:
  %za = zext i8 %a to i32
  br label %merge

b.bb:
  %zb = zext i8 %b to i32
  br label %merge

c:
  br label %merge

merge:
  %p = phi i32 [ 7, %c ], [ %za, %a.bb ], [ %zb, %b.bb ]
  ret i32 %p
}

; CHECK-LABEL: define i32 @f(
; CHECK: merge:
; CHECK: %[[PN:.*]] = phi i8 [ 7, %c ], [ %a, %a.bb ], [ %b, %b.bb ]
; CHECK: %[[W:.*]] = zext i8 %[[PN]] to i32
; CHECK: ret i32 %[[W]]
