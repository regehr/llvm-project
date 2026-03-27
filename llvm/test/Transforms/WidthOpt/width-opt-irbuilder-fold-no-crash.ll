; RUN: opt -passes='width-opt,verify' -S %s | FileCheck %s

; Regression test: several WidthOpt helper functions used cast<Instruction> on
; the result of IRBuilder calls that can fold to a ConstantInt when the source
; operand is a constant.  In the O3 pipeline, passes such as chr run before
; the first instcombine, so they can create TruncInst / BinaryOperator nodes
; with constant operands that instcombine hasn't folded yet.  When width-opt
; then calls B.CreateTrunc / B.CreateAnd / B.CreateICmpNE / B.CreateBinOp on
; those, IRBuilder folds the result to a ConstantInt, and the subsequent
; cast<Instruction>(...) assertion-fails.
;
; The crash cannot be triggered directly in text IR (the IR parser folds
; trunc(constant) and similar immediately), so this file tests the normal
; (non-constant) paths through each affected function to ensure they still
; produce correct results after the cast<Instruction> → dyn_cast<Instruction>
; fix.

; ---- tryFoldTruncOfTrunc ------------------------------------------------
; trunc(trunc(a:i32→i16), i8) → trunc(a:i32→i8)
; The fixed code: B.CreateTrunc(Inner->getOperand(0), ...) result goes through
; dyn_cast<Instruction> for setDebugLoc instead of cast<Instruction>.

define i8 @trunc_of_trunc_i32_to_i16_to_i8(i32 %a) {
  %t16 = trunc i32 %a to i16
  %t8 = trunc i16 %t16 to i8
  ret i8 %t8
}
; CHECK-LABEL: define i8 @trunc_of_trunc_i32_to_i16_to_i8(
; CHECK-NOT: trunc i32 %a to i16
; CHECK: trunc i32 %a to i8
; CHECK: ret i8

; ---- tryFoldTruncToI1ViaLShrAndMask ------------------------------------
; trunc(lshr(x, K)) to i1  →  icmp ne (and x, 1<<K), 0
; The fixed code uses dyn_cast<Instruction> for both B.CreateAnd and
; B.CreateICmpNE results in case they fold (when X is constant).

define i1 @trunc_lshr_to_i1(i32 %x) {
  %sh = lshr i32 %x, 3
  %tr = trunc i32 %sh to i1
  ret i1 %tr
}
; CHECK-LABEL: define i1 @trunc_lshr_to_i1(
; CHECK-NOT: lshr
; CHECK-NOT: trunc
; CHECK: and i32 %x, 8
; CHECK: icmp ne i32
; CHECK: ret i1

; ---- tryFoldTruncToI1WhenSrcIsZeroBounded ------------------------------
; trunc i32 %zext to i1  where %zext = zext i1 → icmp ne %zext, 0
; zext i1→i32 is zero-bounded at 1 bit so width-opt converts this.
; The fixed code uses dyn_cast<Instruction> for B.CreateICmpNE result.

define i1 @trunc_to_i1_zero_bounded(i1 %flag) {
  %wide = zext i1 %flag to i32
  %tr = trunc i32 %wide to i1
  ret i1 %tr
}
; CHECK-LABEL: define i1 @trunc_to_i1_zero_bounded(
; CHECK-NOT: zext
; CHECK-NOT: trunc
; CHECK: ret i1 %flag

; ---- tryFoldTruncNuwToI1 -----------------------------------------------
; trunc nuw i32 %x to i1  →  icmp ne i32 %x, 0
; The fixed code uses dyn_cast<Instruction> for B.CreateICmpNE result.

define i1 @trunc_nuw_to_i1(i32 %x) {
  %tr = trunc nuw i32 %x to i1
  ret i1 %tr
}
; CHECK-LABEL: define i1 @trunc_nuw_to_i1(
; CHECK-NOT: trunc
; CHECK: icmp ne i32 %x, 0
; CHECK: ret i1

; ---- tryNarrowUDivWithRange (zext-bounded operands) --------------------
; udiv of zext-bounded operands with a trunc result → narrow the udiv.
; The fixed code uses Value* / dyn_cast<Instruction> instead of
; cast<Instruction> for B.CreateBinOp and B.CreateZExt.

define i8 @narrow_udiv_zext_bounded(i8 %a, i8 %b) {
  %wa = zext i8 %a to i32
  %wb = zext i8 %b to i32
  %div = udiv i32 %wa, %wb
  %tr = trunc i32 %div to i8
  ret i8 %tr
}
; CHECK-LABEL: define i8 @narrow_udiv_zext_bounded(
; CHECK: udiv i8 %a, %b
; CHECK: ret i8

; ---- tryFoldTruncOfCtpop (zext-bounded ctpop source) --------------------
; trunc(ctpop(zext(a:i8) to i32), i8) → ctpop.i8(a)
; The fixed code uses Value* + dyn_cast<Instruction> for B.CreateCall in case
; the NarrowValue is a constant (ctpop of a constant would fold).

define i8 @trunc_ctpop_zext(i8 %a) {
  %wide = zext i8 %a to i32
  %pop = call i32 @llvm.ctpop.i32(i32 %wide)
  %tr = trunc i32 %pop to i8
  ret i8 %tr
}
; CHECK-LABEL: define i8 @trunc_ctpop_zext(
; CHECK-NOT: zext
; CHECK-NOT: ctpop i32
; CHECK: @llvm.ctpop.i8(i8 %a)
; CHECK: ret i8

declare i32 @llvm.ctpop.i32(i32)
