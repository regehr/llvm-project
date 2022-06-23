; the function below can be simplified by InstCombine + DSE to "ret
; 0".  here we'll test the ability of reduceUsingOpt to run only
; InstCombine and not also DSE.

; RUN: llvm-reduce --delta-passes=using-opt --abort-on-invalid-reduction --test FileCheck --test-arg --check-prefixes=INTERESTINGNESS --test-arg %s --test-arg --input-file %s -o %t
; RUN: FileCheck %s --input-file=%t

; INTERESTINGNESS: store i32

; CHECK: ret i32 0

%struct.foo = type { [2 x i32] }

@g = dso_local global %struct.foo zeroinitializer, align 4

define dso_local i32 @test(i32 noundef %0, i32 noundef %1) {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca %struct.foo, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store i32 %1, ptr %4, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 %5, ptr align 4 @g, i64 8, i1 false)
  %9 = load i32, ptr %3, align 4
  store i32 %9, ptr @g, align 4
  %10 = load i32, ptr %4, align 4
  store i32 %10, ptr getelementptr inbounds ([2 x i32], ptr @g, i64 0, i64 1), align 4
  %11 = load i32, ptr @g, align 4
  %12 = xor i32 %11, -1
  %13 = load i32, ptr getelementptr inbounds ([2 x i32], ptr @g, i64 0, i64 1), align 4
  %14 = xor i32 %13, -1
  %15 = or i32 %12, %14
  store i32 %15, ptr %6, align 4
  %16 = load i32, ptr @g, align 4
  %17 = xor i32 %16, -1
  %18 = load i32, ptr getelementptr inbounds ([2 x i32], ptr @g, i64 0, i64 1), align 4
  %19 = xor i32 %18, -1
  %20 = and i32 %17, %19
  store i32 %20, ptr %7, align 4
  %21 = load i32, ptr %6, align 4
  %22 = load i32, ptr %7, align 4
  %23 = and i32 %21, %22
  %24 = load i32, ptr @g, align 4
  %25 = and i32 %23, %24
  store i32 %25, ptr %8, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 @g, ptr align 4 %5, i64 8, i1 false)
  %26 = load i32, ptr %8, align 4
  ret i32 %26
}

declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1
