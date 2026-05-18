namespace Pattern084 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0 = ssa_0_arg0[0];
	APInt ssa_4_arg0 = ssa_0_arg0[1];
	APInt ssa_5_arg1 = ssa_1_arg1[0];
	APInt ssa_6_arg1 = ssa_1_arg1[1];
	APInt ssa_7_arg2 = ssa_2_arg2[0];
	APInt ssa_8_arg2 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_arg0.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_arg0.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_arg0.getBitWidth());
	APInt ssa_12_bw(ssa_3_arg0.getBitWidth(), ssa_3_arg0.getBitWidth());
	APInt ssa_13_overlap0 = ssa_3_arg0 & ssa_4_arg0;
	APInt ssa_14_overlap1 = ssa_5_arg1 & ssa_6_arg1;
	APInt ssa_15_overlap2 = ssa_7_arg2 & ssa_8_arg2;
	APInt ssa_16_overlap01 = ssa_13_overlap0 | ssa_14_overlap1;
	APInt ssa_17_overlap012 = ssa_16_overlap01 | ssa_15_overlap2;
	bool ssa_18_is_bottom_pre = ssa_17_overlap012.ne(ssa_9_const0);
	APInt ssa_19_shift_kz = ssa_5_arg1 | ssa_7_arg2;
	APInt ssa_20_shift_ko = ssa_6_arg1 & ssa_8_arg2;
	bool ssa_21_shift_ge_bw = ssa_20_shift_ko.uge(ssa_12_bw);
	bool ssa_22_is_bottom = ssa_18_is_bottom_pre || ssa_21_shift_ge_bw;
	bool ssa_23_shift_is_zero = ssa_19_shift_kz.eq(ssa_11_allones);
	bool ssa_24_arg0_is_zero = ssa_3_arg0.eq(ssa_11_allones);
	APInt ssa_25_tz(ssa_19_shift_kz.getBitWidth(), ssa_19_shift_kz.countr_one());
	APInt ssa_26_pow2;
	if (ssa_25_tz.uge(ssa_25_tz.getBitWidth())) {
		ssa_26_pow2 = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_26_pow2 = ssa_10_const1.shl(ssa_25_tz);
	}
	bool ssa_27_pow2_ge_bw = ssa_26_pow2.uge(ssa_12_bw);
	bool ssa_28_cond_zero0 = ssa_23_shift_is_zero || ssa_24_arg0_is_zero;
	bool ssa_29_cond_zero = ssa_28_cond_zero0 || ssa_27_pow2_ge_bw;
	APInt ssa_30_res0_base = ssa_29_cond_zero ? ssa_11_allones : ssa_9_const0;
	APInt ssa_31_res1_base = ssa_29_cond_zero ? ssa_9_const0 : ssa_9_const0;
	APInt ssa_32_arg0_known_mask = ssa_3_arg0 | ssa_4_arg0;
	bool ssa_33_arg0_is_const = ssa_32_arg0_known_mask.eq(ssa_11_allones);
	APInt ssa_34_shift_known_mask = ssa_19_shift_kz | ssa_20_shift_ko;
	bool ssa_35_shift_is_const = ssa_34_shift_known_mask.eq(ssa_11_allones);
	bool ssa_36_const_case = ssa_33_arg0_is_const && ssa_35_shift_is_const;
	APInt ssa_37_lshr_val;
	if (ssa_20_shift_ko.uge(ssa_20_shift_ko.getBitWidth())) {
		ssa_37_lshr_val = APInt(ssa_4_arg0.getBitWidth(), 0);
	} else {
		ssa_37_lshr_val = ssa_4_arg0.lshr(ssa_20_shift_ko);
	}
	APInt ssa_38_res_val = ssa_4_arg0 - ssa_37_lshr_val;
	APInt ssa_39_res_const_zero = ssa_11_allones ^ ssa_38_res_val;
	APInt ssa_40_res0_pre = ssa_36_const_case ? ssa_39_res_const_zero : ssa_30_res0_base;
	APInt ssa_41_res1_pre = ssa_36_const_case ? ssa_38_res_val : ssa_31_res1_base;
	APInt ssa_42_res0 = ssa_22_is_bottom ? ssa_11_allones : ssa_40_res0_pre;
	APInt ssa_43_res1 = ssa_22_is_bottom ? ssa_11_allones : ssa_41_res1_pre;
	std::array<APInt, 2> ssa_44_r = std::array<APInt, 2>{ssa_42_res0, ssa_43_res1};
	return ssa_44_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0 = ssa_0_arg0[0];
	APInt ssa_4_arg0 = ssa_0_arg0[1];
	APInt ssa_5_arg1 = ssa_1_arg1[0];
	APInt ssa_6_arg1 = ssa_1_arg1[1];
	APInt ssa_7_arg2 = ssa_2_arg2[0];
	APInt ssa_8_arg2 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_arg0.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_arg0.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_arg0.getBitWidth());
	APInt ssa_12_bw(ssa_3_arg0.getBitWidth(), ssa_3_arg0.getBitWidth());
	APInt ssa_13_bw_minus1 = ssa_12_bw - ssa_10_const1;
	APInt ssa_14_bw_minus1_lz(ssa_13_bw_minus1.getBitWidth(), ssa_13_bw_minus1.countl_zero());
	APInt ssa_15_shift_high_zero_mask = ssa_9_const0;
	if (ssa_14_bw_minus1_lz.ule(ssa_14_bw_minus1_lz.getBitWidth()))
		ssa_15_shift_high_zero_mask.setHighBits(ssa_14_bw_minus1_lz.getZExtValue());
	else
		ssa_15_shift_high_zero_mask.setHighBits(ssa_14_bw_minus1_lz.getBitWidth());
	APInt ssa_16_shift_kz = ssa_5_arg1 | ssa_7_arg2;
	APInt ssa_17_shift_ko = ssa_6_arg1 & ssa_8_arg2;
	APInt ssa_18_shift_kz2 = ssa_16_shift_kz | ssa_15_shift_high_zero_mask;
	bool ssa_19_shift_is_zero = ssa_18_shift_kz2.eq(ssa_11_allones);
	bool ssa_20_arg0_is_zero = ssa_3_arg0.eq(ssa_11_allones);
	bool ssa_21_cond_zero_base = ssa_19_shift_is_zero || ssa_20_arg0_is_zero;
	APInt ssa_22_sign_mask = ssa_9_const0;
	ssa_22_sign_mask.setSignBit();
	APInt ssa_23_arg0_sign_zero_mask = ssa_3_arg0 & ssa_22_sign_mask;
	APInt ssa_24_arg0_sign_one_mask = ssa_4_arg0 & ssa_22_sign_mask;
	bool ssa_25_arg0_sign_zero = ssa_23_arg0_sign_zero_mask.eq(ssa_22_sign_mask);
	bool ssa_26_arg0_sign_one = ssa_24_arg0_sign_one_mask.eq(ssa_22_sign_mask);
	bool ssa_27_shift_nonzero = ssa_17_shift_ko.ne(ssa_9_const0);
	bool ssa_28_sign_one_cond = ssa_26_arg0_sign_one && ssa_27_shift_nonzero;
	APInt ssa_29_lz(ssa_3_arg0.getBitWidth(), ssa_3_arg0.countl_one());
	APInt ssa_30_lz_mask = ssa_9_const0;
	if (ssa_29_lz.ule(ssa_29_lz.getBitWidth()))
		ssa_30_lz_mask.setHighBits(ssa_29_lz.getZExtValue());
	else
		ssa_30_lz_mask.setHighBits(ssa_29_lz.getBitWidth());
	APInt ssa_31_res0_lead = ssa_25_arg0_sign_zero ? ssa_30_lz_mask : ssa_9_const0;
	APInt ssa_32_res1_sign = ssa_28_sign_one_cond ? ssa_22_sign_mask : ssa_9_const0;
	APInt ssa_33_second_mask;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_33_second_mask = APInt(ssa_22_sign_mask.getBitWidth(), 0);
	} else {
		ssa_33_second_mask = ssa_22_sign_mask.lshr(ssa_10_const1);
	}
	APInt ssa_34_arg0_second_zero_mask = ssa_3_arg0 & ssa_33_second_mask;
	bool ssa_35_arg0_second_zero = ssa_34_arg0_second_zero_mask.eq(ssa_33_second_mask);
	bool ssa_36_arg0_top10 = ssa_26_arg0_sign_one && ssa_35_arg0_second_zero;
	APInt ssa_37_shift_mask_high1 = ssa_11_allones;
	if (ssa_10_const1.ule(ssa_10_const1.getBitWidth()))
		ssa_37_shift_mask_high1.clearLowBits(ssa_10_const1.getZExtValue());
	else
		ssa_37_shift_mask_high1.clearLowBits(ssa_10_const1.getBitWidth());
	APInt ssa_38_shift_high_known = ssa_18_shift_kz2 & ssa_37_shift_mask_high1;
	bool ssa_39_shift_only_bit0 = ssa_38_shift_high_known.eq(ssa_37_shift_mask_high1);
	bool ssa_40_cond_zero_top10 = ssa_36_arg0_top10 && ssa_39_shift_only_bit0;
	bool ssa_41_cond_zero = ssa_21_cond_zero_base || ssa_40_cond_zero_top10;
	APInt ssa_42_res0 = ssa_41_cond_zero ? ssa_11_allones : ssa_31_res0_lead;
	APInt ssa_43_res1 = ssa_41_cond_zero ? ssa_9_const0 : ssa_32_res1_sign;
	std::array<APInt, 2> ssa_44_r = std::array<APInt, 2>{ssa_42_res0, ssa_43_res1};
	return ssa_44_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0 = ssa_0_arg0[0];
	APInt ssa_4_arg0 = ssa_0_arg0[1];
	APInt ssa_5_arg1 = ssa_1_arg1[0];
	APInt ssa_6_arg1 = ssa_1_arg1[1];
	APInt ssa_7_arg2 = ssa_2_arg2[0];
	APInt ssa_8_arg2 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_arg0.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_arg0.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_arg0.getBitWidth());
	APInt ssa_12_bw(ssa_3_arg0.getBitWidth(), ssa_3_arg0.getBitWidth());
	APInt ssa_13_bw_minus1 = ssa_12_bw - ssa_10_const1;
	APInt ssa_14_bw_minus1_lz(ssa_13_bw_minus1.getBitWidth(), ssa_13_bw_minus1.countl_zero());
	APInt ssa_15_shift_high_zero_mask = ssa_9_const0;
	if (ssa_14_bw_minus1_lz.ule(ssa_14_bw_minus1_lz.getBitWidth()))
		ssa_15_shift_high_zero_mask.setHighBits(ssa_14_bw_minus1_lz.getZExtValue());
	else
		ssa_15_shift_high_zero_mask.setHighBits(ssa_14_bw_minus1_lz.getBitWidth());
	APInt ssa_16_shift_kz = ssa_5_arg1 | ssa_7_arg2;
	APInt ssa_17_shift_ko = ssa_6_arg1 & ssa_8_arg2;
	APInt ssa_18_shift_kz2 = ssa_16_shift_kz | ssa_15_shift_high_zero_mask;
	APInt ssa_19_shift_known_mask2 = ssa_18_shift_kz2 | ssa_17_shift_ko;
	bool ssa_20_shift_is_const2 = ssa_19_shift_known_mask2.eq(ssa_11_allones);
	bool ssa_21_shift_ko_zero = ssa_17_shift_ko.eq(ssa_9_const0);
	APInt ssa_22_mask = ssa_18_shift_kz2 ^ ssa_11_allones;
	bool ssa_23_mask_nonzero = ssa_22_mask.ne(ssa_9_const0);
	APInt ssa_24_mask_minus1 = ssa_22_mask - ssa_10_const1;
	APInt ssa_25_mask_and = ssa_22_mask & ssa_24_mask_minus1;
	bool ssa_26_mask_is_pow2 = ssa_25_mask_and.eq(ssa_9_const0);
	bool ssa_27_pow2 = ssa_26_mask_is_pow2 && ssa_23_mask_nonzero;
	APInt ssa_28_arg0_known = ssa_3_arg0 | ssa_4_arg0;
	bool ssa_29_arg0_const = ssa_28_arg0_known.eq(ssa_11_allones);
	bool ssa_30_const_case2 = ssa_29_arg0_const && ssa_20_shift_is_const2;
	bool ssa_31_cond0 = ssa_29_arg0_const && ssa_21_shift_ko_zero;
	bool ssa_32_cond_case = ssa_31_cond0 && ssa_27_pow2;
	bool ssa_33_shift_ge_bw = ssa_22_mask.uge(ssa_12_bw);
	APInt ssa_34_lshr_val;
	if (ssa_22_mask.uge(ssa_22_mask.getBitWidth())) {
		ssa_34_lshr_val = APInt(ssa_4_arg0.getBitWidth(), 0);
	} else {
		ssa_34_lshr_val = ssa_4_arg0.lshr(ssa_22_mask);
	}
	APInt ssa_35_res_val = ssa_4_arg0 - ssa_34_lshr_val;
	APInt ssa_36_sign_mask = ssa_9_const0;
	ssa_36_sign_mask.setSignBit();
	APInt ssa_37_arg0_sign = ssa_4_arg0 & ssa_36_sign_mask;
	bool ssa_38_arg0_sign_one = ssa_37_arg0_sign.eq(ssa_36_sign_mask);
	APInt ssa_39_res_sign = ssa_35_res_val & ssa_36_sign_mask;
	bool ssa_40_res_sign_zero = ssa_39_res_sign.eq(ssa_9_const0);
	bool ssa_41_overflow_zero = ssa_38_arg0_sign_one && ssa_40_res_sign_zero;
	bool ssa_42_cond_zero0 = ssa_33_shift_ge_bw || ssa_41_overflow_zero;
	APInt ssa_43_res0_val = ssa_11_allones ^ ssa_35_res_val;
	APInt ssa_44_res0_case = ssa_42_cond_zero0 ? ssa_11_allones : ssa_43_res0_val;
	APInt ssa_45_res0_pow2 = ssa_32_cond_case ? ssa_44_res0_case : ssa_9_const0;
	APInt ssa_46_lshr_const;
	if (ssa_17_shift_ko.uge(ssa_17_shift_ko.getBitWidth())) {
		ssa_46_lshr_const = APInt(ssa_4_arg0.getBitWidth(), 0);
	} else {
		ssa_46_lshr_const = ssa_4_arg0.lshr(ssa_17_shift_ko);
	}
	APInt ssa_47_res_const_val = ssa_4_arg0 - ssa_46_lshr_const;
	APInt ssa_48_res_const_zero = ssa_11_allones ^ ssa_47_res_const_val;
	APInt ssa_49_res0 = ssa_30_const_case2 ? ssa_48_res_const_zero : ssa_45_res0_pow2;
	APInt ssa_50_res1 = ssa_30_const_case2 ? ssa_47_res_const_val : ssa_9_const0;
	std::array<APInt, 2> ssa_51_r = std::array<APInt, 2>{ssa_49_res0, ssa_50_res1};
	return ssa_51_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0 = ssa_0_arg0[0];
	APInt ssa_4_arg0 = ssa_0_arg0[1];
	APInt ssa_5_arg1 = ssa_1_arg1[0];
	APInt ssa_6_arg1 = ssa_1_arg1[1];
	APInt ssa_7_arg2 = ssa_2_arg2[0];
	APInt ssa_8_arg2 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_arg0.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_arg0.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_arg0.getBitWidth());
	APInt ssa_12_bw(ssa_3_arg0.getBitWidth(), ssa_3_arg0.getBitWidth());
	APInt ssa_13_bw_minus1 = ssa_12_bw - ssa_10_const1;
	APInt ssa_14_shift_kz = ssa_5_arg1 | ssa_7_arg2;
	APInt ssa_15_shift_ko = ssa_6_arg1 & ssa_8_arg2;
	APInt ssa_16_bw_minus1_lz(ssa_13_bw_minus1.getBitWidth(), ssa_13_bw_minus1.countl_zero());
	APInt ssa_17_shift_high_zero_mask = ssa_9_const0;
	if (ssa_16_bw_minus1_lz.ule(ssa_16_bw_minus1_lz.getBitWidth()))
		ssa_17_shift_high_zero_mask.setHighBits(ssa_16_bw_minus1_lz.getZExtValue());
	else
		ssa_17_shift_high_zero_mask.setHighBits(ssa_16_bw_minus1_lz.getBitWidth());
	APInt ssa_18_shift_kz2 = ssa_14_shift_kz | ssa_17_shift_high_zero_mask;
	bool ssa_19_arg0_allones_mask = ssa_4_arg0.eq(ssa_11_allones);
	bool ssa_20_arg0_zero_mask = ssa_3_arg0.eq(ssa_9_const0);
	bool ssa_21_arg0_is_allones = ssa_19_arg0_allones_mask && ssa_20_arg0_zero_mask;
	APInt ssa_22_sign_mask = ssa_9_const0;
	ssa_22_sign_mask.setSignBit();
	bool ssa_23_arg0_min_ones = ssa_4_arg0.eq(ssa_22_sign_mask);
	APInt ssa_24_not_sign_mask = ssa_11_allones ^ ssa_22_sign_mask;
	bool ssa_25_arg0_min_zeros = ssa_3_arg0.eq(ssa_24_not_sign_mask);
	bool ssa_26_arg0_is_min = ssa_23_arg0_min_ones && ssa_25_arg0_min_zeros;
	APInt ssa_27_not_kz2 = ssa_11_allones ^ ssa_18_shift_kz2;
	APInt ssa_28_shift_max = ssa_15_shift_ko | ssa_27_not_kz2;
	bool ssa_29_shift_ge_bw = ssa_28_shift_max.uge(ssa_12_bw);
	APInt ssa_30_max_s = ssa_29_shift_ge_bw ? ssa_13_bw_minus1 : ssa_28_shift_max;
	APInt ssa_31_tz = ssa_12_bw - ssa_30_max_s;
	APInt ssa_32_low_mask = ssa_9_const0;
	if (ssa_31_tz.ule(ssa_31_tz.getBitWidth()))
		ssa_32_low_mask.setLowBits(ssa_31_tz.getZExtValue());
	else
		ssa_32_low_mask.setLowBits(ssa_31_tz.getBitWidth());
	APInt ssa_33_high_mask = ssa_9_const0;
	if (ssa_15_shift_ko.ule(ssa_15_shift_ko.getBitWidth()))
		ssa_33_high_mask.setHighBits(ssa_15_shift_ko.getZExtValue());
	else
		ssa_33_high_mask.setHighBits(ssa_15_shift_ko.getBitWidth());
	APInt ssa_34_res0_allones = ssa_21_arg0_is_allones ? ssa_32_low_mask : ssa_9_const0;
	APInt ssa_35_res1_allones = ssa_21_arg0_is_allones ? ssa_33_high_mask : ssa_9_const0;
	APInt ssa_36_res0_final = ssa_26_arg0_is_min ? ssa_11_allones : ssa_34_res0_allones;
	APInt ssa_37_res1_final = ssa_26_arg0_is_min ? ssa_9_const0 : ssa_35_res1_allones;
	std::array<APInt, 2> ssa_38_r = std::array<APInt, 2>{ssa_36_res0_final, ssa_37_res1_final};
	return ssa_38_r;
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
