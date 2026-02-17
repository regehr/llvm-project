; RUN: rm -f %t.kblog
; RUN: opt -O2 -known-bits-log-file=%t.kblog -disable-output %s 2>/dev/null
; RUN: grep -F 'llvm.ctpop.i16 00000000000????? ????????????????' %t.kblog
; RUN: not grep -F 'llvm.ctpop.i16 ???????????????? ????????????????' %t.kblog

; This regression test checks that we do not log ctpop invocations that return
; all-unknown known-bits solely due to early recursion-depth bailout.

; ModuleID = 'popcount.ll'
source_filename = "popcount.ll"
target triple = "unknown-unknown-unknown"

; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define [2 x i16] @partial_solution_0_body([2 x i16] %.1) local_unnamed_addr #0 {
entry:
  %idx_0 = extractvalue [2 x i16] %.1, 0
  %idx_0.2 = icmp eq i16 %idx_0, 0
  %idx_0_sh = select i1 %idx_0.2, i16 -32768, i16 -16
  %idx_0.7 = insertvalue [2 x i16] zeroinitializer, i16 %idx_0_sh, 0
  %idx_0.8 = insertvalue [2 x i16] %idx_0.7, i16 0, 1
  ret [2 x i16] %idx_0.8
}

; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define [2 x i16] @partial_solution_1_body([2 x i16] %.1) local_unnamed_addr #0 {
entry:
  ret [2 x i16] [i16 -32, i16 0]
}

; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define [2 x i16] @partial_solution_2_body([2 x i16] %.1) local_unnamed_addr #0 {
entry:
  %idx_0 = extractvalue [2 x i16] %.1, 0
  %.3 = tail call range(i16 0, 17) i16 @llvm.ctpop.i16(i16 %idx_0)
  %idx_0_ge = icmp eq i16 %idx_0, -1
  %0 = lshr i16 -1, %.3
  %1 = tail call i16 @llvm.smax.i16(i16 %0, i16 -1)
  %2 = xor i16 %1, -1
  %3 = or i16 %2, -32768
  %idx_0.6 = select i1 %idx_0_ge, i16 -1, i16 %3
  %idx_0.7 = insertvalue [2 x i16] zeroinitializer, i16 %idx_0.6, 0
  %idx_0.8 = insertvalue [2 x i16] %idx_0.7, i16 0, 1
  ret [2 x i16] %idx_0.8
}

; Function Attrs: mustprogress nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.ctpop.i16(i16) #1

; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define [2 x i16] @partial_solution_3_body([2 x i16] %.1) local_unnamed_addr #0 {
entry:
  %idx_0 = extractvalue [2 x i16] %.1, 0
  %idx_0.1 = extractvalue [2 x i16] %.1, 1
  %.3 = add i16 %idx_0.1, %idx_0
  %.4 = tail call range(i16 0, 17) i16 @llvm.ctpop.i16(i16 %.3)
  %idx_0_ge.1 = icmp eq i16 %.3, -1
  %0 = lshr i16 -1, %.4
  %1 = xor i16 %0, -1
  %idx_0_cmp = icmp ne i16 %idx_0.1, 0
  %idx_0.10 = zext i1 %idx_0_cmp to i16
  %2 = tail call i16 @llvm.umax.i16(i16 %1, i16 %idx_0.10)
  %idx_0.11 = select i1 %idx_0_ge.1, i16 -1, i16 %2
  %idx_0.12 = insertvalue [2 x i16] zeroinitializer, i16 %idx_0.11, 0
  %idx_0.13 = insertvalue [2 x i16] %idx_0.12, i16 0, 1
  ret [2 x i16] %idx_0.13
}

; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define i1 @partial_solution_3_cond([2 x i16] %.1) local_unnamed_addr #0 {
entry:
  %idx_0 = extractvalue [2 x i16] %.1, 1
  %idx_0.2 = icmp eq i16 %idx_0, 1
  ret i1 %idx_0.2
}

; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define [2 x i16] @partial_solution_0([2 x i16] %.1) local_unnamed_addr #0 {
entry:
  %idx_0.i = extractvalue [2 x i16] %.1, 0
  %idx_0.2.i = icmp eq i16 %idx_0.i, 0
  %idx_0_sh.i = select i1 %idx_0.2.i, i16 -32768, i16 -16
  %idx_0.7.i = insertvalue [2 x i16] zeroinitializer, i16 %idx_0_sh.i, 0
  %idx_0.8.i = insertvalue [2 x i16] %idx_0.7.i, i16 0, 1
  ret [2 x i16] %idx_0.8.i
}

; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define [2 x i16] @partial_solution_1([2 x i16] %.1) local_unnamed_addr #0 {
entry:
  ret [2 x i16] [i16 -32, i16 0]
}

; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define [2 x i16] @partial_solution_2([2 x i16] %.1) local_unnamed_addr #0 {
entry:
  %idx_0.i = extractvalue [2 x i16] %.1, 0
  %.3.i = tail call range(i16 0, 17) i16 @llvm.ctpop.i16(i16 %idx_0.i)
  %idx_0_ge.i = icmp eq i16 %idx_0.i, -1
  %0 = lshr i16 -1, %.3.i
  %1 = tail call i16 @llvm.smax.i16(i16 %0, i16 -1)
  %2 = xor i16 %1, -1
  %3 = or i16 %2, -32768
  %idx_0.6.i = select i1 %idx_0_ge.i, i16 -1, i16 %3
  %idx_0.7.i = insertvalue [2 x i16] zeroinitializer, i16 %idx_0.6.i, 0
  %idx_0.8.i = insertvalue [2 x i16] %idx_0.7.i, i16 0, 1
  ret [2 x i16] %idx_0.8.i
}

; Function Attrs: alwaysinline nofree norecurse nosync nounwind memory(none)
define [2 x i16] @partial_solution_3([2 x i16] %.1) local_unnamed_addr #2 {
entry:
  %idx_0 = tail call [2 x i16] @getTop([2 x i16] %.1) #4
  %idx_0.1 = extractvalue [2 x i16] %idx_0, 0
  %idx_0.2 = extractvalue [2 x i16] %idx_0, 1
  %idx_0.i = extractvalue [2 x i16] %.1, 0
  %idx_0.1.i = extractvalue [2 x i16] %.1, 1
  %.3.i = add i16 %idx_0.1.i, %idx_0.i
  %.4.i = tail call range(i16 0, 17) i16 @llvm.ctpop.i16(i16 %.3.i)
  %idx_0_ge.1.i = icmp eq i16 %.3.i, -1
  %0 = lshr i16 -1, %.4.i
  %1 = xor i16 %0, -1
  %idx_0_cmp.i = icmp ne i16 %idx_0.1.i, 0
  %idx_0.10.i = zext i1 %idx_0_cmp.i to i16
  %2 = tail call i16 @llvm.umax.i16(i16 %1, i16 %idx_0.10.i)
  %idx_0.11.i = select i1 %idx_0_ge.1.i, i16 -1, i16 %2
  %idx_0.2.i = icmp eq i16 %idx_0.1.i, 1
  %.3 = select i1 %idx_0.2.i, i16 %idx_0.11.i, i16 %idx_0.1
  %.4 = select i1 %idx_0.2.i, i16 0, i16 %idx_0.2
  %idx_0.7 = insertvalue [2 x i16] zeroinitializer, i16 %.3, 0
  %idx_0.8 = insertvalue [2 x i16] %idx_0.7, i16 %.4, 1
  ret [2 x i16] %idx_0.8
}

declare [2 x i16] @getTop([2 x i16]) local_unnamed_addr

; Function Attrs: alwaysinline nofree norecurse nosync nounwind memory(none)
define [2 x i16] @solution([2 x i16] %.1) local_unnamed_addr #2 {
entry:
  %idx_0.i.i = extractvalue [2 x i16] %.1, 0
  %idx_0.2.i.i = icmp eq i16 %idx_0.i.i, 0
  %idx_0_sh.i.i = select i1 %idx_0.2.i.i, i16 -32768, i16 -16
  %idx_0.7.i.i = insertvalue [2 x i16] zeroinitializer, i16 %idx_0_sh.i.i, 0
  %idx_0.8.i.i = insertvalue [2 x i16] %idx_0.7.i.i, i16 0, 1
  %.3.i.i = tail call range(i16 0, 17) i16 @llvm.ctpop.i16(i16 %idx_0.i.i)
  %idx_0_ge.i.i = icmp eq i16 %idx_0.i.i, -1
  %0 = lshr i16 -1, %.3.i.i
  %1 = tail call i16 @llvm.smax.i16(i16 %0, i16 -1)
  %2 = xor i16 %1, -1
  %3 = or i16 %2, -32768
  %idx_0.6.i.i = select i1 %idx_0_ge.i.i, i16 -1, i16 %3
  %idx_0.7.i.i2 = insertvalue [2 x i16] zeroinitializer, i16 %idx_0.6.i.i, 0
  %idx_0.8.i.i3 = insertvalue [2 x i16] %idx_0.7.i.i2, i16 0, 1
  %idx_0.i = tail call [2 x i16] @getTop([2 x i16] %.1) #4
  %idx_0.1.i = extractvalue [2 x i16] %idx_0.i, 0
  %idx_0.2.i = extractvalue [2 x i16] %idx_0.i, 1
  %idx_0.1.i.i = extractvalue [2 x i16] %.1, 1
  %.3.i.i5 = add i16 %idx_0.1.i.i, %idx_0.i.i
  %.4.i.i = tail call range(i16 0, 17) i16 @llvm.ctpop.i16(i16 %.3.i.i5)
  %idx_0_ge.1.i.i = icmp eq i16 %.3.i.i5, -1
  %4 = lshr i16 -1, %.4.i.i
  %5 = xor i16 %4, -1
  %idx_0_cmp.i.i = icmp ne i16 %idx_0.1.i.i, 0
  %idx_0.10.i.i = zext i1 %idx_0_cmp.i.i to i16
  %6 = tail call i16 @llvm.umax.i16(i16 %5, i16 %idx_0.10.i.i)
  %idx_0.11.i.i = select i1 %idx_0_ge.1.i.i, i16 -1, i16 %6
  %idx_0.2.i.i6 = icmp eq i16 %idx_0.1.i.i, 1
  %.3.i = select i1 %idx_0.2.i.i6, i16 %idx_0.11.i.i, i16 %idx_0.1.i
  %.4.i = select i1 %idx_0.2.i.i6, i16 0, i16 %idx_0.2.i
  %idx_0.7.i = insertvalue [2 x i16] zeroinitializer, i16 %.3.i, 0
  %idx_0.8.i = insertvalue [2 x i16] %idx_0.7.i, i16 %.4.i, 1
  %idx_0.4 = tail call [2 x i16] @meet([2 x i16] %idx_0.8.i.i, [2 x i16] [i16 -32, i16 0]) #4
  %idx_0.5 = tail call [2 x i16] @meet([2 x i16] %idx_0.4, [2 x i16] %idx_0.8.i.i3) #4
  %idx_0.6 = tail call [2 x i16] @meet([2 x i16] %idx_0.5, [2 x i16] %idx_0.8.i) #4
  ret [2 x i16] %idx_0.6
}

declare [2 x i16] @meet([2 x i16], [2 x i16]) local_unnamed_addr

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.smax.i16(i16, i16) #3

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.umax.i16(i16, i16) #3

attributes #0 = { alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
attributes #1 = { mustprogress nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { alwaysinline nofree norecurse nosync nounwind memory(none) }
attributes #3 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #4 = { nounwind }
