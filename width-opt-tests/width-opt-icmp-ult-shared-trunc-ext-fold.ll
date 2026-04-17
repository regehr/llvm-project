
define i1 @shared_trunc_ext_folded(i8 %n) {
entry:
  %wide = zext i8 %n to i64
  %tr = trunc i64 %wide to i8
  %div = sdiv i8 -1, %tr
  %rem = srem i8 -1, %div
  %cmp = icmp ult i8 %tr, %rem
  ret i1 %cmp
}


define <4 x i1> @shared_trunc_ext_folded_vec(<4 x i8> %n) {
entry:
  %wide = zext <4 x i8> %n to <4 x i64>
  %tr = trunc <4 x i64> %wide to <4 x i8>
  %div = sdiv <4 x i8> splat (i8 -1), %tr
  %rem = srem <4 x i8> splat (i8 -1), %div
  %cmp = icmp ult <4 x i8> %tr, %rem
  ret <4 x i1> %cmp
}

