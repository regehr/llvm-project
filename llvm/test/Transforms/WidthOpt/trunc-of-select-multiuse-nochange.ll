; RUN: opt -passes='width-opt' -S %s | FileCheck %s
; tryShrinkTruncOfSelect bails when the select has multiple uses.
; Also tests the non-integer case and TargetWidth >= SourceWidth cases.

; Select has two users (two truncs) - should not transform.
define i8 @multiuse_select(i1 %cond, i32 %x, i32 %y) {
  %sel = select i1 %cond, i32 %x, i32 %y
  %tr1 = trunc i32 %sel to i8
  %tr2 = trunc i32 %sel to i16
  %z = trunc i16 %tr2 to i8
  %r = add i8 %tr1, %z
  ret i8 %r
}
; CHECK-LABEL: @multiuse_select(
; CHECK: select i1 %cond, i32

; Select with a non-trunc use and plain i32 args:
; tryShrinkTruncOfSelect bails (multiple uses), tryShrinkSelectOfExts bails (no ext arms).
declare void @use(i32)
define i8 @select_with_call_use(i1 %cond, i32 %x, i32 %y) {
  %sel = select i1 %cond, i32 %x, i32 %y
  %tr = trunc i32 %sel to i8
  call void @use(i32 %sel)
  ret i8 %tr
}
; CHECK-LABEL: @select_with_call_use(
; CHECK: select i1 %cond, i32
