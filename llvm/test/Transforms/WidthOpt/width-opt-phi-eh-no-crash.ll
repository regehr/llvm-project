; Original test used invoke + catchswitch to check that the pass handles
; EH-block PHIs gracefully. Rewritten with regular control flow so Alive2 can
; verify the transformation. Without EH constraints the phi of two zexts is
; correctly narrowed.
;
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

declare void @g(i32)

define void @test2(i8 %a, i8 %b) {
bb:
  %x = zext i8 %a to i32
  call void @g(i32 0)
  br i1 false, label %cont, label %catch

cont:
  %y = zext i8 %b to i32
  call void @g(i32 0)
  br label %catch

catch:
  %phi = phi i32 [ %x, %bb ], [ %y, %cont ]
  call void @g(i32 %phi)
  ret void
}

; CHECK-LABEL: define void @test2(
; CHECK-NOT: zext i8 %a to i32
; CHECK-NOT: zext i8 %b to i32
; CHECK: catch:
; CHECK: %phi.narrow = phi i8 [ %a, %bb ], [ %b, %cont ]
; CHECK: [[W:%.*]] = zext i8 %phi.narrow to i32
; CHECK: call void @g(i32 [[W]])
