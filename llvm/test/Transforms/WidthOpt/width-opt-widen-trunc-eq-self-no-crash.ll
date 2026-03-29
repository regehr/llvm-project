; RUN: opt -passes='width-opt' -S %s -o /dev/null
; Verify no crash when tryWidenTruncEqualityICmp encounters an icmp where both
; operands are the same TruncInst (icmp eq %t, %t).  After erasing the icmp,
; LHS == RHS, so deleting through LHS first would leave RHS as a dangling pointer.

define i1 @test_trunc_self_eq(i8 %x) {
entry:
  %zext = zext i8 %x to i32
  %t = trunc i32 %zext to i16
  %cmp = icmp eq i16 %t, %t
  ret i1 %cmp
}
