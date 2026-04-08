; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; The icmp operands are widened to i32, but a zext of %y to i16 already exists
; later in the block (used by the select).  After narrowing the icmp to i16 the
; pass should reuse that existing zext rather than creating a duplicate.

define i16 @src(i16 %x, i8 %y) {
  %x32 = sext i16 %x to i32
  %y32 = zext i8 %y to i32
  %c = icmp slt i32 %x32, %y32
  %y16 = zext i8 %y to i16
  %r = select i1 %c, i16 %y16, i16 %x
  ret i16 %r
}

; CHECK-LABEL: define i16 @src(
; CHECK:      %[[Y16:.*]] = zext i8 %y to i16
; CHECK-NEXT: %[[C:.*]] = icmp slt i16 %x, %[[Y16]]
; CHECK-NEXT: %[[R:.*]] = select i1 %[[C]], i16 %[[Y16]], i16 %x
; CHECK-NEXT: ret i16 %[[R]]
; CHECK-NOT:  zext i8 %y to i16

define <4 x i16> @src_vec(<4 x i16> %x, <4 x i8> %y) {
  %x32 = sext <4 x i16> %x to <4 x i32>
  %y32 = zext <4 x i8> %y to <4 x i32>
  %c = icmp slt <4 x i32> %x32, %y32
  %y16 = zext <4 x i8> %y to <4 x i16>
  %r = select <4 x i1> %c, <4 x i16> %y16, <4 x i16> %x
  ret <4 x i16> %r
}

; CHECK-LABEL: define <4 x i16> @src_vec(
; CHECK:      %[[Y16:.*]] = zext <4 x i8> %y to <4 x i16>
; CHECK-NEXT: %[[C:.*]] = icmp slt <4 x i16> %x, %[[Y16]]
; CHECK-NEXT: %[[R:.*]] = select <4 x i1> %[[C]], <4 x i16> %[[Y16]], <4 x i16> %x
; CHECK-NEXT: ret <4 x i16> %[[R]]
; CHECK-NOT:  zext <4 x i8> %y to <4 x i16>
