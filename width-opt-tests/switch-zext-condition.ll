
define i32 @zext_switch_basic(i8 %x) {
entry:
  %zx = zext i8 %x to i32
  switch i32 %zx, label %default [
    i32 42,  label %case42
    i32 10,  label %case10
    i32 300, label %case300
  ]
case42:
  ret i32 42
case10:
  ret i32 10
case300:
  ret i32 300
default:
  ret i32 0
}


define i32 @zext_switch_nochange_extra_use(i8 %x, ptr %p) {
entry:
  %zx = zext i8 %x to i32
  store i32 %zx, ptr %p
  switch i32 %zx, label %default [
    i32 42, label %case42
  ]
case42:
  ret i32 42
default:
  ret i32 0
}

