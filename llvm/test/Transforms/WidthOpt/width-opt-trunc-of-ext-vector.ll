; Fixed-vector trunc(ext) folds to the intermediate extension width.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define <2 x i16> @trunc_ext(<2 x i8> %a) {
entry:
  %z = zext <2 x i8> %a to <2 x i32>
  %t = trunc <2 x i32> %z to <2 x i16>
  ret <2 x i16> %t
}

; CHECK-LABEL: define <2 x i16> @trunc_ext(
; CHECK: %[[WIDE:.*]] = zext <2 x i8> %a to <2 x i16>
; CHECK: ret <2 x i16> %[[WIDE]]
