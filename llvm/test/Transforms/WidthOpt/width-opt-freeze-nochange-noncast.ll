; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i16 %x, i16 %y) {
  %sum = add i16 %x, %y
  %fr = freeze i16 %sum
  ret i16 %fr
}

; CHECK-LABEL: define i16 @f(
; CHECK: %[[SUM:.*]] = add i16 %x, %y
; CHECK: %[[FR:.*]] = freeze i16 %[[SUM]]
; CHECK: ret i16 %[[FR]]
