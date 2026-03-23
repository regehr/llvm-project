; Vector ext/icmp patterns are outside the current scalar shrink logic.
; The pass should leave them alone rather than crashing.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i1> @lt_signed_to_large_unsigned_vec(<2 x i8> %sb) {
entry:
  %y = sext <2 x i8> %sb to <2 x i32>
  %c = icmp ult <2 x i32> %y, <i32 1024, i32 2>
  ret <2 x i1> %c
}

; CHECK-LABEL: define <2 x i1> @lt_signed_to_large_unsigned_vec(
; CHECK: %[[Y:.*]] = sext <2 x i8> %sb to <2 x i32>
; CHECK: %[[C:.*]] = icmp ult <2 x i32> %[[Y]], <i32 1024, i32 2>
; CHECK: ret <2 x i1> %[[C]]
