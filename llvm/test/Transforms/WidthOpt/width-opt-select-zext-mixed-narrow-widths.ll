; Mixed-width zext arms can still be shrunk through a common intermediate
; width. Here the select should move to i16 instead of requiring both arms to
; have the same exact narrow width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i1 %c, i8 %a, i16 %b) {
entry:
  %a32 = zext i8 %a to i32
  %b32 = zext i16 %b to i32
  %s = select i1 %c, i32 %a32, i32 %b32
  %x = add i32 %s, 1
  %y = xor i32 %s, 42
  %r = add i32 %x, %y
  ret i32 %r
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[A16:.*]] = zext i8 %a to i16
; CHECK-NOT: %b32 = zext i16 %b to i32
; CHECK: %[[SN:.*]] = select i1 %c, i16 %[[A16]], i16 %b
; CHECK: %[[SW:.*]] = zext i16 %[[SN]] to i32
; CHECK-NOT: select i1 %c, i32

define <4 x i32> @f_vec(<4 x i1> %c, <4 x i8> %a, <4 x i16> %b) {
entry:
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i16> %b to <4 x i32>
  %s = select <4 x i1> %c, <4 x i32> %a32, <4 x i32> %b32
  %x = add <4 x i32> %s, <i32 1, i32 1, i32 1, i32 1>
  %y = xor <4 x i32> %s, <i32 42, i32 42, i32 42, i32 42>
  %r = add <4 x i32> %x, %y
  ret <4 x i32> %r
}

; CHECK-LABEL: define <4 x i32> @f_vec(
; CHECK: %[[A16:.*]] = zext <4 x i8> %a to <4 x i16>
; CHECK-NOT: %b32 = zext <4 x i16> %b to <4 x i32>
; CHECK: %[[SN:.*]] = select <4 x i1> %c, <4 x i16> %[[A16]], <4 x i16> %b
; CHECK: %[[SW:.*]] = zext <4 x i16> %[[SN]] to <4 x i32>
; CHECK-NOT: select <4 x i1> %c, <4 x i32>
