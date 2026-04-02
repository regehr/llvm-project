; Regression test: when tryShrinkTruncOfLowBitsBinOp handles multiple trunc
; users, it sets InsertPt = BO->getNextNode(). If that next node happens to be
; the existing cast that materializeAtWidth finds and wants to reuse, the new
; binop would be inserted before that cast (use-before-def dominance violation).
;
; RUN: opt -passes=width-opt -S %s | FileCheck %s
; CHECK: define

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@g_6   = external global i8
@g_94  = external global i32
@g_108 = external global i64

; After inlining and optimization the IR looks like this.  The critical pattern:
;   entry block contains:
;     %or.i  = or i64 (zext i1 %not..b), 74     ; BO
;     %conv47.i = zext i1 %not..b to i32         ; existing i32 cast = BO->getNextNode()
;   Two blocks each trunc %or.i to i32.
;
; tryShrinkTruncOfLowBitsBinOp sees two trunc-of-or users, sets
; InsertPt = BO->getNextNode() = %conv47.i, calls materializeAtWidth which
; found %conv47.i as a reusable cast.  Without the fix it returns %conv47.i and
; the new "or i32 %conv47.i, 74" is inserted BEFORE %conv47.i.

define noundef i16 @func_1() {
entry:
  %.b = load i1, ptr @g_6, align 1
  %not..b = xor i1 %.b, true
  %conv27.i = zext i1 %not..b to i64
  %or.i = or disjoint i64 %conv27.i, 74
  %conv47.i = zext i1 %not..b to i32
  br i1 %.b, label %exit, label %preheader

preheader:
  %or.i.tr = trunc nuw nsw i64 %or.i to i32
  %v0 = shl nuw nsw i32 %or.i.tr, 1
  store i32 %v0, ptr @g_94, align 4
  %p0 = load volatile ptr, ptr null, align 8
  %l0 = load i32, ptr %p0, align 4
  %x0 = xor i32 %l0, %conv47.i
  store i32 %x0, ptr %p0, align 4
  store i64 0, ptr @g_108, align 8
  br label %loop

loop:
  %or.i.tr4 = trunc nuw nsw i64 %or.i to i32
  %v1 = shl nuw nsw i32 %or.i.tr4, 1
  store i32 %v1, ptr @g_94, align 4
  %p1 = load volatile ptr, ptr null, align 8
  %l1 = load i32, ptr %p1, align 4
  %x1 = xor i32 %l1, %conv47.i
  store i32 %x1, ptr %p1, align 4
  store i64 0, ptr @g_108, align 8
  store i32 0, ptr @g_94, align 4
  br label %loop

exit:
  ret i16 0
}
