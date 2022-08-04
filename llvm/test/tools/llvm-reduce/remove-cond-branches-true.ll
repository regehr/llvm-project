; RUN: llvm-reduce --delta-passes=cond-branches-true --test %python --test-arg %p/Inputs/remove-bbs.py -abort-on-invalid-reduction %s -o %t

; RUN: FileCheck --check-prefix=CHECK-FINAL %s --input-file=%t
; CHECK-FINAL-NOT: br i1
; CHECK-FINAL: br label %x11

define void @f1(ptr %interesting3, i32 %interesting2, i1 %b) {
  %x3 = alloca ptr, i32 0, align 8
  store ptr %interesting3, ptr %interesting3, align 8
  switch i32 %interesting2, label %interesting1 [
    i32 0, label %x6
    i32 1, label %x11
  ]

x4:
  %x5 = call ptr @f2()
  br label %x10

x10:
  br label %x6

x6:
  br i1 %b, label %x11, label %x12

x11:
  br label %interesting1

x12:
  br label %interesting1

interesting1:
  ret void
}

declare ptr @f2()
