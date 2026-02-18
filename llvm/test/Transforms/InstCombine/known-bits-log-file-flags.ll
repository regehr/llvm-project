; RUN: rm -f %t.kblog
; RUN: opt -passes=instcombine -known-bits-log-file=%t.kblog -disable-output %s
; RUN: grep -F 'add.nsw.nuw ' %t.kblog
; RUN: grep -F 'lshr.exact ' %t.kblog
; RUN: grep -F 'udiv.exact ' %t.kblog

define i32 @known_bits_log_flags(i32 %x, i32 %y) {
entry:
  %a = udiv exact i32 %x, %y
  %b = lshr exact i32 %a, 1
  %c = add nsw nuw i32 %b, 5
  %d = shl nsw nuw i32 %c, 1
  %e = ashr exact i32 %d, 1
  %f = and i32 %e, 15
  ret i32 %f
}
