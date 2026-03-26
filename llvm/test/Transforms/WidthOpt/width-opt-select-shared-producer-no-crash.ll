; RUN: opt -passes='width-opt,verify' -S %s | FileCheck %s

; Regression test: tryShrinkSelectOfExts must not double-free the ext producer
; when both arms of the select refer to the same instruction.
;
; Before the fix, the cleanup at the end of the function did:
;   RecursivelyDeleteTriviallyDeadInstructions(TrueExt->Producer);
;   RecursivelyDeleteTriviallyDeadInstructions(FalseExt->Producer); // UAF!
; When TrueExt->Producer == FalseExt->Producer the second call dereferenced
; a freed pointer, causing a heap-corruption crash.

define i8 @shared_sext_producer(i1 %cond, i8 %a) {
  %ext = sext i8 %a to i32
  ; Both arms use the same ext — shared producer.
  %sel = select i1 %cond, i32 %ext, i32 %ext
  %tr = trunc i32 %sel to i8
  ret i8 %tr
}

; CHECK-LABEL: define i8 @shared_sext_producer(
; CHECK-NOT: sext
; CHECK-NOT: trunc
; CHECK: select i1 %cond, i8 %a, i8 %a
; CHECK: ret i8

define i8 @shared_zext_producer(i1 %cond, i8 %a) {
  %ext = zext i8 %a to i32
  %sel = select i1 %cond, i32 %ext, i32 %ext
  %tr = trunc i32 %sel to i8
  ret i8 %tr
}

; CHECK-LABEL: define i8 @shared_zext_producer(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: select i1 %cond, i8 %a, i8 %a
; CHECK: ret i8
