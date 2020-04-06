; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -instsimplify < %s | FileCheck %s

declare void @bar(i8* %a, i8* nonnull %b)

; 'y' must be nonnull.

define i1 @caller1(i8* %x, i8* %y) {
; CHECK-LABEL: @caller1(
; CHECK-NEXT:    call void @bar(i8* [[X:%.*]], i8* [[Y:%.*]])
; CHECK-NEXT:    ret i1 false
;
  call void @bar(i8* %x, i8* %y)
  %null_check = icmp eq i8* %y, null
  ret i1 %null_check
}

; Don't know anything about 'y'.

define i1 @caller2(i8* %x, i8* %y) {
; CHECK-LABEL: @caller2(
; CHECK-NEXT:    call void @bar(i8* [[Y:%.*]], i8* [[X:%.*]])
; CHECK-NEXT:    [[NULL_CHECK:%.*]] = icmp eq i8* [[Y]], null
; CHECK-NEXT:    ret i1 [[NULL_CHECK]]
;
  call void @bar(i8* %y, i8* %x)
  %null_check = icmp eq i8* %y, null
  ret i1 %null_check
}

; 'y' must be nonnull.

define i1 @caller3(i8* %x, i8* %y) {
; CHECK-LABEL: @caller3(
; CHECK-NEXT:    call void @bar(i8* [[X:%.*]], i8* [[Y:%.*]])
; CHECK-NEXT:    ret i1 true
;
  call void @bar(i8* %x, i8* %y)
  %null_check = icmp ne i8* %y, null
  ret i1 %null_check
}

; FIXME: The call is guaranteed to execute, so 'y' must be nonnull throughout.

define i1 @caller4(i8* %x, i8* %y) {
; CHECK-LABEL: @caller4(
; CHECK-NEXT:    [[NULL_CHECK:%.*]] = icmp ne i8* [[Y:%.*]], null
; CHECK-NEXT:    call void @bar(i8* [[X:%.*]], i8* [[Y]])
; CHECK-NEXT:    ret i1 [[NULL_CHECK]]
;
  %null_check = icmp ne i8* %y, null
  call void @bar(i8* %x, i8* %y)
  ret i1 %null_check
}

; The call to bar() does not dominate the null check, so no change.

define i1 @caller5(i8* %x, i8* %y) {
; CHECK-LABEL: @caller5(
; CHECK-NEXT:    [[NULL_CHECK:%.*]] = icmp eq i8* [[Y:%.*]], null
; CHECK-NEXT:    br i1 [[NULL_CHECK]], label [[T:%.*]], label [[F:%.*]]
; CHECK:       t:
; CHECK-NEXT:    ret i1 [[NULL_CHECK]]
; CHECK:       f:
; CHECK-NEXT:    call void @bar(i8* [[X:%.*]], i8* [[Y]])
; CHECK-NEXT:    ret i1 [[NULL_CHECK]]
;
  %null_check = icmp eq i8* %y, null
  br i1 %null_check, label %t, label %f
t:
  ret i1 %null_check
f:
  call void @bar(i8* %x, i8* %y)
  ret i1 %null_check
}

; Make sure that an invoke works similarly to a call.

declare i32 @esfp(...)

define i1 @caller6(i8* %x, i8* %y) personality i8* bitcast (i32 (...)* @esfp to i8*){
; CHECK-LABEL: @caller6(
; CHECK-NEXT:    invoke void @bar(i8* [[X:%.*]], i8* nonnull [[Y:%.*]])
; CHECK-NEXT:    to label [[CONT:%.*]] unwind label [[EXC:%.*]]
; CHECK:       cont:
; CHECK-NEXT:    ret i1 false
; CHECK:       exc:
; CHECK-NEXT:    [[LP:%.*]] = landingpad { i8*, i32 }
; CHECK-NEXT:    filter [0 x i8*] zeroinitializer
; CHECK-NEXT:    unreachable
;
  invoke void @bar(i8* %x, i8* nonnull %y)
  to label %cont unwind label %exc

cont:
  %null_check = icmp eq i8* %y, null
  ret i1 %null_check

exc:
  %lp = landingpad { i8*, i32 }
  filter [0 x i8*] zeroinitializer
  unreachable
}

declare i8* @returningPtr(i8* returned %p)

define i1 @nonnullReturnTest(i8* nonnull %x) {
; CHECK-LABEL: @nonnullReturnTest(
; CHECK-NEXT:    [[X2:%.*]] = call i8* @returningPtr(i8* [[X:%.*]])
; CHECK-NEXT:    ret i1 false
;
  %x2 = call i8* @returningPtr(i8* %x)
  %null_check = icmp eq i8* %x2, null
  ret i1 %null_check
}

define i1 @unknownReturnTest(i8* %x) {
; CHECK-LABEL: @unknownReturnTest(
; CHECK-NEXT:    [[X2:%.*]] = call i8* @returningPtr(i8* [[X:%.*]])
; CHECK-NEXT:    [[NULL_CHECK:%.*]] = icmp eq i8* [[X2]], null
; CHECK-NEXT:    ret i1 [[NULL_CHECK]]
;
  %x2 = call i8* @returningPtr(i8* %x)
  %null_check = icmp eq i8* %x2, null
  ret i1 %null_check
}

; Make sure that if load/store happened, the pointer is nonnull.

define i32 @test_null_after_store(i32* %0) {
; CHECK-LABEL: @test_null_after_store(
; CHECK-NEXT:    store i32 123, i32* [[TMP0:%.*]], align 4
; CHECK-NEXT:    ret i32 2
;
  store i32 123, i32* %0, align 4
  %2 = icmp eq i32* %0, null
  %3 = select i1 %2, i32 1, i32 2
  ret i32 %3
}

define i32 @test_null_after_load(i32* %0) {
; CHECK-LABEL: @test_null_after_load(
; CHECK-NEXT:    ret i32 1
;
  %2 = load i32, i32* %0, align 4
  %3 = icmp eq i32* %0, null
  %4 = select i1 %3, i32 %2, i32 1
  ret i32 %4
}

; Make sure that different address space does not affect null pointer check.

define i32 @test_null_after_store_addrspace(i32 addrspace(1)* %0) {
; CHECK-LABEL: @test_null_after_store_addrspace(
; CHECK-NEXT:    store i32 123, i32 addrspace(1)* [[TMP0:%.*]], align 4
; CHECK-NEXT:    [[TMP2:%.*]] = icmp eq i32 addrspace(1)* [[TMP0]], null
; CHECK-NEXT:    [[TMP3:%.*]] = select i1 [[TMP2]], i32 1, i32 2
; CHECK-NEXT:    ret i32 [[TMP3]]
;
  store i32 123, i32 addrspace(1)* %0, align 4
  %2 = icmp eq i32 addrspace(1)* %0, null
  %3 = select i1 %2, i32 1, i32 2
  ret i32 %3
}

define i32 @test_null_after_load_addrspace(i32 addrspace(1)* %0) {
; CHECK-LABEL: @test_null_after_load_addrspace(
; CHECK-NEXT:    [[TMP2:%.*]] = load i32, i32 addrspace(1)* [[TMP0:%.*]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = icmp eq i32 addrspace(1)* [[TMP0]], null
; CHECK-NEXT:    [[TMP4:%.*]] = select i1 [[TMP3]], i32 [[TMP2]], i32 1
; CHECK-NEXT:    ret i32 [[TMP4]]
;
  %2 = load i32, i32 addrspace(1)* %0, align 4
  %3 = icmp eq i32 addrspace(1)* %0, null
  %4 = select i1 %3, i32 %2, i32 1
  ret i32 %4
}

; Make sure if store happened after the check, nullptr check is not removed.

declare i8* @func(i64)

define i8* @test_load_store_after_check(i8* %0) {
; CHECK-LABEL: @test_load_store_after_check(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[TMP1:%.*]] = call i8* @func(i64 0)
; CHECK-NEXT:    [[NULL_CHECK:%.*]] = icmp eq i8* [[TMP1]], null
; CHECK-NEXT:    br i1 [[NULL_CHECK]], label [[RETURN:%.*]], label [[IF_END:%.*]]
; CHECK:       if.end:
; CHECK-NEXT:    store i8 7, i8* [[TMP1]]
; CHECK-NEXT:    br label [[RETURN]]
; CHECK:       return:
; CHECK-NEXT:    [[RETVAL_0:%.*]] = phi i8* [ [[TMP1]], [[IF_END]] ], [ null, [[ENTRY:%.*]] ]
; CHECK-NEXT:    ret i8* [[RETVAL_0]]
;
entry:
  %1 = call i8* @func(i64 0)
  %null_check = icmp eq i8* %1, null
  br i1 %null_check, label %return, label %if.end

if.end:
  store i8 7, i8* %1
  br label %return

return:
  %retval.0 = phi i8* [ %1, %if.end ], [ null, %entry ]
  ret i8* %retval.0
}
