
define i32 @sext_switch_basic(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  switch i32 %sx, label %default [
    i32 42,  label %case42
    i32 -10, label %caseneg
    i32 200, label %case200
  ]
case42:
  ret i32 42
caseneg:
  ret i32 -10
case200:
  ret i32 200
default:
  ret i32 0
}


define i32 @sext_switch_nochange_extra_use(i8 %x, ptr %p) {
entry:
  %sx = sext i8 %x to i32
  store i32 %sx, ptr %p
  switch i32 %sx, label %default [
    i32 42, label %case42
  ]
case42:
  ret i32 42
default:
  ret i32 0
}

