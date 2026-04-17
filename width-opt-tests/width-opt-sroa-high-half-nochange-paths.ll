


define i8 @sroa_high_half_no_and(i16 %x) {
  %shr = lshr i16 %x, 8
  %t = trunc i16 %shr to i8
  ret i8 %t
}


define i8 @sroa_high_half_bad_low_mask(i8 %hi, i8 %lo) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -255
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}


define i8 @sroa_high_half_v_not_or(i16 %x) {
  %masked = and i16 %x, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}


define i8 @sroa_high_half_no_shl_in_or(i16 %x, i16 %y) {
  %or_val = or i16 %x, %y
  %masked = and i16 %or_val, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}


define i8 @sroa_shl_multi_use(i8 %hi, i8 %lo, ptr %p) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 8
  store i16 %hi_shifted, ptr %p
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}


define i8 @sroa_shl_wrong_amount(i8 %hi, i8 %lo) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 7
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}


define i8 @sroa_ze_wrong_src_width(i4 %hi, i8 %lo) {
  %lo_wide = zext i8 %lo to i16
  %hi_wide = zext i4 %hi to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}


define i8 @sroa_ze_multi_use(i8 %hi, i8 %lo, ptr %p) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  store i16 %hi_wide, ptr %p
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}


define i8 @sroa_low_part_not_bounded(i8 %hi, i16 %lo) {
  %hi_wide = zext i8 %hi to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo, %hi_shifted
  %masked = and i16 %combined, -256
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}
