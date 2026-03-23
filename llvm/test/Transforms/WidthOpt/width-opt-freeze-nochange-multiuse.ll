; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i8 %x) {
  %x16 = zext i8 %x to i16
  %fr = freeze i16 %x16
  %a = add i16 %x16, 1
  %b = add i16 %fr, 2
  %r = add i16 %a, %b
  ret i16 %r
}

; CHECK-LABEL: define i16 @f(
; CHECK: %[[X16:.*]] = zext i8 %x to i16
; CHECK: %[[FR:.*]] = freeze i16 %[[X16]]
; CHECK: ret i16
