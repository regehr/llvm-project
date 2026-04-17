


define i8 @high_half_partial_mask(i8 %hi, i8 %lo) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  ; 3840 = 0x0F00: zeros in bits 0-7, ones only in bits 8-11, zeros in bits 12-15
  %masked = and i16 %combined, 3840
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}



define i8 @high_half_full_mask(i8 %hi, i8 %lo) {
  %hi_wide = zext i8 %hi to i16
  %lo_wide = zext i8 %lo to i16
  %hi_shifted = shl i16 %hi_wide, 8
  %combined = or i16 %lo_wide, %hi_shifted
  ; 65280 = 0xFF00: zeros in bits 0-7, all ones in bits 8-15
  %masked = and i16 %combined, 65280
  %shifted = lshr i16 %masked, 8
  %result = trunc i16 %shifted to i8
  ret i8 %result
}

