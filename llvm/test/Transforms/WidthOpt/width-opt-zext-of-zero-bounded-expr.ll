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

define <4 x i32> @zext_and_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a16 = zext <4 x i8> %a to <4 x i16>
  %b16 = zext <4 x i8> %b to <4 x i16>
  %and = and <4 x i16> %a16, %b16
  %ext = zext <4 x i16> %and to <4 x i32>
  ret <4 x i32> %ext
}

; CHECK-LABEL: define <4 x i32> @zext_and_zexts_vec(
; CHECK-NOT: zext <4 x i8> {{.*}} to <4 x i16>
; CHECK-NOT: and <4 x i16>
; CHECK-NOT: zext <4 x i16>
; CHECK: %[[AND:.*]] = and <4 x i8> %a, %b
; CHECK: %[[EXT:.*]] = zext <4 x i8> %[[AND]] to <4 x i32>
; CHECK: ret <4 x i32> %[[EXT]]

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

define <4 x i32> @zext_or_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a16 = zext <4 x i8> %a to <4 x i16>
  %b16 = zext <4 x i8> %b to <4 x i16>
  %or = or <4 x i16> %a16, %b16
  %ext = zext <4 x i16> %or to <4 x i32>
  ret <4 x i32> %ext
}

; CHECK-LABEL: define <4 x i32> @zext_or_zexts_vec(
; CHECK-NOT: zext <4 x i8> {{.*}} to <4 x i16>
; CHECK-NOT: or <4 x i16>
; CHECK-NOT: zext <4 x i16>
; CHECK: %[[OR:.*]] = or <4 x i8> %a, %b
; CHECK: %[[EXT:.*]] = zext <4 x i8> %[[OR]] to <4 x i32>
; CHECK: ret <4 x i32> %[[EXT]]

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

define <4 x i32> @zext_of_zext_vec(<4 x i8> %a) {
  %a16 = zext <4 x i8> %a to <4 x i16>
  %a32 = zext <4 x i16> %a16 to <4 x i32>
  ret <4 x i32> %a32
}

; CHECK-LABEL: define <4 x i32> @zext_of_zext_vec(
; CHECK-NOT: zext <4 x i8> {{.*}} to <4 x i16>
; CHECK-NOT: zext <4 x i16>
; CHECK: %[[EXT:.*]] = zext <4 x i8> %a to <4 x i32>
; CHECK: ret <4 x i32> %[[EXT]]
