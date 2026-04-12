; RUN: opt -passes='width-opt' -S %s | FileCheck %s

; icmp ult (zext i8 %a), (ctlz i32 %x, i1 false):
; tryShrinkICmpZeroBounded: TargetWidth=8 (from zext i8 %a).
; isZeroBoundedAtWidth(ctlz i32 %x, i1 false, 8) hits the ctlz/cttz path
; (lines 2865-2872 of WidthOpt.cpp): ArgBits=32, is_zero_poison=false,
; MaxVal=32, APInt(64, 32).isIntN(8) → 32 < 256 → true.
; However collectTruncRootedValueCost cannot handle ctlz, so the cost check
; fails and no transformation is performed.

declare i32 @llvm.ctlz.i32(i32, i1)

define i1 @ctlz_bounded_icmp_nochange(i8 %a, i32 %x) {
  %za = zext i8 %a to i32
  %cl = call i32 @llvm.ctlz.i32(i32 %x, i1 false)
  %c = icmp ult i32 %za, %cl
  ret i1 %c
}
; CHECK-LABEL: define i1 @ctlz_bounded_icmp_nochange(
; CHECK:       %za = zext i8 %a to i32
; CHECK:       %cl = call i32 @llvm.ctlz.i32(i32 %x, i1 false)
; CHECK:       %c = icmp ult i32 %za, %cl
; CHECK:       ret i1 %c

; Same for cttz.
declare i32 @llvm.cttz.i32(i32, i1)

define i1 @cttz_bounded_icmp_nochange(i8 %a, i32 %x) {
  %za = zext i8 %a to i32
  %ct = call i32 @llvm.cttz.i32(i32 %x, i1 false)
  %c = icmp ult i32 %za, %ct
  ret i1 %c
}
; CHECK-LABEL: define i1 @cttz_bounded_icmp_nochange(
; CHECK:       %za = zext i8 %a to i32
; CHECK:       %ct = call i32 @llvm.cttz.i32(i32 %x, i1 false)
; CHECK:       %c = icmp ult i32 %za, %ct
; CHECK:       ret i1 %c
