; Vector trunc-rooted add patterns are outside the current scalar trunc-of-add
; shrinking logic. The pass should leave them alone rather than crashing.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i16> @narrow_sext_add_commute(<2 x i16> %x16, <2 x i32> %y32) {
entry:
  %y32op0 = sdiv <2 x i32> %y32, <i32 7, i32 -17>
  %x32 = sext <2 x i16> %x16 to <2 x i32>
  %b = add <2 x i32> %y32op0, %x32
  %r = trunc <2 x i32> %b to <2 x i16>
  ret <2 x i16> %r
}

; CHECK-LABEL: define <2 x i16> @narrow_sext_add_commute(
; CHECK: %[[YOP:.*]] = sdiv <2 x i32> %y32, {{.*}}
; CHECK: %[[X32:.*]] = sext <2 x i16> %x16 to <2 x i32>
; CHECK: %[[ADD:.*]] = add <2 x i32> %[[YOP]], %[[X32]]
; CHECK: %[[R:.*]] = trunc <2 x i32> %[[ADD]] to <2 x i16>
; CHECK: ret <2 x i16> %[[R]]
