; EH blocks such as catchswitch headers may have PHIs but no legal insertion
; point for rebuilding a widened cast after the PHIs. The pass should leave
; those PHIs alone rather than crashing.
; no-alive: Alive2 does not support the invoke-based EH structure in this test.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc18.0.0"

declare i32 @__CxxFrameHandler3(...)
declare void @g(i32)

define void @test2(i8 %a, i8 %b) personality ptr @__CxxFrameHandler3 {
bb:
  %x = zext i8 %a to i32
  invoke void @g(i32 0)
    to label %cont
    unwind label %catch

cont:
  %y = zext i8 %b to i32
  invoke void @g(i32 0)
    to label %unreachable
    unwind label %catch

catch:
  %phi = phi i32 [ %x, %bb ], [ %y, %cont ]
  %cs = catchswitch within none [label %doit] unwind to caller

doit:
  %cl = catchpad within %cs []
  call void @g(i32 %phi)
  unreachable

unreachable:
  unreachable
}

; CHECK-LABEL: define void @test2(
; CHECK: %x = zext i8 %a to i32
; CHECK: %y = zext i8 %b to i32
; CHECK: catch:
; CHECK: %phi = phi i32 [ %x, %bb ], [ %y, %cont ]
; CHECK: %cs = catchswitch within none [label %doit] unwind to caller
