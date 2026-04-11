; RUN: opt -passes='width-opt' -S %s | FileCheck %s
;
; Cover mixed-width operand planning in tryNarrowUDivWithRange(), including
; the path that introduces a new intermediate zext for the narrower operand.

define i16 @udiv_mixed_narrow_widths_trunc(i16 %x, i8 %y) {
entry:
  %x32 = zext i16 %x to i32
  %y32 = zext i8 %y to i32
  %d = udiv i32 %x32, %y32
  %t = trunc i32 %d to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @udiv_mixed_narrow_widths_trunc(
; CHECK-NOT: zext i16 %x to i32
; CHECK-NOT: zext i8 %y to i32
; CHECK: %[[RHS:.*]] = zext i8 %y to i16
; CHECK: %[[D:.*]] = udiv i16 %x, %[[RHS]]
; CHECK-NOT: trunc i32
; CHECK: ret i16 %[[D]]

declare void @llvm.assume(i1)

define i16 @udiv_range_and_zext(i16 %x, i32 %y) {
entry:
  %x32 = zext i16 %x to i32
  %cy = icmp ult i32 %y, 65536
  call void @llvm.assume(i1 %cy)
  %d = udiv i32 %x32, %y
  %t = trunc i32 %d to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @udiv_range_and_zext(
; CHECK: %cy = icmp ult i32 %y, 65536
; CHECK: call void @llvm.assume(i1 %cy)
; CHECK-NOT: zext i16 %x to i32
; CHECK: %[[RHS:.*]] = trunc i32 %y to i16
; CHECK: %[[D:.*]] = udiv i16 %x, %[[RHS]]
; CHECK-NOT: udiv i32
; CHECK: ret i16 %[[D]]
