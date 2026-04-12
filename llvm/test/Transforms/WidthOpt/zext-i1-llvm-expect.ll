; When a zext i1 feeds only an llvm.expect.i64 whose only use is icmp ne/eq
; with zero, we can eliminate the zext and use llvm.expect.i1 instead.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define void @test_expect_ne_zero(i1 %x, ptr %p) {
  %z = zext i1 %x to i64
  %exp = call i64 @llvm.expect.i64(i64 %z, i64 0)
  %cmp = icmp ne i64 %exp, 0
  br i1 %cmp, label %then, label %end
then:
  store i32 1, ptr %p
  br label %end
end:
  ret void
}

; CHECK-LABEL: define void @test_expect_ne_zero(
; CHECK-NOT: zext
; CHECK-NOT: llvm.expect.i64
; CHECK: call i1 @llvm.expect.i1(i1 %x, i1 false)

define void @test_expect_eq_zero(i1 %x, ptr %p) {
  %z = zext i1 %x to i64
  %exp = call i64 @llvm.expect.i64(i64 %z, i64 1)
  %cmp = icmp eq i64 %exp, 0
  br i1 %cmp, label %then, label %end
then:
  store i32 2, ptr %p
  br label %end
end:
  ret void
}

; CHECK-LABEL: define void @test_expect_eq_zero(
; CHECK-NOT: zext
; CHECK-NOT: llvm.expect.i64
; CHECK: call i1 @llvm.expect.i1(i1 %x, i1 true)

; llvm.expect.with.probability variant: same shrink, third argument (probability) kept.
define void @test_expect_with_probability(i1 %x, ptr %p) {
  %z = zext i1 %x to i64
  %exp = call i64 @llvm.expect.with.probability.i64(i64 %z, i64 1, double 0.9)
  %cmp = icmp ne i64 %exp, 0
  br i1 %cmp, label %then, label %end
then:
  store i32 1, ptr %p
  br label %end
end:
  ret void
}

; CHECK-LABEL: define void @test_expect_with_probability(
; CHECK-NOT: zext
; CHECK-NOT: llvm.expect.with.probability.i64
; CHECK: call i1 @llvm.expect.with.probability.i1(i1 %x, i1 true, double 9.000000e-01)

declare i64 @llvm.expect.i64(i64, i64)
declare i64 @llvm.expect.with.probability.i64(i64, i64, double)
