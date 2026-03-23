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
