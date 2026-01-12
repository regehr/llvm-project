; RUN: opt -passes=constraint-elimination -S %s | FileCheck %s --check-prefix=KB
; RUN: opt -passes=constraint-elimination -constraint-elimination-use-known-bits=0 -S %s | FileCheck %s --check-prefix=NOKB

define i1 @knownbits_fallback_lshr(i32 %x) {
; KB-LABEL: @knownbits_fallback_lshr(
; KB:       entry:
; KB-NEXT:    [[SHR:%.*]] = lshr i32 %x, 31
; KB-NEXT:    ret i1 true
; NOKB-LABEL: @knownbits_fallback_lshr(
; NOKB:       entry:
; NOKB-NEXT:    [[SHR:%.*]] = lshr i32 %x, 31
; NOKB-NEXT:    [[CMP:%.*]] = icmp ult i32 [[SHR]], 2
; NOKB-NEXT:    ret i1 [[CMP]]
entry:
  %shr = lshr i32 %x, 31
  %cmp = icmp ult i32 %shr, 2
  ret i1 %cmp
}

define i1 @knownbits_fallback_or_uge(i32 %x) {
; KB-LABEL: @knownbits_fallback_or_uge(
; KB:       entry:
; KB-NEXT:    [[ORVAL:%.*]] = or i32 %x, 8
; KB-NEXT:    ret i1 true
; NOKB-LABEL: @knownbits_fallback_or_uge(
; NOKB:       entry:
; NOKB-NEXT:    [[ORVAL:%.*]] = or i32 %x, 8
; NOKB-NEXT:    [[CMP:%.*]] = icmp uge i32 [[ORVAL]], 8
; NOKB-NEXT:    ret i1 [[CMP]]
entry:
  %or = or i32 %x, 8
  %cmp = icmp uge i32 %or, 8
  ret i1 %cmp
}

define i1 @knownbits_same_sign_negative_slt(i32 %x, i32 %y) {
; KB-LABEL: @knownbits_same_sign_negative_slt(
; KB:       entry:
; KB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; KB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; KB-NEXT:    [[ULT:%.*]] = icmp ult i32 [[AX]], [[AY]]
; KB-NEXT:    br i1 [[ULT]], label %then, label %exit
; KB:       then:
; KB-NEXT:    ret i1 true
; KB:       exit:
; KB-NEXT:    ret i1 false
; NOKB-LABEL: @knownbits_same_sign_negative_slt(
; NOKB:       entry:
; NOKB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; NOKB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; NOKB-NEXT:    [[ULT:%.*]] = icmp ult i32 [[AX]], [[AY]]
; NOKB-NEXT:    br i1 [[ULT]], label %then, label %exit
; NOKB:       then:
; NOKB-NEXT:    [[CMP:%.*]] = icmp slt i32 [[AX]], [[AY]]
; NOKB-NEXT:    ret i1 [[CMP]]
; NOKB:       exit:
; NOKB-NEXT:    ret i1 false
entry:
  %ax = or i32 %x, -2147483648
  %ay = or i32 %y, -2147483648
  %ult = icmp ult i32 %ax, %ay
  br i1 %ult, label %then, label %exit

then:
  %cmp = icmp slt i32 %ax, %ay
  ret i1 %cmp

exit:
  ret i1 false
}

define i1 @knownbits_same_sign_negative_sgt(i32 %x, i32 %y) {
; KB-LABEL: @knownbits_same_sign_negative_sgt(
; KB:       entry:
; KB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; KB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; KB-NEXT:    [[UGT:%.*]] = icmp ugt i32 [[AX]], [[AY]]
; KB-NEXT:    br i1 [[UGT]], label %then, label %exit
; KB:       then:
; KB-NEXT:    ret i1 true
; KB:       exit:
; KB-NEXT:    ret i1 false
; NOKB-LABEL: @knownbits_same_sign_negative_sgt(
; NOKB:       entry:
; NOKB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; NOKB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; NOKB-NEXT:    [[UGT:%.*]] = icmp ugt i32 [[AX]], [[AY]]
; NOKB-NEXT:    br i1 [[UGT]], label %then, label %exit
; NOKB:       then:
; NOKB-NEXT:    [[CMP:%.*]] = icmp sgt i32 [[AX]], [[AY]]
; NOKB-NEXT:    ret i1 [[CMP]]
; NOKB:       exit:
; NOKB-NEXT:    ret i1 false
entry:
  %ax = or i32 %x, -2147483648
  %ay = or i32 %y, -2147483648
  %ugt = icmp ugt i32 %ax, %ay
  br i1 %ugt, label %then, label %exit

then:
  %cmp = icmp sgt i32 %ax, %ay
  ret i1 %cmp

exit:
  ret i1 false
}

define i1 @knownbits_same_sign_negative_ult_from_slt(i32 %x, i32 %y) {
; KB-LABEL: @knownbits_same_sign_negative_ult_from_slt(
; KB:       entry:
; KB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; KB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; KB-NEXT:    [[SLT:%.*]] = icmp slt i32 [[AX]], [[AY]]
; KB-NEXT:    br i1 [[SLT]], label %then, label %exit
; KB:       then:
; KB-NEXT:    ret i1 true
; KB:       exit:
; KB-NEXT:    ret i1 false
; NOKB-LABEL: @knownbits_same_sign_negative_ult_from_slt(
; NOKB:       entry:
; NOKB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; NOKB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; NOKB-NEXT:    [[SLT:%.*]] = icmp slt i32 [[AX]], [[AY]]
; NOKB-NEXT:    br i1 [[SLT]], label %then, label %exit
; NOKB:       then:
; NOKB-NEXT:    [[CMP:%.*]] = icmp ult i32 [[AX]], [[AY]]
; NOKB-NEXT:    ret i1 [[CMP]]
; NOKB:       exit:
; NOKB-NEXT:    ret i1 false
entry:
  %ax = or i32 %x, -2147483648
  %ay = or i32 %y, -2147483648
  %slt = icmp slt i32 %ax, %ay
  br i1 %slt, label %then, label %exit

then:
  %cmp = icmp ult i32 %ax, %ay
  ret i1 %cmp

exit:
  ret i1 false
}

define i1 @knownbits_same_sign_negative_ugt_from_sgt(i32 %x, i32 %y) {
; KB-LABEL: @knownbits_same_sign_negative_ugt_from_sgt(
; KB:       entry:
; KB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; KB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; KB-NEXT:    [[SGT:%.*]] = icmp sgt i32 [[AX]], [[AY]]
; KB-NEXT:    br i1 [[SGT]], label %then, label %exit
; KB:       then:
; KB-NEXT:    ret i1 true
; KB:       exit:
; KB-NEXT:    ret i1 false
; NOKB-LABEL: @knownbits_same_sign_negative_ugt_from_sgt(
; NOKB:       entry:
; NOKB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; NOKB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; NOKB-NEXT:    [[SGT:%.*]] = icmp sgt i32 [[AX]], [[AY]]
; NOKB-NEXT:    br i1 [[SGT]], label %then, label %exit
; NOKB:       then:
; NOKB-NEXT:    [[CMP:%.*]] = icmp ugt i32 [[AX]], [[AY]]
; NOKB-NEXT:    ret i1 [[CMP]]
; NOKB:       exit:
; NOKB-NEXT:    ret i1 false
entry:
  %ax = or i32 %x, -2147483648
  %ay = or i32 %y, -2147483648
  %sgt = icmp sgt i32 %ax, %ay
  br i1 %sgt, label %then, label %exit

then:
  %cmp = icmp ugt i32 %ax, %ay
  ret i1 %cmp

exit:
  ret i1 false
}

define i1 @knownbits_same_sign_negative_ule_from_sle(i32 %x, i32 %y) {
; KB-LABEL: @knownbits_same_sign_negative_ule_from_sle(
; KB:       entry:
; KB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; KB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; KB-NEXT:    [[SLE:%.*]] = icmp sle i32 [[AX]], [[AY]]
; KB-NEXT:    br i1 [[SLE]], label %then, label %exit
; KB:       then:
; KB-NEXT:    ret i1 true
; KB:       exit:
; KB-NEXT:    ret i1 false
; NOKB-LABEL: @knownbits_same_sign_negative_ule_from_sle(
; NOKB:       entry:
; NOKB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; NOKB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; NOKB-NEXT:    [[SLE:%.*]] = icmp sle i32 [[AX]], [[AY]]
; NOKB-NEXT:    br i1 [[SLE]], label %then, label %exit
; NOKB:       then:
; NOKB-NEXT:    [[CMP:%.*]] = icmp ule i32 [[AX]], [[AY]]
; NOKB-NEXT:    ret i1 [[CMP]]
; NOKB:       exit:
; NOKB-NEXT:    ret i1 false
entry:
  %ax = or i32 %x, -2147483648
  %ay = or i32 %y, -2147483648
  %sle = icmp sle i32 %ax, %ay
  br i1 %sle, label %then, label %exit

then:
  %cmp = icmp ule i32 %ax, %ay
  ret i1 %cmp

exit:
  ret i1 false
}

define i1 @knownbits_same_sign_negative_uge_from_sge(i32 %x, i32 %y) {
; KB-LABEL: @knownbits_same_sign_negative_uge_from_sge(
; KB:       entry:
; KB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; KB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; KB-NEXT:    [[SGE:%.*]] = icmp sge i32 [[AX]], [[AY]]
; KB-NEXT:    br i1 [[SGE]], label %then, label %exit
; KB:       then:
; KB-NEXT:    ret i1 true
; KB:       exit:
; KB-NEXT:    ret i1 false
; NOKB-LABEL: @knownbits_same_sign_negative_uge_from_sge(
; NOKB:       entry:
; NOKB-NEXT:    [[AX:%.*]] = or i32 %x, -2147483648
; NOKB-NEXT:    [[AY:%.*]] = or i32 %y, -2147483648
; NOKB-NEXT:    [[SGE:%.*]] = icmp sge i32 [[AX]], [[AY]]
; NOKB-NEXT:    br i1 [[SGE]], label %then, label %exit
; NOKB:       then:
; NOKB-NEXT:    [[CMP:%.*]] = icmp uge i32 [[AX]], [[AY]]
; NOKB-NEXT:    ret i1 [[CMP]]
; NOKB:       exit:
; NOKB-NEXT:    ret i1 false
entry:
  %ax = or i32 %x, -2147483648
  %ay = or i32 %y, -2147483648
  %sge = icmp sge i32 %ax, %ay
  br i1 %sge, label %then, label %exit

then:
  %cmp = icmp uge i32 %ax, %ay
  ret i1 %cmp

exit:
  ret i1 false
}
