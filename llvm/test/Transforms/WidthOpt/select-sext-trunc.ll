; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; A trunc of a select over sext-derived arithmetic can be rebuilt directly at
; the narrower width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i8 %a, i1 %cond) {
entry:
  %conv = sext i8 %a to i32
  %sub = sub nsw i32 0, %conv
  %sel = select i1 %cond, i32 %sub, i32 %conv
  %t = trunc i32 %sel to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @f(
; CHECK: %[[CONV:.*]] = sext i8 %a to i16
; CHECK: %[[SUB:.*]] = sub i16 0, %[[CONV]]
; CHECK: %[[SEL:.*]] = select i1 %cond, i16 %[[SUB]], i16 %[[CONV]]
; CHECK-NOT: sext i8 %a to i32
; CHECK-NOT: trunc i32
; CHECK: ret i16 %[[SEL]]
