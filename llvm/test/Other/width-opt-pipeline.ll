; RUN: opt -disable-output -disable-verify -print-pipeline-passes -passes='function(width-opt)' < %s | FileCheck %s --match-full-lines --check-prefix=EXPLICIT
; RUN: opt -disable-output -disable-verify -print-pipeline-passes -passes='default<O1>' < %s | FileCheck %s --check-prefix=DEFAULT

; EXPLICIT: function(width-opt)
; DEFAULT: width-opt,instcombine

define void @f() {
  ret void
}
