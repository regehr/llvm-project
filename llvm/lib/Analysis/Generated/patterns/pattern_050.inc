namespace Pattern050 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0 = ssa_0_arg0[0];
	APInt ssa_4_arg0 = ssa_0_arg0[1];
	APInt ssa_5_arg1 = ssa_1_arg1[0];
	APInt ssa_6_arg1 = ssa_1_arg1[1];
	APInt ssa_7_arg2 = ssa_2_arg2[0];
	APInt ssa_8_arg2 = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_arg0.getBitWidth());
	APInt ssa_10_zero(ssa_3_arg0.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_arg0.getBitWidth(), 1);
	bool ssa_12_arg0_0_all = ssa_3_arg0.eq(ssa_9_allones);
	bool ssa_13_arg0_1_zero = ssa_4_arg0.eq(ssa_10_zero);
	bool ssa_14_arg0_is_zero = ssa_12_arg0_0_all && ssa_13_arg0_1_zero;
	bool ssa_15_arg1_0_all = ssa_5_arg1.eq(ssa_9_allones);
	bool ssa_16_arg1_1_zero = ssa_6_arg1.eq(ssa_10_zero);
	bool ssa_17_arg1_is_zero = ssa_15_arg1_0_all && ssa_16_arg1_1_zero;
	bool ssa_18_arg2_0_all = ssa_7_arg2.eq(ssa_9_allones);
	bool ssa_19_arg2_1_zero = ssa_8_arg2.eq(ssa_10_zero);
	bool ssa_20_arg2_is_zero = ssa_18_arg2_0_all && ssa_19_arg2_1_zero;
	bool ssa_21_arg0_arg1_zero = ssa_14_arg0_is_zero && ssa_17_arg1_is_zero;
	APInt ssa_22_k0(ssa_3_arg0.getBitWidth(), ssa_3_arg0.countl_one());
	APInt ssa_23_k2(ssa_7_arg2.getBitWidth(), ssa_7_arg2.countl_one());
	APInt ssa_24_sum_k2_shift = ssa_23_k2 + ssa_6_arg1;
	APInt ssa_25_bw(ssa_3_arg0.getBitWidth(), ssa_3_arg0.getBitWidth());
	APInt ssa_26_k2_shift = APIntOps::umin(ssa_24_sum_k2_shift, ssa_25_bw);
	APInt ssa_27_kmin = APIntOps::umin(ssa_22_k0, ssa_26_k2_shift);
	bool ssa_28_kmin_is_zero = ssa_27_kmin.eq(ssa_10_zero);
	APInt ssa_29_kmin_minus1 = ssa_27_kmin - ssa_11_one;
	APInt ssa_30_kmin_nonzero = ssa_28_kmin_is_zero ? ssa_10_zero : ssa_29_kmin_minus1;
	APInt ssa_31_kz_general = ssa_10_zero;
	if (ssa_30_kmin_nonzero.ule(ssa_30_kmin_nonzero.getBitWidth()))
		ssa_31_kz_general.setHighBits(ssa_30_kmin_nonzero.getZExtValue());
	else
		ssa_31_kz_general.setHighBits(ssa_30_kmin_nonzero.getBitWidth());
	APInt ssa_32_k1(ssa_4_arg0.getBitWidth(), ssa_4_arg0.countl_one());
	APInt ssa_33_ko_general = ssa_10_zero;
	if (ssa_32_k1.ule(ssa_32_k1.getBitWidth()))
		ssa_33_ko_general.setHighBits(ssa_32_k1.getZExtValue());
	else
		ssa_33_ko_general.setHighBits(ssa_32_k1.getBitWidth());
	APInt ssa_34_width = ssa_25_bw - ssa_26_k2_shift;
	APInt ssa_35_mask_low = ssa_10_zero;
	if (ssa_34_width.ule(ssa_34_width.getBitWidth()))
		ssa_35_mask_low.setLowBits(ssa_34_width.getZExtValue());
	else
		ssa_35_mask_low.setLowBits(ssa_34_width.getBitWidth());
	APInt ssa_36_mask_high = ssa_9_allones ^ ssa_35_mask_low;
	APInt ssa_37_arg0_low = ssa_3_arg0 & ssa_35_mask_low;
	bool ssa_38_arg0_low_all = ssa_37_arg0_low.eq(ssa_35_mask_low);
	APInt ssa_39_arg0_high_kz = ssa_3_arg0 & ssa_36_mask_high;
	APInt ssa_40_arg0_high_ko = ssa_4_arg0 & ssa_36_mask_high;
	APInt ssa_41_kz_aug = ssa_31_kz_general | ssa_39_arg0_high_kz;
	APInt ssa_42_ko_aug = ssa_33_ko_general | ssa_40_arg0_high_ko;
	APInt ssa_43_kz_gen2 = ssa_38_arg0_low_all ? ssa_41_kz_aug : ssa_31_kz_general;
	APInt ssa_44_ko_gen2 = ssa_38_arg0_low_all ? ssa_42_ko_aug : ssa_33_ko_general;
	APInt ssa_45_sign_mask = ssa_10_zero;
	if (ssa_11_one.ule(ssa_11_one.getBitWidth()))
		ssa_45_sign_mask.setHighBits(ssa_11_one.getZExtValue());
	else
		ssa_45_sign_mask.setHighBits(ssa_11_one.getBitWidth());
	APInt ssa_46_arg0_sign = ssa_3_arg0 & ssa_45_sign_mask;
	bool ssa_47_arg0_sign_zero = ssa_46_arg0_sign.eq(ssa_45_sign_mask);
	APInt ssa_48_arg2_sign = ssa_7_arg2 & ssa_45_sign_mask;
	bool ssa_49_arg2_sign_zero = ssa_48_arg2_sign.eq(ssa_45_sign_mask);
	bool ssa_50_shift_nonzero = ssa_6_arg1.ne(ssa_10_zero);
	bool ssa_51_lshr_sign_zero = ssa_50_shift_nonzero || ssa_49_arg2_sign_zero;
	bool ssa_52_both_nonneg = ssa_47_arg0_sign_zero && ssa_51_lshr_sign_zero;
	APInt ssa_53_kz_sign = ssa_52_both_nonneg ? ssa_45_sign_mask : ssa_10_zero;
	APInt ssa_54_kz_gen3 = ssa_43_kz_gen2 | ssa_53_kz_sign;
	APInt ssa_55_kz_sel1 = ssa_21_arg0_arg1_zero ? ssa_7_arg2 : ssa_54_kz_gen3;
	APInt ssa_56_ko_sel1 = ssa_21_arg0_arg1_zero ? ssa_8_arg2 : ssa_44_ko_gen2;
	APInt ssa_57_kz_sel2 = ssa_20_arg2_is_zero ? ssa_3_arg0 : ssa_55_kz_sel1;
	APInt ssa_58_ko_sel2 = ssa_20_arg2_is_zero ? ssa_4_arg0 : ssa_56_ko_sel1;
	std::array<APInt, 2> ssa_59_r = std::array<APInt, 2>{ssa_57_kz_sel2, ssa_58_ko_sel2};
	return ssa_59_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0 = ssa_0_arg0[0];
	APInt ssa_4_arg0 = ssa_0_arg0[1];
	APInt ssa_5_arg1 = ssa_1_arg1[0];
	APInt ssa_6_arg1 = ssa_1_arg1[1];
	APInt ssa_7_arg2 = ssa_2_arg2[0];
	APInt ssa_8_arg2 = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_arg0.getBitWidth());
	APInt ssa_10_zero(ssa_3_arg0.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_arg0.getBitWidth(), 1);
	APInt ssa_12_arg0_or = ssa_3_arg0 | ssa_4_arg0;
	bool ssa_13_arg0_exact = ssa_12_arg0_or.eq(ssa_9_allones);
	APInt ssa_14_arg1_or = ssa_5_arg1 | ssa_6_arg1;
	bool ssa_15_arg1_exact = ssa_14_arg1_or.eq(ssa_9_allones);
	bool ssa_16_shift_nonzero = ssa_6_arg1.ne(ssa_10_zero);
	APInt ssa_17_sign_mask = ssa_10_zero;
	if (ssa_11_one.ule(ssa_11_one.getBitWidth()))
		ssa_17_sign_mask.setHighBits(ssa_11_one.getZExtValue());
	else
		ssa_17_sign_mask.setHighBits(ssa_11_one.getBitWidth());
	APInt ssa_18_arg0_sign_kz = ssa_3_arg0 & ssa_17_sign_mask;
	bool ssa_19_arg0_sign_zero = ssa_18_arg0_sign_kz.eq(ssa_17_sign_mask);
	bool ssa_20_cond0 = ssa_13_arg0_exact && ssa_15_arg1_exact;
	bool ssa_21_cond1 = ssa_20_cond0 && ssa_16_shift_nonzero;
	bool ssa_22_cond = ssa_21_cond1 && ssa_19_arg0_sign_zero;
	APInt ssa_23_lshr_kz0;
	if (ssa_6_arg1.uge(ssa_6_arg1.getBitWidth())) {
		ssa_23_lshr_kz0 = APInt(ssa_7_arg2.getBitWidth(), 0);
	} else {
		ssa_23_lshr_kz0 = ssa_7_arg2.lshr(ssa_6_arg1);
	}
	APInt ssa_24_highmask = ssa_10_zero;
	if (ssa_6_arg1.ule(ssa_6_arg1.getBitWidth()))
		ssa_24_highmask.setHighBits(ssa_6_arg1.getZExtValue());
	else
		ssa_24_highmask.setHighBits(ssa_6_arg1.getBitWidth());
	APInt ssa_25_lshr_kz = ssa_23_lshr_kz0 | ssa_24_highmask;
	APInt ssa_26_lshr_ko;
	if (ssa_6_arg1.uge(ssa_6_arg1.getBitWidth())) {
		ssa_26_lshr_ko = APInt(ssa_8_arg2.getBitWidth(), 0);
	} else {
		ssa_26_lshr_ko = ssa_8_arg2.lshr(ssa_6_arg1);
	}
	APInt ssa_27_maxb = ssa_9_allones ^ ssa_25_lshr_kz;
	APInt ssa_28_minb = ssa_26_lshr_ko | ssa_10_zero;
	APInt ssa_29_minsum = ssa_4_arg0 + ssa_28_minb;
	APInt ssa_30_maxsum = ssa_4_arg0 + ssa_27_maxb;
	APInt ssa_31_xor_sum = ssa_29_minsum ^ ssa_30_maxsum;
	APInt ssa_32_prefix_len(ssa_31_xor_sum.getBitWidth(), ssa_31_xor_sum.countl_zero());
	APInt ssa_33_prefix_mask = ssa_10_zero;
	if (ssa_32_prefix_len.ule(ssa_32_prefix_len.getBitWidth()))
		ssa_33_prefix_mask.setHighBits(ssa_32_prefix_len.getZExtValue());
	else
		ssa_33_prefix_mask.setHighBits(ssa_32_prefix_len.getBitWidth());
	APInt ssa_34_not_minsum = ssa_9_allones ^ ssa_29_minsum;
	APInt ssa_35_kz_pref = ssa_34_not_minsum & ssa_33_prefix_mask;
	APInt ssa_36_ko_pref = ssa_29_minsum & ssa_33_prefix_mask;
	APInt ssa_37_kz_sel = ssa_22_cond ? ssa_35_kz_pref : ssa_10_zero;
	APInt ssa_38_ko_sel = ssa_22_cond ? ssa_36_ko_pref : ssa_10_zero;
	std::array<APInt, 2> ssa_39_r = std::array<APInt, 2>{ssa_37_kz_sel, ssa_38_ko_sel};
	return ssa_39_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0 = ssa_0_arg0[0];
	APInt ssa_4_arg0 = ssa_0_arg0[1];
	APInt ssa_5_arg1 = ssa_1_arg1[0];
	APInt ssa_6_arg1 = ssa_1_arg1[1];
	APInt ssa_7_arg2 = ssa_2_arg2[0];
	APInt ssa_8_arg2 = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_arg0.getBitWidth());
	APInt ssa_10_zero(ssa_3_arg0.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_arg0.getBitWidth(), 1);
	APInt ssa_12_bw(ssa_3_arg0.getBitWidth(), ssa_3_arg0.getBitWidth());
	APInt ssa_13_arg0_or = ssa_3_arg0 | ssa_4_arg0;
	APInt ssa_14_arg1_or = ssa_5_arg1 | ssa_6_arg1;
	APInt ssa_15_arg2_or = ssa_7_arg2 | ssa_8_arg2;
	bool ssa_16_arg0_exact = ssa_13_arg0_or.eq(ssa_9_allones);
	bool ssa_17_arg1_exact = ssa_14_arg1_or.eq(ssa_9_allones);
	bool ssa_18_arg2_exact = ssa_15_arg2_or.eq(ssa_9_allones);
	bool ssa_19_cond0 = ssa_16_arg0_exact && ssa_17_arg1_exact;
	bool ssa_20_cond_exact = ssa_19_cond0 && ssa_18_arg2_exact;
	bool ssa_21_arg1_ge_bw = ssa_6_arg1.uge(ssa_12_bw);
	bool ssa_22_bad_shift = ssa_17_arg1_exact && ssa_21_arg1_ge_bw;
	APInt ssa_23_sign_mask = ssa_10_zero;
	if (ssa_11_one.ule(ssa_11_one.getBitWidth()))
		ssa_23_sign_mask.setHighBits(ssa_11_one.getZExtValue());
	else
		ssa_23_sign_mask.setHighBits(ssa_11_one.getBitWidth());
	APInt ssa_24_arg0_msb_kz = ssa_3_arg0 & ssa_23_sign_mask;
	bool ssa_25_arg0_msb_zero = ssa_24_arg0_msb_kz.eq(ssa_23_sign_mask);
	APInt ssa_26_lshr_kz;
	if (ssa_6_arg1.uge(ssa_6_arg1.getBitWidth())) {
		ssa_26_lshr_kz = APInt(ssa_7_arg2.getBitWidth(), 0);
	} else {
		ssa_26_lshr_kz = ssa_7_arg2.lshr(ssa_6_arg1);
	}
	APInt ssa_27_lshr_msb_kz = ssa_26_lshr_kz & ssa_23_sign_mask;
	bool ssa_28_lshr_msb_zero = ssa_27_lshr_msb_kz.eq(ssa_23_sign_mask);
	bool ssa_29_cond1 = ssa_17_arg1_exact && ssa_25_arg0_msb_zero;
	bool ssa_30_cond_safe = ssa_29_cond1 && ssa_28_lshr_msb_zero;
	APInt ssa_31_min0 = ssa_4_arg0 | ssa_10_zero;
	APInt ssa_32_max0 = ssa_9_allones ^ ssa_3_arg0;
	APInt ssa_33_min2 = ssa_8_arg2 | ssa_10_zero;
	APInt ssa_34_max2 = ssa_9_allones ^ ssa_7_arg2;
	APInt ssa_35_minb;
	if (ssa_6_arg1.uge(ssa_6_arg1.getBitWidth())) {
		ssa_35_minb = APInt(ssa_33_min2.getBitWidth(), 0);
	} else {
		ssa_35_minb = ssa_33_min2.lshr(ssa_6_arg1);
	}
	APInt ssa_36_maxb;
	if (ssa_6_arg1.uge(ssa_6_arg1.getBitWidth())) {
		ssa_36_maxb = APInt(ssa_34_max2.getBitWidth(), 0);
	} else {
		ssa_36_maxb = ssa_34_max2.lshr(ssa_6_arg1);
	}
	APInt ssa_37_minsum = ssa_31_min0 + ssa_35_minb;
	APInt ssa_38_maxsum = ssa_32_max0 + ssa_36_maxb;
	APInt ssa_39_xor_sum = ssa_37_minsum ^ ssa_38_maxsum;
	APInt ssa_40_prefix_len(ssa_39_xor_sum.getBitWidth(), ssa_39_xor_sum.countl_zero());
	APInt ssa_41_prefix_mask = ssa_10_zero;
	if (ssa_40_prefix_len.ule(ssa_40_prefix_len.getBitWidth()))
		ssa_41_prefix_mask.setHighBits(ssa_40_prefix_len.getZExtValue());
	else
		ssa_41_prefix_mask.setHighBits(ssa_40_prefix_len.getBitWidth());
	APInt ssa_42_not_minsum = ssa_9_allones ^ ssa_37_minsum;
	APInt ssa_43_kz_pref = ssa_42_not_minsum & ssa_41_prefix_mask;
	APInt ssa_44_ko_pref = ssa_37_minsum & ssa_41_prefix_mask;
	APInt ssa_45_kz_tmp = ssa_30_cond_safe ? ssa_43_kz_pref : ssa_10_zero;
	APInt ssa_46_ko_tmp = ssa_30_cond_safe ? ssa_44_ko_pref : ssa_10_zero;
	APInt ssa_47_shift;
	if (ssa_6_arg1.uge(ssa_6_arg1.getBitWidth())) {
		ssa_47_shift = APInt(ssa_8_arg2.getBitWidth(), 0);
	} else {
		ssa_47_shift = ssa_8_arg2.lshr(ssa_6_arg1);
	}
	APInt ssa_48_sum = ssa_4_arg0 + ssa_47_shift;
	APInt ssa_49_kz_exact = ssa_9_allones ^ ssa_48_sum;
	APInt ssa_50_ko_exact = ssa_48_sum | ssa_10_zero;
	APInt ssa_51_kz_sel0 = ssa_20_cond_exact ? ssa_49_kz_exact : ssa_45_kz_tmp;
	APInt ssa_52_ko_sel0 = ssa_20_cond_exact ? ssa_50_ko_exact : ssa_46_ko_tmp;
	APInt ssa_53_kz_sel = ssa_22_bad_shift ? ssa_9_allones : ssa_51_kz_sel0;
	APInt ssa_54_ko_sel = ssa_22_bad_shift ? ssa_9_allones : ssa_52_ko_sel0;
	std::array<APInt, 2> ssa_55_r = std::array<APInt, 2>{ssa_53_kz_sel, ssa_54_ko_sel};
	return ssa_55_r;
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
	std::array<APInt, 2> ssa_6_ = meet(ssa_3_, ssa_4_);
	std::array<APInt, 2> ssa_7_ = meet(ssa_6_, ssa_5_);
	return ssa_7_;
}
}
