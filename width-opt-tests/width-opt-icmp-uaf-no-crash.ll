
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @icmp_lhs_uses_rhs_dangling_ptr(i8 %v) {
entry:
  %zext  = zext i8 %v to i64
  %xor   = xor i64 %zext, 0
  %cmp   = icmp ne i64 %xor, %zext
  ret i1 %cmp
}
