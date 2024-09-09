; RUN: opt < %s -passes=instcombine -S | FileCheck %s

; positive tests
; x * x + c1 > c2 where c1 > c2 => isnan(x)
define i32 @t1(float %x) {
    %1 = fmul float %x, %x
    %2 = fadd float %1, 77.0e+00
    %3 = fcmp ogt float %2, 22.0e+00
    %4 = select i1 %3, i32 1, i32 2
    ret i32 %4
    ; CHECK-LABEL: @t1(
    ; CHECK: [[r1:%.*]] = fcmp ord float %x, 0.000000e+00
    ; CHECK-NEXT: [[r2:%.*]] = select i1 [[r1]], i32 1, i32 2 
    ; CHECK-NEXT: ret i32 [[r2]]
}

; x * x + c1 < c2 where c1 > c2 => false
define i32 @t2(float %x) {
    %1 = fmul float %x, %x
    %2 = fadd float %1, 77.0e+00
    %3 = fcmp olt float %2, 22.0e+00
    %4 = select i1 %3, i32 1, i32 2
    ret i32 %4
    ; CHECK-LABEL: @t2(
    ; CHECK-NEXT: ret i32 2
}

; negative test
; x * x + c1 > c2 where c1 < c2 => no optimization
define i32 @t3(float %x) {
    %1 = fmul float %x, %x
    %2 = fadd float %1, 22.0e+00
    %3 = fcmp ogt float %2, 77.0e+00
    %4 = select i1 %3, i32 1, i32 2
    ret i32 %4
    ; CHECK-LABEL: @t3(
    ; CHECK-NOT:  [[r0:%.*]] = fcmp ord float %x, 0.000000e+00
    ; CHECK:      [[r1:%.*]] = fcmp ogt float %2, 7.700000e+01
    ; CHECK-NEXT: [[r2:%.*]] = select i1 [[r1]], i32 1, i32 2 
    ; CHECK-NEXT: ret i32 [[r2]]
}
