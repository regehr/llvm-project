

define i1 @icmp_ult_umin_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %m = call i32 @llvm.umin.i32(i32 %a32, i32 %b32)
  %cmp = icmp ult i32 %m, 200
  ret i1 %cmp
}
declare i32 @llvm.umin.i32(i32, i32)


define i1 @icmp_ult_umax_zexts(i8 %a, i8 %b) {
  %a32 = zext i8 %a to i32
  %b32 = zext i8 %b to i32
  %m = call i32 @llvm.umax.i32(i32 %a32, i32 %b32)
  %cmp = icmp ult i32 %m, 200
  ret i1 %cmp
}
declare i32 @llvm.umax.i32(i32, i32)


define <4 x i1> @icmp_ult_umin_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %m = call <4 x i32> @llvm.umin.v4i32(<4 x i32> %a32, <4 x i32> %b32)
  %cmp = icmp ult <4 x i32> %m, <i32 200, i32 200, i32 200, i32 200>
  ret <4 x i1> %cmp
}
declare <4 x i32> @llvm.umin.v4i32(<4 x i32>, <4 x i32>)


define <4 x i1> @icmp_ult_umax_zexts_vec(<4 x i8> %a, <4 x i8> %b) {
  %a32 = zext <4 x i8> %a to <4 x i32>
  %b32 = zext <4 x i8> %b to <4 x i32>
  %m = call <4 x i32> @llvm.umax.v4i32(<4 x i32> %a32, <4 x i32> %b32)
  %cmp = icmp ult <4 x i32> %m, <i32 200, i32 200, i32 200, i32 200>
  ret <4 x i1> %cmp
}
declare <4 x i32> @llvm.umax.v4i32(<4 x i32>, <4 x i32>)

