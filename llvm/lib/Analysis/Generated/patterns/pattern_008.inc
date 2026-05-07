namespace Pattern008 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_arg0 = ssa_0_arg0[0];
	APInt ssa_5_arg0 = ssa_0_arg0[1];
	APInt ssa_6_arg1 = ssa_1_arg1[0];
	APInt ssa_7_arg1 = ssa_1_arg1[1];
	APInt ssa_8_arg2 = ssa_2_arg2[0];
	APInt ssa_9_arg2 = ssa_2_arg2[1];
	APInt ssa_10_arg3 = ssa_3_arg3[0];
	APInt ssa_11_arg3 = ssa_3_arg3[1];
	APInt ssa_12_allones = APInt::getAllOnes(ssa_4_arg0.getBitWidth());
	APInt ssa_13_const0(ssa_4_arg0.getBitWidth(), 0);
	APInt ssa_14_arg0_or = ssa_4_arg0 | ssa_5_arg0;
	APInt ssa_15_arg0_unk = ssa_12_allones ^ ssa_14_arg0_or;
	bool ssa_16_arg0_known = ssa_15_arg0_unk.eq(ssa_13_const0);
	APInt ssa_17_arg1_or = ssa_6_arg1 | ssa_7_arg1;
	APInt ssa_18_arg1_unk = ssa_12_allones ^ ssa_17_arg1_or;
	bool ssa_19_arg1_known = ssa_18_arg1_unk.eq(ssa_13_const0);
	APInt ssa_20_arg2_or = ssa_8_arg2 | ssa_9_arg2;
	APInt ssa_21_arg2_unk = ssa_12_allones ^ ssa_20_arg2_or;
	bool ssa_22_arg2_known = ssa_21_arg2_unk.eq(ssa_13_const0);
	APInt ssa_23_arg3_or = ssa_10_arg3 | ssa_11_arg3;
	APInt ssa_24_arg3_unk = ssa_12_allones ^ ssa_23_arg3_or;
	bool ssa_25_arg3_known = ssa_24_arg3_unk.eq(ssa_13_const0);
	bool ssa_26_and01 = ssa_16_arg0_known && ssa_19_arg1_known;
	bool ssa_27_and23 = ssa_22_arg2_known && ssa_25_arg3_known;
	bool ssa_28_all_known = ssa_26_and01 && ssa_27_and23;
	APInt ssa_29_sub = ssa_11_arg3 - ssa_9_arg2;
	APInt ssa_30_ashr;
	if (ssa_7_arg1.uge(ssa_7_arg1.getBitWidth()) && ssa_29_sub.isSignBitSet()) {
		ssa_30_ashr = APInt(ssa_29_sub.getBitWidth(), -1);
	} else if (ssa_7_arg1.uge(ssa_7_arg1.getBitWidth()) && ssa_29_sub.isSignBitClear()) {
		ssa_30_ashr = APInt(ssa_29_sub.getBitWidth(), 0);
	} else {
		ssa_30_ashr = ssa_29_sub.ashr(ssa_7_arg1);
	}
	APInt ssa_31_add = ssa_5_arg0 + ssa_30_ashr;
	APInt ssa_32_shl;
	if (ssa_7_arg1.uge(ssa_7_arg1.getBitWidth())) {
		ssa_32_shl = APInt(ssa_30_ashr.getBitWidth(), 0);
	} else {
		ssa_32_shl = ssa_30_ashr.shl(ssa_7_arg1);
	}
	bool ssa_33_ashr_exact = ssa_32_shl.eq(ssa_29_sub);
	APInt ssa_34_bw(ssa_29_sub.getBitWidth(), ssa_29_sub.getBitWidth());
	bool ssa_35_shift_lt = ssa_7_arg1.ult(ssa_34_bw);
	bool ssa_36_shift_ok = ssa_33_ashr_exact && ssa_35_shift_lt;
	bool ssa_37_arg0_pos = ssa_5_arg0.sgt(ssa_13_const0);
	bool ssa_38_arg0_neg = ssa_5_arg0.slt(ssa_13_const0);
	bool ssa_39_ashr_pos = ssa_30_ashr.sgt(ssa_13_const0);
	bool ssa_40_ashr_neg = ssa_30_ashr.slt(ssa_13_const0);
	bool ssa_41_add_neg = ssa_31_add.slt(ssa_13_const0);
	bool ssa_42_add_nonneg = ssa_31_add.sge(ssa_13_const0);
	bool ssa_43_pos_and = ssa_37_arg0_pos && ssa_39_ashr_pos;
	bool ssa_44_pos_over = ssa_43_pos_and && ssa_41_add_neg;
	bool ssa_45_neg_and = ssa_38_arg0_neg && ssa_40_ashr_neg;
	bool ssa_46_neg_over = ssa_45_neg_and && ssa_42_add_nonneg;
	bool ssa_47_overflow = ssa_44_pos_over || ssa_46_neg_over;
	bool ssa_48_true = -1;
	bool ssa_49_no_over = ssa_47_overflow != ssa_48_true;
	bool ssa_50_ok1 = ssa_36_shift_ok && ssa_49_no_over;
	bool ssa_51_ok = ssa_28_all_known && ssa_50_ok1;
	bool ssa_52_min_ge_bw = ssa_7_arg1.uge(ssa_34_bw);
	bool ssa_53_bottom_cond = ssa_28_all_known || ssa_52_min_ge_bw;
	APInt ssa_54_exact_zero = ssa_12_allones ^ ssa_31_add;
	APInt ssa_55_tmp_zero = ssa_53_bottom_cond ? ssa_12_allones : ssa_13_const0;
	APInt ssa_56_tmp_one = ssa_53_bottom_cond ? ssa_12_allones : ssa_13_const0;
	APInt ssa_57_res_zero = ssa_51_ok ? ssa_54_exact_zero : ssa_55_tmp_zero;
	APInt ssa_58_res_one = ssa_51_ok ? ssa_31_add : ssa_56_tmp_one;
	std::array<APInt, 2> ssa_59_r = std::array<APInt, 2>{ssa_57_res_zero, ssa_58_res_one};
	return ssa_59_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_arg0 = ssa_0_arg0[0];
	APInt ssa_5_arg0 = ssa_0_arg0[1];
	APInt ssa_6_arg1 = ssa_1_arg1[0];
	APInt ssa_7_arg1 = ssa_1_arg1[1];
	APInt ssa_8_arg2 = ssa_2_arg2[0];
	APInt ssa_9_arg2 = ssa_2_arg2[1];
	APInt ssa_10_arg3 = ssa_3_arg3[0];
	APInt ssa_11_arg3 = ssa_3_arg3[1];
	APInt ssa_12_allones = APInt::getAllOnes(ssa_4_arg0.getBitWidth());
	APInt ssa_13_const0(ssa_4_arg0.getBitWidth(), 0);
	APInt ssa_14_const1(ssa_4_arg0.getBitWidth(), 1);
	APInt ssa_15_arg0_or = ssa_4_arg0 | ssa_5_arg0;
	APInt ssa_16_arg0_unk = ssa_12_allones ^ ssa_15_arg0_or;
	bool ssa_17_arg0_known = ssa_16_arg0_unk.eq(ssa_13_const0);
	APInt ssa_18_arg1_or = ssa_6_arg1 | ssa_7_arg1;
	APInt ssa_19_arg1_unk = ssa_12_allones ^ ssa_18_arg1_or;
	bool ssa_20_arg1_known = ssa_19_arg1_unk.eq(ssa_13_const0);
	APInt ssa_21_arg2_or = ssa_8_arg2 | ssa_9_arg2;
	APInt ssa_22_arg2_unk = ssa_12_allones ^ ssa_21_arg2_or;
	bool ssa_23_arg2_known = ssa_22_arg2_unk.eq(ssa_13_const0);
	APInt ssa_24_arg3_or = ssa_10_arg3 | ssa_11_arg3;
	APInt ssa_25_arg3_unk = ssa_12_allones ^ ssa_24_arg3_or;
	bool ssa_26_arg3_known = ssa_25_arg3_unk.eq(ssa_13_const0);
	bool ssa_27_and01 = ssa_17_arg0_known && ssa_20_arg1_known;
	bool ssa_28_and23 = ssa_23_arg2_known && ssa_26_arg3_known;
	bool ssa_29_all_known = ssa_27_and01 && ssa_28_and23;
	APInt ssa_30_sub = ssa_11_arg3 - ssa_9_arg2;
	APInt ssa_31_ashr;
	if (ssa_7_arg1.uge(ssa_7_arg1.getBitWidth()) && ssa_30_sub.isSignBitSet()) {
		ssa_31_ashr = APInt(ssa_30_sub.getBitWidth(), -1);
	} else if (ssa_7_arg1.uge(ssa_7_arg1.getBitWidth()) && ssa_30_sub.isSignBitClear()) {
		ssa_31_ashr = APInt(ssa_30_sub.getBitWidth(), 0);
	} else {
		ssa_31_ashr = ssa_30_sub.ashr(ssa_7_arg1);
	}
	APInt ssa_32_add = ssa_5_arg0 + ssa_31_ashr;
	APInt ssa_33_shl;
	if (ssa_7_arg1.uge(ssa_7_arg1.getBitWidth())) {
		ssa_33_shl = APInt(ssa_31_ashr.getBitWidth(), 0);
	} else {
		ssa_33_shl = ssa_31_ashr.shl(ssa_7_arg1);
	}
	bool ssa_34_ashr_exact = ssa_33_shl.eq(ssa_30_sub);
	APInt ssa_35_bw(ssa_30_sub.getBitWidth(), ssa_30_sub.getBitWidth());
	bool ssa_36_shift_lt = ssa_7_arg1.ult(ssa_35_bw);
	bool ssa_37_shift_ok = ssa_34_ashr_exact && ssa_36_shift_lt;
	bool ssa_38_arg0_pos = ssa_5_arg0.sgt(ssa_13_const0);
	bool ssa_39_arg0_neg = ssa_5_arg0.slt(ssa_13_const0);
	bool ssa_40_ashr_pos = ssa_31_ashr.sgt(ssa_13_const0);
	bool ssa_41_ashr_neg = ssa_31_ashr.slt(ssa_13_const0);
	bool ssa_42_add_neg = ssa_32_add.slt(ssa_13_const0);
	bool ssa_43_add_nonneg = ssa_32_add.sge(ssa_13_const0);
	bool ssa_44_pos_and = ssa_38_arg0_pos && ssa_40_ashr_pos;
	bool ssa_45_pos_over = ssa_44_pos_and && ssa_42_add_neg;
	bool ssa_46_neg_and = ssa_39_arg0_neg && ssa_41_ashr_neg;
	bool ssa_47_neg_over = ssa_46_neg_and && ssa_43_add_nonneg;
	bool ssa_48_overflow = ssa_45_pos_over || ssa_47_neg_over;
	bool ssa_49_true = -1;
	bool ssa_50_no_over = ssa_48_overflow != ssa_49_true;
	bool ssa_51_ok1 = ssa_37_shift_ok && ssa_50_no_over;
	bool ssa_52_ok = ssa_29_all_known && ssa_51_ok1;
	bool ssa_53_ok1_neg = ssa_51_ok1 != ssa_49_true;
	bool ssa_54_bottom_known = ssa_29_all_known && ssa_53_ok1_neg;
	bool ssa_55_shift_ge = ssa_36_shift_lt != ssa_49_true;
	bool ssa_56_bottom_shift = ssa_20_arg1_known && ssa_55_shift_ge;
	APInt ssa_57_pow2;
	if (ssa_7_arg1.uge(ssa_7_arg1.getBitWidth())) {
		ssa_57_pow2 = APInt(ssa_14_const1.getBitWidth(), 0);
	} else {
		ssa_57_pow2 = ssa_14_const1.shl(ssa_7_arg1);
	}
	APInt ssa_58_mask_low = ssa_57_pow2 - ssa_14_const1;
	APInt ssa_59_arg2_low_unk = ssa_22_arg2_unk & ssa_58_mask_low;
	APInt ssa_60_arg3_low_unk = ssa_25_arg3_unk & ssa_58_mask_low;
	bool ssa_61_arg2_low_known = ssa_59_arg2_low_unk.eq(ssa_13_const0);
	bool ssa_62_arg3_low_known = ssa_60_arg3_low_unk.eq(ssa_13_const0);
	bool ssa_63_low_known = ssa_61_arg2_low_known && ssa_62_arg3_low_known;
	APInt ssa_64_arg2_low = ssa_9_arg2 & ssa_58_mask_low;
	APInt ssa_65_arg3_low = ssa_11_arg3 & ssa_58_mask_low;
	APInt ssa_66_sub_low = ssa_65_arg3_low - ssa_64_arg2_low;
	APInt ssa_67_sub_low_mod = ssa_66_sub_low & ssa_58_mask_low;
	bool ssa_68_sub_low_zero = ssa_67_sub_low_mod.eq(ssa_13_const0);
	bool ssa_69_sub_low_ones = ssa_67_sub_low_mod.eq(ssa_58_mask_low);
	bool ssa_70_sub_low_ok = ssa_68_sub_low_zero || ssa_69_sub_low_ones;
	bool ssa_71_sub_low_bad = ssa_70_sub_low_ok != ssa_49_true;
	bool ssa_72_shamt_zero = ssa_7_arg1.eq(ssa_13_const0);
	bool ssa_73_shamt_nonzero = ssa_72_shamt_zero != ssa_49_true;
	bool ssa_74_bottom_ashr0 = ssa_20_arg1_known && ssa_36_shift_lt;
	bool ssa_75_bottom_ashr1 = ssa_74_bottom_ashr0 && ssa_73_shamt_nonzero;
	bool ssa_76_bottom_ashr2 = ssa_75_bottom_ashr1 && ssa_63_low_known;
	bool ssa_77_bottom_ashr = ssa_76_bottom_ashr2 && ssa_71_sub_low_bad;
	APInt ssa_78_diff = ssa_9_arg2 & ssa_10_arg3;
	APInt ssa_79_diff = ssa_8_arg2 & ssa_11_arg3;
	APInt ssa_80_diff_known = ssa_78_diff | ssa_79_diff;
	APInt ssa_81_diff_low = ssa_80_diff_known & ssa_58_mask_low;
	bool ssa_82_diff_low_zero = ssa_81_diff_low.eq(ssa_13_const0);
	bool ssa_83_diff_low_nonzero = ssa_82_diff_low_zero != ssa_49_true;
	APInt ssa_84_a0_one_m = ssa_9_arg2 & ssa_14_const1;
	APInt ssa_85_a0_zero_m = ssa_8_arg2 & ssa_14_const1;
	APInt ssa_86_b0_one_m = ssa_11_arg3 & ssa_14_const1;
	APInt ssa_87_b0_zero_m = ssa_10_arg3 & ssa_14_const1;
	bool ssa_88_a0_one = ssa_84_a0_one_m.eq(ssa_14_const1);
	bool ssa_89_a0_zero = ssa_85_a0_zero_m.eq(ssa_14_const1);
	bool ssa_90_b0_one = ssa_86_b0_one_m.eq(ssa_14_const1);
	bool ssa_91_b0_zero = ssa_87_b0_zero_m.eq(ssa_14_const1);
	bool ssa_92_same_one = ssa_88_a0_one && ssa_90_b0_one;
	bool ssa_93_same_zero = ssa_89_a0_zero && ssa_91_b0_zero;
	bool ssa_94_a0b0_same = ssa_92_same_one || ssa_93_same_zero;
	bool ssa_95_bottom_ashr2 = ssa_20_arg1_known && ssa_36_shift_lt;
	bool ssa_96_bottom_ashr2 = ssa_95_bottom_ashr2 && ssa_73_shamt_nonzero;
	bool ssa_97_bottom_ashr2 = ssa_96_bottom_ashr2 && ssa_83_diff_low_nonzero;
	bool ssa_98_bottom_ashr2 = ssa_97_bottom_ashr2 && ssa_94_a0b0_same;
	APInt ssa_99_arg2_low_max = ssa_64_arg2_low | ssa_59_arg2_low_unk;
	APInt ssa_100_arg3_low_max = ssa_65_arg3_low | ssa_60_arg3_low_unk;
	bool ssa_101_bmin_nonzero = ssa_64_arg2_low.ne(ssa_13_const0);
	APInt ssa_102_bmin_minus1 = ssa_64_arg2_low - ssa_14_const1;
	APInt ssa_103_bmax_minus1 = ssa_99_arg2_low_max - ssa_14_const1;
	bool ssa_104_amax_lt = ssa_100_arg3_low_max.ult(ssa_102_bmin_minus1);
	bool ssa_105_amin_gt = ssa_65_arg3_low.ugt(ssa_103_bmax_minus1);
	bool ssa_106_disjoint = ssa_104_amax_lt || ssa_105_amin_gt;
	bool ssa_107_bottom_ashr3 = ssa_20_arg1_known && ssa_36_shift_lt;
	bool ssa_108_bottom_ashr3 = ssa_107_bottom_ashr3 && ssa_73_shamt_nonzero;
	bool ssa_109_bottom_ashr3 = ssa_108_bottom_ashr3 && ssa_83_diff_low_nonzero;
	bool ssa_110_bottom_ashr3 = ssa_109_bottom_ashr3 && ssa_101_bmin_nonzero;
	bool ssa_111_bottom_ashr3 = ssa_110_bottom_ashr3 && ssa_106_disjoint;
	bool ssa_112_bottom_or0 = ssa_54_bottom_known || ssa_56_bottom_shift;
	bool ssa_113_bottom_or1 = ssa_112_bottom_or0 || ssa_77_bottom_ashr;
	bool ssa_114_bottom_or2 = ssa_113_bottom_or1 || ssa_98_bottom_ashr2;
	bool ssa_115_bottom_cond = ssa_114_bottom_or2 || ssa_111_bottom_ashr3;
	APInt ssa_116_exact_zero = ssa_12_allones ^ ssa_32_add;
	APInt ssa_117_tmp_zero = ssa_115_bottom_cond ? ssa_12_allones : ssa_13_const0;
	APInt ssa_118_tmp_one = ssa_115_bottom_cond ? ssa_12_allones : ssa_13_const0;
	APInt ssa_119_res_zero = ssa_52_ok ? ssa_116_exact_zero : ssa_117_tmp_zero;
	APInt ssa_120_res_one = ssa_52_ok ? ssa_32_add : ssa_118_tmp_one;
	std::array<APInt, 2> ssa_121_r = std::array<APInt, 2>{ssa_119_res_zero, ssa_120_res_one};
	return ssa_121_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_arg0 = ssa_0_arg0[0];
	APInt ssa_5_arg0 = ssa_0_arg0[1];
	APInt ssa_6_arg1 = ssa_1_arg1[0];
	APInt ssa_7_arg1 = ssa_1_arg1[1];
	APInt ssa_8_arg2 = ssa_2_arg2[0];
	APInt ssa_9_arg2 = ssa_2_arg2[1];
	APInt ssa_10_arg3 = ssa_3_arg3[0];
	APInt ssa_11_arg3 = ssa_3_arg3[1];
	APInt ssa_12_allones = APInt::getAllOnes(ssa_4_arg0.getBitWidth());
	APInt ssa_13_const0(ssa_4_arg0.getBitWidth(), 0);
	APInt ssa_14_const1(ssa_4_arg0.getBitWidth(), 1);
	APInt ssa_15_arg0_or = ssa_4_arg0 | ssa_5_arg0;
	APInt ssa_16_arg0_unk = ssa_12_allones ^ ssa_15_arg0_or;
	bool ssa_17_arg0_known = ssa_16_arg0_unk.eq(ssa_13_const0);
	APInt ssa_18_arg1_or = ssa_6_arg1 | ssa_7_arg1;
	APInt ssa_19_arg1_unk = ssa_12_allones ^ ssa_18_arg1_or;
	bool ssa_20_arg1_known = ssa_19_arg1_unk.eq(ssa_13_const0);
	APInt ssa_21_bw(ssa_4_arg0.getBitWidth(), ssa_4_arg0.getBitWidth());
	APInt ssa_22_bw_minus1 = ssa_21_bw - ssa_14_const1;
	bool ssa_23_shift_lt = ssa_7_arg1.ult(ssa_21_bw);
	bool ssa_24_shamt_eq_bw1 = ssa_7_arg1.eq(ssa_22_bw_minus1);
	APInt ssa_25_pow2;
	if (ssa_7_arg1.uge(ssa_7_arg1.getBitWidth())) {
		ssa_25_pow2 = APInt(ssa_14_const1.getBitWidth(), 0);
	} else {
		ssa_25_pow2 = ssa_14_const1.shl(ssa_7_arg1);
	}
	APInt ssa_26_mask_low = ssa_25_pow2 - ssa_14_const1;
	APInt ssa_27_high_mask = ssa_12_allones ^ ssa_26_mask_low;
	APInt ssa_28_arg2_or = ssa_8_arg2 | ssa_9_arg2;
	APInt ssa_29_arg2_unk = ssa_12_allones ^ ssa_28_arg2_or;
	APInt ssa_30_arg3_or = ssa_10_arg3 | ssa_11_arg3;
	APInt ssa_31_arg3_unk = ssa_12_allones ^ ssa_30_arg3_or;
	APInt ssa_32_arg2_high_unk = ssa_29_arg2_unk & ssa_27_high_mask;
	APInt ssa_33_arg3_high_unk = ssa_31_arg3_unk & ssa_27_high_mask;
	bool ssa_34_arg2_high_known = ssa_32_arg2_high_unk.eq(ssa_13_const0);
	bool ssa_35_arg3_high_known = ssa_33_arg3_high_unk.eq(ssa_13_const0);
	APInt ssa_36_diff = ssa_9_arg2 & ssa_10_arg3;
	APInt ssa_37_diff = ssa_8_arg2 & ssa_11_arg3;
	APInt ssa_38_diff_known = ssa_36_diff | ssa_37_diff;
	APInt ssa_39_diff_high = ssa_38_diff_known & ssa_27_high_mask;
	bool ssa_40_diff_high_zero = ssa_39_diff_high.eq(ssa_13_const0);
	bool ssa_41_high_known = ssa_34_arg2_high_known && ssa_35_arg3_high_known;
	bool ssa_42_high_equal = ssa_41_high_known && ssa_40_diff_high_zero;
	bool ssa_43_ok0 = ssa_20_arg1_known && ssa_23_shift_lt;
	bool ssa_44_ok = ssa_43_ok0 && ssa_42_high_equal;
	bool ssa_45_true = -1;
	bool ssa_46_shift_ge = ssa_23_shift_lt != ssa_45_true;
	bool ssa_47_bottom_shift = ssa_20_arg1_known && ssa_46_shift_ge;
	APInt ssa_48_arg2_low_unk = ssa_29_arg2_unk & ssa_26_mask_low;
	APInt ssa_49_arg3_low_unk = ssa_31_arg3_unk & ssa_26_mask_low;
	bool ssa_50_arg2_low_known = ssa_48_arg2_low_unk.eq(ssa_13_const0);
	bool ssa_51_arg3_low_known = ssa_49_arg3_low_unk.eq(ssa_13_const0);
	bool ssa_52_low_known = ssa_50_arg2_low_known && ssa_51_arg3_low_known;
	APInt ssa_53_diff_low = ssa_38_diff_known & ssa_26_mask_low;
	bool ssa_54_diff_low_zero = ssa_53_diff_low.eq(ssa_13_const0);
	bool ssa_55_diff_low_nonzero = ssa_54_diff_low_zero != ssa_45_true;
	bool ssa_56_bottom_low0 = ssa_52_low_known && ssa_55_diff_low_nonzero;
	bool ssa_57_bottom_low1 = ssa_56_bottom_low0 && ssa_20_arg1_known;
	bool ssa_58_bottom_low = ssa_57_bottom_low1 && ssa_23_shift_lt;
	bool ssa_59_bottom_cond0 = ssa_47_bottom_shift || ssa_58_bottom_low;
	APInt ssa_60_tmp_zero = ssa_59_bottom_cond0 ? ssa_12_allones : ssa_13_const0;
	APInt ssa_61_tmp_one = ssa_59_bottom_cond0 ? ssa_12_allones : ssa_13_const0;
	APInt ssa_62_min_val;
	if (ssa_22_bw_minus1.uge(ssa_22_bw_minus1.getBitWidth())) {
		ssa_62_min_val = APInt(ssa_14_const1.getBitWidth(), 0);
	} else {
		ssa_62_min_val = ssa_14_const1.shl(ssa_22_bw_minus1);
	}
	bool ssa_63_arg0_eq_min = ssa_5_arg0.eq(ssa_62_min_val);
	bool ssa_64_cond_bw1 = ssa_17_arg0_known && ssa_20_arg1_known;
	bool ssa_65_cond_bw1 = ssa_64_cond_bw1 && ssa_24_shamt_eq_bw1;
	bool ssa_66_cond_min = ssa_65_cond_bw1 && ssa_63_arg0_eq_min;
	bool ssa_67_arg0_eq_min_not = ssa_63_arg0_eq_min != ssa_45_true;
	bool ssa_68_cond_bw1_nonmin = ssa_65_cond_bw1 && ssa_67_arg0_eq_min_not;
	APInt ssa_69_arg0_minus1 = ssa_5_arg0 - ssa_14_const1;
	APInt ssa_70_arg0_not = ssa_12_allones ^ ssa_5_arg0;
	APInt ssa_71_arg0_minus1_not = ssa_12_allones ^ ssa_69_arg0_minus1;
	APInt ssa_72_inter_one = ssa_5_arg0 & ssa_69_arg0_minus1;
	APInt ssa_73_inter_zero = ssa_70_arg0_not & ssa_71_arg0_minus1_not;
	APInt ssa_74_res1_zero = ssa_68_cond_bw1_nonmin ? ssa_73_inter_zero : ssa_60_tmp_zero;
	APInt ssa_75_res1_one = ssa_68_cond_bw1_nonmin ? ssa_72_inter_one : ssa_61_tmp_one;
	APInt ssa_76_res2_zero = ssa_66_cond_min ? ssa_4_arg0 : ssa_74_res1_zero;
	APInt ssa_77_res2_one = ssa_66_cond_min ? ssa_5_arg0 : ssa_75_res1_one;
	APInt ssa_78_res_zero = ssa_44_ok ? ssa_4_arg0 : ssa_76_res2_zero;
	APInt ssa_79_res_one = ssa_44_ok ? ssa_5_arg0 : ssa_77_res2_one;
	std::array<APInt, 2> ssa_80_r = std::array<APInt, 2>{ssa_78_res_zero, ssa_79_res_one};
	return ssa_80_r;
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
std::array<APInt, 2> solution(std::array<APInt, 2> ssa_0_, std::array<APInt, 2> ssa_1_, std::array<APInt, 2> ssa_2_, std::array<APInt, 2> ssa_3_) {
	std::array<APInt, 2> ssa_4_ = partial_solution_0(ssa_0_, ssa_1_, ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_5_ = partial_solution_1(ssa_0_, ssa_1_, ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_6_ = partial_solution_2(ssa_0_, ssa_1_, ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_7_ = meet(ssa_4_, ssa_5_);
	std::array<APInt, 2> ssa_8_ = meet(ssa_7_, ssa_6_);
	return ssa_8_;
}
}
