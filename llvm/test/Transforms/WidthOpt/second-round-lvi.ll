; RUN: opt -passes='width-opt' -S %s | FileCheck %s
; After round 1 structural pass narrows icmp sgt i32 (sext i8 %x), 0 to icmp sgt i8 %x, 0,
; LVI in round 2 can prove %x >= 0 in the true branch, marking the zext nneg.
; This exercises lines 4664-4665 in WidthOpt.cpp (second-round LVI triggering
; a second structural pass).

define i32 @second_round_lvi(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %cmp = icmp sgt i32 %sx, 0
  br i1 %cmp, label %then, label %else

then:
  %zx = zext i8 %x to i32
  ret i32 %zx

else:
  ret i32 0
}
; CHECK-LABEL: @second_round_lvi(
; CHECK: icmp sgt i8 %x, 0
; CHECK: zext nneg i8 %x to i32
