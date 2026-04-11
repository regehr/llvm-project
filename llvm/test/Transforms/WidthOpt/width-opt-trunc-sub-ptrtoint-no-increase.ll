; Trunc-rooted sub should not be narrowed when both operands are wide values
; (e.g. ptrtoint) that cannot themselves be narrowed without inserting new trunc
; instructions. Narrowing sub(ptrtoint(p1), ptrtoint(p2)) would replace
; trunc(sub i64) with sub(trunc i64, trunc i64), increasing instruction count.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; CHECK-LABEL: define i32 @ptrtoint_sub_no_increase(
; CHECK:      %p1i = ptrtoint ptr %p1 to i64
; CHECK:      %p2i = ptrtoint ptr %p2 to i64
; CHECK:      %diff = sub i64 %p1i, %p2i
; CHECK:      %tr = trunc i64 %diff to i32
; CHECK-NOT:  trunc i64 %p1i
; CHECK-NOT:  trunc i64 %p2i
; CHECK:      ret i32 %tr
define i32 @ptrtoint_sub_no_increase(ptr %p1, ptr %p2) {
  %p1i = ptrtoint ptr %p1 to i64
  %p2i = ptrtoint ptr %p2 to i64
  %diff = sub i64 %p1i, %p2i
  %tr = trunc i64 %diff to i32
  ret i32 %tr
}
