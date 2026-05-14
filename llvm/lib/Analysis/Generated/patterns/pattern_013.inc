namespace Pattern013 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_lhs0 = ssa_0_lhs[0];
	APInt ssa_3_lhs1 = ssa_0_lhs[1];
	APInt ssa_4_rhs0 = ssa_1_rhs[0];
	APInt ssa_5_rhs1 = ssa_1_rhs[1];
	APInt ssa_6_zero(ssa_2_lhs0.getBitWidth(), 0);
	APInt ssa_7_one(ssa_2_lhs0.getBitWidth(), 1);
	APInt ssa_8_lsb(ssa_2_lhs0.getBitWidth(), 1);
	APInt ssa_9_lhs_mask = ssa_2_lhs0 | ssa_3_lhs1;
	APInt ssa_10_rhs_mask = ssa_4_rhs0 | ssa_5_rhs1;
	APInt ssa_11_bit1;
	if (ssa_7_one.uge(ssa_7_one.getBitWidth())) {
		ssa_11_bit1 = APInt(ssa_7_one.getBitWidth(), 0);
	} else {
		ssa_11_bit1 = ssa_7_one.shl(ssa_7_one);
	}
	APInt ssa_12_a0b0 = ssa_2_lhs0 & ssa_4_rhs0;
	APInt ssa_13_sumkz0 = ssa_12_a0b0 & ssa_8_lsb;
	APInt ssa_14_a1b0 = ssa_3_lhs1 & ssa_4_rhs0;
	APInt ssa_15_a0b1 = ssa_2_lhs0 & ssa_5_rhs1;
	APInt ssa_16_sumko0 = ssa_14_a1b0 | ssa_15_a0b1;
	APInt ssa_17_sumko0_lsb = ssa_16_sumko0 & ssa_8_lsb;
	APInt ssa_18_a0_val = ssa_3_lhs1 & ssa_8_lsb;
	APInt ssa_19_b0_val = ssa_5_rhs1 & ssa_8_lsb;
	APInt ssa_20_carry0 = ssa_18_a0_val & ssa_19_b0_val;
	APInt ssa_21_carry1;
	if (ssa_7_one.uge(ssa_7_one.getBitWidth())) {
		ssa_21_carry1 = APInt(ssa_20_carry0.getBitWidth(), 0);
	} else {
		ssa_21_carry1 = ssa_20_carry0.shl(ssa_7_one);
	}
	APInt ssa_22_a1_val = ssa_3_lhs1 & ssa_11_bit1;
	APInt ssa_23_b1_val = ssa_5_rhs1 & ssa_11_bit1;
	APInt ssa_24_sum1_val0 = ssa_22_a1_val ^ ssa_23_b1_val;
	APInt ssa_25_sum1_val = ssa_24_sum1_val0 ^ ssa_21_carry1;
	APInt ssa_26_a0_known = ssa_9_lhs_mask & ssa_8_lsb;
	APInt ssa_27_b0_known = ssa_10_rhs_mask & ssa_8_lsb;
	APInt ssa_28_a1_known = ssa_9_lhs_mask & ssa_11_bit1;
	APInt ssa_29_b1_known = ssa_10_rhs_mask & ssa_11_bit1;
	bool ssa_30_a0_known_eq = ssa_26_a0_known.eq(ssa_8_lsb);
	bool ssa_31_b0_known_eq = ssa_27_b0_known.eq(ssa_8_lsb);
	bool ssa_32_cond_carry1 = ssa_30_a0_known_eq && ssa_31_b0_known_eq;
	bool ssa_33_a1_known_eq = ssa_28_a1_known.eq(ssa_11_bit1);
	bool ssa_34_b1_known_eq = ssa_29_b1_known.eq(ssa_11_bit1);
	bool ssa_35_cond_bit1 = ssa_33_a1_known_eq && ssa_34_b1_known_eq;
	bool ssa_36_cond_bit1 = ssa_35_cond_bit1 && ssa_32_cond_carry1;
	APInt ssa_37_bit1_if_known = ssa_36_cond_bit1 ? ssa_11_bit1 : ssa_6_zero;
	APInt ssa_38_sum1_one = ssa_36_cond_bit1 ? ssa_25_sum1_val : ssa_6_zero;
	APInt ssa_39_sum1_zero = ssa_37_bit1_if_known ^ ssa_38_sum1_one;
	APInt ssa_40_sumkz = ssa_13_sumkz0 | ssa_39_sum1_zero;
	APInt ssa_41_sumko = ssa_17_sumko0_lsb | ssa_38_sum1_one;
	APInt ssa_42_res0_base = ssa_4_rhs0 | ssa_40_sumkz;
	APInt ssa_43_res1_base = ssa_5_rhs1 & ssa_41_sumko;
	APInt ssa_44_allones = APInt::getAllOnes(ssa_2_lhs0.getBitWidth());
	bool ssa_45_c1 = ssa_5_rhs1.eq(ssa_44_allones);
	bool ssa_46_c2 = ssa_4_rhs0.eq(ssa_6_zero);
	bool ssa_47_cond_allones = ssa_45_c1 && ssa_46_c2;
	APInt ssa_48_tz(ssa_2_lhs0.getBitWidth(), ssa_2_lhs0.countr_one());
	APInt ssa_49_lowmask = ssa_6_zero;
	if (ssa_48_tz.ule(ssa_48_tz.getBitWidth()))
		ssa_49_lowmask.setLowBits(ssa_48_tz.getZExtValue());
	else
		ssa_49_lowmask.setLowBits(ssa_48_tz.getBitWidth());
	APInt ssa_50_bitk;
	if (ssa_48_tz.uge(ssa_48_tz.getBitWidth())) {
		ssa_50_bitk = APInt(ssa_7_one.getBitWidth(), 0);
	} else {
		ssa_50_bitk = ssa_7_one.shl(ssa_48_tz);
	}
	APInt ssa_51_bitk_knownone = ssa_3_lhs1 & ssa_50_bitk;
	bool ssa_52_bitk_nonzero = ssa_50_bitk.ne(ssa_6_zero);
	bool ssa_53_bitk_eq = ssa_51_bitk_knownone.eq(ssa_50_bitk);
	bool ssa_54_cond_stop0 = ssa_52_bitk_nonzero && ssa_53_bitk_eq;
	APInt ssa_55_k1 = ssa_48_tz + ssa_7_one;
	APInt ssa_56_mask_le_k = ssa_6_zero;
	if (ssa_55_k1.ule(ssa_55_k1.getBitWidth()))
		ssa_56_mask_le_k.setLowBits(ssa_55_k1.getZExtValue());
	else
		ssa_56_mask_le_k.setLowBits(ssa_55_k1.getBitWidth());
	APInt ssa_57_above_mask = ssa_56_mask_le_k ^ ssa_44_allones;
	APInt ssa_58_above_known1 = ssa_3_lhs1 & ssa_57_above_mask;
	APInt ssa_59_above_known0 = ssa_2_lhs0 & ssa_57_above_mask;
	APInt ssa_60_above_known1_sel = ssa_54_cond_stop0 ? ssa_58_above_known1 : ssa_6_zero;
	APInt ssa_61_above_known0_sel = ssa_54_cond_stop0 ? ssa_59_above_known0 : ssa_6_zero;
	APInt ssa_62_dec_known1 = ssa_49_lowmask | ssa_60_above_known1_sel;
	APInt ssa_63_dec_known0 = ssa_51_bitk_knownone | ssa_61_above_known0_sel;
	APInt ssa_64_res0_allones = ssa_47_cond_allones ? ssa_63_dec_known0 : ssa_42_res0_base;
	APInt ssa_65_res1_allones = ssa_47_cond_allones ? ssa_62_dec_known1 : ssa_43_res1_base;
	bool ssa_66_cl = ssa_9_lhs_mask.eq(ssa_44_allones);
	bool ssa_67_cr = ssa_10_rhs_mask.eq(ssa_44_allones);
	bool ssa_68_cond_const = ssa_66_cl && ssa_67_cr;
	APInt ssa_69_sum_const = ssa_3_lhs1 + ssa_5_rhs1;
	APInt ssa_70_res_const = ssa_5_rhs1 & ssa_69_sum_const;
	APInt ssa_71_res0_const = ssa_70_res_const ^ ssa_44_allones;
	APInt ssa_72_res0 = ssa_68_cond_const ? ssa_71_res0_const : ssa_64_res0_allones;
	APInt ssa_73_res1 = ssa_68_cond_const ? ssa_70_res_const : ssa_65_res1_allones;
	std::array<APInt, 2> ssa_74_r = std::array<APInt, 2>{ssa_72_res0, ssa_73_res1};
	return ssa_74_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_lhs0 = ssa_0_lhs[0];
	APInt ssa_3_lhs1 = ssa_0_lhs[1];
	APInt ssa_4_rhs0 = ssa_1_rhs[0];
	APInt ssa_5_rhs1 = ssa_1_rhs[1];
	APInt ssa_6_zero(ssa_2_lhs0.getBitWidth(), 0);
	APInt ssa_7_allones = APInt::getAllOnes(ssa_2_lhs0.getBitWidth());
	bool ssa_8_lhs0_eq = ssa_2_lhs0.eq(ssa_7_allones);
	bool ssa_9_lhs1_eq = ssa_3_lhs1.eq(ssa_6_zero);
	bool ssa_10_cond_lhs_zero = ssa_8_lhs0_eq && ssa_9_lhs1_eq;
	APInt ssa_11_rhs_mask = ssa_4_rhs0 | ssa_5_rhs1;
	bool ssa_12_rhs_known = ssa_11_rhs_mask.eq(ssa_7_allones);
	APInt ssa_13_tz(ssa_5_rhs1.getBitWidth(), ssa_5_rhs1.countr_zero());
	APInt ssa_14_clz(ssa_5_rhs1.getBitWidth(), ssa_5_rhs1.countl_zero());
	APInt ssa_15_bw(ssa_5_rhs1.getBitWidth(), ssa_5_rhs1.getBitWidth());
	APInt ssa_16_k = ssa_15_bw - ssa_14_clz;
	APInt ssa_17_lowmask = ssa_6_zero;
	if (ssa_16_k.ule(ssa_16_k.getBitWidth()))
		ssa_17_lowmask.setLowBits(ssa_16_k.getZExtValue());
	else
		ssa_17_lowmask.setLowBits(ssa_16_k.getBitWidth());
	APInt ssa_18_mask = ssa_17_lowmask;
	if (ssa_13_tz.ule(ssa_13_tz.getBitWidth()))
		ssa_18_mask.clearLowBits(ssa_13_tz.getZExtValue());
	else
		ssa_18_mask.clearLowBits(ssa_13_tz.getBitWidth());
	APInt ssa_19_lhs0_mask = ssa_2_lhs0 & ssa_18_mask;
	APInt ssa_20_lhs1_mask = ssa_3_lhs1 & ssa_18_mask;
	bool ssa_21_lhs0_mask_eq = ssa_19_lhs0_mask.eq(ssa_18_mask);
	bool ssa_22_lhs1_mask_eq = ssa_20_lhs1_mask.eq(ssa_6_zero);
	bool ssa_23_cond_lhs_mask = ssa_21_lhs0_mask_eq && ssa_22_lhs1_mask_eq;
	bool ssa_24_cond_rhs = ssa_12_rhs_known && ssa_23_cond_lhs_mask;
	bool ssa_25_cond = ssa_10_cond_lhs_zero || ssa_24_cond_rhs;
	APInt ssa_26_res0 = ssa_25_cond ? ssa_4_rhs0 : ssa_6_zero;
	APInt ssa_27_res1 = ssa_25_cond ? ssa_5_rhs1 : ssa_6_zero;
	std::array<APInt, 2> ssa_28_r = std::array<APInt, 2>{ssa_26_res0, ssa_27_res1};
	return ssa_28_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_lhs0 = ssa_0_lhs[0];
	APInt ssa_3_lhs1 = ssa_0_lhs[1];
	APInt ssa_4_rhs0 = ssa_1_rhs[0];
	APInt ssa_5_rhs1 = ssa_1_rhs[1];
	APInt ssa_6_zero(ssa_2_lhs0.getBitWidth(), 0);
	APInt ssa_7_one(ssa_2_lhs0.getBitWidth(), 1);
	APInt ssa_8_tz_a(ssa_2_lhs0.getBitWidth(), ssa_2_lhs0.countr_one());
	APInt ssa_9_lowmask = ssa_6_zero;
	if (ssa_8_tz_a.ule(ssa_8_tz_a.getBitWidth()))
		ssa_9_lowmask.setLowBits(ssa_8_tz_a.getZExtValue());
	else
		ssa_9_lowmask.setLowBits(ssa_8_tz_a.getBitWidth());
	APInt ssa_10_b1_low = ssa_5_rhs1 & ssa_9_lowmask;
	APInt ssa_11_bitk;
	if (ssa_8_tz_a.uge(ssa_8_tz_a.getBitWidth())) {
		ssa_11_bitk = APInt(ssa_7_one.getBitWidth(), 0);
	} else {
		ssa_11_bitk = ssa_7_one.shl(ssa_8_tz_a);
	}
	bool ssa_12_bitk_nonzero = ssa_11_bitk.ne(ssa_6_zero);
	APInt ssa_13_a1_bit = ssa_3_lhs1 & ssa_11_bitk;
	bool ssa_14_a1_bit_eq = ssa_13_a1_bit.eq(ssa_11_bitk);
	APInt ssa_15_a0_bit = ssa_2_lhs0 & ssa_11_bitk;
	bool ssa_16_a0_bit_eq = ssa_15_a0_bit.eq(ssa_11_bitk);
	APInt ssa_17_b1_bit = ssa_5_rhs1 & ssa_11_bitk;
	bool ssa_18_b1_bit_eq = ssa_17_b1_bit.eq(ssa_11_bitk);
	bool ssa_19_cond_a1 = ssa_12_bitk_nonzero && ssa_14_a1_bit_eq;
	bool ssa_20_cond_a0b1 = ssa_12_bitk_nonzero && ssa_16_a0_bit_eq;
	bool ssa_21_cond_a0b1 = ssa_20_cond_a0b1 && ssa_18_b1_bit_eq;
	APInt ssa_22_kz_extra = ssa_19_cond_a1 ? ssa_11_bitk : ssa_6_zero;
	APInt ssa_23_ko_extra = ssa_21_cond_a0b1 ? ssa_11_bitk : ssa_6_zero;
	APInt ssa_24_both_zero = ssa_2_lhs0 & ssa_4_rhs0;
	APInt ssa_25_clz_a0(ssa_2_lhs0.getBitWidth(), ssa_2_lhs0.countl_one());
	APInt ssa_26_mask_lz = ssa_6_zero;
	if (ssa_25_clz_a0.ule(ssa_25_clz_a0.getBitWidth()))
		ssa_26_mask_lz.setHighBits(ssa_25_clz_a0.getZExtValue());
	else
		ssa_26_mask_lz.setHighBits(ssa_25_clz_a0.getBitWidth());
	APInt ssa_27_both_zero_lz = ssa_24_both_zero & ssa_26_mask_lz;
	bool ssa_28_both_zero_lz_nonzero = ssa_27_both_zero_lz.ne(ssa_6_zero);
	APInt ssa_29_clz_both_lz(ssa_27_both_zero_lz.getBitWidth(), ssa_27_both_zero_lz.countl_zero());
	APInt ssa_30_mask_above_lz = ssa_6_zero;
	if (ssa_29_clz_both_lz.ule(ssa_29_clz_both_lz.getBitWidth()))
		ssa_30_mask_above_lz.setHighBits(ssa_29_clz_both_lz.getZExtValue());
	else
		ssa_30_mask_above_lz.setHighBits(ssa_29_clz_both_lz.getBitWidth());
	APInt ssa_31_mask_above_lz_sel = ssa_28_both_zero_lz_nonzero ? ssa_30_mask_above_lz : ssa_6_zero;
	APInt ssa_32_b1_high = ssa_5_rhs1 & ssa_31_mask_above_lz_sel;
	APInt ssa_33_res0 = ssa_4_rhs0 | ssa_22_kz_extra;
	APInt ssa_34_res1 = ssa_10_b1_low | ssa_23_ko_extra;
	APInt ssa_35_res1 = ssa_34_res1 | ssa_32_b1_high;
	APInt ssa_36_res1 = ssa_35_res1 | ssa_6_zero;
	std::array<APInt, 2> ssa_37_r = std::array<APInt, 2>{ssa_33_res0, ssa_36_res1};
	return ssa_37_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_lhs0 = ssa_0_lhs[0];
	APInt ssa_3_lhs1 = ssa_0_lhs[1];
	APInt ssa_4_rhs0 = ssa_1_rhs[0];
	APInt ssa_5_rhs1 = ssa_1_rhs[1];
	APInt ssa_6_zero(ssa_2_lhs0.getBitWidth(), 0);
	APInt ssa_7_one(ssa_2_lhs0.getBitWidth(), 1);
	APInt ssa_8_lsb = ssa_7_one | ssa_6_zero;
	APInt ssa_9_bit1;
	if (ssa_7_one.uge(ssa_7_one.getBitWidth())) {
		ssa_9_bit1 = APInt(ssa_7_one.getBitWidth(), 0);
	} else {
		ssa_9_bit1 = ssa_7_one.shl(ssa_7_one);
	}
	APInt ssa_10_allones = APInt::getAllOnes(ssa_2_lhs0.getBitWidth());
	APInt ssa_11_max_a = ssa_10_allones ^ ssa_2_lhs0;
	APInt ssa_12_max_b = ssa_10_allones ^ ssa_4_rhs0;
	APInt ssa_13_min_a = ssa_3_lhs1 | ssa_6_zero;
	APInt ssa_14_min_b = ssa_5_rhs1 | ssa_6_zero;
	APInt ssa_15_sum_min = ssa_13_min_a + ssa_14_min_b;
	APInt ssa_16_sum_max = ssa_11_max_a + ssa_12_max_b;
	bool ssa_17_ovf;
	ssa_11_max_a.uadd_ov(ssa_12_max_b, ssa_17_ovf);
	bool ssa_18_true = ssa_6_zero.eq(ssa_6_zero);
	bool ssa_19_no_ovf = ssa_17_ovf != ssa_18_true;
	APInt ssa_20_diff = ssa_15_sum_min ^ ssa_16_sum_max;
	APInt ssa_21_clz(ssa_20_diff.getBitWidth(), ssa_20_diff.countl_zero());
	APInt ssa_22_mask_high = ssa_6_zero;
	if (ssa_21_clz.ule(ssa_21_clz.getBitWidth()))
		ssa_22_mask_high.setHighBits(ssa_21_clz.getZExtValue());
	else
		ssa_22_mask_high.setHighBits(ssa_21_clz.getBitWidth());
	APInt ssa_23_sum_knownone = ssa_22_mask_high & ssa_15_sum_min;
	APInt ssa_24_sum_min_not = ssa_10_allones ^ ssa_15_sum_min;
	APInt ssa_25_sum_knownzero = ssa_22_mask_high & ssa_24_sum_min_not;
	APInt ssa_26_b0_kz = ssa_4_rhs0 & ssa_8_lsb;
	bool ssa_27_b0_kz_eq = ssa_26_b0_kz.eq(ssa_8_lsb);
	APInt ssa_28_a1_ko = ssa_3_lhs1 & ssa_9_bit1;
	bool ssa_29_a1_ko_eq = ssa_28_a1_ko.eq(ssa_9_bit1);
	APInt ssa_30_a1_kz = ssa_2_lhs0 & ssa_9_bit1;
	bool ssa_31_a1_kz_eq = ssa_30_a1_kz.eq(ssa_9_bit1);
	APInt ssa_32_b1_ko = ssa_5_rhs1 & ssa_9_bit1;
	bool ssa_33_b1_ko_eq = ssa_32_b1_ko.eq(ssa_9_bit1);
	bool ssa_34_cond_kz_bit1 = ssa_27_b0_kz_eq && ssa_29_a1_ko_eq;
	bool ssa_35_cond_ko_bit1 = ssa_27_b0_kz_eq && ssa_31_a1_kz_eq;
	bool ssa_36_cond_ko_bit1 = ssa_35_cond_ko_bit1 && ssa_33_b1_ko_eq;
	APInt ssa_37_bit1_kz_extra = ssa_34_cond_kz_bit1 ? ssa_9_bit1 : ssa_6_zero;
	APInt ssa_38_bit1_ko_extra = ssa_36_cond_ko_bit1 ? ssa_9_bit1 : ssa_6_zero;
	APInt ssa_39_kz_base = ssa_4_rhs0 | ssa_37_bit1_kz_extra;
	APInt ssa_40_kz_tmp = ssa_39_kz_base | ssa_25_sum_knownzero;
	APInt ssa_41_ko_tmp = ssa_5_rhs1 & ssa_23_sum_knownone;
	APInt ssa_42_ko_base = ssa_41_ko_tmp | ssa_38_bit1_ko_extra;
	APInt ssa_43_kz = ssa_19_no_ovf ? ssa_40_kz_tmp : ssa_39_kz_base;
	APInt ssa_44_ko = ssa_19_no_ovf ? ssa_42_ko_base : ssa_38_bit1_ko_extra;
	std::array<APInt, 2> ssa_45_r = std::array<APInt, 2>{ssa_43_kz, ssa_44_ko};
	return ssa_45_r;
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
std::array<APInt, 2> solution(std::array<APInt, 2> ssa_0_, std::array<APInt, 2> ssa_1_) {
	std::array<APInt, 2> ssa_2_ = partial_solution_0(ssa_0_, ssa_1_);
	std::array<APInt, 2> ssa_3_ = partial_solution_1(ssa_0_, ssa_1_);
	std::array<APInt, 2> ssa_4_ = partial_solution_2(ssa_0_, ssa_1_);
	std::array<APInt, 2> ssa_5_ = partial_solution_3(ssa_0_, ssa_1_);
	std::array<APInt, 2> ssa_6_ = meet(ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_7_ = meet(ssa_6_, ssa_4_);
	std::array<APInt, 2> ssa_8_ = meet(ssa_7_, ssa_5_);
	return ssa_8_;
}
}
