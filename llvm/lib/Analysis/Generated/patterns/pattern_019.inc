namespace Pattern019 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_12_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_13_bw_minus1 = ssa_12_bw - ssa_10_const1;
	APInt ssa_14_signmask;
	if (ssa_13_bw_minus1.uge(ssa_13_bw_minus1.getBitWidth())) {
		ssa_14_signmask = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_14_signmask = ssa_10_const1.shl(ssa_13_bw_minus1);
	}
	bool ssa_15_true = ssa_9_const0.eq(ssa_9_const0);
	APInt ssa_16_known0 = ssa_3_a0 | ssa_4_a1;
	APInt ssa_17_known1 = ssa_5_b0 | ssa_6_b1;
	APInt ssa_18_known2 = ssa_7_c0 | ssa_8_c1;
	bool ssa_19_allknown0 = ssa_16_known0.eq(ssa_11_allones);
	bool ssa_20_allknown1 = ssa_17_known1.eq(ssa_11_allones);
	bool ssa_21_allknown2 = ssa_18_known2.eq(ssa_11_allones);
	bool ssa_22_allknown01 = ssa_19_allknown0 && ssa_20_allknown1;
	bool ssa_23_allknown = ssa_22_allknown01 && ssa_21_allknown2;
	APInt ssa_24_t0 = ssa_6_b1 + ssa_8_c1;
	APInt ssa_25_sum_min = ssa_4_a1 + ssa_24_t0;
	APInt ssa_26_exact_kz = ssa_11_allones ^ ssa_25_sum_min;
	APInt ssa_27_max0 = ssa_11_allones ^ ssa_3_a0;
	APInt ssa_28_max1 = ssa_11_allones ^ ssa_5_b0;
	APInt ssa_29_max2 = ssa_11_allones ^ ssa_7_c0;
	APInt ssa_30_tmax = ssa_28_max1 + ssa_29_max2;
	APInt ssa_31_sum_max = ssa_27_max0 + ssa_30_tmax;
	APInt ssa_32_diff = ssa_25_sum_min ^ ssa_31_sum_max;
	APInt ssa_33_clz(ssa_32_diff.getBitWidth(), ssa_32_diff.countl_zero());
	APInt ssa_34_shift_amt = ssa_12_bw - ssa_33_clz;
	APInt ssa_35_mask;
	if (ssa_34_shift_amt.uge(ssa_34_shift_amt.getBitWidth())) {
		ssa_35_mask = APInt(ssa_11_allones.getBitWidth(), 0);
	} else {
		ssa_35_mask = ssa_11_allones.shl(ssa_34_shift_amt);
	}
	APInt ssa_36_sum_min_not = ssa_11_allones ^ ssa_25_sum_min;
	APInt ssa_37_prefix_ko = ssa_25_sum_min & ssa_35_mask;
	APInt ssa_38_prefix_kz = ssa_36_sum_min_not & ssa_35_mask;
	APInt ssa_39_sign_a0 = ssa_3_a0 & ssa_14_signmask;
	APInt ssa_40_sign_a1 = ssa_4_a1 & ssa_14_signmask;
	APInt ssa_41_sign_b0 = ssa_5_b0 & ssa_14_signmask;
	APInt ssa_42_sign_b1 = ssa_6_b1 & ssa_14_signmask;
	APInt ssa_43_sign_c0 = ssa_7_c0 & ssa_14_signmask;
	APInt ssa_44_sign_c1 = ssa_8_c1 & ssa_14_signmask;
	bool ssa_45_sign0_zero = ssa_39_sign_a0.eq(ssa_14_signmask);
	bool ssa_46_sign0_one = ssa_40_sign_a1.eq(ssa_14_signmask);
	bool ssa_47_sign1_zero = ssa_41_sign_b0.eq(ssa_14_signmask);
	bool ssa_48_sign1_one = ssa_42_sign_b1.eq(ssa_14_signmask);
	bool ssa_49_sign2_zero = ssa_43_sign_c0.eq(ssa_14_signmask);
	bool ssa_50_sign2_one = ssa_44_sign_c1.eq(ssa_14_signmask);
	bool ssa_51_sign0_known = ssa_45_sign0_zero || ssa_46_sign0_one;
	bool ssa_52_sign1_known = ssa_47_sign1_zero || ssa_48_sign1_one;
	bool ssa_53_sign2_known = ssa_49_sign2_zero || ssa_50_sign2_one;
	bool ssa_54_sign_known01 = ssa_51_sign0_known && ssa_52_sign1_known;
	bool ssa_55_all_sign_known = ssa_54_sign_known01 && ssa_53_sign2_known;
	bool ssa_56_all_sign_zero01 = ssa_45_sign0_zero && ssa_47_sign1_zero;
	bool ssa_57_all_sign_zero = ssa_56_all_sign_zero01 && ssa_49_sign2_zero;
	bool ssa_58_all_sign_one01 = ssa_46_sign0_one && ssa_48_sign1_one;
	bool ssa_59_all_sign_one = ssa_58_all_sign_one01 && ssa_50_sign2_one;
	APInt ssa_60_sign_t0_min = ssa_24_t0 & ssa_14_signmask;
	APInt ssa_61_sign_sum_min = ssa_25_sum_min & ssa_14_signmask;
	APInt ssa_62_sign_max0 = ssa_27_max0 & ssa_14_signmask;
	APInt ssa_63_sign_max1 = ssa_28_max1 & ssa_14_signmask;
	APInt ssa_64_sign_max2 = ssa_29_max2 & ssa_14_signmask;
	APInt ssa_65_sign_tmax = ssa_30_tmax & ssa_14_signmask;
	APInt ssa_66_sign_sum_max = ssa_31_sum_max & ssa_14_signmask;
	bool ssa_67_bc_same_min = ssa_42_sign_b1.eq(ssa_44_sign_c1);
	bool ssa_68_t0_same_min = ssa_60_sign_t0_min.eq(ssa_42_sign_b1);
	bool ssa_69_t0_diff_min = ssa_68_t0_same_min != ssa_15_true;
	bool ssa_70_ov_t0_min = ssa_67_bc_same_min && ssa_69_t0_diff_min;
	bool ssa_71_a_t0_same_min = ssa_40_sign_a1.eq(ssa_60_sign_t0_min);
	bool ssa_72_sum_same_min = ssa_61_sign_sum_min.eq(ssa_40_sign_a1);
	bool ssa_73_sum_diff_min = ssa_72_sum_same_min != ssa_15_true;
	bool ssa_74_ov_sum_min = ssa_71_a_t0_same_min && ssa_73_sum_diff_min;
	bool ssa_75_ov_min01 = ssa_70_ov_t0_min || ssa_74_ov_sum_min;
	bool ssa_76_no_ov_min = ssa_75_ov_min01 != ssa_15_true;
	bool ssa_77_bc_same_max = ssa_63_sign_max1.eq(ssa_64_sign_max2);
	bool ssa_78_t0_same_max = ssa_65_sign_tmax.eq(ssa_63_sign_max1);
	bool ssa_79_t0_diff_max = ssa_78_t0_same_max != ssa_15_true;
	bool ssa_80_ov_t0_max = ssa_77_bc_same_max && ssa_79_t0_diff_max;
	bool ssa_81_a_t0_same_max = ssa_62_sign_max0.eq(ssa_65_sign_tmax);
	bool ssa_82_sum_same_max = ssa_66_sign_sum_max.eq(ssa_62_sign_max0);
	bool ssa_83_sum_diff_max = ssa_82_sum_same_max != ssa_15_true;
	bool ssa_84_ov_sum_max = ssa_81_a_t0_same_max && ssa_83_sum_diff_max;
	bool ssa_85_ov_max01 = ssa_80_ov_t0_max || ssa_84_ov_sum_max;
	bool ssa_86_no_ov_max = ssa_85_ov_max01 != ssa_15_true;
	bool ssa_87_safe01 = ssa_55_all_sign_known && ssa_76_no_ov_min;
	bool ssa_88_safe_range = ssa_87_safe01 && ssa_86_no_ov_max;
	APInt ssa_89_res_kz_sign = ssa_57_all_sign_zero ? ssa_14_signmask : ssa_9_const0;
	APInt ssa_90_res_ko_sign = ssa_59_all_sign_one ? ssa_14_signmask : ssa_9_const0;
	APInt ssa_91_res_kz_base = ssa_88_safe_range ? ssa_38_prefix_kz : ssa_89_res_kz_sign;
	APInt ssa_92_res_ko_base = ssa_88_safe_range ? ssa_37_prefix_ko : ssa_90_res_ko_sign;
	APInt ssa_93_res_kz = ssa_23_allknown ? ssa_26_exact_kz : ssa_91_res_kz_base;
	APInt ssa_94_res_ko = ssa_23_allknown ? ssa_25_sum_min : ssa_92_res_ko_base;
	std::array<APInt, 2> ssa_95_r = std::array<APInt, 2>{ssa_93_res_kz, ssa_94_res_ko};
	return ssa_95_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_12_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_13_bw_minus1 = ssa_12_bw - ssa_10_const1;
	APInt ssa_14_signmask;
	if (ssa_13_bw_minus1.uge(ssa_13_bw_minus1.getBitWidth())) {
		ssa_14_signmask = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_14_signmask = ssa_10_const1.shl(ssa_13_bw_minus1);
	}
	bool ssa_15_true = ssa_9_const0.eq(ssa_9_const0);
	APInt ssa_16_known0 = ssa_3_a0 | ssa_4_a1;
	APInt ssa_17_known1 = ssa_5_b0 | ssa_6_b1;
	APInt ssa_18_known2 = ssa_7_c0 | ssa_8_c1;
	APInt ssa_19_tz0(ssa_16_known0.getBitWidth(), ssa_16_known0.countr_one());
	APInt ssa_20_tz1(ssa_17_known1.getBitWidth(), ssa_17_known1.countr_one());
	APInt ssa_21_tz2(ssa_18_known2.getBitWidth(), ssa_18_known2.countr_one());
	APInt ssa_22_tmin01 = APIntOps::umin(ssa_19_tz0, ssa_20_tz1);
	APInt ssa_23_k = APIntOps::umin(ssa_22_tmin01, ssa_21_tz2);
	APInt ssa_24_pow2;
	if (ssa_23_k.uge(ssa_23_k.getBitWidth())) {
		ssa_24_pow2 = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_24_pow2 = ssa_10_const1.shl(ssa_23_k);
	}
	APInt ssa_25_mask_low0 = ssa_24_pow2 - ssa_10_const1;
	bool ssa_26_k_eq_bw = ssa_23_k.eq(ssa_12_bw);
	APInt ssa_27_mask_low = ssa_26_k_eq_bw ? ssa_11_allones : ssa_25_mask_low0;
	APInt ssa_28_a_low = ssa_4_a1 & ssa_27_mask_low;
	APInt ssa_29_b_low = ssa_6_b1 & ssa_27_mask_low;
	APInt ssa_30_c_low = ssa_8_c1 & ssa_27_mask_low;
	APInt ssa_31_sum_low0 = ssa_28_a_low + ssa_29_b_low;
	APInt ssa_32_sum_low1 = ssa_31_sum_low0 + ssa_30_c_low;
	APInt ssa_33_sum_low = ssa_32_sum_low1 & ssa_27_mask_low;
	APInt ssa_34_low_ko = ssa_33_sum_low & ssa_11_allones;
	APInt ssa_35_low_kz0 = ssa_11_allones ^ ssa_33_sum_low;
	APInt ssa_36_low_kz = ssa_35_low_kz0 & ssa_27_mask_low;
	APInt ssa_37_sign_a0 = ssa_3_a0 & ssa_14_signmask;
	APInt ssa_38_sign_a1 = ssa_4_a1 & ssa_14_signmask;
	APInt ssa_39_sign_b0 = ssa_5_b0 & ssa_14_signmask;
	APInt ssa_40_sign_b1 = ssa_6_b1 & ssa_14_signmask;
	APInt ssa_41_sign_c0 = ssa_7_c0 & ssa_14_signmask;
	APInt ssa_42_sign_c1 = ssa_8_c1 & ssa_14_signmask;
	bool ssa_43_sign0_zero = ssa_37_sign_a0.eq(ssa_14_signmask);
	bool ssa_44_sign0_one = ssa_38_sign_a1.eq(ssa_14_signmask);
	bool ssa_45_sign1_zero = ssa_39_sign_b0.eq(ssa_14_signmask);
	bool ssa_46_sign1_one = ssa_40_sign_b1.eq(ssa_14_signmask);
	bool ssa_47_sign2_zero = ssa_41_sign_c0.eq(ssa_14_signmask);
	bool ssa_48_sign2_one = ssa_42_sign_c1.eq(ssa_14_signmask);
	bool ssa_49_sign0_known = ssa_43_sign0_zero || ssa_44_sign0_one;
	bool ssa_50_sign1_known = ssa_45_sign1_zero || ssa_46_sign1_one;
	bool ssa_51_sign2_known = ssa_47_sign2_zero || ssa_48_sign2_one;
	bool ssa_52_sign_known01 = ssa_49_sign0_known && ssa_50_sign1_known;
	bool ssa_53_all_sign_known = ssa_52_sign_known01 && ssa_51_sign2_known;
	APInt ssa_54_signmask_inv = ssa_11_allones ^ ssa_14_signmask;
	APInt ssa_55_a1_or_sign = ssa_4_a1 | ssa_14_signmask;
	APInt ssa_56_b1_or_sign = ssa_6_b1 | ssa_14_signmask;
	APInt ssa_57_c1_or_sign = ssa_8_c1 | ssa_14_signmask;
	APInt ssa_58_min_a = ssa_49_sign0_known ? ssa_4_a1 : ssa_55_a1_or_sign;
	APInt ssa_59_min_b = ssa_50_sign1_known ? ssa_6_b1 : ssa_56_b1_or_sign;
	APInt ssa_60_min_c = ssa_51_sign2_known ? ssa_8_c1 : ssa_57_c1_or_sign;
	APInt ssa_61_max0 = ssa_11_allones ^ ssa_3_a0;
	APInt ssa_62_max1 = ssa_11_allones ^ ssa_5_b0;
	APInt ssa_63_max2 = ssa_11_allones ^ ssa_7_c0;
	APInt ssa_64_max_a0 = ssa_61_max0 & ssa_54_signmask_inv;
	APInt ssa_65_max_b0 = ssa_62_max1 & ssa_54_signmask_inv;
	APInt ssa_66_max_c0 = ssa_63_max2 & ssa_54_signmask_inv;
	APInt ssa_67_max_a = ssa_49_sign0_known ? ssa_61_max0 : ssa_64_max_a0;
	APInt ssa_68_max_b = ssa_50_sign1_known ? ssa_62_max1 : ssa_65_max_b0;
	APInt ssa_69_max_c = ssa_51_sign2_known ? ssa_63_max2 : ssa_66_max_c0;
	APInt ssa_70_sign_min_a = ssa_58_min_a & ssa_14_signmask;
	APInt ssa_71_sign_min_b = ssa_59_min_b & ssa_14_signmask;
	APInt ssa_72_sign_min_c = ssa_60_min_c & ssa_14_signmask;
	APInt ssa_73_sign_max_a = ssa_67_max_a & ssa_14_signmask;
	APInt ssa_74_sign_max_b = ssa_68_max_b & ssa_14_signmask;
	APInt ssa_75_sign_max_c = ssa_69_max_c & ssa_14_signmask;
	APInt ssa_76_tmin_bc = ssa_59_min_b + ssa_60_min_c;
	APInt ssa_77_sign_tmin_bc = ssa_76_tmin_bc & ssa_14_signmask;
	bool ssa_78_same_sign_min_bc = ssa_71_sign_min_b.eq(ssa_72_sign_min_c);
	bool ssa_79_tmin_bc_same = ssa_77_sign_tmin_bc.eq(ssa_71_sign_min_b);
	bool ssa_80_tmin_bc_diff = ssa_79_tmin_bc_same != ssa_15_true;
	bool ssa_81_ov_tmin_bc = ssa_78_same_sign_min_bc && ssa_80_tmin_bc_diff;
	bool ssa_82_sign_min_b_one = ssa_71_sign_min_b.eq(ssa_14_signmask);
	bool ssa_83_ov_tmin_bc_neg = ssa_81_ov_tmin_bc && ssa_82_sign_min_b_one;
	APInt ssa_84_tmin_bc_clamped = ssa_83_ov_tmin_bc_neg ? ssa_14_signmask : ssa_76_tmin_bc;
	APInt ssa_85_sign_tmin_bc_clamped = ssa_84_tmin_bc_clamped & ssa_14_signmask;
	APInt ssa_86_sum_min_s = ssa_84_tmin_bc_clamped + ssa_58_min_a;
	APInt ssa_87_sign_sum_min_s = ssa_86_sum_min_s & ssa_14_signmask;
	bool ssa_88_same_sign_min_s = ssa_70_sign_min_a.eq(ssa_85_sign_tmin_bc_clamped);
	bool ssa_89_sum_min_s_same = ssa_87_sign_sum_min_s.eq(ssa_70_sign_min_a);
	bool ssa_90_sum_min_s_diff = ssa_89_sum_min_s_same != ssa_15_true;
	bool ssa_91_ov_sum_min_s = ssa_88_same_sign_min_s && ssa_90_sum_min_s_diff;
	bool ssa_92_sign_min_a_one = ssa_70_sign_min_a.eq(ssa_14_signmask);
	bool ssa_93_ov_sum_min_s_neg = ssa_91_ov_sum_min_s && ssa_92_sign_min_a_one;
	APInt ssa_94_range_min = ssa_93_ov_sum_min_s_neg ? ssa_14_signmask : ssa_86_sum_min_s;
	APInt ssa_95_tmax_bc = ssa_68_max_b + ssa_69_max_c;
	APInt ssa_96_sign_tmax_bc = ssa_95_tmax_bc & ssa_14_signmask;
	bool ssa_97_same_sign_max_bc = ssa_74_sign_max_b.eq(ssa_75_sign_max_c);
	bool ssa_98_tmax_bc_same = ssa_96_sign_tmax_bc.eq(ssa_74_sign_max_b);
	bool ssa_99_tmax_bc_diff = ssa_98_tmax_bc_same != ssa_15_true;
	bool ssa_100_ov_tmax_bc = ssa_97_same_sign_max_bc && ssa_99_tmax_bc_diff;
	bool ssa_101_sign_max_b_zero = ssa_74_sign_max_b.eq(ssa_9_const0);
	bool ssa_102_ov_tmax_bc_pos = ssa_100_ov_tmax_bc && ssa_101_sign_max_b_zero;
	APInt ssa_103_tmax_bc_clamped = ssa_102_ov_tmax_bc_pos ? ssa_54_signmask_inv : ssa_95_tmax_bc;
	APInt ssa_104_sign_tmax_bc_clamped = ssa_103_tmax_bc_clamped & ssa_14_signmask;
	APInt ssa_105_sum_max_s = ssa_103_tmax_bc_clamped + ssa_67_max_a;
	APInt ssa_106_sign_sum_max_s = ssa_105_sum_max_s & ssa_14_signmask;
	bool ssa_107_same_sign_max_s = ssa_73_sign_max_a.eq(ssa_104_sign_tmax_bc_clamped);
	bool ssa_108_sum_max_s_same = ssa_106_sign_sum_max_s.eq(ssa_73_sign_max_a);
	bool ssa_109_sum_max_s_diff = ssa_108_sum_max_s_same != ssa_15_true;
	bool ssa_110_ov_sum_max_s = ssa_107_same_sign_max_s && ssa_109_sum_max_s_diff;
	bool ssa_111_sign_max_a_zero = ssa_73_sign_max_a.eq(ssa_9_const0);
	bool ssa_112_ov_sum_max_s_pos = ssa_110_ov_sum_max_s && ssa_111_sign_max_a_zero;
	APInt ssa_113_range_max = ssa_112_ov_sum_max_s_pos ? ssa_54_signmask_inv : ssa_105_sum_max_s;
	bool ssa_114_allknown0 = ssa_16_known0.eq(ssa_11_allones);
	bool ssa_115_allknown1 = ssa_17_known1.eq(ssa_11_allones);
	bool ssa_116_allknown2 = ssa_18_known2.eq(ssa_11_allones);
	bool ssa_117_allknown01 = ssa_114_allknown0 && ssa_115_allknown1;
	bool ssa_118_allknown02 = ssa_114_allknown0 && ssa_116_allknown2;
	bool ssa_119_allknown12 = ssa_115_allknown1 && ssa_116_allknown2;
	bool ssa_120_allknown = ssa_117_allknown01 && ssa_116_allknown2;
	APInt ssa_121_t0 = ssa_6_b1 + ssa_8_c1;
	APInt ssa_122_sum_min = ssa_4_a1 + ssa_121_t0;
	APInt ssa_123_exact_kz = ssa_11_allones ^ ssa_122_sum_min;
	APInt ssa_124_tmax = ssa_62_max1 + ssa_63_max2;
	APInt ssa_125_sum_max = ssa_61_max0 + ssa_124_tmax;
	APInt ssa_126_diff = ssa_94_range_min ^ ssa_113_range_max;
	APInt ssa_127_clz(ssa_126_diff.getBitWidth(), ssa_126_diff.countl_zero());
	APInt ssa_128_shift_amt = ssa_12_bw - ssa_127_clz;
	APInt ssa_129_mask;
	if (ssa_128_shift_amt.uge(ssa_128_shift_amt.getBitWidth())) {
		ssa_129_mask = APInt(ssa_11_allones.getBitWidth(), 0);
	} else {
		ssa_129_mask = ssa_11_allones.shl(ssa_128_shift_amt);
	}
	APInt ssa_130_range_min_not = ssa_11_allones ^ ssa_94_range_min;
	APInt ssa_131_prefix_ko = ssa_94_range_min & ssa_129_mask;
	APInt ssa_132_prefix_kz = ssa_130_range_min_not & ssa_129_mask;
	bool ssa_133_all_sign_zero01 = ssa_43_sign0_zero && ssa_45_sign1_zero;
	bool ssa_134_all_sign_zero = ssa_133_all_sign_zero01 && ssa_47_sign2_zero;
	bool ssa_135_all_sign_one01 = ssa_44_sign0_one && ssa_46_sign1_one;
	bool ssa_136_all_sign_one = ssa_135_all_sign_one01 && ssa_48_sign2_one;
	APInt ssa_137_sign_t0_min = ssa_121_t0 & ssa_14_signmask;
	APInt ssa_138_sign_sum_min = ssa_122_sum_min & ssa_14_signmask;
	APInt ssa_139_sign_max0 = ssa_61_max0 & ssa_14_signmask;
	APInt ssa_140_sign_max1 = ssa_62_max1 & ssa_14_signmask;
	APInt ssa_141_sign_max2 = ssa_63_max2 & ssa_14_signmask;
	APInt ssa_142_sign_tmax = ssa_124_tmax & ssa_14_signmask;
	APInt ssa_143_sign_sum_max = ssa_125_sum_max & ssa_14_signmask;
	bool ssa_144_bc_same_min = ssa_40_sign_b1.eq(ssa_42_sign_c1);
	bool ssa_145_t0_same_min = ssa_137_sign_t0_min.eq(ssa_40_sign_b1);
	bool ssa_146_t0_diff_min = ssa_145_t0_same_min != ssa_15_true;
	bool ssa_147_ov_t0_min = ssa_144_bc_same_min && ssa_146_t0_diff_min;
	bool ssa_148_a_t0_same_min = ssa_38_sign_a1.eq(ssa_137_sign_t0_min);
	bool ssa_149_sum_same_min = ssa_138_sign_sum_min.eq(ssa_38_sign_a1);
	bool ssa_150_sum_diff_min = ssa_149_sum_same_min != ssa_15_true;
	bool ssa_151_ov_sum_min = ssa_148_a_t0_same_min && ssa_150_sum_diff_min;
	bool ssa_152_ov_min01 = ssa_147_ov_t0_min || ssa_151_ov_sum_min;
	bool ssa_153_no_ov_min = ssa_152_ov_min01 != ssa_15_true;
	bool ssa_154_bc_same_max = ssa_140_sign_max1.eq(ssa_141_sign_max2);
	bool ssa_155_t0_same_max = ssa_142_sign_tmax.eq(ssa_140_sign_max1);
	bool ssa_156_t0_diff_max = ssa_155_t0_same_max != ssa_15_true;
	bool ssa_157_ov_t0_max = ssa_154_bc_same_max && ssa_156_t0_diff_max;
	bool ssa_158_a_t0_same_max = ssa_139_sign_max0.eq(ssa_142_sign_tmax);
	bool ssa_159_sum_same_max = ssa_143_sign_sum_max.eq(ssa_139_sign_max0);
	bool ssa_160_sum_diff_max = ssa_159_sum_same_max != ssa_15_true;
	bool ssa_161_ov_sum_max = ssa_158_a_t0_same_max && ssa_160_sum_diff_max;
	bool ssa_162_ov_max01 = ssa_157_ov_t0_max || ssa_161_ov_sum_max;
	bool ssa_163_no_ov_max = ssa_162_ov_max01 != ssa_15_true;
	bool ssa_164_safe01 = ssa_53_all_sign_known && ssa_153_no_ov_min;
	bool ssa_165_safe_range = ssa_164_safe01 && ssa_163_no_ov_max;
	APInt ssa_166_res_kz_sign = ssa_134_all_sign_zero ? ssa_14_signmask : ssa_9_const0;
	APInt ssa_167_res_ko_sign = ssa_136_all_sign_one ? ssa_14_signmask : ssa_9_const0;
	APInt ssa_168_res_kz_base = ssa_132_prefix_kz | ssa_166_res_kz_sign;
	APInt ssa_169_res_ko_base = ssa_131_prefix_ko | ssa_167_res_ko_sign;
	APInt ssa_170_res_kz = ssa_120_allknown ? ssa_123_exact_kz : ssa_168_res_kz_base;
	APInt ssa_171_res_ko = ssa_120_allknown ? ssa_122_sum_min : ssa_169_res_ko_base;
	APInt ssa_172_res_kz2 = ssa_170_res_kz | ssa_36_low_kz;
	APInt ssa_173_res_ko2 = ssa_171_res_ko | ssa_34_low_ko;
	APInt ssa_174_k_ab = ssa_4_a1 + ssa_6_b1;
	APInt ssa_175_tz_ab(ssa_174_k_ab.getBitWidth(), ssa_174_k_ab.countr_zero());
	bool ssa_176_tz_ab_eq_bw = ssa_175_tz_ab.eq(ssa_12_bw);
	APInt ssa_177_known2_shift_ab;
	if (ssa_175_tz_ab.uge(ssa_175_tz_ab.getBitWidth())) {
		ssa_177_known2_shift_ab = APInt(ssa_18_known2.getBitWidth(), 0);
	} else {
		ssa_177_known2_shift_ab = ssa_18_known2.lshr(ssa_175_tz_ab);
	}
	APInt ssa_178_len_ab(ssa_177_known2_shift_ab.getBitWidth(), ssa_177_known2_shift_ab.countr_one());
	APInt ssa_179_len_ab_sel = ssa_176_tz_ab_eq_bw ? ssa_9_const0 : ssa_178_len_ab;
	APInt ssa_180_pow2_ab;
	if (ssa_179_len_ab_sel.uge(ssa_179_len_ab_sel.getBitWidth())) {
		ssa_180_pow2_ab = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_180_pow2_ab = ssa_10_const1.shl(ssa_179_len_ab_sel);
	}
	APInt ssa_181_mask_ab0 = ssa_180_pow2_ab - ssa_10_const1;
	APInt ssa_182_mask_ab1_pre;
	if (ssa_175_tz_ab.uge(ssa_175_tz_ab.getBitWidth())) {
		ssa_182_mask_ab1_pre = APInt(ssa_181_mask_ab0.getBitWidth(), 0);
	} else {
		ssa_182_mask_ab1_pre = ssa_181_mask_ab0.shl(ssa_175_tz_ab);
	}
	APInt ssa_183_mask_ab1 = ssa_176_tz_ab_eq_bw ? ssa_9_const0 : ssa_182_mask_ab1_pre;
	APInt ssa_184_x_ab = ssa_8_c1 & ssa_183_mask_ab1;
	APInt ssa_185_sum_ab = ssa_184_x_ab + ssa_174_k_ab;
	APInt ssa_186_sum_ab_block = ssa_185_sum_ab & ssa_183_mask_ab1;
	APInt ssa_187_ko_ab0 = ssa_186_sum_ab_block & ssa_11_allones;
	APInt ssa_188_kz_ab0 = ssa_183_mask_ab1 & ssa_11_allones;
	APInt ssa_189_kz_ab1 = ssa_188_kz_ab0 ^ ssa_186_sum_ab_block;
	APInt ssa_190_ko_ab = ssa_117_allknown01 ? ssa_187_ko_ab0 : ssa_9_const0;
	APInt ssa_191_kz_ab = ssa_117_allknown01 ? ssa_189_kz_ab1 : ssa_9_const0;
	APInt ssa_192_k_ac = ssa_4_a1 + ssa_8_c1;
	APInt ssa_193_tz_ac(ssa_192_k_ac.getBitWidth(), ssa_192_k_ac.countr_zero());
	bool ssa_194_tz_ac_eq_bw = ssa_193_tz_ac.eq(ssa_12_bw);
	APInt ssa_195_known1_shift_ac;
	if (ssa_193_tz_ac.uge(ssa_193_tz_ac.getBitWidth())) {
		ssa_195_known1_shift_ac = APInt(ssa_17_known1.getBitWidth(), 0);
	} else {
		ssa_195_known1_shift_ac = ssa_17_known1.lshr(ssa_193_tz_ac);
	}
	APInt ssa_196_len_ac(ssa_195_known1_shift_ac.getBitWidth(), ssa_195_known1_shift_ac.countr_one());
	APInt ssa_197_len_ac_sel = ssa_194_tz_ac_eq_bw ? ssa_9_const0 : ssa_196_len_ac;
	APInt ssa_198_pow2_ac;
	if (ssa_197_len_ac_sel.uge(ssa_197_len_ac_sel.getBitWidth())) {
		ssa_198_pow2_ac = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_198_pow2_ac = ssa_10_const1.shl(ssa_197_len_ac_sel);
	}
	APInt ssa_199_mask_ac0 = ssa_198_pow2_ac - ssa_10_const1;
	APInt ssa_200_mask_ac1_pre;
	if (ssa_193_tz_ac.uge(ssa_193_tz_ac.getBitWidth())) {
		ssa_200_mask_ac1_pre = APInt(ssa_199_mask_ac0.getBitWidth(), 0);
	} else {
		ssa_200_mask_ac1_pre = ssa_199_mask_ac0.shl(ssa_193_tz_ac);
	}
	APInt ssa_201_mask_ac1 = ssa_194_tz_ac_eq_bw ? ssa_9_const0 : ssa_200_mask_ac1_pre;
	APInt ssa_202_x_ac = ssa_6_b1 & ssa_201_mask_ac1;
	APInt ssa_203_sum_ac = ssa_202_x_ac + ssa_192_k_ac;
	APInt ssa_204_sum_ac_block = ssa_203_sum_ac & ssa_201_mask_ac1;
	APInt ssa_205_ko_ac0 = ssa_204_sum_ac_block & ssa_11_allones;
	APInt ssa_206_kz_ac0 = ssa_201_mask_ac1 & ssa_11_allones;
	APInt ssa_207_kz_ac1 = ssa_206_kz_ac0 ^ ssa_204_sum_ac_block;
	APInt ssa_208_ko_ac = ssa_118_allknown02 ? ssa_205_ko_ac0 : ssa_9_const0;
	APInt ssa_209_kz_ac = ssa_118_allknown02 ? ssa_207_kz_ac1 : ssa_9_const0;
	APInt ssa_210_k_bc = ssa_6_b1 + ssa_8_c1;
	APInt ssa_211_tz_bc(ssa_210_k_bc.getBitWidth(), ssa_210_k_bc.countr_zero());
	bool ssa_212_tz_bc_eq_bw = ssa_211_tz_bc.eq(ssa_12_bw);
	APInt ssa_213_known0_shift_bc;
	if (ssa_211_tz_bc.uge(ssa_211_tz_bc.getBitWidth())) {
		ssa_213_known0_shift_bc = APInt(ssa_16_known0.getBitWidth(), 0);
	} else {
		ssa_213_known0_shift_bc = ssa_16_known0.lshr(ssa_211_tz_bc);
	}
	APInt ssa_214_len_bc(ssa_213_known0_shift_bc.getBitWidth(), ssa_213_known0_shift_bc.countr_one());
	APInt ssa_215_len_bc_sel = ssa_212_tz_bc_eq_bw ? ssa_9_const0 : ssa_214_len_bc;
	APInt ssa_216_pow2_bc;
	if (ssa_215_len_bc_sel.uge(ssa_215_len_bc_sel.getBitWidth())) {
		ssa_216_pow2_bc = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_216_pow2_bc = ssa_10_const1.shl(ssa_215_len_bc_sel);
	}
	APInt ssa_217_mask_bc0 = ssa_216_pow2_bc - ssa_10_const1;
	APInt ssa_218_mask_bc1_pre;
	if (ssa_211_tz_bc.uge(ssa_211_tz_bc.getBitWidth())) {
		ssa_218_mask_bc1_pre = APInt(ssa_217_mask_bc0.getBitWidth(), 0);
	} else {
		ssa_218_mask_bc1_pre = ssa_217_mask_bc0.shl(ssa_211_tz_bc);
	}
	APInt ssa_219_mask_bc1 = ssa_212_tz_bc_eq_bw ? ssa_9_const0 : ssa_218_mask_bc1_pre;
	APInt ssa_220_x_bc = ssa_4_a1 & ssa_219_mask_bc1;
	APInt ssa_221_sum_bc = ssa_220_x_bc + ssa_210_k_bc;
	APInt ssa_222_sum_bc_block = ssa_221_sum_bc & ssa_219_mask_bc1;
	APInt ssa_223_ko_bc0 = ssa_222_sum_bc_block & ssa_11_allones;
	APInt ssa_224_kz_bc0 = ssa_219_mask_bc1 & ssa_11_allones;
	APInt ssa_225_kz_bc1 = ssa_224_kz_bc0 ^ ssa_222_sum_bc_block;
	APInt ssa_226_ko_bc = ssa_119_allknown12 ? ssa_223_ko_bc0 : ssa_9_const0;
	APInt ssa_227_kz_bc = ssa_119_allknown12 ? ssa_225_kz_bc1 : ssa_9_const0;
	APInt ssa_228_res_kz3 = ssa_172_res_kz2 | ssa_191_kz_ab;
	APInt ssa_229_res_ko3 = ssa_173_res_ko2 | ssa_190_ko_ab;
	APInt ssa_230_res_kz4 = ssa_228_res_kz3 | ssa_209_kz_ac;
	APInt ssa_231_res_ko4 = ssa_229_res_ko3 | ssa_208_ko_ac;
	APInt ssa_232_res_kz5 = ssa_230_res_kz4 | ssa_227_kz_bc;
	APInt ssa_233_res_ko5 = ssa_231_res_ko4 | ssa_226_ko_bc;
	std::array<APInt, 2> ssa_234_r = std::array<APInt, 2>{ssa_232_res_kz5, ssa_233_res_ko5};
	return ssa_234_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_11_const2(ssa_3_a0.getBitWidth(), 2);
	APInt ssa_12_const3(ssa_3_a0.getBitWidth(), 3);
	APInt ssa_13_allones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_14_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_15_bw_minus1 = ssa_14_bw - ssa_10_const1;
	APInt ssa_16_signmask;
	if (ssa_15_bw_minus1.uge(ssa_15_bw_minus1.getBitWidth())) {
		ssa_16_signmask = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_16_signmask = ssa_10_const1.shl(ssa_15_bw_minus1);
	}
	APInt ssa_17_sign_plus2 = ssa_16_signmask + ssa_11_const2;
	APInt ssa_18_known_a = ssa_3_a0 | ssa_4_a1;
	APInt ssa_19_known_b = ssa_5_b0 | ssa_6_b1;
	APInt ssa_20_known_c = ssa_7_c0 | ssa_8_c1;
	bool ssa_21_allknown_a = ssa_18_known_a.eq(ssa_13_allones);
	bool ssa_22_allknown_b = ssa_19_known_b.eq(ssa_13_allones);
	bool ssa_23_allknown_c = ssa_20_known_c.eq(ssa_13_allones);
	APInt ssa_24_max_a = ssa_13_allones ^ ssa_3_a0;
	APInt ssa_25_max_b = ssa_13_allones ^ ssa_5_b0;
	APInt ssa_26_max_c = ssa_13_allones ^ ssa_7_c0;
	APInt ssa_27_sum_min_ab = ssa_4_a1 + ssa_6_b1;
	APInt ssa_28_sum_max_ab = ssa_24_max_a + ssa_25_max_b;
	APInt ssa_29_and_min_ab = ssa_4_a1 & ssa_6_b1;
	APInt ssa_30_or_min_ab = ssa_4_a1 | ssa_6_b1;
	APInt ssa_31_sum_min_ab_not = ssa_13_allones ^ ssa_27_sum_min_ab;
	APInt ssa_32_or_min_ab_and = ssa_30_or_min_ab & ssa_31_sum_min_ab_not;
	APInt ssa_33_carry_min_ab = ssa_29_and_min_ab | ssa_32_or_min_ab_and;
	APInt ssa_34_and_max_ab = ssa_24_max_a & ssa_25_max_b;
	APInt ssa_35_or_max_ab = ssa_24_max_a | ssa_25_max_b;
	APInt ssa_36_sum_max_ab_not = ssa_13_allones ^ ssa_28_sum_max_ab;
	APInt ssa_37_or_max_ab_and = ssa_35_or_max_ab & ssa_36_sum_max_ab_not;
	APInt ssa_38_carry_max_ab = ssa_34_and_max_ab | ssa_37_or_max_ab_and;
	APInt ssa_39_carry_min_ab_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_39_carry_min_ab_sh = APInt(ssa_33_carry_min_ab.getBitWidth(), 0);
	} else {
		ssa_39_carry_min_ab_sh = ssa_33_carry_min_ab.shl(ssa_10_const1);
	}
	APInt ssa_40_carry_max_ab_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_40_carry_max_ab_sh = APInt(ssa_38_carry_max_ab.getBitWidth(), 0);
	} else {
		ssa_40_carry_max_ab_sh = ssa_38_carry_max_ab.shl(ssa_10_const1);
	}
	APInt ssa_41_carry_xor_ab = ssa_39_carry_min_ab_sh ^ ssa_40_carry_max_ab_sh;
	APInt ssa_42_carry_known_ab = ssa_13_allones ^ ssa_41_carry_xor_ab;
	APInt ssa_43_known_ab0 = ssa_18_known_a & ssa_19_known_b;
	APInt ssa_44_known_ab1 = ssa_43_known_ab0 & ssa_42_carry_known_ab;
	APInt ssa_45_sum_val_ab0 = ssa_4_a1 ^ ssa_6_b1;
	APInt ssa_46_sum_val_ab1 = ssa_45_sum_val_ab0 ^ ssa_39_carry_min_ab_sh;
	APInt ssa_47_ko_ab = ssa_44_known_ab1 & ssa_46_sum_val_ab1;
	APInt ssa_48_sum_val_ab1_not = ssa_13_allones ^ ssa_46_sum_val_ab1;
	APInt ssa_49_kz_ab = ssa_44_known_ab1 & ssa_48_sum_val_ab1_not;
	APInt ssa_50_sum_min_ac = ssa_4_a1 + ssa_8_c1;
	APInt ssa_51_sum_max_ac = ssa_24_max_a + ssa_26_max_c;
	APInt ssa_52_and_min_ac = ssa_4_a1 & ssa_8_c1;
	APInt ssa_53_or_min_ac = ssa_4_a1 | ssa_8_c1;
	APInt ssa_54_sum_min_ac_not = ssa_13_allones ^ ssa_50_sum_min_ac;
	APInt ssa_55_or_min_ac_and = ssa_53_or_min_ac & ssa_54_sum_min_ac_not;
	APInt ssa_56_carry_min_ac = ssa_52_and_min_ac | ssa_55_or_min_ac_and;
	APInt ssa_57_and_max_ac = ssa_24_max_a & ssa_26_max_c;
	APInt ssa_58_or_max_ac = ssa_24_max_a | ssa_26_max_c;
	APInt ssa_59_sum_max_ac_not = ssa_13_allones ^ ssa_51_sum_max_ac;
	APInt ssa_60_or_max_ac_and = ssa_58_or_max_ac & ssa_59_sum_max_ac_not;
	APInt ssa_61_carry_max_ac = ssa_57_and_max_ac | ssa_60_or_max_ac_and;
	APInt ssa_62_carry_min_ac_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_62_carry_min_ac_sh = APInt(ssa_56_carry_min_ac.getBitWidth(), 0);
	} else {
		ssa_62_carry_min_ac_sh = ssa_56_carry_min_ac.shl(ssa_10_const1);
	}
	APInt ssa_63_carry_max_ac_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_63_carry_max_ac_sh = APInt(ssa_61_carry_max_ac.getBitWidth(), 0);
	} else {
		ssa_63_carry_max_ac_sh = ssa_61_carry_max_ac.shl(ssa_10_const1);
	}
	APInt ssa_64_carry_xor_ac = ssa_62_carry_min_ac_sh ^ ssa_63_carry_max_ac_sh;
	APInt ssa_65_carry_known_ac = ssa_13_allones ^ ssa_64_carry_xor_ac;
	APInt ssa_66_known_ac0 = ssa_18_known_a & ssa_20_known_c;
	APInt ssa_67_known_ac1 = ssa_66_known_ac0 & ssa_65_carry_known_ac;
	APInt ssa_68_sum_val_ac0 = ssa_4_a1 ^ ssa_8_c1;
	APInt ssa_69_sum_val_ac1 = ssa_68_sum_val_ac0 ^ ssa_62_carry_min_ac_sh;
	APInt ssa_70_ko_ac = ssa_67_known_ac1 & ssa_69_sum_val_ac1;
	APInt ssa_71_sum_val_ac1_not = ssa_13_allones ^ ssa_69_sum_val_ac1;
	APInt ssa_72_kz_ac = ssa_67_known_ac1 & ssa_71_sum_val_ac1_not;
	APInt ssa_73_sum_min_bc = ssa_6_b1 + ssa_8_c1;
	APInt ssa_74_sum_max_bc = ssa_25_max_b + ssa_26_max_c;
	APInt ssa_75_and_min_bc = ssa_6_b1 & ssa_8_c1;
	APInt ssa_76_or_min_bc = ssa_6_b1 | ssa_8_c1;
	APInt ssa_77_sum_min_bc_not = ssa_13_allones ^ ssa_73_sum_min_bc;
	APInt ssa_78_or_min_bc_and = ssa_76_or_min_bc & ssa_77_sum_min_bc_not;
	APInt ssa_79_carry_min_bc = ssa_75_and_min_bc | ssa_78_or_min_bc_and;
	APInt ssa_80_and_max_bc = ssa_25_max_b & ssa_26_max_c;
	APInt ssa_81_or_max_bc = ssa_25_max_b | ssa_26_max_c;
	APInt ssa_82_sum_max_bc_not = ssa_13_allones ^ ssa_74_sum_max_bc;
	APInt ssa_83_or_max_bc_and = ssa_81_or_max_bc & ssa_82_sum_max_bc_not;
	APInt ssa_84_carry_max_bc = ssa_80_and_max_bc | ssa_83_or_max_bc_and;
	APInt ssa_85_carry_min_bc_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_85_carry_min_bc_sh = APInt(ssa_79_carry_min_bc.getBitWidth(), 0);
	} else {
		ssa_85_carry_min_bc_sh = ssa_79_carry_min_bc.shl(ssa_10_const1);
	}
	APInt ssa_86_carry_max_bc_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_86_carry_max_bc_sh = APInt(ssa_84_carry_max_bc.getBitWidth(), 0);
	} else {
		ssa_86_carry_max_bc_sh = ssa_84_carry_max_bc.shl(ssa_10_const1);
	}
	APInt ssa_87_carry_xor_bc = ssa_85_carry_min_bc_sh ^ ssa_86_carry_max_bc_sh;
	APInt ssa_88_carry_known_bc = ssa_13_allones ^ ssa_87_carry_xor_bc;
	APInt ssa_89_known_bc0 = ssa_19_known_b & ssa_20_known_c;
	APInt ssa_90_known_bc1 = ssa_89_known_bc0 & ssa_88_carry_known_bc;
	APInt ssa_91_sum_val_bc0 = ssa_6_b1 ^ ssa_8_c1;
	APInt ssa_92_sum_val_bc1 = ssa_91_sum_val_bc0 ^ ssa_85_carry_min_bc_sh;
	APInt ssa_93_ko_bc = ssa_90_known_bc1 & ssa_92_sum_val_bc1;
	APInt ssa_94_sum_val_bc1_not = ssa_13_allones ^ ssa_92_sum_val_bc1;
	APInt ssa_95_kz_bc = ssa_90_known_bc1 & ssa_94_sum_val_bc1_not;
	APInt ssa_96_known_ab = ssa_49_kz_ab | ssa_47_ko_ab;
	APInt ssa_97_max_ab = ssa_13_allones ^ ssa_49_kz_ab;
	APInt ssa_98_sum_min_abc = ssa_47_ko_ab + ssa_8_c1;
	APInt ssa_99_sum_max_abc = ssa_97_max_ab + ssa_26_max_c;
	APInt ssa_100_and_min_abc = ssa_47_ko_ab & ssa_8_c1;
	APInt ssa_101_or_min_abc = ssa_47_ko_ab | ssa_8_c1;
	APInt ssa_102_sum_min_abc_not = ssa_13_allones ^ ssa_98_sum_min_abc;
	APInt ssa_103_or_min_abc_and = ssa_101_or_min_abc & ssa_102_sum_min_abc_not;
	APInt ssa_104_carry_min_abc = ssa_100_and_min_abc | ssa_103_or_min_abc_and;
	APInt ssa_105_and_max_abc = ssa_97_max_ab & ssa_26_max_c;
	APInt ssa_106_or_max_abc = ssa_97_max_ab | ssa_26_max_c;
	APInt ssa_107_sum_max_abc_not = ssa_13_allones ^ ssa_99_sum_max_abc;
	APInt ssa_108_or_max_abc_and = ssa_106_or_max_abc & ssa_107_sum_max_abc_not;
	APInt ssa_109_carry_max_abc = ssa_105_and_max_abc | ssa_108_or_max_abc_and;
	APInt ssa_110_carry_min_abc_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_110_carry_min_abc_sh = APInt(ssa_104_carry_min_abc.getBitWidth(), 0);
	} else {
		ssa_110_carry_min_abc_sh = ssa_104_carry_min_abc.shl(ssa_10_const1);
	}
	APInt ssa_111_carry_max_abc_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_111_carry_max_abc_sh = APInt(ssa_109_carry_max_abc.getBitWidth(), 0);
	} else {
		ssa_111_carry_max_abc_sh = ssa_109_carry_max_abc.shl(ssa_10_const1);
	}
	APInt ssa_112_carry_xor_abc = ssa_110_carry_min_abc_sh ^ ssa_111_carry_max_abc_sh;
	APInt ssa_113_carry_known_abc = ssa_13_allones ^ ssa_112_carry_xor_abc;
	APInt ssa_114_known_abc0 = ssa_96_known_ab & ssa_20_known_c;
	APInt ssa_115_known_abc1 = ssa_114_known_abc0 & ssa_113_carry_known_abc;
	APInt ssa_116_sum_val_abc0 = ssa_47_ko_ab ^ ssa_8_c1;
	APInt ssa_117_sum_val_abc1 = ssa_116_sum_val_abc0 ^ ssa_110_carry_min_abc_sh;
	APInt ssa_118_ko_abc = ssa_115_known_abc1 & ssa_117_sum_val_abc1;
	APInt ssa_119_sum_val_abc1_not = ssa_13_allones ^ ssa_117_sum_val_abc1;
	APInt ssa_120_kz_abc = ssa_115_known_abc1 & ssa_119_sum_val_abc1_not;
	APInt ssa_121_known_ac = ssa_72_kz_ac | ssa_70_ko_ac;
	APInt ssa_122_max_ac = ssa_13_allones ^ ssa_72_kz_ac;
	APInt ssa_123_sum_min_acb = ssa_70_ko_ac + ssa_6_b1;
	APInt ssa_124_sum_max_acb = ssa_122_max_ac + ssa_25_max_b;
	APInt ssa_125_and_min_acb = ssa_70_ko_ac & ssa_6_b1;
	APInt ssa_126_or_min_acb = ssa_70_ko_ac | ssa_6_b1;
	APInt ssa_127_sum_min_acb_not = ssa_13_allones ^ ssa_123_sum_min_acb;
	APInt ssa_128_or_min_acb_and = ssa_126_or_min_acb & ssa_127_sum_min_acb_not;
	APInt ssa_129_carry_min_acb = ssa_125_and_min_acb | ssa_128_or_min_acb_and;
	APInt ssa_130_and_max_acb = ssa_122_max_ac & ssa_25_max_b;
	APInt ssa_131_or_max_acb = ssa_122_max_ac | ssa_25_max_b;
	APInt ssa_132_sum_max_acb_not = ssa_13_allones ^ ssa_124_sum_max_acb;
	APInt ssa_133_or_max_acb_and = ssa_131_or_max_acb & ssa_132_sum_max_acb_not;
	APInt ssa_134_carry_max_acb = ssa_130_and_max_acb | ssa_133_or_max_acb_and;
	APInt ssa_135_carry_min_acb_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_135_carry_min_acb_sh = APInt(ssa_129_carry_min_acb.getBitWidth(), 0);
	} else {
		ssa_135_carry_min_acb_sh = ssa_129_carry_min_acb.shl(ssa_10_const1);
	}
	APInt ssa_136_carry_max_acb_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_136_carry_max_acb_sh = APInt(ssa_134_carry_max_acb.getBitWidth(), 0);
	} else {
		ssa_136_carry_max_acb_sh = ssa_134_carry_max_acb.shl(ssa_10_const1);
	}
	APInt ssa_137_carry_xor_acb = ssa_135_carry_min_acb_sh ^ ssa_136_carry_max_acb_sh;
	APInt ssa_138_carry_known_acb = ssa_13_allones ^ ssa_137_carry_xor_acb;
	APInt ssa_139_known_acb0 = ssa_121_known_ac & ssa_19_known_b;
	APInt ssa_140_known_acb1 = ssa_139_known_acb0 & ssa_138_carry_known_acb;
	APInt ssa_141_sum_val_acb0 = ssa_70_ko_ac ^ ssa_6_b1;
	APInt ssa_142_sum_val_acb1 = ssa_141_sum_val_acb0 ^ ssa_135_carry_min_acb_sh;
	APInt ssa_143_ko_acb = ssa_140_known_acb1 & ssa_142_sum_val_acb1;
	APInt ssa_144_sum_val_acb1_not = ssa_13_allones ^ ssa_142_sum_val_acb1;
	APInt ssa_145_kz_acb = ssa_140_known_acb1 & ssa_144_sum_val_acb1_not;
	APInt ssa_146_known_bc = ssa_95_kz_bc | ssa_93_ko_bc;
	APInt ssa_147_max_bc = ssa_13_allones ^ ssa_95_kz_bc;
	APInt ssa_148_sum_min_bca = ssa_93_ko_bc + ssa_4_a1;
	APInt ssa_149_sum_max_bca = ssa_147_max_bc + ssa_24_max_a;
	APInt ssa_150_and_min_bca = ssa_93_ko_bc & ssa_4_a1;
	APInt ssa_151_or_min_bca = ssa_93_ko_bc | ssa_4_a1;
	APInt ssa_152_sum_min_bca_not = ssa_13_allones ^ ssa_148_sum_min_bca;
	APInt ssa_153_or_min_bca_and = ssa_151_or_min_bca & ssa_152_sum_min_bca_not;
	APInt ssa_154_carry_min_bca = ssa_150_and_min_bca | ssa_153_or_min_bca_and;
	APInt ssa_155_and_max_bca = ssa_147_max_bc & ssa_24_max_a;
	APInt ssa_156_or_max_bca = ssa_147_max_bc | ssa_24_max_a;
	APInt ssa_157_sum_max_bca_not = ssa_13_allones ^ ssa_149_sum_max_bca;
	APInt ssa_158_or_max_bca_and = ssa_156_or_max_bca & ssa_157_sum_max_bca_not;
	APInt ssa_159_carry_max_bca = ssa_155_and_max_bca | ssa_158_or_max_bca_and;
	APInt ssa_160_carry_min_bca_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_160_carry_min_bca_sh = APInt(ssa_154_carry_min_bca.getBitWidth(), 0);
	} else {
		ssa_160_carry_min_bca_sh = ssa_154_carry_min_bca.shl(ssa_10_const1);
	}
	APInt ssa_161_carry_max_bca_sh;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_161_carry_max_bca_sh = APInt(ssa_159_carry_max_bca.getBitWidth(), 0);
	} else {
		ssa_161_carry_max_bca_sh = ssa_159_carry_max_bca.shl(ssa_10_const1);
	}
	APInt ssa_162_carry_xor_bca = ssa_160_carry_min_bca_sh ^ ssa_161_carry_max_bca_sh;
	APInt ssa_163_carry_known_bca = ssa_13_allones ^ ssa_162_carry_xor_bca;
	APInt ssa_164_known_bca0 = ssa_146_known_bc & ssa_18_known_a;
	APInt ssa_165_known_bca1 = ssa_164_known_bca0 & ssa_163_carry_known_bca;
	APInt ssa_166_sum_val_bca0 = ssa_93_ko_bc ^ ssa_4_a1;
	APInt ssa_167_sum_val_bca1 = ssa_166_sum_val_bca0 ^ ssa_160_carry_min_bca_sh;
	APInt ssa_168_ko_bca = ssa_165_known_bca1 & ssa_167_sum_val_bca1;
	APInt ssa_169_sum_val_bca1_not = ssa_13_allones ^ ssa_167_sum_val_bca1;
	APInt ssa_170_kz_bca = ssa_165_known_bca1 & ssa_169_sum_val_bca1_not;
	APInt ssa_171_res_kz0 = ssa_120_kz_abc | ssa_145_kz_acb;
	APInt ssa_172_res_ko0 = ssa_118_ko_abc | ssa_143_ko_acb;
	APInt ssa_173_res_kz1 = ssa_171_res_kz0 | ssa_170_kz_bca;
	APInt ssa_174_res_ko1 = ssa_172_res_ko0 | ssa_168_ko_bca;
	APInt ssa_175_a_low2 = ssa_3_a0 & ssa_12_const3;
	APInt ssa_176_b_low2 = ssa_5_b0 & ssa_12_const3;
	APInt ssa_177_c_low2 = ssa_7_c0 & ssa_12_const3;
	bool ssa_178_a_low2_zero = ssa_175_a_low2.eq(ssa_12_const3);
	bool ssa_179_b_low2_zero = ssa_176_b_low2.eq(ssa_12_const3);
	bool ssa_180_c_low2_zero = ssa_177_c_low2.eq(ssa_12_const3);
	bool ssa_181_a_is_min2 = ssa_4_a1.eq(ssa_17_sign_plus2);
	bool ssa_182_b_is_min2 = ssa_6_b1.eq(ssa_17_sign_plus2);
	bool ssa_183_c_is_min2 = ssa_8_c1.eq(ssa_17_sign_plus2);
	bool ssa_184_cond_a0 = ssa_21_allknown_a && ssa_181_a_is_min2;
	bool ssa_185_cond_a1 = ssa_184_cond_a0 && ssa_179_b_low2_zero;
	bool ssa_186_cond_a = ssa_185_cond_a1 && ssa_180_c_low2_zero;
	bool ssa_187_cond_b0 = ssa_22_allknown_b && ssa_182_b_is_min2;
	bool ssa_188_cond_b1 = ssa_187_cond_b0 && ssa_178_a_low2_zero;
	bool ssa_189_cond_b = ssa_188_cond_b1 && ssa_180_c_low2_zero;
	bool ssa_190_cond_c0 = ssa_23_allknown_c && ssa_183_c_is_min2;
	bool ssa_191_cond_c1 = ssa_190_cond_c0 && ssa_178_a_low2_zero;
	bool ssa_192_cond_c = ssa_191_cond_c1 && ssa_179_b_low2_zero;
	bool ssa_193_cond_ab = ssa_186_cond_a || ssa_189_cond_b;
	bool ssa_194_cond_any = ssa_193_cond_ab || ssa_192_cond_c;
	APInt ssa_195_sign_known = ssa_194_cond_any ? ssa_16_signmask : ssa_9_const0;
	APInt ssa_196_res_ko2 = ssa_174_res_ko1 | ssa_195_sign_known;
	std::array<APInt, 2> ssa_197_r = std::array<APInt, 2>{ssa_173_res_kz1, ssa_196_res_ko2};
	return ssa_197_r;
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
