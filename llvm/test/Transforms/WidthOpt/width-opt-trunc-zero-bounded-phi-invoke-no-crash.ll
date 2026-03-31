; RUN: opt -non-global-value-max-name-size=-1 -passes=width-opt -S %s -o - | FileCheck %s

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @invoke_phi_trunc_no_crash() personality ptr null {
entry:
  br i1 false, label %merge, label %invoke.block

invoke.block:
  %call = invoke i64 null(ptr null, i64 0)
          to label %merge unwind label %lpad

merge:
  %wide = phi i64 [ %call, %invoke.block ], [ 0, %entry ]
  %narrow = trunc i64 %wide to i32
  ret i32 %narrow

lpad:
  %lp = landingpad { ptr, i32 }
          cleanup
  ret i32 0
}

; CHECK-LABEL: define i32 @invoke_phi_trunc_no_crash()
; CHECK: %call = invoke i64 null(ptr null, i64 0)
; CHECK: merge:
; CHECK: %wide = phi i64 [ %call, %invoke.block ], [ 0, %entry ]
; CHECK: %narrow = trunc i64 %wide to i32
; CHECK: ret i32 %narrow
