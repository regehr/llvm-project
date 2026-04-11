; Current LLVM (/Users/regehr/llvm-project/for-alive/bin/opt -passes='default<O2>' -S): YES
; Demanded bits can change a shared sext into a shared zext even with multiple
; uses, but that rewrite is only break-even locally and is now disabled.
; RUN: opt -passes='width-opt' -S %s | FileCheck %s

define i32 @f(i16 %a) {
entry:
  %ext = sext i16 %a to i32
  %and = and i32 %ext, 65280
  %lsr = lshr i32 %ext, 8
  %and2 = and i32 %lsr, 255
  %or = or i32 %and, %and2
  ret i32 %or
}

; CHECK-LABEL: define i32 @f(
; CHECK: %[[EXT:.*]] = sext i16 %a to i32
; CHECK: %[[AND:.*]] = and i32 %[[EXT]], 65280
; CHECK: %[[LSR:.*]] = lshr i32 %[[EXT]], 8
; CHECK: %[[AND2:.*]] = and i32 %[[LSR]], 255
; CHECK: %[[OR:.*]] = or i32 %[[AND]], %[[AND2]]
; CHECK: ret i32 %[[OR]]
