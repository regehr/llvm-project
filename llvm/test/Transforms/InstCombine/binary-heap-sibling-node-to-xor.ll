; RUN: opt -O2 -S < %s | FileCheck %s

; Positive Tests
define i4 @sibling-node-i4(i4 %0) {
; CHECK-LABEL: @sibling-node-i4(
; CHECK:       [[d:%.+]] = xor i4 %0, 1
; CHECK-NEXT:  ret i4 [[d]]
;
  %a = urem i4 %0, 2
  %b = icmp eq i4 %a, 1
  br i1 %b, label %right,label %left
  left:
  %c = add i4 %0, 1
  ret i4 %c
  right:
  %d = sub i4 %0, 1
  ret i4 %d
}


define i16 @sibling-node-i16(i16 %0) {
; CHECK-LABEL: @sibling-node-i16(
; CHECK:       [[d:%.+]] = xor i16 %0, 1
; CHECK-NEXT:  ret i16 [[d]]
;
  %a = and i16 %0, 1
  %b = icmp eq i16 %a, 0
  %c = select i1 %b, i16 1, i16 -1
  %d = add i16 %c, %0
  ret i16 %d
}


; Negative Tests
define i8 @sibling-node-i8-fail(i8 %0) {
; CHECK-LABEL: @sibling-node-i8-fail(
; CHECK:      [[a:%.+]] = and i8 %0, 1
; CHECK-NEXT: [[b:%.+]] = icmp eq i8 [[a]], 0
; CHECK-NEXT: [[c:%.+]] = select i1 [[b]], i8 -1, i8 1
; CHECK-NEXT: [[d:%.+]] = add i8 [[c]], %0
; CHECK-NEXT: ret i8 [[d]]
;
  %a = urem i8 %0, 2
  %b = icmp eq i8 %a, 1
  br i1 %b, label %right,label %left
  left:
  %c = sub i8 %0, 1
  ret i8 %c
  right:
  %d = add i8 %0, 1
  ret i8 %d
}
