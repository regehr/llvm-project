; RUN: opt -passes='print<width-components>' -disable-output %s 2>&1 | FileCheck %s

define i16 @f(i8 %x, i8 %y) {
entry:
  %x16 = zext i8 %x to i16
  %y16 = zext i8 %y to i16
  %cmp = icmp ult i16 %x16, %y16
  %sel = select i1 %cmp, i16 %y16, i16 %x16
  ret i16 %sel
}

; CHECK: Width components for function 'f':
; CHECK-DAG: component {{[0-9]+}}: width=i8, fixed=true
; CHECK-DAG: component {{[0-9]+}}: width=i8, fixed=true
; CHECK-DAG: component {{[0-9]+}}: width=i16, fixed=false
; CHECK-DAG: component {{[0-9]+}}: width=i1, fixed=true
; CHECK-DAG: %cmp [conditional]
; CHECK-DAG: %sel [poly]
