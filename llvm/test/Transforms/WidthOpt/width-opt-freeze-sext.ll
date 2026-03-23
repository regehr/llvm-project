; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i8 %x) {
  %x16 = sext i8 %x to i16
  %fr = freeze i16 %x16
  ret i16 %fr
}

; CHECK-LABEL: define i16 @f(
; CHECK: %[[FR:.*]] = freeze i8 %x
; CHECK: %[[SE:.*]] = sext i8 %[[FR]] to i16
; CHECK: ret i16 %[[SE]]
