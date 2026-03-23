; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i8 @f(i16 %x) {
  %x8 = trunc i16 %x to i8
  %fr = freeze i8 %x8
  ret i8 %fr
}

; CHECK-LABEL: define i8 @f(
; CHECK: %[[FR:.*]] = freeze i16 %x
; CHECK: %[[TR:.*]] = trunc i16 %[[FR]] to i8
; CHECK: ret i8 %[[TR]]
