namespace Pattern018 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_11_bmask = ssa_5_b0 | ssa_6_b1;
	APInt ssa_12_cmask = ssa_7_c0 | ssa_8_c1;
	APInt ssa_13_bu = ssa_9_allones ^ ssa_11_bmask;
	APInt ssa_14_cu = ssa_9_allones ^ ssa_12_cmask;
	APInt ssa_15_bmax = ssa_6_b1 | ssa_13_bu;
	APInt ssa_16_cmax = ssa_8_c1 | ssa_14_cu;
	APInt ssa_17_sum_min = ssa_6_b1 + ssa_8_c1;
	APInt ssa_18_sum_max = ssa_15_bmax + ssa_16_cmax;
	bool ssa_19_ov_max;
	ssa_15_bmax.uadd_ov(ssa_16_cmax, ssa_19_ov_max);
	bool ssa_20_ov_min;
	ssa_6_b1.uadd_ov(ssa_8_c1, ssa_20_ov_min);
	bool ssa_21_true = ssa_3_a0.eq(ssa_3_a0);
	bool ssa_22_not_ov_max = ssa_19_ov_max != ssa_21_true;
	bool ssa_23_safe = ssa_22_not_ov_max || ssa_20_ov_min;
	APInt ssa_24_diff = ssa_17_sum_min ^ ssa_18_sum_max;
	APInt ssa_25_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_26_clz(ssa_24_diff.getBitWidth(), ssa_24_diff.countl_zero());
	APInt ssa_27_n_minus_clz = ssa_25_bw - ssa_26_clz;
	APInt ssa_28_maskLower = ssa_10_zero;
	if (ssa_27_n_minus_clz.ule(ssa_27_n_minus_clz.getBitWidth()))
		ssa_28_maskLower.setLowBits(ssa_27_n_minus_clz.getZExtValue());
	else
		ssa_28_maskLower.setLowBits(ssa_27_n_minus_clz.getBitWidth());
	APInt ssa_29_common_mask = ssa_9_allones ^ ssa_28_maskLower;
	APInt ssa_30_sum_and = ssa_17_sum_min & ssa_18_sum_max;
	APInt ssa_31_sum_not_min = ssa_9_allones ^ ssa_17_sum_min;
	APInt ssa_32_sum_not_max = ssa_9_allones ^ ssa_18_sum_max;
	APInt ssa_33_sum_not_and = ssa_31_sum_not_min & ssa_32_sum_not_max;
	APInt ssa_34_sum1_range = ssa_30_sum_and & ssa_29_common_mask;
	APInt ssa_35_sum0_range = ssa_33_sum_not_and & ssa_29_common_mask;
	APInt ssa_36_sum1_range_sel = ssa_23_safe ? ssa_34_sum1_range : ssa_10_zero;
	APInt ssa_37_sum0_range_sel = ssa_23_safe ? ssa_35_sum0_range : ssa_10_zero;
	APInt ssa_38_k_b(ssa_11_bmask.getBitWidth(), ssa_11_bmask.countr_one());
	APInt ssa_39_k_c(ssa_12_cmask.getBitWidth(), ssa_12_cmask.countr_one());
	APInt ssa_40_k = APIntOps::umin(ssa_38_k_b, ssa_39_k_c);
	APInt ssa_41_maskLow_exact = ssa_10_zero;
	if (ssa_40_k.ule(ssa_40_k.getBitWidth()))
		ssa_41_maskLow_exact.setLowBits(ssa_40_k.getZExtValue());
	else
		ssa_41_maskLow_exact.setLowBits(ssa_40_k.getBitWidth());
	APInt ssa_42_b_low = ssa_6_b1 & ssa_41_maskLow_exact;
	APInt ssa_43_c_low = ssa_8_c1 & ssa_41_maskLow_exact;
	APInt ssa_44_sum_low = ssa_42_b_low + ssa_43_c_low;
	APInt ssa_45_sum_low_masked = ssa_44_sum_low & ssa_41_maskLow_exact;
	APInt ssa_46_sum_not_low = ssa_9_allones ^ ssa_45_sum_low_masked;
	APInt ssa_47_sum0_low = ssa_46_sum_not_low & ssa_41_maskLow_exact;
	APInt ssa_48_sum1 = ssa_36_sum1_range_sel | ssa_45_sum_low_masked;
	APInt ssa_49_sum0 = ssa_37_sum0_range_sel | ssa_47_sum0_low;
	APInt ssa_50_t1 = ssa_8_c1 & ssa_48_sum1;
	APInt ssa_51_t0_base = ssa_7_c0 | ssa_49_sum0;
	APInt ssa_52_t0 = ssa_51_t0_base | ssa_4_a1;
	APInt ssa_53_res0 = ssa_3_a0 & ssa_52_t0;
	APInt ssa_54_res1 = ssa_4_a1 | ssa_50_t1;
	APInt ssa_55_conflict = ssa_4_a1 & ssa_50_t1;
	bool ssa_56_conflict_zero = ssa_55_conflict.eq(ssa_10_zero);
	APInt ssa_57_res0_sel = ssa_56_conflict_zero ? ssa_53_res0 : ssa_9_allones;
	APInt ssa_58_res1_sel = ssa_56_conflict_zero ? ssa_54_res1 : ssa_9_allones;
	std::array<APInt, 2> ssa_59_r = std::array<APInt, 2>{ssa_57_res0_sel, ssa_58_res1_sel};
	return ssa_59_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_11_bmask = ssa_5_b0 | ssa_6_b1;
	APInt ssa_12_cmask = ssa_7_c0 | ssa_8_c1;
	APInt ssa_13_bu = ssa_9_allones ^ ssa_11_bmask;
	APInt ssa_14_cu = ssa_9_allones ^ ssa_12_cmask;
	APInt ssa_15_bmax = ssa_6_b1 | ssa_13_bu;
	APInt ssa_16_cmax = ssa_8_c1 | ssa_14_cu;
	APInt ssa_17_sum_min = ssa_6_b1 + ssa_8_c1;
	APInt ssa_18_sum_max = ssa_15_bmax + ssa_16_cmax;
	bool ssa_19_ov_max;
	ssa_15_bmax.uadd_ov(ssa_16_cmax, ssa_19_ov_max);
	bool ssa_20_ov_min;
	ssa_6_b1.uadd_ov(ssa_8_c1, ssa_20_ov_min);
	bool ssa_21_true = ssa_3_a0.eq(ssa_3_a0);
	bool ssa_22_not_ov_max = ssa_19_ov_max != ssa_21_true;
	bool ssa_23_safe = ssa_22_not_ov_max || ssa_20_ov_min;
	bool ssa_24_wrap = ssa_23_safe != ssa_21_true;
	APInt ssa_25_sum0_constraint = ssa_4_a1 & ssa_8_c1;
	APInt ssa_26_diff = ssa_17_sum_min ^ ssa_18_sum_max;
	APInt ssa_27_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_28_clz(ssa_26_diff.getBitWidth(), ssa_26_diff.countl_zero());
	APInt ssa_29_n_minus_clz = ssa_27_bw - ssa_28_clz;
	APInt ssa_30_maskLower = ssa_10_zero;
	if (ssa_29_n_minus_clz.ule(ssa_29_n_minus_clz.getBitWidth()))
		ssa_30_maskLower.setLowBits(ssa_29_n_minus_clz.getZExtValue());
	else
		ssa_30_maskLower.setLowBits(ssa_29_n_minus_clz.getBitWidth());
	APInt ssa_31_common_mask = ssa_9_allones ^ ssa_30_maskLower;
	APInt ssa_32_sum_and = ssa_17_sum_min & ssa_18_sum_max;
	APInt ssa_33_sum_not_min = ssa_9_allones ^ ssa_17_sum_min;
	APInt ssa_34_sum_not_max = ssa_9_allones ^ ssa_18_sum_max;
	APInt ssa_35_sum_not_and = ssa_33_sum_not_min & ssa_34_sum_not_max;
	APInt ssa_36_sum1_range = ssa_32_sum_and & ssa_31_common_mask;
	APInt ssa_37_sum0_range = ssa_35_sum_not_and & ssa_31_common_mask;
	APInt ssa_38_sum1_range_sel = ssa_23_safe ? ssa_36_sum1_range : ssa_10_zero;
	APInt ssa_39_sum0_range_sel = ssa_23_safe ? ssa_37_sum0_range : ssa_10_zero;
	APInt ssa_40_diff1 = ssa_17_sum_min ^ ssa_9_allones;
	APInt ssa_41_clz1(ssa_40_diff1.getBitWidth(), ssa_40_diff1.countl_zero());
	APInt ssa_42_n_minus_clz1 = ssa_27_bw - ssa_41_clz1;
	APInt ssa_43_maskLower1 = ssa_10_zero;
	if (ssa_42_n_minus_clz1.ule(ssa_42_n_minus_clz1.getBitWidth()))
		ssa_43_maskLower1.setLowBits(ssa_42_n_minus_clz1.getZExtValue());
	else
		ssa_43_maskLower1.setLowBits(ssa_42_n_minus_clz1.getBitWidth());
	APInt ssa_44_common_mask1 = ssa_9_allones ^ ssa_43_maskLower1;
	APInt ssa_45_known1_r1 = ssa_17_sum_min & ssa_44_common_mask1;
	APInt ssa_46_conflict_r1 = ssa_25_sum0_constraint & ssa_45_known1_r1;
	bool ssa_47_conflict_r1_zero = ssa_46_conflict_r1.eq(ssa_10_zero);
	bool ssa_48_conflict_r1_nz = ssa_47_conflict_r1_zero != ssa_21_true;
	bool ssa_49_use_range2 = ssa_24_wrap && ssa_48_conflict_r1_nz;
	APInt ssa_50_clz2(ssa_18_sum_max.getBitWidth(), ssa_18_sum_max.countl_zero());
	APInt ssa_51_n_minus_clz2 = ssa_27_bw - ssa_50_clz2;
	APInt ssa_52_maskLower2 = ssa_10_zero;
	if (ssa_51_n_minus_clz2.ule(ssa_51_n_minus_clz2.getBitWidth()))
		ssa_52_maskLower2.setLowBits(ssa_51_n_minus_clz2.getZExtValue());
	else
		ssa_52_maskLower2.setLowBits(ssa_51_n_minus_clz2.getBitWidth());
	APInt ssa_53_common_mask2 = ssa_9_allones ^ ssa_52_maskLower2;
	APInt ssa_54_sum_not_max2 = ssa_9_allones ^ ssa_18_sum_max;
	APInt ssa_55_sum0_range2 = ssa_54_sum_not_max2 & ssa_53_common_mask2;
	APInt ssa_56_sum0_range2_sel = ssa_49_use_range2 ? ssa_55_sum0_range2 : ssa_10_zero;
	APInt ssa_57_sum0_range_tmp = ssa_39_sum0_range_sel | ssa_56_sum0_range2_sel;
	APInt ssa_58_sum1_range_tmp = ssa_38_sum1_range_sel | ssa_10_zero;
	APInt ssa_59_sum0_range_final = ssa_57_sum0_range_tmp | ssa_25_sum0_constraint;
	APInt ssa_60_k_b(ssa_11_bmask.getBitWidth(), ssa_11_bmask.countr_one());
	APInt ssa_61_k_c(ssa_12_cmask.getBitWidth(), ssa_12_cmask.countr_one());
	APInt ssa_62_k = APIntOps::umin(ssa_60_k_b, ssa_61_k_c);
	APInt ssa_63_maskLow_exact = ssa_10_zero;
	if (ssa_62_k.ule(ssa_62_k.getBitWidth()))
		ssa_63_maskLow_exact.setLowBits(ssa_62_k.getZExtValue());
	else
		ssa_63_maskLow_exact.setLowBits(ssa_62_k.getBitWidth());
	APInt ssa_64_b_low = ssa_6_b1 & ssa_63_maskLow_exact;
	APInt ssa_65_c_low = ssa_8_c1 & ssa_63_maskLow_exact;
	APInt ssa_66_sum_low = ssa_64_b_low + ssa_65_c_low;
	APInt ssa_67_sum_low_masked = ssa_66_sum_low & ssa_63_maskLow_exact;
	APInt ssa_68_sum_not_low = ssa_9_allones ^ ssa_67_sum_low_masked;
	APInt ssa_69_sum0_low = ssa_68_sum_not_low & ssa_63_maskLow_exact;
	APInt ssa_70_sum1 = ssa_58_sum1_range_tmp | ssa_67_sum_low_masked;
	APInt ssa_71_sum0 = ssa_59_sum0_range_final | ssa_69_sum0_low;
	APInt ssa_72_conflict_sum = ssa_70_sum1 & ssa_25_sum0_constraint;
	bool ssa_73_conflict_sum_zero = ssa_72_conflict_sum.eq(ssa_10_zero);
	APInt ssa_74_t1 = ssa_8_c1 & ssa_70_sum1;
	APInt ssa_75_t0_base = ssa_7_c0 | ssa_71_sum0;
	APInt ssa_76_t0 = ssa_75_t0_base | ssa_4_a1;
	APInt ssa_77_res0 = ssa_3_a0 & ssa_76_t0;
	APInt ssa_78_res1 = ssa_4_a1 | ssa_74_t1;
	APInt ssa_79_conflict = ssa_4_a1 & ssa_74_t1;
	bool ssa_80_conflict_zero = ssa_79_conflict.eq(ssa_10_zero);
	bool ssa_81_ok = ssa_80_conflict_zero && ssa_73_conflict_sum_zero;
	APInt ssa_82_res0_sel = ssa_81_ok ? ssa_77_res0 : ssa_9_allones;
	APInt ssa_83_res1_sel = ssa_81_ok ? ssa_78_res1 : ssa_9_allones;
	std::array<APInt, 2> ssa_84_r = std::array<APInt, 2>{ssa_82_res0_sel, ssa_83_res1_sel};
	return ssa_84_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_11_bmask = ssa_5_b0 | ssa_6_b1;
	APInt ssa_12_cmask = ssa_7_c0 | ssa_8_c1;
	APInt ssa_13_bu = ssa_9_allones ^ ssa_11_bmask;
	APInt ssa_14_cu = ssa_9_allones ^ ssa_12_cmask;
	APInt ssa_15_bmax = ssa_6_b1 | ssa_13_bu;
	APInt ssa_16_cmax = ssa_8_c1 | ssa_14_cu;
	APInt ssa_17_sum_min = ssa_6_b1 + ssa_8_c1;
	APInt ssa_18_sum_max = ssa_15_bmax + ssa_16_cmax;
	bool ssa_19_ov_max;
	ssa_15_bmax.uadd_ov(ssa_16_cmax, ssa_19_ov_max);
	bool ssa_20_ov_min;
	ssa_6_b1.uadd_ov(ssa_8_c1, ssa_20_ov_min);
	bool ssa_21_true = ssa_3_a0.eq(ssa_3_a0);
	bool ssa_22_not_ov_max = ssa_19_ov_max != ssa_21_true;
	bool ssa_23_safe = ssa_22_not_ov_max || ssa_20_ov_min;
	bool ssa_24_wrap = ssa_23_safe != ssa_21_true;
	APInt ssa_25_sum0_constraint = ssa_4_a1 & ssa_8_c1;
	APInt ssa_26_diff = ssa_17_sum_min ^ ssa_18_sum_max;
	APInt ssa_27_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_28_clz(ssa_26_diff.getBitWidth(), ssa_26_diff.countl_zero());
	APInt ssa_29_n_minus_clz = ssa_27_bw - ssa_28_clz;
	APInt ssa_30_maskLower = ssa_10_zero;
	if (ssa_29_n_minus_clz.ule(ssa_29_n_minus_clz.getBitWidth()))
		ssa_30_maskLower.setLowBits(ssa_29_n_minus_clz.getZExtValue());
	else
		ssa_30_maskLower.setLowBits(ssa_29_n_minus_clz.getBitWidth());
	APInt ssa_31_common_mask = ssa_9_allones ^ ssa_30_maskLower;
	APInt ssa_32_sum_and = ssa_17_sum_min & ssa_18_sum_max;
	APInt ssa_33_sum_not_min = ssa_9_allones ^ ssa_17_sum_min;
	APInt ssa_34_sum_not_max = ssa_9_allones ^ ssa_18_sum_max;
	APInt ssa_35_sum_not_and = ssa_33_sum_not_min & ssa_34_sum_not_max;
	APInt ssa_36_sum1_range = ssa_32_sum_and & ssa_31_common_mask;
	APInt ssa_37_sum0_range = ssa_35_sum_not_and & ssa_31_common_mask;
	APInt ssa_38_sum1_range_sel = ssa_23_safe ? ssa_36_sum1_range : ssa_10_zero;
	APInt ssa_39_sum0_range_sel = ssa_23_safe ? ssa_37_sum0_range : ssa_10_zero;
	APInt ssa_40_diff1 = ssa_17_sum_min ^ ssa_9_allones;
	APInt ssa_41_clz1(ssa_40_diff1.getBitWidth(), ssa_40_diff1.countl_zero());
	APInt ssa_42_n_minus_clz1 = ssa_27_bw - ssa_41_clz1;
	APInt ssa_43_maskLower1 = ssa_10_zero;
	if (ssa_42_n_minus_clz1.ule(ssa_42_n_minus_clz1.getBitWidth()))
		ssa_43_maskLower1.setLowBits(ssa_42_n_minus_clz1.getZExtValue());
	else
		ssa_43_maskLower1.setLowBits(ssa_42_n_minus_clz1.getBitWidth());
	APInt ssa_44_common_mask1 = ssa_9_allones ^ ssa_43_maskLower1;
	APInt ssa_45_known1_r1 = ssa_17_sum_min & ssa_44_common_mask1;
	APInt ssa_46_conflict_r1 = ssa_25_sum0_constraint & ssa_45_known1_r1;
	bool ssa_47_conflict_r1_zero = ssa_46_conflict_r1.eq(ssa_10_zero);
	bool ssa_48_conflict_r1_nz = ssa_47_conflict_r1_zero != ssa_21_true;
	bool ssa_49_use_range2 = ssa_24_wrap && ssa_48_conflict_r1_nz;
	APInt ssa_50_clz2(ssa_18_sum_max.getBitWidth(), ssa_18_sum_max.countl_zero());
	APInt ssa_51_n_minus_clz2 = ssa_27_bw - ssa_50_clz2;
	APInt ssa_52_maskLower2 = ssa_10_zero;
	if (ssa_51_n_minus_clz2.ule(ssa_51_n_minus_clz2.getBitWidth()))
		ssa_52_maskLower2.setLowBits(ssa_51_n_minus_clz2.getZExtValue());
	else
		ssa_52_maskLower2.setLowBits(ssa_51_n_minus_clz2.getBitWidth());
	APInt ssa_53_common_mask2 = ssa_9_allones ^ ssa_52_maskLower2;
	APInt ssa_54_sum_not_max2 = ssa_9_allones ^ ssa_18_sum_max;
	APInt ssa_55_sum0_range2 = ssa_54_sum_not_max2 & ssa_53_common_mask2;
	APInt ssa_56_sum0_range2_sel = ssa_49_use_range2 ? ssa_55_sum0_range2 : ssa_10_zero;
	APInt ssa_57_sum0_range_tmp = ssa_39_sum0_range_sel | ssa_56_sum0_range2_sel;
	APInt ssa_58_sum1_range_tmp = ssa_38_sum1_range_sel | ssa_10_zero;
	APInt ssa_59_sum0_range_final = ssa_57_sum0_range_tmp | ssa_25_sum0_constraint;
	APInt ssa_60_one(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_61_start = ssa_25_sum0_constraint & ssa_25_sum0_constraint;
	bool ssa_62_has_start = ssa_61_start.ne(ssa_10_zero);
	APInt ssa_63_ctz(ssa_61_start.getBitWidth(), ssa_61_start.countr_zero());
	APInt ssa_64_n_plus1 = ssa_63_ctz + ssa_60_one;
	APInt ssa_65_c0b1 = ssa_7_c0 & ssa_6_b1;
	APInt ssa_66_prop = ssa_8_c1 | ssa_65_c0b1;
	APInt ssa_67_prop_shift;
	if (ssa_64_n_plus1.uge(ssa_64_n_plus1.getBitWidth())) {
		ssa_67_prop_shift = APInt(ssa_66_prop.getBitWidth(), 0);
	} else {
		ssa_67_prop_shift = ssa_66_prop.lshr(ssa_64_n_plus1);
	}
	APInt ssa_68_len(ssa_67_prop_shift.getBitWidth(), ssa_67_prop_shift.countr_one());
	APInt ssa_69_mask_len = ssa_10_zero;
	if (ssa_68_len.ule(ssa_68_len.getBitWidth()))
		ssa_69_mask_len.setLowBits(ssa_68_len.getZExtValue());
	else
		ssa_69_mask_len.setLowBits(ssa_68_len.getBitWidth());
	APInt ssa_70_carry_mask_tmp;
	if (ssa_64_n_plus1.uge(ssa_64_n_plus1.getBitWidth())) {
		ssa_70_carry_mask_tmp = APInt(ssa_69_mask_len.getBitWidth(), 0);
	} else {
		ssa_70_carry_mask_tmp = ssa_69_mask_len.shl(ssa_64_n_plus1);
	}
	APInt ssa_71_carry_mask = ssa_62_has_start ? ssa_70_carry_mask_tmp : ssa_10_zero;
	APInt ssa_72_sum1_carry_tmp = ssa_71_carry_mask & ssa_8_c1;
	APInt ssa_73_sum1_carry = ssa_72_sum1_carry_tmp & ssa_6_b1;
	APInt ssa_74_sum0_carry1_tmp = ssa_71_carry_mask & ssa_8_c1;
	APInt ssa_75_sum0_carry1 = ssa_74_sum0_carry1_tmp & ssa_5_b0;
	APInt ssa_76_sum0_carry2_tmp = ssa_71_carry_mask & ssa_7_c0;
	APInt ssa_77_sum0_carry2 = ssa_76_sum0_carry2_tmp & ssa_6_b1;
	APInt ssa_78_sum0_carry = ssa_75_sum0_carry1 | ssa_77_sum0_carry2;
	APInt ssa_79_gen1 = ssa_6_b1 & ssa_8_c1;
	bool ssa_80_has_gen1 = ssa_79_gen1.ne(ssa_10_zero);
	APInt ssa_81_ctz_gen1(ssa_79_gen1.getBitWidth(), ssa_79_gen1.countr_zero());
	APInt ssa_82_n_plus1_gen1 = ssa_81_ctz_gen1 + ssa_60_one;
	APInt ssa_83_prop1 = ssa_6_b1 | ssa_8_c1;
	APInt ssa_84_prop1_shift;
	if (ssa_82_n_plus1_gen1.uge(ssa_82_n_plus1_gen1.getBitWidth())) {
		ssa_84_prop1_shift = APInt(ssa_83_prop1.getBitWidth(), 0);
	} else {
		ssa_84_prop1_shift = ssa_83_prop1.lshr(ssa_82_n_plus1_gen1);
	}
	APInt ssa_85_len_gen(ssa_84_prop1_shift.getBitWidth(), ssa_84_prop1_shift.countr_one());
	APInt ssa_86_mask_len_gen = ssa_10_zero;
	if (ssa_85_len_gen.ule(ssa_85_len_gen.getBitWidth()))
		ssa_86_mask_len_gen.setLowBits(ssa_85_len_gen.getZExtValue());
	else
		ssa_86_mask_len_gen.setLowBits(ssa_85_len_gen.getBitWidth());
	APInt ssa_87_carry_mask_gen_tmp;
	if (ssa_82_n_plus1_gen1.uge(ssa_82_n_plus1_gen1.getBitWidth())) {
		ssa_87_carry_mask_gen_tmp = APInt(ssa_86_mask_len_gen.getBitWidth(), 0);
	} else {
		ssa_87_carry_mask_gen_tmp = ssa_86_mask_len_gen.shl(ssa_82_n_plus1_gen1);
	}
	APInt ssa_88_carry_mask_gen = ssa_80_has_gen1 ? ssa_87_carry_mask_gen_tmp : ssa_10_zero;
	APInt ssa_89_sum1_carry_gen_tmp = ssa_88_carry_mask_gen & ssa_8_c1;
	APInt ssa_90_sum1_carry_gen = ssa_89_sum1_carry_gen_tmp & ssa_6_b1;
	APInt ssa_91_sum0_carry_gen1_tmp = ssa_88_carry_mask_gen & ssa_8_c1;
	APInt ssa_92_sum0_carry_gen1 = ssa_91_sum0_carry_gen1_tmp & ssa_5_b0;
	APInt ssa_93_sum0_carry_gen2_tmp = ssa_88_carry_mask_gen & ssa_7_c0;
	APInt ssa_94_sum0_carry_gen2 = ssa_93_sum0_carry_gen2_tmp & ssa_6_b1;
	APInt ssa_95_sum0_carry_gen = ssa_92_sum0_carry_gen1 | ssa_94_sum0_carry_gen2;
	APInt ssa_96_kill = ssa_5_b0 & ssa_7_c0;
	APInt ssa_97_p_kill = ssa_82_n_plus1_gen1 + ssa_85_len_gen;
	APInt ssa_98_bit_kill;
	if (ssa_97_p_kill.uge(ssa_97_p_kill.getBitWidth())) {
		ssa_98_bit_kill = APInt(ssa_60_one.getBitWidth(), 0);
	} else {
		ssa_98_bit_kill = ssa_60_one.shl(ssa_97_p_kill);
	}
	APInt ssa_99_sum1_kill_tmp = ssa_98_bit_kill & ssa_96_kill;
	APInt ssa_100_sum1_kill = ssa_80_has_gen1 ? ssa_99_sum1_kill_tmp : ssa_10_zero;
	bool ssa_101_has_kill = ssa_96_kill.ne(ssa_10_zero);
	APInt ssa_102_ctz_kill(ssa_96_kill.getBitWidth(), ssa_96_kill.countr_zero());
	APInt ssa_103_shift_kill0 = ssa_102_ctz_kill + ssa_60_one;
	APInt ssa_104_gen_zero = ssa_5_b0 | ssa_7_c0;
	APInt ssa_105_gen_zero_shift;
	if (ssa_103_shift_kill0.uge(ssa_103_shift_kill0.getBitWidth())) {
		ssa_105_gen_zero_shift = APInt(ssa_104_gen_zero.getBitWidth(), 0);
	} else {
		ssa_105_gen_zero_shift = ssa_104_gen_zero.lshr(ssa_103_shift_kill0);
	}
	APInt ssa_106_len_kill_prop(ssa_105_gen_zero_shift.getBitWidth(), ssa_105_gen_zero_shift.countr_one());
	APInt ssa_107_len_kill_prop_plus = ssa_106_len_kill_prop + ssa_60_one;
	APInt ssa_108_mask_kill_prop = ssa_10_zero;
	if (ssa_107_len_kill_prop_plus.ule(ssa_107_len_kill_prop_plus.getBitWidth()))
		ssa_108_mask_kill_prop.setLowBits(ssa_107_len_kill_prop_plus.getZExtValue());
	else
		ssa_108_mask_kill_prop.setLowBits(ssa_107_len_kill_prop_plus.getBitWidth());
	APInt ssa_109_carry0_kill_tmp;
	if (ssa_103_shift_kill0.uge(ssa_103_shift_kill0.getBitWidth())) {
		ssa_109_carry0_kill_tmp = APInt(ssa_108_mask_kill_prop.getBitWidth(), 0);
	} else {
		ssa_109_carry0_kill_tmp = ssa_108_mask_kill_prop.shl(ssa_103_shift_kill0);
	}
	APInt ssa_110_carry0_kill = ssa_101_has_kill ? ssa_109_carry0_kill_tmp : ssa_10_zero;
	APInt ssa_111_b1c0 = ssa_6_b1 & ssa_7_c0;
	APInt ssa_112_b0c1 = ssa_5_b0 & ssa_8_c1;
	APInt ssa_113_bxor_c1 = ssa_111_b1c0 | ssa_112_b0c1;
	APInt ssa_114_b1c1 = ssa_6_b1 & ssa_8_c1;
	APInt ssa_115_b0c0 = ssa_5_b0 & ssa_7_c0;
	APInt ssa_116_bxor_c0 = ssa_114_b1c1 | ssa_115_b0c0;
	APInt ssa_117_sum1_kill0 = ssa_110_carry0_kill & ssa_113_bxor_c1;
	APInt ssa_118_sum0_kill0 = ssa_110_carry0_kill & ssa_116_bxor_c0;
	APInt ssa_119_start0 = ssa_61_start & ssa_5_b0;
	bool ssa_120_has_start0 = ssa_119_start0.ne(ssa_10_zero);
	APInt ssa_121_ctz0(ssa_119_start0.getBitWidth(), ssa_119_start0.countr_zero());
	APInt ssa_122_low_mask = ssa_10_zero;
	if (ssa_121_ctz0.ule(ssa_121_ctz0.getBitWidth()))
		ssa_122_low_mask.setLowBits(ssa_121_ctz0.getZExtValue());
	else
		ssa_122_low_mask.setLowBits(ssa_121_ctz0.getBitWidth());
	APInt ssa_123_b0c1_low = ssa_112_b0c1 & ssa_122_low_mask;
	APInt ssa_124_shift_left = ssa_27_bw - ssa_121_ctz0;
	APInt ssa_125_b0c1_aligned;
	if (ssa_124_shift_left.uge(ssa_124_shift_left.getBitWidth())) {
		ssa_125_b0c1_aligned = APInt(ssa_123_b0c1_low.getBitWidth(), 0);
	} else {
		ssa_125_b0c1_aligned = ssa_123_b0c1_low.shl(ssa_124_shift_left);
	}
	APInt ssa_126_len_down(ssa_125_b0c1_aligned.getBitWidth(), ssa_125_b0c1_aligned.countl_one());
	APInt ssa_127_mask_down_len = ssa_10_zero;
	if (ssa_126_len_down.ule(ssa_126_len_down.getBitWidth()))
		ssa_127_mask_down_len.setLowBits(ssa_126_len_down.getZExtValue());
	else
		ssa_127_mask_down_len.setLowBits(ssa_126_len_down.getBitWidth());
	APInt ssa_128_ctz_minus_len = ssa_121_ctz0 - ssa_126_len_down;
	APInt ssa_129_carry_down_mask_tmp;
	if (ssa_128_ctz_minus_len.uge(ssa_128_ctz_minus_len.getBitWidth())) {
		ssa_129_carry_down_mask_tmp = APInt(ssa_127_mask_down_len.getBitWidth(), 0);
	} else {
		ssa_129_carry_down_mask_tmp = ssa_127_mask_down_len.shl(ssa_128_ctz_minus_len);
	}
	APInt ssa_130_carry_down_mask = ssa_120_has_start0 ? ssa_129_carry_down_mask_tmp : ssa_10_zero;
	APInt ssa_131_sum0_carry_down = ssa_78_sum0_carry | ssa_130_carry_down_mask;
	APInt ssa_132_sum0_carry_down2 = ssa_131_sum0_carry_down | ssa_95_sum0_carry_gen;
	APInt ssa_133_sum0_carry_down3 = ssa_132_sum0_carry_down2 | ssa_118_sum0_kill0;
	APInt ssa_134_sum1_carry2 = ssa_73_sum1_carry | ssa_90_sum1_carry_gen;
	APInt ssa_135_sum1_carry3 = ssa_134_sum1_carry2 | ssa_100_sum1_kill;
	APInt ssa_136_sum1_carry4 = ssa_135_sum1_carry3 | ssa_117_sum1_kill0;
	APInt ssa_137_k_b(ssa_11_bmask.getBitWidth(), ssa_11_bmask.countr_one());
	APInt ssa_138_k_c(ssa_12_cmask.getBitWidth(), ssa_12_cmask.countr_one());
	APInt ssa_139_k = APIntOps::umin(ssa_137_k_b, ssa_138_k_c);
	APInt ssa_140_maskLow_exact = ssa_10_zero;
	if (ssa_139_k.ule(ssa_139_k.getBitWidth()))
		ssa_140_maskLow_exact.setLowBits(ssa_139_k.getZExtValue());
	else
		ssa_140_maskLow_exact.setLowBits(ssa_139_k.getBitWidth());
	APInt ssa_141_b_low = ssa_6_b1 & ssa_140_maskLow_exact;
	APInt ssa_142_c_low = ssa_8_c1 & ssa_140_maskLow_exact;
	APInt ssa_143_sum_low = ssa_141_b_low + ssa_142_c_low;
	APInt ssa_144_sum_low_masked = ssa_143_sum_low & ssa_140_maskLow_exact;
	APInt ssa_145_sum_not_low = ssa_9_allones ^ ssa_144_sum_low_masked;
	APInt ssa_146_sum0_low = ssa_145_sum_not_low & ssa_140_maskLow_exact;
	APInt ssa_147_k_gen(ssa_104_gen_zero.getBitWidth(), ssa_104_gen_zero.countr_one());
	APInt ssa_148_mask_low_gen = ssa_10_zero;
	if (ssa_147_k_gen.ule(ssa_147_k_gen.getBitWidth()))
		ssa_148_mask_low_gen.setLowBits(ssa_147_k_gen.getZExtValue());
	else
		ssa_148_mask_low_gen.setLowBits(ssa_147_k_gen.getBitWidth());
	APInt ssa_149_bit_k_gen;
	if (ssa_147_k_gen.uge(ssa_147_k_gen.getBitWidth())) {
		ssa_149_bit_k_gen = APInt(ssa_60_one.getBitWidth(), 0);
	} else {
		ssa_149_bit_k_gen = ssa_60_one.shl(ssa_147_k_gen);
	}
	APInt ssa_150_mask_carry0 = ssa_148_mask_low_gen | ssa_149_bit_k_gen;
	APInt ssa_151_carry0_b0 = ssa_150_mask_carry0 & ssa_5_b0;
	APInt ssa_152_sum1_carry0 = ssa_151_carry0_b0 & ssa_8_c1;
	APInt ssa_153_sum0_carry0 = ssa_151_carry0_b0 & ssa_7_c0;
	APInt ssa_154_sum1_tmp1 = ssa_58_sum1_range_tmp | ssa_144_sum_low_masked;
	APInt ssa_155_sum1_tmp2 = ssa_154_sum1_tmp1 | ssa_136_sum1_carry4;
	APInt ssa_156_sum1 = ssa_155_sum1_tmp2 | ssa_152_sum1_carry0;
	APInt ssa_157_sum0_tmp1 = ssa_59_sum0_range_final | ssa_146_sum0_low;
	APInt ssa_158_sum0_tmp2 = ssa_157_sum0_tmp1 | ssa_133_sum0_carry_down3;
	APInt ssa_159_sum0 = ssa_158_sum0_tmp2 | ssa_153_sum0_carry0;
	APInt ssa_160_conflict_sum = ssa_156_sum1 & ssa_25_sum0_constraint;
	bool ssa_161_conflict_sum_zero = ssa_160_conflict_sum.eq(ssa_10_zero);
	APInt ssa_162_t0_extra = ssa_150_mask_carry0 & ssa_6_b1;
	APInt ssa_163_t1 = ssa_8_c1 & ssa_156_sum1;
	APInt ssa_164_t0_base = ssa_7_c0 | ssa_159_sum0;
	APInt ssa_165_t0_tmp1 = ssa_164_t0_base | ssa_162_t0_extra;
	APInt ssa_166_t0 = ssa_165_t0_tmp1 | ssa_4_a1;
	APInt ssa_167_res0 = ssa_3_a0 & ssa_166_t0;
	APInt ssa_168_res1 = ssa_4_a1 | ssa_163_t1;
	APInt ssa_169_conflict = ssa_4_a1 & ssa_163_t1;
	bool ssa_170_conflict_zero = ssa_169_conflict.eq(ssa_10_zero);
	bool ssa_171_ok = ssa_170_conflict_zero && ssa_161_conflict_sum_zero;
	APInt ssa_172_res0_sel = ssa_171_ok ? ssa_167_res0 : ssa_9_allones;
	APInt ssa_173_res1_sel = ssa_171_ok ? ssa_168_res1 : ssa_9_allones;
	std::array<APInt, 2> ssa_174_r = std::array<APInt, 2>{ssa_172_res0_sel, ssa_173_res1_sel};
	return ssa_174_r;
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
