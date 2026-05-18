namespace Pattern499 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_11_sign_mask = APInt::getSignedMinValue(ssa_3_a0z.getBitWidth());
	APInt ssa_12_not_sign_mask = ssa_11_sign_mask ^ ssa_10_allones;
	APInt ssa_13_mask0 = ssa_3_a0z & ssa_4_a0o;
	APInt ssa_14_mask1 = ssa_5_a1z & ssa_6_a1o;
	APInt ssa_15_mask2 = ssa_7_a2z & ssa_8_a2o;
	bool ssa_16_is_bottom0 = ssa_13_mask0.ne(ssa_9_zero);
	bool ssa_17_is_bottom1 = ssa_14_mask1.ne(ssa_9_zero);
	bool ssa_18_is_bottom2 = ssa_15_mask2.ne(ssa_9_zero);
	bool ssa_19_bottom01 = ssa_16_is_bottom0 || ssa_17_is_bottom1;
	bool ssa_20_bottom_inputs = ssa_19_bottom01 || ssa_18_is_bottom2;
	APInt ssa_21_known_mask0 = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_22_known_mask1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_23_known_mask2 = ssa_7_a2z | ssa_8_a2o;
	bool ssa_24_is_known0 = ssa_21_known_mask0.eq(ssa_10_allones);
	bool ssa_25_is_known1 = ssa_22_known_mask1.eq(ssa_10_allones);
	bool ssa_26_is_known2 = ssa_23_known_mask2.eq(ssa_10_allones);
	bool ssa_27_known01 = ssa_24_is_known0 && ssa_25_is_known1;
	bool ssa_28_all_known = ssa_27_known01 && ssa_26_is_known2;
	APInt ssa_29_bw(ssa_3_a0z.getBitWidth(), ssa_3_a0z.getBitWidth());
	bool ssa_30_shift_ge_bw = ssa_4_a0o.uge(ssa_29_bw);
	APInt ssa_31_sum = ssa_6_a1o + ssa_8_a2o;
	bool ssa_32_ov;
	ssa_6_a1o.sadd_ov(ssa_8_a2o, ssa_32_ov);
	bool ssa_33_true = -1;
	bool ssa_34_noov = ssa_32_ov != ssa_33_true;
	APInt ssa_35_ashr;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_31_sum.isSignBitSet()) {
		ssa_35_ashr = APInt(ssa_31_sum.getBitWidth(), -1);
	} else if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_31_sum.isSignBitClear()) {
		ssa_35_ashr = APInt(ssa_31_sum.getBitWidth(), 0);
	} else {
		ssa_35_ashr = ssa_31_sum.ashr(ssa_4_a0o);
	}
	APInt ssa_36_shl;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_36_shl = APInt(ssa_35_ashr.getBitWidth(), 0);
	} else {
		ssa_36_shl = ssa_35_ashr.shl(ssa_4_a0o);
	}
	bool ssa_37_exact = ssa_36_shl.eq(ssa_31_sum);
	bool ssa_38_shift_lt_bw = ssa_4_a0o.ult(ssa_29_bw);
	bool ssa_39_cond0 = ssa_34_noov && ssa_37_exact;
	bool ssa_40_constraints = ssa_39_cond0 && ssa_38_shift_lt_bw;
	bool ssa_41_cond_exact = ssa_28_all_known && ssa_40_constraints;
	bool ssa_42_not_constraints = ssa_40_constraints != ssa_33_true;
	bool ssa_43_known_bad = ssa_28_all_known && ssa_42_not_constraints;
	APInt ssa_44_a1_sign_zero_mask = ssa_5_a1z & ssa_11_sign_mask;
	APInt ssa_45_a2_sign_zero_mask = ssa_7_a2z & ssa_11_sign_mask;
	bool ssa_46_a1_sign_zero = ssa_44_a1_sign_zero_mask.ne(ssa_9_zero);
	bool ssa_47_a2_sign_zero = ssa_45_a2_sign_zero_mask.ne(ssa_9_zero);
	APInt ssa_48_unknown1 = ssa_22_known_mask1 ^ ssa_10_allones;
	APInt ssa_49_unknown2 = ssa_23_known_mask2 ^ ssa_10_allones;
	APInt ssa_50_a1_min_neg = ssa_6_a1o | ssa_11_sign_mask;
	APInt ssa_51_a2_min_neg = ssa_8_a2o | ssa_11_sign_mask;
	APInt ssa_52_a1_min = ssa_46_a1_sign_zero ? ssa_6_a1o : ssa_50_a1_min_neg;
	APInt ssa_53_a2_min = ssa_47_a2_sign_zero ? ssa_8_a2o : ssa_51_a2_min_neg;
	APInt ssa_54_a1_tmp_max = ssa_6_a1o | ssa_48_unknown1;
	APInt ssa_55_a2_tmp_max = ssa_8_a2o | ssa_49_unknown2;
	APInt ssa_56_a1_max_pos = ssa_54_a1_tmp_max & ssa_12_not_sign_mask;
	APInt ssa_57_a2_max_pos = ssa_55_a2_tmp_max & ssa_12_not_sign_mask;
	APInt ssa_58_a1_sign_one_mask = ssa_6_a1o & ssa_11_sign_mask;
	APInt ssa_59_a2_sign_one_mask = ssa_8_a2o & ssa_11_sign_mask;
	bool ssa_60_a1_sign_one = ssa_58_a1_sign_one_mask.ne(ssa_9_zero);
	bool ssa_61_a2_sign_one = ssa_59_a2_sign_one_mask.ne(ssa_9_zero);
	APInt ssa_62_a1_max = ssa_60_a1_sign_one ? ssa_54_a1_tmp_max : ssa_56_a1_max_pos;
	APInt ssa_63_a2_max = ssa_61_a2_sign_one ? ssa_55_a2_tmp_max : ssa_57_a2_max_pos;
	APInt ssa_64_min_sum = ssa_52_a1_min + ssa_53_a2_min;
	APInt ssa_65_max_sum = ssa_62_a1_max + ssa_63_a2_max;
	bool ssa_66_ov_min;
	ssa_52_a1_min.sadd_ov(ssa_53_a2_min, ssa_66_ov_min);
	bool ssa_67_ov_max;
	ssa_62_a1_max.sadd_ov(ssa_63_a2_max, ssa_67_ov_max);
	bool ssa_68_no_ov_min = ssa_66_ov_min != ssa_33_true;
	bool ssa_69_no_ov_max = ssa_67_ov_max != ssa_33_true;
	bool ssa_70_range_safe = ssa_68_no_ov_min && ssa_69_no_ov_max;
	APInt ssa_71_mask = ssa_9_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_71_mask.setLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_71_mask.setLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_72_not_mask = ssa_71_mask ^ ssa_10_allones;
	APInt ssa_73_min_plus_mask = ssa_64_min_sum + ssa_71_mask;
	APInt ssa_74_rounded = ssa_73_min_plus_mask & ssa_72_not_mask;
	bool ssa_75_exists = ssa_74_rounded.sle(ssa_65_max_sum);
	bool ssa_76_not_exists = ssa_75_exists != ssa_33_true;
	bool ssa_77_no_multiple0 = ssa_70_range_safe && ssa_24_is_known0;
	bool ssa_78_no_multiple = ssa_77_no_multiple0 && ssa_76_not_exists;
	APInt ssa_79_a1_low_one = ssa_6_a1o & ssa_71_mask;
	APInt ssa_80_a2_low_one = ssa_8_a2o & ssa_71_mask;
	APInt ssa_81_a1_low_zero = ssa_5_a1z & ssa_71_mask;
	APInt ssa_82_a2_low_zero = ssa_7_a2z & ssa_71_mask;
	APInt ssa_83_a1_low_known = ssa_79_a1_low_one | ssa_81_a1_low_zero;
	APInt ssa_84_a2_low_known = ssa_80_a2_low_one | ssa_82_a2_low_zero;
	APInt ssa_85_a1_low_unknown = ssa_83_a1_low_known ^ ssa_71_mask;
	APInt ssa_86_a2_low_unknown = ssa_84_a2_low_known ^ ssa_71_mask;
	APInt ssa_87_a1_low_min = ssa_79_a1_low_one | ssa_9_zero;
	APInt ssa_88_a2_low_min = ssa_80_a2_low_one | ssa_9_zero;
	APInt ssa_89_a1_low_max = ssa_79_a1_low_one | ssa_85_a1_low_unknown;
	APInt ssa_90_a2_low_max = ssa_80_a2_low_one | ssa_86_a2_low_unknown;
	APInt ssa_91_sum_low_min = ssa_87_a1_low_min + ssa_88_a2_low_min;
	APInt ssa_92_sum_low_max = ssa_89_a1_low_max + ssa_90_a2_low_max;
	APInt ssa_93_sum_low_plus_mask = ssa_91_sum_low_min + ssa_71_mask;
	APInt ssa_94_rounded_low = ssa_93_sum_low_plus_mask & ssa_72_not_mask;
	bool ssa_95_exists_low = ssa_94_rounded_low.ule(ssa_92_sum_low_max);
	bool ssa_96_not_exists_low = ssa_95_exists_low != ssa_33_true;
	bool ssa_97_no_multiple_low = ssa_24_is_known0 && ssa_96_not_exists_low;
	bool ssa_98_bottom_or = ssa_20_bottom_inputs || ssa_30_shift_ge_bw;
	bool ssa_99_bottom_or2 = ssa_98_bottom_or || ssa_43_known_bad;
	bool ssa_100_no_multiple_any = ssa_78_no_multiple || ssa_97_no_multiple_low;
	bool ssa_101_bottom_cond = ssa_99_bottom_or2 || ssa_100_no_multiple_any;
	APInt ssa_102_allones_res = APInt::getAllOnes(ssa_35_ashr.getBitWidth());
	APInt ssa_103_res_zero = ssa_35_ashr ^ ssa_102_allones_res;
	APInt ssa_104_res_zero_sel = ssa_41_cond_exact ? ssa_103_res_zero : ssa_9_zero;
	APInt ssa_105_res_one_sel = ssa_41_cond_exact ? ssa_35_ashr : ssa_9_zero;
	APInt ssa_106_res_zero_final = ssa_101_bottom_cond ? ssa_10_allones : ssa_104_res_zero_sel;
	APInt ssa_107_res_one_final = ssa_101_bottom_cond ? ssa_10_allones : ssa_105_res_one_sel;
	std::array<APInt, 2> ssa_108_r = std::array<APInt, 2>{ssa_106_res_zero_final, ssa_107_res_one_final};
	return ssa_108_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_11_two(ssa_3_a0z.getBitWidth(), 2);
	APInt ssa_12_three(ssa_3_a0z.getBitWidth(), 3);
	APInt ssa_13_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_14_known_mask0 = ssa_3_a0z | ssa_4_a0o;
	bool ssa_15_is_known0 = ssa_14_known_mask0.eq(ssa_13_allones);
	APInt ssa_16_mask = ssa_9_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_16_mask.setLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_16_mask.setLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_17_a1_low_one = ssa_6_a1o & ssa_16_mask;
	APInt ssa_18_a2_low_one = ssa_8_a2o & ssa_16_mask;
	APInt ssa_19_a1_low_zero = ssa_5_a1z & ssa_16_mask;
	APInt ssa_20_a2_low_zero = ssa_7_a2z & ssa_16_mask;
	APInt ssa_21_a1_low_known = ssa_17_a1_low_one | ssa_19_a1_low_zero;
	APInt ssa_22_a2_low_known = ssa_18_a2_low_one | ssa_20_a2_low_zero;
	APInt ssa_23_a1_low_unknown = ssa_21_a1_low_known ^ ssa_16_mask;
	APInt ssa_24_a2_low_unknown = ssa_22_a2_low_known ^ ssa_16_mask;
	bool ssa_25_a1_low_all = ssa_23_a1_low_unknown.eq(ssa_9_zero);
	bool ssa_26_a2_low_all = ssa_24_a2_low_unknown.eq(ssa_9_zero);
	bool ssa_27_low_all = ssa_25_a1_low_all && ssa_26_a2_low_all;
	bool ssa_28_low_all_known_shift = ssa_27_low_all && ssa_15_is_known0;
	APInt ssa_29_sum_low_val = ssa_17_a1_low_one + ssa_18_a2_low_one;
	APInt ssa_30_sum_low_bits = ssa_29_sum_low_val & ssa_16_mask;
	bool ssa_31_sum_low_zero = ssa_30_sum_low_bits.eq(ssa_9_zero);
	bool ssa_32_true = -1;
	bool ssa_33_sum_low_nonzero = ssa_31_sum_low_zero != ssa_32_true;
	bool ssa_34_bottom_low_all = ssa_28_low_all_known_shift && ssa_33_sum_low_nonzero;
	APInt ssa_35_a1_lsb_one = ssa_6_a1o & ssa_10_one;
	APInt ssa_36_a2_lsb_one = ssa_8_a2o & ssa_10_one;
	APInt ssa_37_a1_lsb_zero = ssa_5_a1z & ssa_10_one;
	APInt ssa_38_a2_lsb_zero = ssa_7_a2z & ssa_10_one;
	APInt ssa_39_a1_lsb_known = ssa_35_a1_lsb_one | ssa_37_a1_lsb_zero;
	APInt ssa_40_a2_lsb_known = ssa_36_a2_lsb_one | ssa_38_a2_lsb_zero;
	bool ssa_41_a1_lsb_is_known = ssa_39_a1_lsb_known.eq(ssa_10_one);
	bool ssa_42_a2_lsb_is_known = ssa_40_a2_lsb_known.eq(ssa_10_one);
	bool ssa_43_lsb_known = ssa_41_a1_lsb_is_known && ssa_42_a2_lsb_is_known;
	bool ssa_44_shift_min_ge1 = ssa_4_a0o.ne(ssa_9_zero);
	APInt ssa_45_sum_lsb = ssa_35_a1_lsb_one ^ ssa_36_a2_lsb_one;
	bool ssa_46_sum_lsb_zero = ssa_45_sum_lsb.eq(ssa_9_zero);
	bool ssa_47_sum_lsb_one = ssa_46_sum_lsb_zero != ssa_32_true;
	bool ssa_48_lsb_and_shift = ssa_43_lsb_known && ssa_44_shift_min_ge1;
	bool ssa_49_bottom_lsb = ssa_48_lsb_and_shift && ssa_47_sum_lsb_one;
	APInt ssa_50_mask2 = ssa_9_zero;
	if (ssa_11_two.ule(ssa_11_two.getBitWidth()))
		ssa_50_mask2.setLowBits(ssa_11_two.getZExtValue());
	else
		ssa_50_mask2.setLowBits(ssa_11_two.getBitWidth());
	APInt ssa_51_a1_low2_one = ssa_6_a1o & ssa_50_mask2;
	APInt ssa_52_a2_low2_one = ssa_8_a2o & ssa_50_mask2;
	APInt ssa_53_a1_low2_zero = ssa_5_a1z & ssa_50_mask2;
	APInt ssa_54_a2_low2_zero = ssa_7_a2z & ssa_50_mask2;
	APInt ssa_55_a1_low2_known = ssa_51_a1_low2_one | ssa_53_a1_low2_zero;
	APInt ssa_56_a2_low2_known = ssa_52_a2_low2_one | ssa_54_a2_low2_zero;
	APInt ssa_57_a1_low2_unknown = ssa_55_a1_low2_known ^ ssa_50_mask2;
	APInt ssa_58_a2_low2_unknown = ssa_56_a2_low2_known ^ ssa_50_mask2;
	bool ssa_59_a1_low2_all = ssa_57_a1_low2_unknown.eq(ssa_9_zero);
	bool ssa_60_a2_low2_all = ssa_58_a2_low2_unknown.eq(ssa_9_zero);
	bool ssa_61_low2_all = ssa_59_a1_low2_all && ssa_60_a2_low2_all;
	APInt ssa_62_sum_low2_val = ssa_51_a1_low2_one + ssa_52_a2_low2_one;
	APInt ssa_63_sum_low2_bits = ssa_62_sum_low2_val & ssa_50_mask2;
	bool ssa_64_sum_low2_zero = ssa_63_sum_low2_bits.eq(ssa_9_zero);
	bool ssa_65_sum_low2_nonzero = ssa_64_sum_low2_zero != ssa_32_true;
	bool ssa_66_shift_min_ge2 = ssa_4_a0o.uge(ssa_11_two);
	bool ssa_67_low2_shift = ssa_61_low2_all && ssa_66_shift_min_ge2;
	bool ssa_68_bottom_low2 = ssa_67_low2_shift && ssa_65_sum_low2_nonzero;
	APInt ssa_69_mask3 = ssa_9_zero;
	if (ssa_12_three.ule(ssa_12_three.getBitWidth()))
		ssa_69_mask3.setLowBits(ssa_12_three.getZExtValue());
	else
		ssa_69_mask3.setLowBits(ssa_12_three.getBitWidth());
	APInt ssa_70_a1_low3_one = ssa_6_a1o & ssa_69_mask3;
	APInt ssa_71_a2_low3_one = ssa_8_a2o & ssa_69_mask3;
	APInt ssa_72_a1_low3_zero = ssa_5_a1z & ssa_69_mask3;
	APInt ssa_73_a2_low3_zero = ssa_7_a2z & ssa_69_mask3;
	APInt ssa_74_a1_low3_known = ssa_70_a1_low3_one | ssa_72_a1_low3_zero;
	APInt ssa_75_a2_low3_known = ssa_71_a2_low3_one | ssa_73_a2_low3_zero;
	APInt ssa_76_a1_low3_unknown = ssa_74_a1_low3_known ^ ssa_69_mask3;
	APInt ssa_77_a2_low3_unknown = ssa_75_a2_low3_known ^ ssa_69_mask3;
	bool ssa_78_a1_low3_all = ssa_76_a1_low3_unknown.eq(ssa_9_zero);
	bool ssa_79_a2_low3_all = ssa_77_a2_low3_unknown.eq(ssa_9_zero);
	bool ssa_80_low3_all = ssa_78_a1_low3_all && ssa_79_a2_low3_all;
	APInt ssa_81_sum_low3_val = ssa_70_a1_low3_one + ssa_71_a2_low3_one;
	APInt ssa_82_sum_low3_bits = ssa_81_sum_low3_val & ssa_69_mask3;
	bool ssa_83_sum_low3_zero = ssa_82_sum_low3_bits.eq(ssa_9_zero);
	bool ssa_84_sum_low3_nonzero = ssa_83_sum_low3_zero != ssa_32_true;
	bool ssa_85_shift_min_ge3 = ssa_4_a0o.uge(ssa_12_three);
	bool ssa_86_low3_shift = ssa_80_low3_all && ssa_85_shift_min_ge3;
	bool ssa_87_bottom_low3 = ssa_86_low3_shift && ssa_84_sum_low3_nonzero;
	bool ssa_88_bottom_or0 = ssa_34_bottom_low_all || ssa_49_bottom_lsb;
	bool ssa_89_bottom_or1 = ssa_88_bottom_or0 || ssa_68_bottom_low2;
	bool ssa_90_bottom_cond = ssa_89_bottom_or1 || ssa_87_bottom_low3;
	APInt ssa_91_res_zero = ssa_90_bottom_cond ? ssa_13_allones : ssa_9_zero;
	APInt ssa_92_res_one = ssa_90_bottom_cond ? ssa_13_allones : ssa_9_zero;
	std::array<APInt, 2> ssa_93_r = std::array<APInt, 2>{ssa_91_res_zero, ssa_92_res_one};
	return ssa_93_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_12_sign_mask = APInt::getSignedMinValue(ssa_3_a0z.getBitWidth());
	APInt ssa_13_a1_sign_zero_mask = ssa_5_a1z & ssa_12_sign_mask;
	APInt ssa_14_a2_sign_zero_mask = ssa_7_a2z & ssa_12_sign_mask;
	APInt ssa_15_a1_sign_one_mask = ssa_6_a1o & ssa_12_sign_mask;
	APInt ssa_16_a2_sign_one_mask = ssa_8_a2o & ssa_12_sign_mask;
	bool ssa_17_a1_sign_zero = ssa_13_a1_sign_zero_mask.ne(ssa_9_zero);
	bool ssa_18_a2_sign_zero = ssa_14_a2_sign_zero_mask.ne(ssa_9_zero);
	bool ssa_19_a1_sign_one = ssa_15_a1_sign_one_mask.ne(ssa_9_zero);
	bool ssa_20_a2_sign_one = ssa_16_a2_sign_one_mask.ne(ssa_9_zero);
	bool ssa_21_a1_z_all = ssa_5_a1z.eq(ssa_11_allones);
	bool ssa_22_a1_o_zero = ssa_6_a1o.eq(ssa_9_zero);
	bool ssa_23_a2_z_all = ssa_7_a2z.eq(ssa_11_allones);
	bool ssa_24_a2_o_zero = ssa_8_a2o.eq(ssa_9_zero);
	bool ssa_25_a1_is_zero = ssa_21_a1_z_all && ssa_22_a1_o_zero;
	bool ssa_26_a2_is_zero = ssa_23_a2_z_all && ssa_24_a2_o_zero;
	bool ssa_27_both_sign_zero = ssa_17_a1_sign_zero && ssa_18_a2_sign_zero;
	bool ssa_28_both_sign_one = ssa_19_a1_sign_one && ssa_20_a2_sign_one;
	bool ssa_29_a1_one_a2_zero = ssa_19_a1_sign_one && ssa_26_a2_is_zero;
	bool ssa_30_a2_one_a1_zero = ssa_20_a2_sign_one && ssa_25_a1_is_zero;
	bool ssa_31_one_zero_or = ssa_29_a1_one_a2_zero || ssa_30_a2_one_a1_zero;
	bool ssa_32_sum_sign_one = ssa_28_both_sign_one || ssa_31_one_zero_or;
	bool ssa_33_false = 0;
	bool ssa_34_sum_sign_zero = ssa_27_both_sign_zero || ssa_33_false;
	APInt ssa_35_known_mask0 = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_36_known_mask1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_37_known_mask2 = ssa_7_a2z | ssa_8_a2o;
	bool ssa_38_is_known0 = ssa_35_known_mask0.eq(ssa_11_allones);
	APInt ssa_39_unknown1 = ssa_36_known_mask1 ^ ssa_11_allones;
	APInt ssa_40_unknown2 = ssa_37_known_mask2 ^ ssa_11_allones;
	APInt ssa_41_max1 = ssa_6_a1o | ssa_39_unknown1;
	APInt ssa_42_max2 = ssa_8_a2o | ssa_40_unknown2;
	APInt ssa_43_sum_max = ssa_41_max1 + ssa_42_max2;
	bool ssa_44_ov_max;
	ssa_41_max1.sadd_ov(ssa_42_max2, ssa_44_ov_max);
	bool ssa_45_true = -1;
	bool ssa_46_no_ov_max = ssa_44_ov_max != ssa_45_true;
	APInt ssa_47_mask_low = ssa_9_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_47_mask_low.setLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_47_mask_low.setLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_48_not_mask_low = ssa_47_mask_low ^ ssa_11_allones;
	APInt ssa_49_sum_max_high = ssa_43_sum_max & ssa_48_not_mask_low;
	bool ssa_50_sum_max_high_zero = ssa_49_sum_max_high.eq(ssa_9_zero);
	bool ssa_51_cond_zero0 = ssa_38_is_known0 && ssa_27_both_sign_zero;
	bool ssa_52_cond_zero1 = ssa_51_cond_zero0 && ssa_46_no_ov_max;
	bool ssa_53_cond_zero = ssa_52_cond_zero1 && ssa_50_sum_max_high_zero;
	APInt ssa_54_top_mask = ssa_9_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_54_top_mask.setHighBits(ssa_4_a0o.getZExtValue());
	else
		ssa_54_top_mask.setHighBits(ssa_4_a0o.getBitWidth());
	APInt ssa_55_top_or_sign = ssa_54_top_mask | ssa_12_sign_mask;
	APInt ssa_56_mask = ssa_38_is_known0 ? ssa_55_top_or_sign : ssa_12_sign_mask;
	APInt ssa_57_res_zero_sign = ssa_34_sum_sign_zero ? ssa_56_mask : ssa_9_zero;
	APInt ssa_58_res_one_sign = ssa_32_sum_sign_one ? ssa_56_mask : ssa_9_zero;
	APInt ssa_59_res_zero = ssa_53_cond_zero ? ssa_11_allones : ssa_57_res_zero_sign;
	APInt ssa_60_res_one = ssa_53_cond_zero ? ssa_9_zero : ssa_58_res_one_sign;
	std::array<APInt, 2> ssa_61_r = std::array<APInt, 2>{ssa_59_res_zero, ssa_60_res_one};
	return ssa_61_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_12_sign_mask = APInt::getSignedMinValue(ssa_3_a0z.getBitWidth());
	APInt ssa_13_not_sign_mask = ssa_12_sign_mask ^ ssa_11_allones;
	APInt ssa_14_mask0 = ssa_3_a0z & ssa_4_a0o;
	APInt ssa_15_mask1 = ssa_5_a1z & ssa_6_a1o;
	APInt ssa_16_mask2 = ssa_7_a2z & ssa_8_a2o;
	bool ssa_17_is_bottom0 = ssa_14_mask0.ne(ssa_9_zero);
	bool ssa_18_is_bottom1 = ssa_15_mask1.ne(ssa_9_zero);
	bool ssa_19_is_bottom2 = ssa_16_mask2.ne(ssa_9_zero);
	bool ssa_20_bottom01 = ssa_17_is_bottom0 || ssa_18_is_bottom1;
	bool ssa_21_bottom_inputs = ssa_20_bottom01 || ssa_19_is_bottom2;
	APInt ssa_22_known_mask0 = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_23_known_mask1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_24_known_mask2 = ssa_7_a2z | ssa_8_a2o;
	bool ssa_25_is_known0 = ssa_22_known_mask0.eq(ssa_11_allones);
	APInt ssa_26_bw(ssa_3_a0z.getBitWidth(), ssa_3_a0z.getBitWidth());
	bool ssa_27_shift_ge_bw = ssa_4_a0o.uge(ssa_26_bw);
	bool ssa_28_known_shift_ge_bw = ssa_25_is_known0 && ssa_27_shift_ge_bw;
	bool ssa_29_bottom_cond0 = ssa_21_bottom_inputs || ssa_28_known_shift_ge_bw;
	bool ssa_30_a1_z_all = ssa_5_a1z.eq(ssa_11_allones);
	bool ssa_31_a1_o_zero = ssa_6_a1o.eq(ssa_9_zero);
	bool ssa_32_a1_is_zero = ssa_30_a1_z_all && ssa_31_a1_o_zero;
	bool ssa_33_a2_z_all = ssa_7_a2z.eq(ssa_11_allones);
	bool ssa_34_a2_o_zero = ssa_8_a2o.eq(ssa_9_zero);
	bool ssa_35_a2_is_zero = ssa_33_a2_z_all && ssa_34_a2_o_zero;
	bool ssa_36_cond1 = ssa_25_is_known0 && ssa_32_a1_is_zero;
	bool ssa_37_cond2 = ssa_25_is_known0 && ssa_35_a2_is_zero;
	APInt ssa_38_high_mask = ssa_9_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_38_high_mask.setHighBits(ssa_4_a0o.getZExtValue());
	else
		ssa_38_high_mask.setHighBits(ssa_4_a0o.getBitWidth());
	APInt ssa_39_low_mask = ssa_38_high_mask ^ ssa_11_allones;
	APInt ssa_40_a2_sign_zero_mask = ssa_7_a2z & ssa_12_sign_mask;
	APInt ssa_41_a2_sign_one_mask = ssa_8_a2o & ssa_12_sign_mask;
	bool ssa_42_a2_sign_zero = ssa_40_a2_sign_zero_mask.ne(ssa_9_zero);
	bool ssa_43_a2_sign_one = ssa_41_a2_sign_one_mask.ne(ssa_9_zero);
	APInt ssa_44_a2z_shift;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_44_a2z_shift = APInt(ssa_7_a2z.getBitWidth(), 0);
	} else {
		ssa_44_a2z_shift = ssa_7_a2z.lshr(ssa_4_a0o);
	}
	APInt ssa_45_a2o_shift;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_45_a2o_shift = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_45_a2o_shift = ssa_8_a2o.lshr(ssa_4_a0o);
	}
	APInt ssa_46_a2z_low = ssa_44_a2z_shift & ssa_39_low_mask;
	APInt ssa_47_a2o_low = ssa_45_a2o_shift & ssa_39_low_mask;
	APInt ssa_48_a2z_high = ssa_42_a2_sign_zero ? ssa_38_high_mask : ssa_9_zero;
	APInt ssa_49_a2o_high = ssa_43_a2_sign_one ? ssa_38_high_mask : ssa_9_zero;
	APInt ssa_50_a2_res_zero = ssa_46_a2z_low | ssa_48_a2z_high;
	APInt ssa_51_a2_res_one = ssa_47_a2o_low | ssa_49_a2o_high;
	APInt ssa_52_a1_sign_zero_mask = ssa_5_a1z & ssa_12_sign_mask;
	APInt ssa_53_a1_sign_one_mask = ssa_6_a1o & ssa_12_sign_mask;
	bool ssa_54_a1_sign_zero = ssa_52_a1_sign_zero_mask.ne(ssa_9_zero);
	bool ssa_55_a1_sign_one = ssa_53_a1_sign_one_mask.ne(ssa_9_zero);
	APInt ssa_56_a1z_shift;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_56_a1z_shift = APInt(ssa_5_a1z.getBitWidth(), 0);
	} else {
		ssa_56_a1z_shift = ssa_5_a1z.lshr(ssa_4_a0o);
	}
	APInt ssa_57_a1o_shift;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_57_a1o_shift = APInt(ssa_6_a1o.getBitWidth(), 0);
	} else {
		ssa_57_a1o_shift = ssa_6_a1o.lshr(ssa_4_a0o);
	}
	APInt ssa_58_a1z_low = ssa_56_a1z_shift & ssa_39_low_mask;
	APInt ssa_59_a1o_low = ssa_57_a1o_shift & ssa_39_low_mask;
	APInt ssa_60_a1z_high = ssa_54_a1_sign_zero ? ssa_38_high_mask : ssa_9_zero;
	APInt ssa_61_a1o_high = ssa_55_a1_sign_one ? ssa_38_high_mask : ssa_9_zero;
	APInt ssa_62_a1_res_zero = ssa_58_a1z_low | ssa_60_a1z_high;
	APInt ssa_63_a1_res_one = ssa_59_a1o_low | ssa_61_a1o_high;
	APInt ssa_64_res_zero_sel = ssa_36_cond1 ? ssa_50_a2_res_zero : ssa_9_zero;
	APInt ssa_65_res_one_sel = ssa_36_cond1 ? ssa_51_a2_res_one : ssa_9_zero;
	APInt ssa_66_res_zero_sel2 = ssa_37_cond2 ? ssa_62_a1_res_zero : ssa_64_res_zero_sel;
	APInt ssa_67_res_one_sel2 = ssa_37_cond2 ? ssa_63_a1_res_one : ssa_65_res_one_sel;
	APInt ssa_68_unknown1 = ssa_23_known_mask1 ^ ssa_11_allones;
	APInt ssa_69_unknown2 = ssa_24_known_mask2 ^ ssa_11_allones;
	APInt ssa_70_a1_min_neg = ssa_6_a1o | ssa_12_sign_mask;
	APInt ssa_71_a2_min_neg = ssa_8_a2o | ssa_12_sign_mask;
	APInt ssa_72_a1_min = ssa_54_a1_sign_zero ? ssa_6_a1o : ssa_70_a1_min_neg;
	APInt ssa_73_a2_min = ssa_42_a2_sign_zero ? ssa_8_a2o : ssa_71_a2_min_neg;
	APInt ssa_74_a1_tmp_max = ssa_6_a1o | ssa_68_unknown1;
	APInt ssa_75_a2_tmp_max = ssa_8_a2o | ssa_69_unknown2;
	APInt ssa_76_a1_max_pos = ssa_74_a1_tmp_max & ssa_13_not_sign_mask;
	APInt ssa_77_a2_max_pos = ssa_75_a2_tmp_max & ssa_13_not_sign_mask;
	APInt ssa_78_a1_max = ssa_55_a1_sign_one ? ssa_74_a1_tmp_max : ssa_76_a1_max_pos;
	APInt ssa_79_a2_max = ssa_43_a2_sign_one ? ssa_75_a2_tmp_max : ssa_77_a2_max_pos;
	APInt ssa_80_min_sum = ssa_72_a1_min + ssa_73_a2_min;
	APInt ssa_81_max_sum = ssa_78_a1_max + ssa_79_a2_max;
	bool ssa_82_ov_min;
	ssa_72_a1_min.sadd_ov(ssa_73_a2_min, ssa_82_ov_min);
	bool ssa_83_ov_max;
	ssa_78_a1_max.sadd_ov(ssa_79_a2_max, ssa_83_ov_max);
	bool ssa_84_true = -1;
	bool ssa_85_no_ov_min = ssa_82_ov_min != ssa_84_true;
	bool ssa_86_no_ov_max = ssa_83_ov_max != ssa_84_true;
	bool ssa_87_range_safe = ssa_85_no_ov_min && ssa_86_no_ov_max;
	APInt ssa_88_pow;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_88_pow = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_88_pow = ssa_10_one.shl(ssa_4_a0o);
	}
	bool ssa_89_max_lt_pow = ssa_81_max_sum.slt(ssa_88_pow);
	bool ssa_90_min_neg = ssa_80_min_sum.isNegative();
	bool ssa_91_min_nonneg = ssa_90_min_neg != ssa_84_true;
	bool ssa_92_range0 = ssa_89_max_lt_pow && ssa_91_min_nonneg;
	bool ssa_93_range1 = ssa_92_range0 && ssa_87_range_safe;
	bool ssa_94_cond_zero_range = ssa_93_range1 && ssa_25_is_known0;
	APInt ssa_95_res_zero_sel3 = ssa_94_cond_zero_range ? ssa_11_allones : ssa_66_res_zero_sel2;
	APInt ssa_96_res_one_sel3 = ssa_94_cond_zero_range ? ssa_9_zero : ssa_67_res_one_sel2;
	APInt ssa_97_res_zero_final = ssa_29_bottom_cond0 ? ssa_11_allones : ssa_95_res_zero_sel3;
	APInt ssa_98_res_one_final = ssa_29_bottom_cond0 ? ssa_11_allones : ssa_96_res_one_sel3;
	std::array<APInt, 2> ssa_99_r = std::array<APInt, 2>{ssa_97_res_zero_final, ssa_98_res_one_final};
	return ssa_99_r;
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
