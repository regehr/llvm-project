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

; The pass now reuses the single zext for both the icmp and the select,
; eliminating the duplicate that used to appear in earlier output.
; CHECK-LABEL: define i16 @f(
; CHECK:      %[[Y16:.*]] = zext i8 %y to i16
; CHECK-NEXT: %[[CMP:.*]] = icmp ne i16 %x, %[[Y16]]
; CHECK-NEXT: %[[R:.*]] = select i1 %[[CMP]], i16 %[[Y16]], i16 %x
; CHECK-NEXT: ret i16 %[[R]]
; CHECK-NOT:  zext i8 %y to i16
