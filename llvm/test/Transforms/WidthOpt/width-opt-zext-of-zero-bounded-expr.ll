; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; When the operand of a zext is structurally zero-bounded at a width narrower
; than the zext's source type, we can rebuild the operand at the narrow width
; and emit a single zext from that narrower type to the final wide type.

; zext(and(zext(a:i8→i16), zext(b:i8→i16)) to i32)
; => and(a, b):i8 then zext to i32
define i32 @zext_and_zexts(i8 %a, i8 %b) {
  %a16 = zext i8 %a to i16
  %b16 = zext i8 %b to i16
  %and = and i16 %a16, %b16
  %ext = zext i16 %and to i32
  ret i32 %ext
}

; CHECK-LABEL: define i32 @zext_and_zexts(
; CHECK-NOT: zext i8 {{.*}} to i16
; CHECK-NOT: and i16
; CHECK-NOT: zext i16
; CHECK: %[[AND:.*]] = and i8 %a, %b
; CHECK: %[[EXT:.*]] = zext i8 %[[AND]] to i32
; CHECK: ret i32 %[[EXT]]

; zext(or(zext(a:i8→i16), zext(b:i8→i16)) to i32)
; => or(a, b):i8 then zext to i32
define i32 @zext_or_zexts(i8 %a, i8 %b) {
  %a16 = zext i8 %a to i16
  %b16 = zext i8 %b to i16
  %or = or i16 %a16, %b16
  %ext = zext i16 %or to i32
  ret i32 %ext
}

; CHECK-LABEL: define i32 @zext_or_zexts(
; CHECK-NOT: zext i8 {{.*}} to i16
; CHECK-NOT: or i16
; CHECK-NOT: zext i16
; CHECK: %[[OR:.*]] = or i8 %a, %b
; CHECK: %[[EXT:.*]] = zext i8 %[[OR]] to i32
; CHECK: ret i32 %[[EXT]]

; zext(zext(a:i8→i16) to i32) => zext(a:i8→i32)
define i32 @zext_of_zext(i8 %a) {
  %a16 = zext i8 %a to i16
  %a32 = zext i16 %a16 to i32
  ret i32 %a32
}

; CHECK-LABEL: define i32 @zext_of_zext(
; CHECK-NOT: zext i8 {{.*}} to i16
; CHECK-NOT: zext i16
; CHECK: %[[EXT:.*]] = zext i8 %a to i32
; CHECK: ret i32 %[[EXT]]
