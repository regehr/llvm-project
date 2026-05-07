namespace Pattern020 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_10_am = ssa_3_a0 | ssa_4_a1;
	APInt ssa_11_bm = ssa_5_b0 | ssa_6_b1;
	APInt ssa_12_cm = ssa_7_c0 | ssa_8_c1;
	APInt ssa_13_tka(ssa_10_am.getBitWidth(), ssa_10_am.countr_one());
	APInt ssa_14_tkb(ssa_11_bm.getBitWidth(), ssa_11_bm.countr_one());
	APInt ssa_15_tkc(ssa_12_cm.getBitWidth(), ssa_12_cm.countr_one());
	APInt ssa_16_tbc = APIntOps::umin(ssa_14_tkb, ssa_15_tkc);
	APInt ssa_17_k = APIntOps::umin(ssa_13_tka, ssa_16_tbc);
	APInt ssa_18_mask_low = ssa_9_const0;
	if (ssa_17_k.ule(ssa_17_k.getBitWidth()))
		ssa_18_mask_low.setLowBits(ssa_17_k.getZExtValue());
	else
		ssa_18_mask_low.setLowBits(ssa_17_k.getBitWidth());
	APInt ssa_19_low_a = ssa_4_a1 & ssa_18_mask_low;
	APInt ssa_20_low_b = ssa_6_b1 & ssa_18_mask_low;
	APInt ssa_21_low_c = ssa_8_c1 & ssa_18_mask_low;
	APInt ssa_22_sub_low = ssa_21_low_c - ssa_20_low_b;
	APInt ssa_23_sum_low = ssa_19_low_a + ssa_22_sub_low;
	APInt ssa_24_sum_low_bits = ssa_23_sum_low & ssa_18_mask_low;
	APInt ssa_25_res0 = ssa_24_sum_low_bits ^ ssa_18_mask_low;
	APInt ssa_26_res1 = ssa_24_sum_low_bits & ssa_18_mask_low;
	std::array<APInt, 2> ssa_27_r = std::array<APInt, 2>{ssa_25_res0, ssa_26_res1};
	return ssa_27_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_10_ones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_11_maxA = ssa_10_ones ^ ssa_3_a0;
	APInt ssa_12_maxB = ssa_10_ones ^ ssa_5_b0;
	APInt ssa_13_maxC = ssa_10_ones ^ ssa_7_c0;
	APInt ssa_14_sum_min = ssa_4_a1 + ssa_8_c1;
	APInt ssa_15_sum_max = ssa_11_maxA + ssa_13_maxC;
	APInt ssa_16_minRes = ssa_14_sum_min - ssa_12_maxB;
	APInt ssa_17_maxRes = ssa_15_sum_max - ssa_6_b1;
	APInt ssa_18_diff = ssa_16_minRes ^ ssa_17_maxRes;
	APInt ssa_19_lz(ssa_18_diff.getBitWidth(), ssa_18_diff.countl_zero());
	APInt ssa_20_mask_high = ssa_9_const0;
	if (ssa_19_lz.ule(ssa_19_lz.getBitWidth()))
		ssa_20_mask_high.setHighBits(ssa_19_lz.getZExtValue());
	else
		ssa_20_mask_high.setHighBits(ssa_19_lz.getBitWidth());
	bool ssa_21_true = ssa_9_const0.eq(ssa_9_const0);
	bool ssa_22_overflow_add = ssa_15_sum_max.ult(ssa_11_maxA);
	bool ssa_23_underflow_sub = ssa_14_sum_min.ult(ssa_12_maxB);
	bool ssa_24_bad1 = ssa_22_overflow_add || ssa_23_underflow_sub;
	bool ssa_25_no_overflow = ssa_24_bad1 != ssa_21_true;
	bool ssa_26_wrap = ssa_17_maxRes.ult(ssa_16_minRes);
	bool ssa_27_not_wrap = ssa_26_wrap != ssa_21_true;
	APInt ssa_28_am = ssa_3_a0 | ssa_4_a1;
	APInt ssa_29_bm = ssa_5_b0 | ssa_6_b1;
	APInt ssa_30_cm = ssa_7_c0 | ssa_8_c1;
	APInt ssa_31_cla(ssa_28_am.getBitWidth(), ssa_28_am.countl_one());
	APInt ssa_32_clb(ssa_29_bm.getBitWidth(), ssa_29_bm.countl_one());
	APInt ssa_33_clc(ssa_30_cm.getBitWidth(), ssa_30_cm.countl_one());
	APInt ssa_34_pma = ssa_9_const0;
	if (ssa_31_cla.ule(ssa_31_cla.getBitWidth()))
		ssa_34_pma.setHighBits(ssa_31_cla.getZExtValue());
	else
		ssa_34_pma.setHighBits(ssa_31_cla.getBitWidth());
	APInt ssa_35_pmb = ssa_9_const0;
	if (ssa_32_clb.ule(ssa_32_clb.getBitWidth()))
		ssa_35_pmb.setHighBits(ssa_32_clb.getZExtValue());
	else
		ssa_35_pmb.setHighBits(ssa_32_clb.getBitWidth());
	APInt ssa_36_pmc = ssa_9_const0;
	if (ssa_33_clc.ule(ssa_33_clc.getBitWidth()))
		ssa_36_pmc.setHighBits(ssa_33_clc.getZExtValue());
	else
		ssa_36_pmc.setHighBits(ssa_33_clc.getBitWidth());
	bool ssa_37_eqA = ssa_28_am.eq(ssa_34_pma);
	bool ssa_38_eqB = ssa_29_bm.eq(ssa_35_pmb);
	bool ssa_39_eqC = ssa_30_cm.eq(ssa_36_pmc);
	bool ssa_40_ab = ssa_37_eqA && ssa_38_eqB;
	bool ssa_41_prefix_ok = ssa_40_ab && ssa_39_eqC;
	bool ssa_42_prefix_good = ssa_41_prefix_ok && ssa_27_not_wrap;
	bool ssa_43_good = ssa_25_no_overflow || ssa_42_prefix_good;
	APInt ssa_44_mask_high2 = ssa_43_good ? ssa_20_mask_high : ssa_9_const0;
	APInt ssa_45_not_minRes = ssa_16_minRes ^ ssa_10_ones;
	APInt ssa_46_knownOne_range = ssa_16_minRes & ssa_44_mask_high2;
	APInt ssa_47_knownZero_range = ssa_45_not_minRes & ssa_44_mask_high2;
	APInt ssa_48_tka(ssa_28_am.getBitWidth(), ssa_28_am.countr_one());
	APInt ssa_49_tkb(ssa_29_bm.getBitWidth(), ssa_29_bm.countr_one());
	APInt ssa_50_tkc(ssa_30_cm.getBitWidth(), ssa_30_cm.countr_one());
	APInt ssa_51_tbc = APIntOps::umin(ssa_49_tkb, ssa_50_tkc);
	APInt ssa_52_k = APIntOps::umin(ssa_48_tka, ssa_51_tbc);
	APInt ssa_53_mask_low = ssa_9_const0;
	if (ssa_52_k.ule(ssa_52_k.getBitWidth()))
		ssa_53_mask_low.setLowBits(ssa_52_k.getZExtValue());
	else
		ssa_53_mask_low.setLowBits(ssa_52_k.getBitWidth());
	APInt ssa_54_low_a = ssa_4_a1 & ssa_53_mask_low;
	APInt ssa_55_low_b = ssa_6_b1 & ssa_53_mask_low;
	APInt ssa_56_low_c = ssa_8_c1 & ssa_53_mask_low;
	APInt ssa_57_sub_low = ssa_56_low_c - ssa_55_low_b;
	APInt ssa_58_sum_low = ssa_54_low_a + ssa_57_sub_low;
	APInt ssa_59_sum_low_bits = ssa_58_sum_low & ssa_53_mask_low;
	APInt ssa_60_res0_low = ssa_59_sum_low_bits ^ ssa_53_mask_low;
	APInt ssa_61_res1_low = ssa_59_sum_low_bits & ssa_53_mask_low;
	APInt ssa_62_res0 = ssa_60_res0_low | ssa_47_knownZero_range;
	APInt ssa_63_res1 = ssa_61_res1_low | ssa_46_knownOne_range;
	std::array<APInt, 2> ssa_64_r = std::array<APInt, 2>{ssa_62_res0, ssa_63_res1};
	return ssa_64_r;
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
	APInt ssa_11_ones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_12_maxA = ssa_11_ones ^ ssa_3_a0;
	APInt ssa_13_maxB = ssa_11_ones ^ ssa_5_b0;
	APInt ssa_14_maxC = ssa_11_ones ^ ssa_7_c0;
	APInt ssa_15_sum_min = ssa_4_a1 + ssa_8_c1;
	APInt ssa_16_sum_max = ssa_12_maxA + ssa_14_maxC;
	APInt ssa_17_minRes = ssa_15_sum_min - ssa_13_maxB;
	APInt ssa_18_maxRes = ssa_16_sum_max - ssa_6_b1;
	APInt ssa_19_diff = ssa_17_minRes ^ ssa_18_maxRes;
	APInt ssa_20_lz(ssa_19_diff.getBitWidth(), ssa_19_diff.countl_zero());
	APInt ssa_21_mask_high = ssa_9_const0;
	if (ssa_20_lz.ule(ssa_20_lz.getBitWidth()))
		ssa_21_mask_high.setHighBits(ssa_20_lz.getZExtValue());
	else
		ssa_21_mask_high.setHighBits(ssa_20_lz.getBitWidth());
	bool ssa_22_true = ssa_9_const0.eq(ssa_9_const0);
	bool ssa_23_overflow_add = ssa_16_sum_max.ult(ssa_12_maxA);
	bool ssa_24_underflow_sub = ssa_15_sum_min.ult(ssa_13_maxB);
	bool ssa_25_bad1 = ssa_23_overflow_add || ssa_24_underflow_sub;
	bool ssa_26_no_overflow = ssa_25_bad1 != ssa_22_true;
	bool ssa_27_wrap = ssa_18_maxRes.ult(ssa_17_minRes);
	bool ssa_28_not_wrap = ssa_27_wrap != ssa_22_true;
	APInt ssa_29_am = ssa_3_a0 | ssa_4_a1;
	APInt ssa_30_bm = ssa_5_b0 | ssa_6_b1;
	APInt ssa_31_cm = ssa_7_c0 | ssa_8_c1;
	APInt ssa_32_cla(ssa_29_am.getBitWidth(), ssa_29_am.countl_one());
	APInt ssa_33_clb(ssa_30_bm.getBitWidth(), ssa_30_bm.countl_one());
	APInt ssa_34_clc(ssa_31_cm.getBitWidth(), ssa_31_cm.countl_one());
	APInt ssa_35_pma = ssa_9_const0;
	if (ssa_32_cla.ule(ssa_32_cla.getBitWidth()))
		ssa_35_pma.setHighBits(ssa_32_cla.getZExtValue());
	else
		ssa_35_pma.setHighBits(ssa_32_cla.getBitWidth());
	APInt ssa_36_pmb = ssa_9_const0;
	if (ssa_33_clb.ule(ssa_33_clb.getBitWidth()))
		ssa_36_pmb.setHighBits(ssa_33_clb.getZExtValue());
	else
		ssa_36_pmb.setHighBits(ssa_33_clb.getBitWidth());
	APInt ssa_37_pmc = ssa_9_const0;
	if (ssa_34_clc.ule(ssa_34_clc.getBitWidth()))
		ssa_37_pmc.setHighBits(ssa_34_clc.getZExtValue());
	else
		ssa_37_pmc.setHighBits(ssa_34_clc.getBitWidth());
	bool ssa_38_eqA = ssa_29_am.eq(ssa_35_pma);
	bool ssa_39_eqB = ssa_30_bm.eq(ssa_36_pmb);
	bool ssa_40_eqC = ssa_31_cm.eq(ssa_37_pmc);
	bool ssa_41_ab = ssa_38_eqA && ssa_39_eqB;
	bool ssa_42_prefix_ok = ssa_41_ab && ssa_40_eqC;
	bool ssa_43_prefix_good = ssa_42_prefix_ok && ssa_28_not_wrap;
	bool ssa_44_good = ssa_26_no_overflow || ssa_43_prefix_good;
	APInt ssa_45_mask_high2 = ssa_44_good ? ssa_21_mask_high : ssa_9_const0;
	APInt ssa_46_not_minRes = ssa_17_minRes ^ ssa_11_ones;
	APInt ssa_47_knownOne_range = ssa_17_minRes & ssa_45_mask_high2;
	APInt ssa_48_knownZero_range = ssa_46_not_minRes & ssa_45_mask_high2;
	APInt ssa_49_tka(ssa_29_am.getBitWidth(), ssa_29_am.countr_one());
	APInt ssa_50_tkb(ssa_30_bm.getBitWidth(), ssa_30_bm.countr_one());
	APInt ssa_51_tkc(ssa_31_cm.getBitWidth(), ssa_31_cm.countr_one());
	APInt ssa_52_tbc = APIntOps::umin(ssa_50_tkb, ssa_51_tkc);
	APInt ssa_53_k = APIntOps::umin(ssa_49_tka, ssa_52_tbc);
	APInt ssa_54_mask_low = ssa_9_const0;
	if (ssa_53_k.ule(ssa_53_k.getBitWidth()))
		ssa_54_mask_low.setLowBits(ssa_53_k.getZExtValue());
	else
		ssa_54_mask_low.setLowBits(ssa_53_k.getBitWidth());
	APInt ssa_55_low_a = ssa_4_a1 & ssa_54_mask_low;
	APInt ssa_56_low_b = ssa_6_b1 & ssa_54_mask_low;
	APInt ssa_57_low_c = ssa_8_c1 & ssa_54_mask_low;
	APInt ssa_58_sub_low = ssa_57_low_c - ssa_56_low_b;
	APInt ssa_59_sum_low = ssa_55_low_a + ssa_58_sub_low;
	APInt ssa_60_sum_low_bits = ssa_59_sum_low & ssa_54_mask_low;
	APInt ssa_61_res0_low = ssa_60_sum_low_bits ^ ssa_54_mask_low;
	APInt ssa_62_res1_low = ssa_60_sum_low_bits & ssa_54_mask_low;
	APInt ssa_63_known_bc = ssa_30_bm & ssa_31_cm;
	APInt ssa_64_neq_bc = ssa_6_b1 ^ ssa_8_c1;
	APInt ssa_65_eq_bc = ssa_64_neq_bc ^ ssa_11_ones;
	APInt ssa_66_eq_known = ssa_65_eq_bc & ssa_63_known_bc;
	APInt ssa_67_tk_eq(ssa_66_eq_known.getBitWidth(), ssa_66_eq_known.countr_one());
	APInt ssa_68_mask_low_eq = ssa_9_const0;
	if (ssa_67_tk_eq.ule(ssa_67_tk_eq.getBitWidth()))
		ssa_68_mask_low_eq.setLowBits(ssa_67_tk_eq.getZExtValue());
	else
		ssa_68_mask_low_eq.setLowBits(ssa_67_tk_eq.getBitWidth());
	APInt ssa_69_res0_eq = ssa_3_a0 & ssa_68_mask_low_eq;
	APInt ssa_70_res1_eq = ssa_4_a1 & ssa_68_mask_low_eq;
	APInt ssa_71_res0_low2 = ssa_61_res0_low | ssa_69_res0_eq;
	APInt ssa_72_res1_low2 = ssa_62_res1_low | ssa_70_res1_eq;
	APInt ssa_73_ac_zero = ssa_3_a0 | ssa_7_c0;
	APInt ssa_74_tcz(ssa_73_ac_zero.getBitWidth(), ssa_73_ac_zero.countr_one());
	APInt ssa_75_mask_carry = ssa_9_const0;
	if (ssa_74_tcz.ule(ssa_74_tcz.getBitWidth()))
		ssa_75_mask_carry.setLowBits(ssa_74_tcz.getZExtValue());
	else
		ssa_75_mask_carry.setLowBits(ssa_74_tcz.getBitWidth());
	APInt ssa_76_ac_known = ssa_29_am & ssa_31_cm;
	APInt ssa_77_mask_ac = ssa_75_mask_carry & ssa_76_ac_known;
	APInt ssa_78_sum_ac = ssa_4_a1 ^ ssa_8_c1;
	APInt ssa_79_not_sum_ac = ssa_78_sum_ac ^ ssa_11_ones;
	APInt ssa_80_knownOne_add = ssa_78_sum_ac & ssa_77_mask_ac;
	APInt ssa_81_knownZero_add = ssa_79_not_sum_ac & ssa_77_mask_ac;
	bool ssa_82_eq_b0_ones = ssa_5_b0.eq(ssa_11_ones);
	bool ssa_83_eq_b1_zero = ssa_6_b1.eq(ssa_9_const0);
	bool ssa_84_b_is_zero = ssa_82_eq_b0_ones && ssa_83_eq_b1_zero;
	APInt ssa_85_knownOne_add_sel = ssa_84_b_is_zero ? ssa_80_knownOne_add : ssa_9_const0;
	APInt ssa_86_knownZero_add_sel = ssa_84_b_is_zero ? ssa_81_knownZero_add : ssa_9_const0;
	APInt ssa_87_ab0 = ssa_3_a0 & ssa_5_b0;
	APInt ssa_88_borrow_zero_mask = ssa_4_a1 | ssa_87_ab0;
	APInt ssa_89_tbz(ssa_88_borrow_zero_mask.getBitWidth(), ssa_88_borrow_zero_mask.countr_one());
	APInt ssa_90_mask_borrow = ssa_9_const0;
	if (ssa_89_tbz.ule(ssa_89_tbz.getBitWidth()))
		ssa_90_mask_borrow.setLowBits(ssa_89_tbz.getZExtValue());
	else
		ssa_90_mask_borrow.setLowBits(ssa_89_tbz.getBitWidth());
	APInt ssa_91_ab_known = ssa_29_am & ssa_30_bm;
	APInt ssa_92_mask_ab = ssa_90_mask_borrow & ssa_91_ab_known;
	APInt ssa_93_sub_bits = ssa_4_a1 ^ ssa_6_b1;
	APInt ssa_94_not_sub_bits = ssa_93_sub_bits ^ ssa_11_ones;
	APInt ssa_95_knownOne_sub = ssa_93_sub_bits & ssa_92_mask_ab;
	APInt ssa_96_knownZero_sub = ssa_94_not_sub_bits & ssa_92_mask_ab;
	bool ssa_97_eq_c0_ones = ssa_7_c0.eq(ssa_11_ones);
	bool ssa_98_eq_c1_zero = ssa_8_c1.eq(ssa_9_const0);
	bool ssa_99_c_is_zero = ssa_97_eq_c0_ones && ssa_98_eq_c1_zero;
	APInt ssa_100_knownOne_sub_sel = ssa_99_c_is_zero ? ssa_95_knownOne_sub : ssa_9_const0;
	APInt ssa_101_knownZero_sub_sel = ssa_99_c_is_zero ? ssa_96_knownZero_sub : ssa_9_const0;
	bool ssa_102_no_underflow = ssa_8_c1.uge(ssa_13_maxB);
	APInt ssa_103_maxD = ssa_14_maxC - ssa_6_b1;
	APInt ssa_104_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_105_lzD(ssa_103_maxD.getBitWidth(), ssa_103_maxD.countl_zero());
	APInt ssa_106_kD = ssa_104_bw - ssa_105_lzD;
	APInt ssa_107_mask_low_kD = ssa_9_const0;
	if (ssa_106_kD.ule(ssa_106_kD.getBitWidth()))
		ssa_107_mask_low_kD.setLowBits(ssa_106_kD.getZExtValue());
	else
		ssa_107_mask_low_kD.setLowBits(ssa_106_kD.getBitWidth());
	APInt ssa_108_maxA_low = ssa_12_maxA & ssa_107_mask_low_kD;
	APInt ssa_109_sum_max_low = ssa_108_maxA_low + ssa_103_maxD;
	bool ssa_110_no_carry_kD = ssa_109_sum_max_low.ule(ssa_107_mask_low_kD);
	bool ssa_111_cond_kD = ssa_102_no_underflow && ssa_110_no_carry_kD;
	APInt ssa_112_kD1 = ssa_106_kD + ssa_10_const1;
	APInt ssa_113_mask_low_kD1 = ssa_9_const0;
	if (ssa_112_kD1.ule(ssa_112_kD1.getBitWidth()))
		ssa_113_mask_low_kD1.setLowBits(ssa_112_kD1.getZExtValue());
	else
		ssa_113_mask_low_kD1.setLowBits(ssa_112_kD1.getBitWidth());
	APInt ssa_114_mask_bit_kD = ssa_113_mask_low_kD1;
	if (ssa_106_kD.ule(ssa_106_kD.getBitWidth()))
		ssa_114_mask_bit_kD.clearLowBits(ssa_106_kD.getZExtValue());
	else
		ssa_114_mask_bit_kD.clearLowBits(ssa_106_kD.getBitWidth());
	APInt ssa_115_bitk_zero = ssa_3_a0 & ssa_114_mask_bit_kD;
	APInt ssa_116_bitk_one = ssa_4_a1 & ssa_114_mask_bit_kD;
	APInt ssa_117_bitk_zero_sel = ssa_111_cond_kD ? ssa_115_bitk_zero : ssa_9_const0;
	APInt ssa_118_bitk_one_sel = ssa_111_cond_kD ? ssa_116_bitk_one : ssa_9_const0;
	bool ssa_119_eq_a1_ones = ssa_4_a1.eq(ssa_11_ones);
	bool ssa_120_eq_a0_zero = ssa_3_a0.eq(ssa_9_const0);
	bool ssa_121_a_all_ones = ssa_119_eq_a1_ones && ssa_120_eq_a0_zero;
	bool ssa_122_no_underflow_gt = ssa_8_c1.ugt(ssa_13_maxB);
	bool ssa_123_cond_all_ones = ssa_121_a_all_ones && ssa_122_no_underflow_gt;
	APInt ssa_124_minD = ssa_8_c1 - ssa_13_maxB;
	APInt ssa_125_minD1 = ssa_124_minD - ssa_10_const1;
	APInt ssa_126_maxD1 = ssa_103_maxD - ssa_10_const1;
	APInt ssa_127_diffD1 = ssa_125_minD1 ^ ssa_126_maxD1;
	APInt ssa_128_lzD1(ssa_127_diffD1.getBitWidth(), ssa_127_diffD1.countl_zero());
	APInt ssa_129_mask_high_D1 = ssa_9_const0;
	if (ssa_128_lzD1.ule(ssa_128_lzD1.getBitWidth()))
		ssa_129_mask_high_D1.setHighBits(ssa_128_lzD1.getZExtValue());
	else
		ssa_129_mask_high_D1.setHighBits(ssa_128_lzD1.getBitWidth());
	APInt ssa_130_not_minD1 = ssa_125_minD1 ^ ssa_11_ones;
	APInt ssa_131_knownOne_D1 = ssa_125_minD1 & ssa_129_mask_high_D1;
	APInt ssa_132_knownZero_D1 = ssa_130_not_minD1 & ssa_129_mask_high_D1;
	APInt ssa_133_knownOne_D1_sel = ssa_123_cond_all_ones ? ssa_131_knownOne_D1 : ssa_9_const0;
	APInt ssa_134_knownZero_D1_sel = ssa_123_cond_all_ones ? ssa_132_knownZero_D1 : ssa_9_const0;
	bool ssa_135_eq_a_known = ssa_29_am.eq(ssa_11_ones);
	bool ssa_136_eq_b_known = ssa_30_bm.eq(ssa_11_ones);
	bool ssa_137_eq_a_b_val = ssa_4_a1.eq(ssa_6_b1);
	bool ssa_138_ab_known2 = ssa_135_eq_a_known && ssa_136_eq_b_known;
	bool ssa_139_a_eq_b = ssa_138_ab_known2 && ssa_137_eq_a_b_val;
	APInt ssa_140_res0_eqab = ssa_139_a_eq_b ? ssa_7_c0 : ssa_9_const0;
	APInt ssa_141_res1_eqab = ssa_139_a_eq_b ? ssa_8_c1 : ssa_9_const0;
	APInt ssa_142_mask_low_tka = ssa_9_const0;
	if (ssa_49_tka.ule(ssa_49_tka.getBitWidth()))
		ssa_142_mask_low_tka.setLowBits(ssa_49_tka.getZExtValue());
	else
		ssa_142_mask_low_tka.setLowBits(ssa_49_tka.getBitWidth());
	APInt ssa_143_mask_a_range = ssa_142_mask_low_tka;
	if (ssa_106_kD.ule(ssa_106_kD.getBitWidth()))
		ssa_143_mask_a_range.clearLowBits(ssa_106_kD.getZExtValue());
	else
		ssa_143_mask_a_range.clearLowBits(ssa_106_kD.getBitWidth());
	APInt ssa_144_a_range_zero = ssa_3_a0 & ssa_143_mask_a_range;
	APInt ssa_145_a_range_one = ssa_4_a1 & ssa_143_mask_a_range;
	APInt ssa_146_a_range_zero_sel = ssa_111_cond_kD ? ssa_144_a_range_zero : ssa_9_const0;
	APInt ssa_147_a_range_one_sel = ssa_111_cond_kD ? ssa_145_a_range_one : ssa_9_const0;
	APInt ssa_148_res0a = ssa_71_res0_low2 | ssa_48_knownZero_range;
	APInt ssa_149_res1a = ssa_72_res1_low2 | ssa_47_knownOne_range;
	APInt ssa_150_res0b = ssa_148_res0a | ssa_86_knownZero_add_sel;
	APInt ssa_151_res1b = ssa_149_res1a | ssa_85_knownOne_add_sel;
	APInt ssa_152_res0c = ssa_150_res0b | ssa_101_knownZero_sub_sel;
	APInt ssa_153_res1c = ssa_151_res1b | ssa_100_knownOne_sub_sel;
	APInt ssa_154_res0d = ssa_152_res0c | ssa_117_bitk_zero_sel;
	APInt ssa_155_res1d = ssa_153_res1c | ssa_118_bitk_one_sel;
	APInt ssa_156_res0e = ssa_154_res0d | ssa_134_knownZero_D1_sel;
	APInt ssa_157_res1e = ssa_155_res1d | ssa_133_knownOne_D1_sel;
	APInt ssa_158_res0f = ssa_156_res0e | ssa_140_res0_eqab;
	APInt ssa_159_res1f = ssa_157_res1e | ssa_141_res1_eqab;
	APInt ssa_160_res0g = ssa_158_res0f | ssa_146_a_range_zero_sel;
	APInt ssa_161_res1 = ssa_159_res1f | ssa_147_a_range_one_sel;
	APInt ssa_162_res0 = ssa_160_res0g | ssa_9_const0;
	std::array<APInt, 2> ssa_163_r = std::array<APInt, 2>{ssa_162_res0, ssa_161_res1};
	return ssa_163_r;
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
