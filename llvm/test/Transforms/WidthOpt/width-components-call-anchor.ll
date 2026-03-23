; RUN: opt -passes='print<width-components>' -disable-output %s 2>&1 | FileCheck %s

declare i16 @g(i16)

define i16 @f(i16 %x, ptr %p, ptr %fp) {
entry:
  %ld = load i16, ptr %p, align 2
  %pi = ptrtoint ptr %p to i64
  %tr = trunc i64 %pi to i16
  %call.fixed = call i16 @g(i16 %ld)
  %callee = load ptr, ptr %fp, align 8
  %call.indirect = call i16 %callee(i16 %ld)
  %sum0 = add i16 %ld, %call.fixed
  %sum1 = add i16 %sum0, %call.indirect
  %sum2 = add i16 %sum1, %tr
  ret i16 %sum2
}

; CHECK: Width components for function 'f':
; CHECK-DAG: [arg]
; CHECK-DAG: [anchor]
