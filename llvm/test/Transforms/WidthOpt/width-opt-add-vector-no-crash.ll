; Vector add/zext patterns are outside the current scalar add widening logic.
; The pass should leave them alone rather than crashing.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i32> @add_nuw_zext_add_vec(<2 x i16> %x) {
entry:
  %add = add nuw <2 x i16> %x, <i16 -42, i16 5>
  %ext = zext <2 x i16> %add to <2 x i32>
  %r = add <2 x i32> %ext, <i32 356, i32 -12>
  ret <2 x i32> %r
}

; CHECK-LABEL: define <2 x i32> @add_nuw_zext_add_vec(
; CHECK: %[[ADD:.*]] = add nuw <2 x i16> %x, {{.*}}
; CHECK: %[[EXT:.*]] = zext <2 x i16> %[[ADD]] to <2 x i32>
; CHECK: %[[R:.*]] = add <2 x i32> %[[EXT]], {{.*}}
; CHECK: ret <2 x i32> %[[R]]
