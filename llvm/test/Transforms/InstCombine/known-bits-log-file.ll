; RUN: rm -f %t.kblog
; RUN: opt -passes=instcombine -known-bits-log-file=%t.kblog -disable-output %s
; RUN: test -s %t.kblog
; RUN: awk 'END { exit !(NR > 1) }' %t.kblog
; RUN: not grep -Ev '^[^[:space:]]+ [01?]+( (NA|[01?]+))*$' %t.kblog
; RUN: not grep -E '^(icmp|extractvalue)\b' %t.kblog

define i1 @known_bits_log(i8 %x, i8 %y) {
entry:
  %y.mask = and i8 %y, 63
  %range = shl i8 %y.mask, 1
  %cmp0 = icmp sge i8 %x, 0
  %cmp1 = icmp slt i8 %x, %range
  %both = and i1 %cmp0, %cmp1
  ret i1 %both
}
