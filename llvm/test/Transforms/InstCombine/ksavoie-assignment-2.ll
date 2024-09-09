; RUN: opt < %s -passes='instcombine<no-verify-fixpoint>' -S | FileCheck %s

define noundef i32 @reduce-case(i32 noundef %x, i32 noundef %y) unnamed_addr #0 {
; CHECK-LABEL: @reduce-case(
; CHECK-NEXT:    [[A:%.*]] = and i32 [[Y:%.*]], 1
; CHECK-NEXT:    [[B:%.*]] = xor i32 [[A:%.*]], -1
; CHECK-NEXT:    [[C:%.*]] = and i32 [[B:%.*]], [[X:%.*]]
; CHECK-NEXT:    ret i32 [[C:%.*]]

  %1 = and i32 %x, 1
  %2 = xor i32 %y, -1
  %3 = and i32 %1, %2
  %4 = and i32 %x, -2
  %5 = or disjoint i32 %3, %4 

  ret i32 %5
}

define noundef i32 @reduce-case-fail-diff-const(i32 noundef %x, i32 noundef %y) unnamed_addr #0 {
; CHECK-LABEL: @reduce-case-fail-diff-const(
; CHECK-NEXT:    [[A:%.*]] = and i32 [[X:%.*]], 255
; CHECK-NEXT:    [[B:%.*]] = xor i32 [[Y:%.*]], -1
; CHECK-NEXT:    [[C:%.*]] = and i32 [[A:%.*]], [[B:%.*]]
; CHECK-NEXT:    [[D:%.*]] = and i32 [[X:%.*]], -2
; CHECK-NEXT:    [[E:%.*]] = or disjoint i32 [[C:%.*]], [[D:%.*]] 
; CHECK-NEXT:    ret i32 [[E:%.*]]

  %1 = and i32 %x, 255
  %2 = xor i32 %y, -1
  %3 = and i32 %1, %2
  %4 = and i32 %x, -2
  %5 = or disjoint i32 %3, %4 

  ret i32 %5
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind nonlazybind willreturn memory(none) uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }
