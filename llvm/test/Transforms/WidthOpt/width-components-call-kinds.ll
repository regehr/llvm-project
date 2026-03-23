; RUN: opt -passes='print<width-components>' -disable-output %s 2>&1 | FileCheck %s

declare i16 @llvm.smax.i16(i16, i16)
declare i16 @llvm.bswap.i16(i16)

define i16 @f(i16 %x) {
entry:
  %a = call i16 @llvm.smax.i16(i16 %x, i16 7)
  %b = call i16 @llvm.bswap.i16(i16 %x)
  %c = call i16 asm "", "=r,0"(i16 %x)
  %d = add i16 %a, %b
  %e = add i16 %d, %c
  ret i16 %e
}

; CHECK: Width components for function 'f':
; CHECK-DAG: %a [conditional]
; CHECK-DAG: %b [anchor]
; CHECK-DAG: %c [anchor]
