; RUN: opt -passes=width-opt -S %s | FileCheck %s

; Scalar ne: zext + icmp ne both removed, expect narrowed to i1.
define void @test_expect_ne_zero(i1 %x, ptr %p) {
; CHECK-LABEL: @test_expect_ne_zero(
; CHECK-NEXT:    %[[E:.*]] = call i1 @llvm.expect.i1(i1 %x, i1 false)
; CHECK-NEXT:    br i1 %[[E]], label %then, label %end
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

; Scalar eq: zext removed, icmp replaced with xor, expect narrowed to i1.
define void @test_expect_eq_zero(i1 %x, ptr %p) {
; CHECK-LABEL: @test_expect_eq_zero(
; CHECK-NEXT:    %[[E:.*]] = call i1 @llvm.expect.i1(i1 %x, i1 true)
; CHECK-NEXT:    %[[X:.*]] = xor i1 %[[E]], true
; CHECK-NEXT:    br i1 %[[X]], label %then, label %end
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

; Scalar expect.with.probability: zext + icmp ne both removed.
define void @test_expect_with_probability(i1 %x, ptr %p) {
; CHECK-LABEL: @test_expect_with_probability(
; CHECK-NEXT:    %[[E:.*]] = call i1 @llvm.expect.with.probability.i1(i1 %x, i1 true, double 9.000000e-01)
; CHECK-NEXT:    br i1 %[[E]], label %then, label %end
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

; Vector ne: same optimization applies to <4 x i1>.
define void @test_expect_ne_zero_vec(<4 x i1> %x, ptr %p) {
; CHECK-LABEL: @test_expect_ne_zero_vec(
; CHECK-NEXT:    %[[E:.*]] = call <4 x i1> @llvm.expect.v4i1(<4 x i1> %x, <4 x i1> zeroinitializer)
; CHECK-NEXT:    store <4 x i1> %[[E]], ptr %p
  %z = zext <4 x i1> %x to <4 x i32>
  %exp = call <4 x i32> @llvm.expect.v4i32(<4 x i32> %z, <4 x i32> zeroinitializer)
  %cmp = icmp ne <4 x i32> %exp, zeroinitializer
  store <4 x i1> %cmp, ptr %p
  ret void
}

; Vector eq: negation via xor.
define void @test_expect_eq_zero_vec(<4 x i1> %x, ptr %p) {
; CHECK-LABEL: @test_expect_eq_zero_vec(
; CHECK-NEXT:    %[[E:.*]] = call <4 x i1> @llvm.expect.v4i1(<4 x i1> %x, <4 x i1> splat (i1 true))
; CHECK-NEXT:    %[[X:.*]] = xor <4 x i1> %[[E]], splat (i1 true)
; CHECK-NEXT:    store <4 x i1> %[[X]], ptr %p
  %z = zext <4 x i1> %x to <4 x i32>
  %exp = call <4 x i32> @llvm.expect.v4i32(<4 x i32> %z, <4 x i32> <i32 1, i32 1, i32 1, i32 1>)
  %cmp = icmp eq <4 x i32> %exp, zeroinitializer
  store <4 x i1> %cmp, ptr %p
  ret void
}

declare i64 @llvm.expect.i64(i64, i64)
declare i64 @llvm.expect.with.probability.i64(i64, i64, double)
declare <4 x i32> @llvm.expect.v4i32(<4 x i32>, <4 x i32>)
