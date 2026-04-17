
define i8 @trunc_lshr_consts() {
  %r = lshr i32 256, 1
  %t = trunc i32 %r to i8
  ret i8 %t
}

define i8 @trunc_ashr_consts() {
  %r = ashr i32 -256, 1
  %t = trunc i32 %r to i8
  ret i8 %t
}

define i8 @trunc_shl_consts() {
  %r = shl i32 1, 4
  %t = trunc i32 %r to i8
  ret i8 %t
}

define i8 @trunc_and_consts() {
  %r = and i32 511, 255
  %t = trunc i32 %r to i8
  ret i8 %t
}

define i8 @trunc_or_consts() {
  %r = or i32 256, 15
  %t = trunc i32 %r to i8
  ret i8 %t
}

define i8 @trunc_mul_consts() {
  %r = mul i32 300, 3
  %t = trunc i32 %r to i8
  ret i8 %t
}
