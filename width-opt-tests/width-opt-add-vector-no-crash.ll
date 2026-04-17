
define <2 x i32> @add_nuw_zext_add_vec(<2 x i16> %x) {
entry:
  %add = add nuw <2 x i16> %x, <i16 -42, i16 5>
  %ext = zext <2 x i16> %add to <2 x i32>
  %r = add <2 x i32> %ext, <i32 356, i32 -12>
  ret <2 x i32> %r
}

