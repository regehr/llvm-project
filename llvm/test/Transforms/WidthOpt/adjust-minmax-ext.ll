; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; InstCombine adjusts ext(select(cmp)) into a narrower min/max shape and widens once.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i64 @f(i32 %a) {
entry:
  %a_ext = sext i32 %a to i64
  %cmp = icmp sgt i32 %a, -1
  %max = select i1 %cmp, i64 %a_ext, i64 0
  ret i64 %max
}

; CHECK-LABEL: define i64 @f(
; CHECK-NOT: sext i32 %a to i64
; CHECK: %[[CMP:.*]] = icmp sgt i32 %a, -1
; CHECK: %[[MAX:.*]] = select i1 %[[CMP]], i32 %a, i32 0
; CHECK: %[[EXT:.*]] = sext i32 %[[MAX]] to i64
; CHECK: ret i64 %[[EXT]]
