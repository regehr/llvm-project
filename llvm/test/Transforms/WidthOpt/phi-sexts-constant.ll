; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): NO
; LLVM shrinks the analogous zext PHI, but not this sext PHI with a signed constant.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i8 %sel, i8 %a, i8 %b) {
entry:
  switch i8 %sel, label %c [
    i8 0, label %a.bb
    i8 1, label %b.bb
  ]

a.bb:
  %sa = sext i8 %a to i32
  br label %merge

b.bb:
  %sb = sext i8 %b to i32
  br label %merge

c:
  br label %merge

merge:
  %p = phi i32 [ %sa, %a.bb ], [ %sb, %b.bb ], [ -1, %c ]
  ret i32 %p
}

; CHECK-LABEL: define i32 @f(
; CHECK: merge:
; CHECK: %[[PN:.*]] = phi i8 [ %a, %a.bb ], [ %b, %b.bb ], [ -1, %c ]
; CHECK: %[[W:.*]] = sext i8 %[[PN]] to i32
; CHECK: ret i32 %[[W]]
