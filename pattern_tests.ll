; RUN: opt -debug -debug-only=value-tracking -passes=instcombine -disable-output %s

define i1 @p003(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %t0 = add i32 %arg1, %arg2
  %root = add i32 %arg0, %t0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p004(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %t0 = sub i32 %arg2, %arg1
  %root = ashr exact i32 %t0, %arg0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p008(i32 %arg0, i32 %arg1, i32 %arg2, i32 %arg3) {
entry:
  %sub = sub i32 %arg3, %arg2
  %ashr = ashr exact i32 %sub, %arg1
  %root = add nsw i32 %arg0, %ashr
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p012(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %t0 = sub i32 %arg2, %arg1
  %root = sdiv exact i32 %t0, %arg0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p013(i32 %arg0, i32 %arg1) {
entry:
  %t0 = add i32 %arg0, %arg1
  %root = and i32 %arg1, %t0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p014(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %add = add i32 %arg0, %arg1
  %and0 = and i32 %arg0, %add
  %and1 = and i32 %arg1, %arg2
  %root = or disjoint i32 %and1, %and0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p018(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %t0 = add i32 %arg1, %arg2
  %t1 = and i32 %arg2, %t0
  %root = or disjoint i32 %arg0, %t1
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p019(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %t0 = add nsw i32 %arg1, %arg2
  %root = add nsw i32 %arg0, %t0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p020(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %sub = sub i32 %arg2, %arg1
  %root = add i32 %arg0, %sub
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p022(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %sh = shl i32 %arg2, %arg1
  %root = add i32 %arg0, %sh
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p025(i32 %arg0, i32 %arg1, i32 %arg2, i32 %arg3) {
entry:
  %t0 = add i32 %arg2, %arg3
  %t1 = add i32 %arg1, %t0
  %root = add i32 %arg0, %t1
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p026(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %mul = mul i32 %arg1, %arg2
  %root = add i32 %arg0, %mul
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p027(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %add = add i32 %arg1, %arg2
  %root = lshr i32 %add, %arg0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p032(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %add = add nsw nuw i32 %arg1, %arg2
  %root = lshr i32 %add, %arg0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p034(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %inner = add nsw nuw i32 %arg1, %arg2
  %root = add nsw nuw i32 %arg0, %inner
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p037(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %t0 = ashr exact i32 %arg2, %arg0
  %root = add nsw i32 %arg1, %t0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p038(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %t0 = ashr exact i32 %arg2, %arg1
  %root = add nsw i32 %arg0, %t0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p045(i32 %arg0, i32 %arg1) {
entry:
  %add = add i32 %arg0, %arg1
  %root = and i32 %arg0, %add
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p046(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %mul = mul nsw nuw i32 %arg1, %arg2
  %root = add nsw nuw i32 %arg0, %mul
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p047(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %t0 = add i32 %arg1, %arg2
  %root = sub i32 %t0, %arg0
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}

define i1 @p050(i32 %arg0, i32 %arg1, i32 %arg2) {
entry:
  %sh = lshr i32 %arg2, %arg1
  %root = add nsw nuw i32 %arg0, %sh
  %cmp = icmp sge i32 %root, 0
  ret i1 %cmp
}
