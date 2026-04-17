
define i32 @and_sext_mask_no_nneg(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r  = and i32 %sx, 255
  ret i32 %r
}

define <4 x i32> @and_sext_mask_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> %sx, splat (i32 255)
  ret <4 x i32> %r
}

define i32 @and_sext_partial_mask_no_nneg(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r  = and i32 %sx, 173
  ret i32 %r
}

define <4 x i32> @and_sext_partial_mask_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> %sx, splat (i32 173)
  ret <4 x i32> %r
}

define i32 @and_sext_reversed_no_nneg(i8 %x) {
entry:
  %sx = sext i8 %x to i32
  %r  = and i32 255, %sx
  ret i32 %r
}

define <4 x i32> @and_sext_reversed_no_nneg_vec(<4 x i8> %x) {
entry:
  %sx = sext <4 x i8> %x to <4 x i32>
  %r  = and <4 x i32> splat (i32 255), %sx
  ret <4 x i32> %r
}
