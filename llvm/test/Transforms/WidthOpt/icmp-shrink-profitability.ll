; tryShrinkICmp must not narrow a mixed sext/zext comparison when doing so
; would add more cast instructions than it removes.  The canonical case is a
; zext+sext equality where the sext producer has multiple uses: narrowing to
; i33 would emit two new casts (zext→i33, sext→i33) while only one producer
; (the single-use zext) is removable, making the transformation unprofitable.
;
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; Single-use producers: profitable, transformation fires.
; sext i16→i32 and zext i8→i32 both have one use; narrowing to i16 removes
; both and emits one new zext i8→i16.
define i1 @single_use_profitable(i16 %x, i8 %y) {
  %sx = sext i16 %x to i32
  %zy = zext i8 %y to i32
  %c = icmp eq i32 %sx, %zy
  ret i1 %c
}

; CHECK-LABEL: define i1 @single_use_profitable(
; CHECK:       %[[ZY:.*]] = zext i8 %y to i16
; CHECK:       %[[CMP:.*]] = icmp eq i16 %x, %[[ZY]]
; CHECK-NOT:   icmp eq i32
; CHECK:       ret i1 %[[CMP]]

; Multi-use sext producer: unprofitable.
; Narrowing the equality from i64 to i33 would need two new casts
; (zext i32→i33, sext i32→i33) but %sx has two uses so only the zext
; producer can be removed — added cost (2) exceeds removed cost (1).
; The icmp must remain at i64.
define i32 @multi_use_sext_no_narrow(i32 %x, i32 %y, ptr %p) {
  %sx = sext i32 %x to i64
  %zy = zext i32 %y to i64
  %c = icmp eq i64 %zy, %sx
  store i64 %sx, ptr %p
  %r = select i1 %c, i32 1, i32 0
  ret i32 %r
}

; CHECK-LABEL: define i32 @multi_use_sext_no_narrow(
; CHECK:       %sx = sext i32 %x to i64
; CHECK:       %zy = zext i32 %y to i64
; CHECK:       %c = icmp eq i64 %zy, %sx
; CHECK-NOT:   i33
; CHECK:       ret i32
