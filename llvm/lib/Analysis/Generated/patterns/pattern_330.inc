namespace Pattern330 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_12_a0_full_mask = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_13_a1_full_mask = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_14_a2_full_mask = ssa_7_a2z | ssa_8_a2o;
	bool ssa_15_a0_full = ssa_12_a0_full_mask.eq(ssa_9_allones);
	bool ssa_16_a1_full = ssa_13_a1_full_mask.eq(ssa_9_allones);
	bool ssa_17_a2_full = ssa_14_a2_full_mask.eq(ssa_9_allones);
	bool ssa_18_full_num = ssa_16_a1_full && ssa_17_a2_full;
	bool ssa_19_a12_eq = ssa_6_a1o.eq(ssa_8_a2o);
	bool ssa_20_cond_zero = ssa_18_full_num && ssa_19_a12_eq;
	bool ssa_21_a0_is_one = ssa_4_a0o.eq(ssa_11_one);
	bool ssa_22_a1_is_zero = ssa_6_a1o.eq(ssa_10_zero);
	bool ssa_23_case1 = ssa_15_a0_full && ssa_16_a1_full;
	bool ssa_24_case1 = ssa_21_a0_is_one && ssa_22_a1_is_zero;
	bool ssa_25_cond_case1 = ssa_23_case1 && ssa_24_case1;
	bool ssa_26_true = -1;
	bool ssa_27_not_cond_zero = ssa_20_cond_zero != ssa_26_true;
	bool ssa_28_cond_case1_only = ssa_25_cond_case1 && ssa_27_not_cond_zero;
	APInt ssa_29_sel0_zero = ssa_20_cond_zero ? ssa_9_allones : ssa_10_zero;
	APInt ssa_30_sel1_zero = ssa_20_cond_zero ? ssa_10_zero : ssa_10_zero;
	APInt ssa_31_sel0_case1 = ssa_28_cond_case1_only ? ssa_7_a2z : ssa_29_sel0_zero;
	APInt ssa_32_sel1_case1 = ssa_28_cond_case1_only ? ssa_8_a2o : ssa_30_sel1_zero;
	std::array<APInt, 2> ssa_33_r = std::array<APInt, 2>{ssa_31_sel0_case1, ssa_32_sel1_case1};
	return ssa_33_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_a0_full_mask = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_12_a1_full_mask = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_13_a2_full_mask = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_14_sign_mask = ssa_10_zero;
	ssa_14_sign_mask.setSignBit();
	APInt ssa_15_a0_sign_zero_mask = ssa_3_a0z & ssa_14_sign_mask;
	APInt ssa_16_a1_sign_zero_mask = ssa_5_a1z & ssa_14_sign_mask;
	APInt ssa_17_a2_sign_zero_mask = ssa_7_a2z & ssa_14_sign_mask;
	bool ssa_18_a0_sign_zero = ssa_15_a0_sign_zero_mask.eq(ssa_14_sign_mask);
	bool ssa_19_a1_sign_zero = ssa_16_a1_sign_zero_mask.eq(ssa_14_sign_mask);
	bool ssa_20_a2_sign_zero = ssa_17_a2_sign_zero_mask.eq(ssa_14_sign_mask);
	APInt ssa_21_a0_sign_one_mask = ssa_4_a0o & ssa_14_sign_mask;
	bool ssa_22_a0_sign_one = ssa_21_a0_sign_one_mask.eq(ssa_14_sign_mask);
	bool ssa_23_a0_full = ssa_11_a0_full_mask.eq(ssa_9_allones);
	APInt ssa_24_a1_unknown = ssa_12_a1_full_mask ^ ssa_9_allones;
	APInt ssa_25_a2_unknown = ssa_13_a2_full_mask ^ ssa_9_allones;
	APInt ssa_26_a1_max = ssa_6_a1o | ssa_24_a1_unknown;
	APInt ssa_27_a2_max = ssa_8_a2o | ssa_25_a2_unknown;
	APInt ssa_28_max = APIntOps::umax(ssa_26_a1_max, ssa_27_a2_max);
	bool ssa_29_a0_gt_max = ssa_4_a0o.ugt(ssa_28_max);
	APInt ssa_30_a0_abs = ~ssa_4_a0o;
	bool ssa_31_a0_abs_gt_max = ssa_30_a0_abs.ugt(ssa_28_max);
	bool ssa_32_zero_pos = ssa_18_a0_sign_zero && ssa_19_a1_sign_zero;
	bool ssa_33_zero_pos = ssa_32_zero_pos && ssa_20_a2_sign_zero;
	bool ssa_34_zero_pos = ssa_33_zero_pos && ssa_29_a0_gt_max;
	bool ssa_35_zero_neg = ssa_23_a0_full && ssa_22_a0_sign_one;
	bool ssa_36_zero_neg = ssa_35_zero_neg && ssa_19_a1_sign_zero;
	bool ssa_37_zero_neg = ssa_36_zero_neg && ssa_20_a2_sign_zero;
	bool ssa_38_zero_neg = ssa_37_zero_neg && ssa_31_a0_abs_gt_max;
	bool ssa_39_zero_cond = ssa_34_zero_pos || ssa_38_zero_neg;
	APInt ssa_40_sel_z = ssa_39_zero_cond ? ssa_9_allones : ssa_10_zero;
	std::array<APInt, 2> ssa_41_r = std::array<APInt, 2>{ssa_40_sel_z, ssa_10_zero};
	return ssa_41_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_12_eight(ssa_3_a0z.getBitWidth(), 8);
	APInt ssa_13_a0_full_mask = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_14_a1_full_mask = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_15_a2_full_mask = ssa_7_a2z | ssa_8_a2o;
	bool ssa_16_a0_full = ssa_13_a0_full_mask.eq(ssa_9_allones);
	bool ssa_17_a1_full = ssa_14_a1_full_mask.eq(ssa_9_allones);
	bool ssa_18_a2_full = ssa_15_a2_full_mask.eq(ssa_9_allones);
	bool ssa_19_a01_full = ssa_16_a0_full && ssa_17_a1_full;
	bool ssa_20_a012_full = ssa_19_a01_full && ssa_18_a2_full;
	bool ssa_21_a0_small = ssa_4_a0o.ule(ssa_12_eight);
	bool ssa_22_cond = ssa_20_a012_full && ssa_21_a0_small;
	APInt ssa_23_a0_val = ssa_22_cond ? ssa_4_a0o : ssa_11_one;
	APInt ssa_24_a1_val = ssa_22_cond ? ssa_6_a1o : ssa_10_zero;
	APInt ssa_25_a2_val = ssa_22_cond ? ssa_8_a2o : ssa_10_zero;
	APInt ssa_26_sub = ssa_25_a2_val - ssa_24_a1_val;
	APInt ssa_27_div;
	if (ssa_26_sub.isMinSignedValue() && ssa_23_a0_val == -1) {
		ssa_27_div = APInt::getSignedMinValue(ssa_26_sub.getBitWidth());
	} else if (ssa_23_a0_val == 0 && ssa_26_sub.isNonNegative()) {
		ssa_27_div = APInt(ssa_26_sub.getBitWidth(), -1);
	} else if (ssa_23_a0_val == 0 && ssa_26_sub.isNegative()) {
		ssa_27_div = APInt(ssa_26_sub.getBitWidth(), 1);
	} else {
		ssa_27_div = ssa_26_sub.sdiv(ssa_23_a0_val);
	}
	APInt ssa_28_div_z = ssa_27_div ^ ssa_9_allones;
	APInt ssa_29_sel_z = ssa_22_cond ? ssa_28_div_z : ssa_10_zero;
	APInt ssa_30_sel_o = ssa_22_cond ? ssa_27_div : ssa_10_zero;
	std::array<APInt, 2> ssa_31_r = std::array<APInt, 2>{ssa_29_sel_z, ssa_30_sel_o};
	return ssa_31_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_two(ssa_3_a0z.getBitWidth(), 2);
	APInt ssa_12_three(ssa_3_a0z.getBitWidth(), 3);
	APInt ssa_13_four(ssa_3_a0z.getBitWidth(), 4);
	APInt ssa_14_seven(ssa_3_a0z.getBitWidth(), 7);
	APInt ssa_15_fifteen(ssa_3_a0z.getBitWidth(), 15);
	APInt ssa_16_a1_full_mask = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_17_a2_full_mask = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_18_sign_mask = ssa_10_zero;
	ssa_18_sign_mask.setSignBit();
	APInt ssa_19_a1_sign_zero_mask = ssa_5_a1z & ssa_18_sign_mask;
	APInt ssa_20_a2_sign_zero_mask = ssa_7_a2z & ssa_18_sign_mask;
	bool ssa_21_a1_sign_zero = ssa_19_a1_sign_zero_mask.eq(ssa_18_sign_mask);
	bool ssa_22_a2_sign_zero = ssa_20_a2_sign_zero_mask.eq(ssa_18_sign_mask);
	APInt ssa_23_a1_unknown = ssa_16_a1_full_mask ^ ssa_9_allones;
	APInt ssa_24_a2_unknown = ssa_17_a2_full_mask ^ ssa_9_allones;
	APInt ssa_25_a1_max = ssa_6_a1o | ssa_23_a1_unknown;
	APInt ssa_26_a2_max = ssa_8_a2o | ssa_24_a2_unknown;
	APInt ssa_27_max = APIntOps::umax(ssa_25_a1_max, ssa_26_a2_max);
	APInt ssa_28_low2_mask = ssa_10_zero;
	if (ssa_11_two.ule(ssa_11_two.getBitWidth()))
		ssa_28_low2_mask.setLowBits(ssa_11_two.getZExtValue());
	else
		ssa_28_low2_mask.setLowBits(ssa_11_two.getBitWidth());
	APInt ssa_29_low3_mask = ssa_10_zero;
	if (ssa_12_three.ule(ssa_12_three.getBitWidth()))
		ssa_29_low3_mask.setLowBits(ssa_12_three.getZExtValue());
	else
		ssa_29_low3_mask.setLowBits(ssa_12_three.getBitWidth());
	APInt ssa_30_low4_mask = ssa_10_zero;
	if (ssa_13_four.ule(ssa_13_four.getBitWidth()))
		ssa_30_low4_mask.setLowBits(ssa_13_four.getZExtValue());
	else
		ssa_30_low4_mask.setLowBits(ssa_13_four.getBitWidth());
	APInt ssa_31_low2_known = ssa_3_a0z & ssa_28_low2_mask;
	APInt ssa_32_low3_known = ssa_3_a0z & ssa_29_low3_mask;
	APInt ssa_33_low4_known = ssa_3_a0z & ssa_30_low4_mask;
	bool ssa_34_low2_zero = ssa_31_low2_known.eq(ssa_28_low2_mask);
	bool ssa_35_low3_zero = ssa_32_low3_known.eq(ssa_29_low3_mask);
	bool ssa_36_low4_zero = ssa_33_low4_known.eq(ssa_30_low4_mask);
	bool ssa_37_max_le = ssa_27_max.ule(ssa_12_three);
	bool ssa_38_max_le = ssa_27_max.ule(ssa_14_seven);
	bool ssa_39_max_le = ssa_27_max.ule(ssa_15_fifteen);
	bool ssa_40_zero_cond2 = ssa_34_low2_zero && ssa_37_max_le;
	bool ssa_41_zero_cond3 = ssa_35_low3_zero && ssa_38_max_le;
	bool ssa_42_zero_cond4 = ssa_36_low4_zero && ssa_39_max_le;
	bool ssa_43_zero_cond23 = ssa_40_zero_cond2 || ssa_41_zero_cond3;
	bool ssa_44_zero_cond0 = ssa_43_zero_cond23 || ssa_42_zero_cond4;
	bool ssa_45_zero_sign = ssa_21_a1_sign_zero && ssa_22_a2_sign_zero;
	bool ssa_46_zero_cond = ssa_45_zero_sign && ssa_44_zero_cond0;
	APInt ssa_47_sel_z = ssa_46_zero_cond ? ssa_9_allones : ssa_10_zero;
	APInt ssa_48_sel_o = ssa_46_zero_cond ? ssa_10_zero : ssa_10_zero;
	std::array<APInt, 2> ssa_49_r = std::array<APInt, 2>{ssa_47_sel_z, ssa_48_sel_o};
	return ssa_49_r;
}
std::array<APInt, 2> meet(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1) {
	APInt ssa_2_arg00 = ssa_0_arg0[0];
	APInt ssa_3_arg01 = ssa_0_arg0[1];
	APInt ssa_4_arg10 = ssa_1_arg1[0];
	APInt ssa_5_arg11 = ssa_1_arg1[1];
	APInt ssa_6_and0 = ssa_2_arg00 | ssa_4_arg10;
	APInt ssa_7_and1 = ssa_3_arg01 | ssa_5_arg11;
	std::array<APInt, 2> ssa_8_result = std::array<APInt, 2>{ssa_6_and0, ssa_7_and1};
	return ssa_8_result;
}
std::array<APInt, 2> solution(std::array<APInt, 2> ssa_0_, std::array<APInt, 2> ssa_1_, std::array<APInt, 2> ssa_2_) {
	std::array<APInt, 2> ssa_3_ = partial_solution_0(ssa_0_, ssa_1_, ssa_2_);
	std::array<APInt, 2> ssa_4_ = partial_solution_1(ssa_0_, ssa_1_, ssa_2_);
	std::array<APInt, 2> ssa_5_ = partial_solution_2(ssa_0_, ssa_1_, ssa_2_);
	std::array<APInt, 2> ssa_6_ = partial_solution_3(ssa_0_, ssa_1_, ssa_2_);
	std::array<APInt, 2> ssa_7_ = meet(ssa_3_, ssa_4_);
	std::array<APInt, 2> ssa_8_ = meet(ssa_7_, ssa_5_);
	std::array<APInt, 2> ssa_9_ = meet(ssa_8_, ssa_6_);
	return ssa_9_;
}
}
