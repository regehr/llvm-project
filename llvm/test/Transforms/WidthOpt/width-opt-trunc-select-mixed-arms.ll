; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i1 %c, i32 %x, i8 %y) {
entry:
  %sy = sext i8 %y to i32
  %sel = select i1 %c, i32 %x, i32 %sy
  %t = trunc i32 %sel to i16
  ret i16 %t
}

; CHECK-LABEL: define i16 @f(
; CHECK: %[[X16:.*]] = trunc i32 %x to i16
; CHECK: %[[Y16:.*]] = sext i8 %y to i16
; CHECK: %[[SEL:.*]] = select i1 %c, i16 %[[X16]], i16 %[[Y16]]
; CHECK-NOT: trunc i32 %sel to i16
; CHECK: ret i16 %[[SEL]]
