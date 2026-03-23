; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i1 %c, i8 %k) {
entry:
  %s = select i1 %c, i8 1, i8 2
  %cmp = icmp eq i8 %s, %k
  %x = zext i8 %s to i16
  %cmp.z = zext i1 %cmp to i16
  %r = add i16 %x, %cmp.z
  ret i16 %r
}

; CHECK-LABEL: define i16 @f(
; CHECK: %[[S:.*]] = select i1 %c, i8 1, i8 2
; CHECK: %[[CMP:.*]] = icmp eq i8 %[[S]], %k
; CHECK: %[[X:.*]] = zext nneg i8 %[[S]] to i16
; CHECK: %[[CMPZ:.*]] = zext i1 %[[CMP]] to i16
; CHECK: %[[R:.*]] = add i16 %[[X]], %[[CMPZ]]
; CHECK: ret i16 %[[R]]
; CHECK-NOT: select i1 %c, i16 1, i16 2
