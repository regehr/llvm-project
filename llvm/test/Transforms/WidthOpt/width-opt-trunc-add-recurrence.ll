; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Loop-carried recurrences using low-bit-preserving operations truncated at
; exit.  When the init and step are materializable at i8, the whole recurrence
; can run at i8 width.  Covers add, sub, mul, and, or, xor.

define i8 @add_loop(i8 %init, i8 %step, i32 %n) {
entry:
  %init32 = zext i8 %init to i32
  %step32 = zext i8 %step to i32
  br label %loop
loop:
  %p = phi i32 [ %init32, %entry ], [ %next, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %next = add i32 %p, %step32
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, %n
  br i1 %done, label %exit, label %loop
exit:
  %t = trunc i32 %next to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @add_loop(
; CHECK: %[[PN:.*]] = phi i8 [ %init, %entry ], [ %[[ADD:.*]], %loop ]
; CHECK: %[[ADD]] = add i8 %[[PN]], %step
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: ret i8 %[[ADD]]

; Constant step variant.

define i8 @add_loop_const_step(i8 %init, i32 %n) {
entry:
  %init32 = zext i8 %init to i32
  br label %loop
loop:
  %p = phi i32 [ %init32, %entry ], [ %next, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %next = add i32 %p, 3
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, %n
  br i1 %done, label %exit, label %loop
exit:
  %t = trunc i32 %next to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @add_loop_const_step(
; CHECK: %[[PN:.*]] = phi i8 [ %init, %entry ], [ %[[ADD:.*]], %loop ]
; CHECK: %[[ADD]] = add i8 %[[PN]], 3
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: ret i8 %[[ADD]]

; Sub recurrence.

define i8 @sub_loop(i8 %init, i8 %step, i32 %n) {
entry:
  %init32 = zext i8 %init to i32
  %step32 = zext i8 %step to i32
  br label %loop
loop:
  %p = phi i32 [ %init32, %entry ], [ %next, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %next = sub i32 %p, %step32
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, %n
  br i1 %done, label %exit, label %loop
exit:
  %t = trunc i32 %next to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @sub_loop(
; CHECK: %[[PN:.*]] = phi i8 [ %init, %entry ], [ %[[SUB:.*]], %loop ]
; CHECK: %[[SUB]] = sub i8 %[[PN]], %step
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: ret i8 %[[SUB]]

; Xor recurrence.

define i8 @xor_loop(i8 %init, i8 %key, i32 %n) {
entry:
  %init32 = zext i8 %init to i32
  %key32 = zext i8 %key to i32
  br label %loop
loop:
  %p = phi i32 [ %init32, %entry ], [ %next, %loop ]
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %next = xor i32 %p, %key32
  %inc = add i32 %i, 1
  %done = icmp eq i32 %inc, %n
  br i1 %done, label %exit, label %loop
exit:
  %t = trunc i32 %next to i8
  ret i8 %t
}

; CHECK-LABEL: define i8 @xor_loop(
; CHECK: %[[PN:.*]] = phi i8 [ %init, %entry ], [ %[[XOR:.*]], %loop ]
; CHECK: %[[XOR]] = xor i8 %[[PN]], %key
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: ret i8 %[[XOR]]
