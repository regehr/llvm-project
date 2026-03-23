; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): NO
; The mixed-width compare is narrowed independently of the select.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i16 @f(i16 %x, i8 %y) {
  %x32 = sext i16 %x to i32
  %y32 = zext i8 %y to i32
  %c = icmp ne i32 %x32, %y32
  %y16 = zext i8 %y to i16
  %r = select i1 %c, i16 %y16, i16 %x
  ret i16 %r
}

; CHECK-LABEL: define i16 @f(
; CHECK: %[[YCMP:.*]] = zext i8 %y to i16
; CHECK: %[[CMP:.*]] = icmp ne i16 %x, %[[YCMP]]
; CHECK: %[[YSEL:.*]] = zext i8 %y to i16
; CHECK: %[[R:.*]] = select i1 %[[CMP]], i16 %[[YSEL]], i16 %x
; CHECK-NOT: icmp ne i32
; CHECK: ret i16 %[[R]]
