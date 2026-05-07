namespace Pattern032 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_s0 = ssa_0_arg0[0];
	APInt ssa_4_s1 = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_all1 = APInt::getAllOnes(ssa_3_s0.getBitWidth());
	APInt ssa_10_zero(ssa_3_s0.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_s0.getBitWidth(), 1);
	APInt ssa_12_sign_mask = ssa_10_zero;
	if (ssa_11_one.ule(ssa_11_one.getBitWidth()))
		ssa_12_sign_mask.setHighBits(ssa_11_one.getZExtValue());
	else
		ssa_12_sign_mask.setHighBits(ssa_11_one.getBitWidth());
	APInt ssa_13_max_shift = ssa_3_s0 ^ ssa_9_all1;
	APInt ssa_14_max1 = ssa_5_a1z ^ ssa_9_all1;
	APInt ssa_15_max2 = ssa_7_a2z ^ ssa_9_all1;
	APInt ssa_16_min_sum = ssa_6_a1o + ssa_8_a2o;
	APInt ssa_17_sum_max_raw = ssa_14_max1 + ssa_15_max2;
	bool ssa_18_ov = ssa_17_sum_max_raw.ult(ssa_14_max1);
	APInt ssa_19_max_sum0 = ssa_18_ov ? ssa_9_all1 : ssa_17_sum_max_raw;
	APInt ssa_20_a1_signz = ssa_5_a1z & ssa_12_sign_mask;
	APInt ssa_21_a2_signz = ssa_7_a2z & ssa_12_sign_mask;
	bool ssa_22_a1_nonneg = ssa_20_a1_signz.eq(ssa_12_sign_mask);
	bool ssa_23_a2_nonneg = ssa_21_a2_signz.eq(ssa_12_sign_mask);
	bool ssa_24_both_nonneg = ssa_22_a1_nonneg && ssa_23_a2_nonneg;
	APInt ssa_25_max_signed = ssa_9_all1 ^ ssa_12_sign_mask;
	APInt ssa_26_max_sum_nn = APIntOps::umin(ssa_19_max_sum0, ssa_25_max_signed);
	APInt ssa_27_max_sum = ssa_24_both_nonneg ? ssa_26_max_sum_nn : ssa_19_max_sum0;
	APInt ssa_28_diff_sum = ssa_16_min_sum ^ ssa_27_max_sum;
	APInt ssa_29_lz_sum(ssa_28_diff_sum.getBitWidth(), ssa_28_diff_sum.countl_zero());
	APInt ssa_30_mask_sum_prefix = ssa_10_zero;
	if (ssa_29_lz_sum.ule(ssa_29_lz_sum.getBitWidth()))
		ssa_30_mask_sum_prefix.setHighBits(ssa_29_lz_sum.getZExtValue());
	else
		ssa_30_mask_sum_prefix.setHighBits(ssa_29_lz_sum.getBitWidth());
	APInt ssa_31_sum_known_one_prefix = ssa_30_mask_sum_prefix & ssa_16_min_sum;
	APInt ssa_32_not_min_sum = ssa_16_min_sum ^ ssa_9_all1;
	APInt ssa_33_sum_known_zero_prefix = ssa_30_mask_sum_prefix & ssa_32_not_min_sum;
	APInt ssa_34_min_res;
	if (ssa_13_max_shift.uge(ssa_13_max_shift.getBitWidth())) {
		ssa_34_min_res = APInt(ssa_16_min_sum.getBitWidth(), 0);
	} else {
		ssa_34_min_res = ssa_16_min_sum.lshr(ssa_13_max_shift);
	}
	APInt ssa_35_max_res;
	if (ssa_4_s1.uge(ssa_4_s1.getBitWidth())) {
		ssa_35_max_res = APInt(ssa_27_max_sum.getBitWidth(), 0);
	} else {
		ssa_35_max_res = ssa_27_max_sum.lshr(ssa_4_s1);
	}
	APInt ssa_36_diff = ssa_34_min_res ^ ssa_35_max_res;
	APInt ssa_37_lz(ssa_36_diff.getBitWidth(), ssa_36_diff.countl_zero());
	APInt ssa_38_mask_prefix = ssa_10_zero;
	if (ssa_37_lz.ule(ssa_37_lz.getBitWidth()))
		ssa_38_mask_prefix.setHighBits(ssa_37_lz.getZExtValue());
	else
		ssa_38_mask_prefix.setHighBits(ssa_37_lz.getBitWidth());
	APInt ssa_39_known_one_prefix = ssa_38_mask_prefix & ssa_34_min_res;
	APInt ssa_40_not_min_res = ssa_34_min_res ^ ssa_9_all1;
	APInt ssa_41_known_zero_prefix = ssa_38_mask_prefix & ssa_40_not_min_res;
	APInt ssa_42_kz_base = ssa_10_zero;
	if (ssa_4_s1.ule(ssa_4_s1.getBitWidth()))
		ssa_42_kz_base.setHighBits(ssa_4_s1.getZExtValue());
	else
		ssa_42_kz_base.setHighBits(ssa_4_s1.getBitWidth());
	APInt ssa_43_tz1(ssa_5_a1z.getBitWidth(), ssa_5_a1z.countr_one());
	APInt ssa_44_tz2(ssa_7_a2z.getBitWidth(), ssa_7_a2z.countr_one());
	APInt ssa_45_k = APIntOps::umin(ssa_43_tz1, ssa_44_tz2);
	APInt ssa_46_known_mask = ssa_3_s0 | ssa_4_s1;
	bool ssa_47_shift_exact = ssa_46_known_mask.eq(ssa_9_all1);
	bool ssa_48_s_le_k = ssa_4_s1.ule(ssa_45_k);
	APInt ssa_49_k_minus_s = ssa_45_k - ssa_4_s1;
	APInt ssa_50_t = ssa_48_s_le_k ? ssa_49_k_minus_s : ssa_10_zero;
	APInt ssa_51_low_zero = ssa_10_zero;
	if (ssa_50_t.ule(ssa_50_t.getBitWidth()))
		ssa_51_low_zero.setLowBits(ssa_50_t.getZExtValue());
	else
		ssa_51_low_zero.setLowBits(ssa_50_t.getBitWidth());
	APInt ssa_52_low_zero_sel = ssa_47_shift_exact ? ssa_51_low_zero : ssa_10_zero;
	APInt ssa_53_known_mask1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_54_known_mask2 = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_55_known_mask12 = ssa_53_known_mask1 & ssa_54_known_mask2;
	APInt ssa_56_kbits(ssa_55_known_mask12.getBitWidth(), ssa_55_known_mask12.countr_one());
	APInt ssa_57_mask_low = ssa_10_zero;
	if (ssa_56_kbits.ule(ssa_56_kbits.getBitWidth()))
		ssa_57_mask_low.setLowBits(ssa_56_kbits.getZExtValue());
	else
		ssa_57_mask_low.setLowBits(ssa_56_kbits.getBitWidth());
	APInt ssa_58_low1 = ssa_6_a1o & ssa_57_mask_low;
	APInt ssa_59_low2 = ssa_8_a2o & ssa_57_mask_low;
	APInt ssa_60_sum_low = ssa_58_low1 + ssa_59_low2;
	APInt ssa_61_known_one_low = ssa_60_sum_low & ssa_57_mask_low;
	APInt ssa_62_not_sum_low = ssa_60_sum_low ^ ssa_9_all1;
	APInt ssa_63_known_zero_low = ssa_62_not_sum_low & ssa_57_mask_low;
	APInt ssa_64_known_one_low_shift;
	if (ssa_4_s1.uge(ssa_4_s1.getBitWidth())) {
		ssa_64_known_one_low_shift = APInt(ssa_61_known_one_low.getBitWidth(), 0);
	} else {
		ssa_64_known_one_low_shift = ssa_61_known_one_low.lshr(ssa_4_s1);
	}
	APInt ssa_65_known_zero_low_shift;
	if (ssa_4_s1.uge(ssa_4_s1.getBitWidth())) {
		ssa_65_known_zero_low_shift = APInt(ssa_63_known_zero_low.getBitWidth(), 0);
	} else {
		ssa_65_known_zero_low_shift = ssa_63_known_zero_low.lshr(ssa_4_s1);
	}
	APInt ssa_66_known_one_low_sel = ssa_47_shift_exact ? ssa_64_known_one_low_shift : ssa_10_zero;
	APInt ssa_67_known_zero_low_sel = ssa_47_shift_exact ? ssa_65_known_zero_low_shift : ssa_10_zero;
	APInt ssa_68_sum_known_one_shift;
	if (ssa_4_s1.uge(ssa_4_s1.getBitWidth())) {
		ssa_68_sum_known_one_shift = APInt(ssa_31_sum_known_one_prefix.getBitWidth(), 0);
	} else {
		ssa_68_sum_known_one_shift = ssa_31_sum_known_one_prefix.lshr(ssa_4_s1);
	}
	APInt ssa_69_sum_known_zero_shift;
	if (ssa_4_s1.uge(ssa_4_s1.getBitWidth())) {
		ssa_69_sum_known_zero_shift = APInt(ssa_33_sum_known_zero_prefix.getBitWidth(), 0);
	} else {
		ssa_69_sum_known_zero_shift = ssa_33_sum_known_zero_prefix.lshr(ssa_4_s1);
	}
	APInt ssa_70_sum_known_one_sel = ssa_47_shift_exact ? ssa_68_sum_known_one_shift : ssa_10_zero;
	APInt ssa_71_sum_known_zero_sel = ssa_47_shift_exact ? ssa_69_sum_known_zero_shift : ssa_10_zero;
	APInt ssa_72_res0_pre = ssa_42_kz_base | ssa_41_known_zero_prefix;
	APInt ssa_73_res0_mid = ssa_72_res0_pre | ssa_52_low_zero_sel;
	APInt ssa_74_res0_mid2 = ssa_73_res0_mid | ssa_67_known_zero_low_sel;
	APInt ssa_75_res0 = ssa_74_res0_mid2 | ssa_71_sum_known_zero_sel;
	APInt ssa_76_res1_mid = ssa_39_known_one_prefix | ssa_66_known_one_low_sel;
	APInt ssa_77_res1 = ssa_76_res1_mid | ssa_70_sum_known_one_sel;
	std::array<APInt, 2> ssa_78_r = std::array<APInt, 2>{ssa_75_res0, ssa_77_res1};
	return ssa_78_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_s0 = ssa_0_arg0[0];
	APInt ssa_4_s1 = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_all1 = APInt::getAllOnes(ssa_3_s0.getBitWidth());
	APInt ssa_10_zero(ssa_3_s0.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_s0.getBitWidth(), 1);
	bool ssa_12_true = -1;
	APInt ssa_13_sign_mask = ssa_10_zero;
	if (ssa_11_one.ule(ssa_11_one.getBitWidth()))
		ssa_13_sign_mask.setHighBits(ssa_11_one.getZExtValue());
	else
		ssa_13_sign_mask.setHighBits(ssa_11_one.getBitWidth());
	APInt ssa_14_sign_mask_not = ssa_13_sign_mask ^ ssa_9_all1;
	APInt ssa_15_lsb_mask = ssa_10_zero;
	if (ssa_11_one.ule(ssa_11_one.getBitWidth()))
		ssa_15_lsb_mask.setLowBits(ssa_11_one.getZExtValue());
	else
		ssa_15_lsb_mask.setLowBits(ssa_11_one.getBitWidth());
	APInt ssa_16_all1_xor_lsb = ssa_9_all1 ^ ssa_15_lsb_mask;
	APInt ssa_17_max_signed = ssa_9_all1 ^ ssa_13_sign_mask;
	APInt ssa_18_max1 = ssa_5_a1z ^ ssa_9_all1;
	APInt ssa_19_max2 = ssa_7_a2z ^ ssa_9_all1;
	APInt ssa_20_a1_signz = ssa_5_a1z & ssa_13_sign_mask;
	APInt ssa_21_a1_signo = ssa_6_a1o & ssa_13_sign_mask;
	APInt ssa_22_a2_signz = ssa_7_a2z & ssa_13_sign_mask;
	APInt ssa_23_a2_signo = ssa_8_a2o & ssa_13_sign_mask;
	bool ssa_24_a1_sign_zero = ssa_20_a1_signz.eq(ssa_13_sign_mask);
	bool ssa_25_a1_sign_one = ssa_21_a1_signo.eq(ssa_13_sign_mask);
	bool ssa_26_a2_sign_zero = ssa_22_a2_signz.eq(ssa_13_sign_mask);
	bool ssa_27_a2_sign_one = ssa_23_a2_signo.eq(ssa_13_sign_mask);
	bool ssa_28_a1_nonneg_possible = ssa_25_a1_sign_one != ssa_12_true;
	bool ssa_29_a1_neg_possible = ssa_24_a1_sign_zero != ssa_12_true;
	bool ssa_30_a2_nonneg_possible = ssa_27_a2_sign_one != ssa_12_true;
	bool ssa_31_a2_neg_possible = ssa_26_a2_sign_zero != ssa_12_true;
	bool ssa_32_case_nn = ssa_28_a1_nonneg_possible && ssa_30_a2_nonneg_possible;
	bool ssa_33_case_np = ssa_28_a1_nonneg_possible && ssa_31_a2_neg_possible;
	bool ssa_34_case_pn = ssa_29_a1_neg_possible && ssa_30_a2_nonneg_possible;
	bool ssa_35_case_pp = ssa_29_a1_neg_possible && ssa_31_a2_neg_possible;
	APInt ssa_36_a1_min_nonneg = ssa_6_a1o & ssa_14_sign_mask_not;
	APInt ssa_37_a1_max_nonneg = ssa_18_max1 & ssa_14_sign_mask_not;
	APInt ssa_38_a1_min_neg = ssa_6_a1o | ssa_13_sign_mask;
	APInt ssa_39_a1_max_neg = ssa_18_max1 | ssa_13_sign_mask;
	APInt ssa_40_a2_min_nonneg = ssa_8_a2o & ssa_14_sign_mask_not;
	APInt ssa_41_a2_max_nonneg = ssa_19_max2 & ssa_14_sign_mask_not;
	APInt ssa_42_a2_min_neg = ssa_8_a2o | ssa_13_sign_mask;
	APInt ssa_43_a2_max_neg = ssa_19_max2 | ssa_13_sign_mask;
	APInt ssa_44_min_nn = ssa_36_a1_min_nonneg + ssa_40_a2_min_nonneg;
	APInt ssa_45_max_nn_raw = ssa_37_a1_max_nonneg + ssa_41_a2_max_nonneg;
	bool ssa_46_ov_max_nn = ssa_45_max_nn_raw.ult(ssa_37_a1_max_nonneg);
	APInt ssa_47_max_nn0 = ssa_46_ov_max_nn ? ssa_9_all1 : ssa_45_max_nn_raw;
	APInt ssa_48_max_nn = APIntOps::umin(ssa_47_max_nn0, ssa_17_max_signed);
	bool ssa_49_ov_min_nn = ssa_44_min_nn.ult(ssa_36_a1_min_nonneg);
	bool ssa_50_min_nn_ov_ok = ssa_49_ov_min_nn != ssa_12_true;
	bool ssa_51_case_nn_ok = ssa_32_case_nn && ssa_50_min_nn_ov_ok;
	APInt ssa_52_min_np = ssa_36_a1_min_nonneg + ssa_42_a2_min_neg;
	APInt ssa_53_max_np_raw = ssa_37_a1_max_nonneg + ssa_43_a2_max_neg;
	bool ssa_54_ov_max_np = ssa_53_max_np_raw.ult(ssa_37_a1_max_nonneg);
	APInt ssa_55_max_np = ssa_54_ov_max_np ? ssa_9_all1 : ssa_53_max_np_raw;
	bool ssa_56_ov_min_np = ssa_52_min_np.ult(ssa_36_a1_min_nonneg);
	bool ssa_57_min_np_ov_ok = ssa_56_ov_min_np != ssa_12_true;
	bool ssa_58_case_np_ok = ssa_33_case_np && ssa_57_min_np_ov_ok;
	APInt ssa_59_min_pn = ssa_38_a1_min_neg + ssa_40_a2_min_nonneg;
	APInt ssa_60_max_pn_raw = ssa_39_a1_max_neg + ssa_41_a2_max_nonneg;
	bool ssa_61_ov_max_pn = ssa_60_max_pn_raw.ult(ssa_39_a1_max_neg);
	APInt ssa_62_max_pn = ssa_61_ov_max_pn ? ssa_9_all1 : ssa_60_max_pn_raw;
	bool ssa_63_ov_min_pn = ssa_59_min_pn.ult(ssa_38_a1_min_neg);
	bool ssa_64_min_pn_ov_ok = ssa_63_ov_min_pn != ssa_12_true;
	bool ssa_65_case_pn_ok = ssa_34_case_pn && ssa_64_min_pn_ov_ok;
	APInt ssa_66_min_pp_raw = ssa_38_a1_min_neg + ssa_42_a2_min_neg;
	APInt ssa_67_min_pp = APIntOps::umax(ssa_66_min_pp_raw, ssa_13_sign_mask);
	APInt ssa_68_max_pp_raw = ssa_39_a1_max_neg + ssa_43_a2_max_neg;
	bool ssa_69_ov_max_pp = ssa_68_max_pp_raw.ult(ssa_39_a1_max_neg);
	APInt ssa_70_max_pp = ssa_69_ov_max_pp ? ssa_9_all1 : ssa_68_max_pp_raw;
	bool ssa_71_ov_min_pp = ssa_66_min_pp_raw.ult(ssa_38_a1_min_neg);
	bool ssa_72_min_pp_ov_ok = ssa_71_ov_min_pp != ssa_12_true;
	bool ssa_73_case_pp_ok = ssa_35_case_pp && ssa_72_min_pp_ov_ok;
	APInt ssa_74_diff_nn = ssa_44_min_nn ^ ssa_48_max_nn;
	APInt ssa_75_lz_nn(ssa_74_diff_nn.getBitWidth(), ssa_74_diff_nn.countl_zero());
	APInt ssa_76_mask_nn = ssa_10_zero;
	if (ssa_75_lz_nn.ule(ssa_75_lz_nn.getBitWidth()))
		ssa_76_mask_nn.setHighBits(ssa_75_lz_nn.getZExtValue());
	else
		ssa_76_mask_nn.setHighBits(ssa_75_lz_nn.getBitWidth());
	APInt ssa_77_known_one_nn = ssa_76_mask_nn & ssa_44_min_nn;
	APInt ssa_78_not_min_nn = ssa_44_min_nn ^ ssa_9_all1;
	APInt ssa_79_known_zero_nn = ssa_76_mask_nn & ssa_78_not_min_nn;
	APInt ssa_80_diff_np = ssa_52_min_np ^ ssa_55_max_np;
	APInt ssa_81_lz_np(ssa_80_diff_np.getBitWidth(), ssa_80_diff_np.countl_zero());
	APInt ssa_82_mask_np = ssa_10_zero;
	if (ssa_81_lz_np.ule(ssa_81_lz_np.getBitWidth()))
		ssa_82_mask_np.setHighBits(ssa_81_lz_np.getZExtValue());
	else
		ssa_82_mask_np.setHighBits(ssa_81_lz_np.getBitWidth());
	APInt ssa_83_known_one_np = ssa_82_mask_np & ssa_52_min_np;
	APInt ssa_84_not_min_np = ssa_52_min_np ^ ssa_9_all1;
	APInt ssa_85_known_zero_np = ssa_82_mask_np & ssa_84_not_min_np;
	APInt ssa_86_diff_pn = ssa_59_min_pn ^ ssa_62_max_pn;
	APInt ssa_87_lz_pn(ssa_86_diff_pn.getBitWidth(), ssa_86_diff_pn.countl_zero());
	APInt ssa_88_mask_pn = ssa_10_zero;
	if (ssa_87_lz_pn.ule(ssa_87_lz_pn.getBitWidth()))
		ssa_88_mask_pn.setHighBits(ssa_87_lz_pn.getZExtValue());
	else
		ssa_88_mask_pn.setHighBits(ssa_87_lz_pn.getBitWidth());
	APInt ssa_89_known_one_pn = ssa_88_mask_pn & ssa_59_min_pn;
	APInt ssa_90_not_min_pn = ssa_59_min_pn ^ ssa_9_all1;
	APInt ssa_91_known_zero_pn = ssa_88_mask_pn & ssa_90_not_min_pn;
	APInt ssa_92_diff_pp = ssa_67_min_pp ^ ssa_70_max_pp;
	APInt ssa_93_lz_pp(ssa_92_diff_pp.getBitWidth(), ssa_92_diff_pp.countl_zero());
	APInt ssa_94_mask_pp = ssa_10_zero;
	if (ssa_93_lz_pp.ule(ssa_93_lz_pp.getBitWidth()))
		ssa_94_mask_pp.setHighBits(ssa_93_lz_pp.getZExtValue());
	else
		ssa_94_mask_pp.setHighBits(ssa_93_lz_pp.getBitWidth());
	APInt ssa_95_known_one_pp = ssa_94_mask_pp & ssa_67_min_pp;
	APInt ssa_96_not_min_pp = ssa_67_min_pp ^ ssa_9_all1;
	APInt ssa_97_known_zero_pp = ssa_94_mask_pp & ssa_96_not_min_pp;
	APInt ssa_98_known_mask1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_99_known_mask2 = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_100_known_mask12 = ssa_98_known_mask1 & ssa_99_known_mask2;
	APInt ssa_101_kbits(ssa_100_known_mask12.getBitWidth(), ssa_100_known_mask12.countr_one());
	APInt ssa_102_mask_low = ssa_10_zero;
	if (ssa_101_kbits.ule(ssa_101_kbits.getBitWidth()))
		ssa_102_mask_low.setLowBits(ssa_101_kbits.getZExtValue());
	else
		ssa_102_mask_low.setLowBits(ssa_101_kbits.getBitWidth());
	APInt ssa_103_low1 = ssa_6_a1o & ssa_102_mask_low;
	APInt ssa_104_low2 = ssa_8_a2o & ssa_102_mask_low;
	APInt ssa_105_sum_low = ssa_103_low1 + ssa_104_low2;
	APInt ssa_106_known_one_low = ssa_105_sum_low & ssa_102_mask_low;
	APInt ssa_107_not_sum_low = ssa_105_sum_low ^ ssa_9_all1;
	APInt ssa_108_known_zero_low = ssa_107_not_sum_low & ssa_102_mask_low;
	APInt ssa_109_known_zero_nn2 = ssa_79_known_zero_nn | ssa_108_known_zero_low;
	APInt ssa_110_known_one_nn2 = ssa_77_known_one_nn | ssa_106_known_one_low;
	APInt ssa_111_known_zero_np2 = ssa_85_known_zero_np | ssa_108_known_zero_low;
	APInt ssa_112_known_one_np2 = ssa_83_known_one_np | ssa_106_known_one_low;
	APInt ssa_113_known_zero_pn2 = ssa_91_known_zero_pn | ssa_108_known_zero_low;
	APInt ssa_114_known_one_pn2 = ssa_89_known_one_pn | ssa_106_known_one_low;
	APInt ssa_115_known_zero_pp2 = ssa_97_known_zero_pp | ssa_108_known_zero_low;
	APInt ssa_116_known_one_pp2 = ssa_95_known_one_pp | ssa_106_known_one_low;
	APInt ssa_117_kz_nn = ssa_51_case_nn_ok ? ssa_109_known_zero_nn2 : ssa_9_all1;
	APInt ssa_118_ko_nn = ssa_51_case_nn_ok ? ssa_110_known_one_nn2 : ssa_9_all1;
	APInt ssa_119_kz_np = ssa_58_case_np_ok ? ssa_111_known_zero_np2 : ssa_9_all1;
	APInt ssa_120_ko_np = ssa_58_case_np_ok ? ssa_112_known_one_np2 : ssa_9_all1;
	APInt ssa_121_kz_pn = ssa_65_case_pn_ok ? ssa_113_known_zero_pn2 : ssa_9_all1;
	APInt ssa_122_ko_pn = ssa_65_case_pn_ok ? ssa_114_known_one_pn2 : ssa_9_all1;
	APInt ssa_123_kz_pp = ssa_73_case_pp_ok ? ssa_115_known_zero_pp2 : ssa_9_all1;
	APInt ssa_124_ko_pp = ssa_73_case_pp_ok ? ssa_116_known_one_pp2 : ssa_9_all1;
	APInt ssa_125_sum_kz0 = ssa_117_kz_nn & ssa_119_kz_np;
	APInt ssa_126_sum_kz1 = ssa_125_sum_kz0 & ssa_121_kz_pn;
	APInt ssa_127_sum_kz = ssa_126_sum_kz1 & ssa_123_kz_pp;
	APInt ssa_128_sum_ko0 = ssa_118_ko_nn & ssa_120_ko_np;
	APInt ssa_129_sum_ko1 = ssa_128_sum_ko0 & ssa_122_ko_pn;
	APInt ssa_130_sum_ko = ssa_129_sum_ko1 & ssa_124_ko_pp;
	APInt ssa_131_known_mask = ssa_3_s0 | ssa_4_s1;
	bool ssa_132_shift_exact = ssa_131_known_mask.eq(ssa_9_all1);
	bool ssa_133_s1_zero = ssa_4_s1.eq(ssa_10_zero);
	bool ssa_134_s0_match = ssa_3_s0.eq(ssa_16_all1_xor_lsb);
	bool ssa_135_shift01 = ssa_133_s1_zero && ssa_134_s0_match;
	APInt ssa_136_kz_shift_exact;
	if (ssa_4_s1.uge(ssa_4_s1.getBitWidth())) {
		ssa_136_kz_shift_exact = APInt(ssa_127_sum_kz.getBitWidth(), 0);
	} else {
		ssa_136_kz_shift_exact = ssa_127_sum_kz.lshr(ssa_4_s1);
	}
	APInt ssa_137_ko_shift_exact;
	if (ssa_4_s1.uge(ssa_4_s1.getBitWidth())) {
		ssa_137_ko_shift_exact = APInt(ssa_130_sum_ko.getBitWidth(), 0);
	} else {
		ssa_137_ko_shift_exact = ssa_130_sum_ko.lshr(ssa_4_s1);
	}
	APInt ssa_138_kz_top_exact = ssa_10_zero;
	if (ssa_4_s1.ule(ssa_4_s1.getBitWidth()))
		ssa_138_kz_top_exact.setHighBits(ssa_4_s1.getZExtValue());
	else
		ssa_138_kz_top_exact.setHighBits(ssa_4_s1.getBitWidth());
	APInt ssa_139_kz_exact = ssa_136_kz_shift_exact | ssa_138_kz_top_exact;
	APInt ssa_140_kz1_shift;
	if (ssa_11_one.uge(ssa_11_one.getBitWidth())) {
		ssa_140_kz1_shift = APInt(ssa_127_sum_kz.getBitWidth(), 0);
	} else {
		ssa_140_kz1_shift = ssa_127_sum_kz.lshr(ssa_11_one);
	}
	APInt ssa_141_ko1_shift;
	if (ssa_11_one.uge(ssa_11_one.getBitWidth())) {
		ssa_141_ko1_shift = APInt(ssa_130_sum_ko.getBitWidth(), 0);
	} else {
		ssa_141_ko1_shift = ssa_130_sum_ko.lshr(ssa_11_one);
	}
	APInt ssa_142_kz1_top = ssa_10_zero;
	if (ssa_11_one.ule(ssa_11_one.getBitWidth()))
		ssa_142_kz1_top.setHighBits(ssa_11_one.getZExtValue());
	else
		ssa_142_kz1_top.setHighBits(ssa_11_one.getBitWidth());
	APInt ssa_143_kz1 = ssa_140_kz1_shift | ssa_142_kz1_top;
	APInt ssa_144_kz_common = ssa_127_sum_kz & ssa_143_kz1;
	APInt ssa_145_ko_common = ssa_130_sum_ko & ssa_141_ko1_shift;
	APInt ssa_146_kz_sel_exact = ssa_132_shift_exact ? ssa_139_kz_exact : ssa_10_zero;
	APInt ssa_147_ko_sel_exact = ssa_132_shift_exact ? ssa_137_ko_shift_exact : ssa_10_zero;
	APInt ssa_148_kz_res = ssa_135_shift01 ? ssa_144_kz_common : ssa_146_kz_sel_exact;
	APInt ssa_149_ko_res = ssa_135_shift01 ? ssa_145_ko_common : ssa_147_ko_sel_exact;
	std::array<APInt, 2> ssa_150_r = std::array<APInt, 2>{ssa_148_kz_res, ssa_149_ko_res};
	return ssa_150_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_s0 = ssa_0_arg0[0];
	APInt ssa_4_s1 = ssa_0_arg0[1];
	APInt ssa_5_all1 = APInt::getAllOnes(ssa_3_s0.getBitWidth());
	APInt ssa_6_zero(ssa_3_s0.getBitWidth(), 0);
	APInt ssa_7_one(ssa_3_s0.getBitWidth(), 1);
	APInt ssa_8_known_mask = ssa_3_s0 | ssa_4_s1;
	bool ssa_9_shift_exact = ssa_8_known_mask.eq(ssa_5_all1);
	APInt ssa_10_bw(ssa_3_s0.getBitWidth(), ssa_3_s0.getBitWidth());
	bool ssa_11_shift_ge_bw = ssa_4_s1.uge(ssa_10_bw);
	bool ssa_12_exact_bad = ssa_9_shift_exact && ssa_11_shift_ge_bw;
	APInt ssa_13_sign_mask = ssa_6_zero;
	if (ssa_7_one.ule(ssa_7_one.getBitWidth()))
		ssa_13_sign_mask.setHighBits(ssa_7_one.getZExtValue());
	else
		ssa_13_sign_mask.setHighBits(ssa_7_one.getBitWidth());
	APInt ssa_14_msb1 = ssa_4_s1 & ssa_13_sign_mask;
	bool ssa_15_msb_known_one = ssa_14_msb1.eq(ssa_13_sign_mask);
	bool ssa_16_bad = ssa_15_msb_known_one || ssa_12_exact_bad;
	APInt ssa_17_kz = ssa_16_bad ? ssa_5_all1 : ssa_6_zero;
	APInt ssa_18_ko = ssa_16_bad ? ssa_5_all1 : ssa_6_zero;
	std::array<APInt, 2> ssa_19_r = std::array<APInt, 2>{ssa_17_kz, ssa_18_ko};
	return ssa_19_r;
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
