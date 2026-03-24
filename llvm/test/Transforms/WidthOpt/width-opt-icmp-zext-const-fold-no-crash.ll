; An icmp whose operands are zexts of constants triggered a cast<ICmpInst>
; assertion when IRBuilder folded B.CreateICmp(pred, C1, C2) to a ConstantInt
; rather than creating a new ICmpInst.  The constant operands arise when
; IPSCCP replaces a load-from-null with undef, which then folds through zext.
; RUN: opt -passes='width-opt' -S %s -o /dev/null

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @icmp_of_const_zexts() {
entry:
  %a = zext i8 0 to i32
  %b = zext i1 false to i32
  %cmp = icmp sgt i32 %a, %b
  ret i1 %cmp
}
