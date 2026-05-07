namespace Pattern026 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_12_bitwidth(ssa_3_a0z.getBitWidth(), ssa_3_a0z.getBitWidth());
	APInt ssa_13_a0_or = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_14_a1_or = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_15_a2_or = ssa_7_a2z | ssa_8_a2o;
	bool ssa_16_a0_known = ssa_13_a0_or.eq(ssa_11_allones);
	bool ssa_17_a1_known = ssa_14_a1_or.eq(ssa_11_allones);
	bool ssa_18_a2_known = ssa_15_a2_or.eq(ssa_11_allones);
	bool ssa_19_a0_zero = ssa_3_a0z.eq(ssa_11_allones);
	bool ssa_20_a1_zero = ssa_5_a1z.eq(ssa_11_allones);
	bool ssa_21_a2_zero = ssa_7_a2z.eq(ssa_11_allones);
	bool ssa_22_a1_is1 = ssa_6_a1o.eq(ssa_10_const1);
	bool ssa_23_a2_is1 = ssa_8_a2o.eq(ssa_10_const1);
	bool ssa_24_a1_one = ssa_17_a1_known && ssa_22_a1_is1;
	bool ssa_25_a2_one = ssa_18_a2_known && ssa_23_a2_is1;
	bool ssa_26_prod_zero_cond = ssa_20_a1_zero || ssa_21_a2_zero;
	APInt ssa_27_prod_z1 = ssa_26_prod_zero_cond ? ssa_11_allones : ssa_9_const0;
	APInt ssa_28_prod_o1 = ssa_26_prod_zero_cond ? ssa_9_const0 : ssa_9_const0;
	APInt ssa_29_prod_z2 = ssa_24_a1_one ? ssa_7_a2z : ssa_27_prod_z1;
	APInt ssa_30_prod_o2 = ssa_24_a1_one ? ssa_8_a2o : ssa_28_prod_o1;
	APInt ssa_31_prod_z3 = ssa_25_a2_one ? ssa_5_a1z : ssa_29_prod_z2;
	APInt ssa_32_prod_o3 = ssa_25_a2_one ? ssa_6_a1o : ssa_30_prod_o2;
	bool ssa_33_both_known = ssa_17_a1_known && ssa_18_a2_known;
	APInt ssa_34_prod_const = ssa_6_a1o * ssa_8_a2o;
	APInt ssa_35_prod_const_z = ssa_11_allones ^ ssa_34_prod_const;
	APInt ssa_36_prod_z4 = ssa_33_both_known ? ssa_35_prod_const_z : ssa_31_prod_z3;
	APInt ssa_37_prod_o4 = ssa_33_both_known ? ssa_34_prod_const : ssa_32_prod_o3;
	APInt ssa_38_tz1(ssa_5_a1z.getBitWidth(), ssa_5_a1z.countr_one());
	APInt ssa_39_tz2(ssa_7_a2z.getBitWidth(), ssa_7_a2z.countr_one());
	APInt ssa_40_tzp = ssa_38_tz1 + ssa_39_tz2;
	APInt ssa_41_tzp_clamp = APIntOps::umin(ssa_40_tzp, ssa_12_bitwidth);
	APInt ssa_42_one_shift_p;
	if (ssa_41_tzp_clamp.uge(ssa_41_tzp_clamp.getBitWidth())) {
		ssa_42_one_shift_p = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_42_one_shift_p = ssa_10_const1.shl(ssa_41_tzp_clamp);
	}
	APInt ssa_43_lowmask_p = ssa_42_one_shift_p - ssa_10_const1;
	APInt ssa_44_prod_z5 = ssa_36_prod_z4 | ssa_43_lowmask_p;
	APInt ssa_45_lz1(ssa_5_a1z.getBitWidth(), ssa_5_a1z.countl_one());
	APInt ssa_46_lz2(ssa_7_a2z.getBitWidth(), ssa_7_a2z.countl_one());
	APInt ssa_47_lz_sum = ssa_45_lz1 + ssa_46_lz2;
	bool ssa_48_lz_ge = ssa_47_lz_sum.uge(ssa_12_bitwidth);
	APInt ssa_49_max1 = ssa_11_allones ^ ssa_5_a1z;
	APInt ssa_50_max2 = ssa_11_allones ^ ssa_7_a2z;
	APInt ssa_51_max_prod = ssa_49_max1 * ssa_50_max2;
	APInt ssa_52_lz_max_prod(ssa_51_max_prod.getBitWidth(), ssa_51_max_prod.countl_zero());
	APInt ssa_53_lz_count = ssa_48_lz_ge ? ssa_52_lz_max_prod : ssa_9_const0;
	APInt ssa_54_bw_minus_lz = ssa_12_bitwidth - ssa_53_lz_count;
	APInt ssa_55_highmask_p;
	if (ssa_54_bw_minus_lz.uge(ssa_54_bw_minus_lz.getBitWidth())) {
		ssa_55_highmask_p = APInt(ssa_11_allones.getBitWidth(), 0);
	} else {
		ssa_55_highmask_p = ssa_11_allones.shl(ssa_54_bw_minus_lz);
	}
	APInt ssa_56_prod_z6 = ssa_44_prod_z5 | ssa_55_highmask_p;
	APInt ssa_57_prod_or = ssa_56_prod_z6 | ssa_37_prod_o4;
	bool ssa_58_prod_known = ssa_57_prod_or.eq(ssa_11_allones);
	bool ssa_59_prod_zero = ssa_56_prod_z6.eq(ssa_11_allones);
	APInt ssa_60_res_z0 = ssa_19_a0_zero ? ssa_56_prod_z6 : ssa_9_const0;
	APInt ssa_61_res_o0 = ssa_19_a0_zero ? ssa_37_prod_o4 : ssa_9_const0;
	APInt ssa_62_res_z1 = ssa_59_prod_zero ? ssa_3_a0z : ssa_60_res_z0;
	APInt ssa_63_res_o1 = ssa_59_prod_zero ? ssa_4_a0o : ssa_61_res_o0;
	bool ssa_64_both_known2 = ssa_16_a0_known && ssa_58_prod_known;
	APInt ssa_65_sum_const = ssa_4_a0o + ssa_37_prod_o4;
	APInt ssa_66_sum_const_z = ssa_11_allones ^ ssa_65_sum_const;
	APInt ssa_67_res_z2 = ssa_64_both_known2 ? ssa_66_sum_const_z : ssa_62_res_z1;
	APInt ssa_68_res_o2 = ssa_64_both_known2 ? ssa_65_sum_const : ssa_63_res_o1;
	APInt ssa_69_tz_a0(ssa_3_a0z.getBitWidth(), ssa_3_a0z.countr_one());
	APInt ssa_70_tz_prod(ssa_56_prod_z6.getBitWidth(), ssa_56_prod_z6.countr_one());
	APInt ssa_71_tz_min = APIntOps::umin(ssa_69_tz_a0, ssa_70_tz_prod);
	APInt ssa_72_one_shift;
	if (ssa_71_tz_min.uge(ssa_71_tz_min.getBitWidth())) {
		ssa_72_one_shift = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_72_one_shift = ssa_10_const1.shl(ssa_71_tz_min);
	}
	APInt ssa_73_lowmask = ssa_72_one_shift - ssa_10_const1;
	APInt ssa_74_res_z3 = ssa_67_res_z2 | ssa_73_lowmask;
	APInt ssa_75_one_shift_a0;
	if (ssa_69_tz_a0.uge(ssa_69_tz_a0.getBitWidth())) {
		ssa_75_one_shift_a0 = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_75_one_shift_a0 = ssa_10_const1.shl(ssa_69_tz_a0);
	}
	APInt ssa_76_lowmask_a0 = ssa_75_one_shift_a0 - ssa_10_const1;
	APInt ssa_77_res_zero_low = ssa_3_a0z & ssa_43_lowmask_p;
	APInt ssa_78_res_one_low = ssa_4_a0o & ssa_43_lowmask_p;
	APInt ssa_79_prod_zero_low = ssa_56_prod_z6 & ssa_76_lowmask_a0;
	APInt ssa_80_prod_one_low = ssa_37_prod_o4 & ssa_76_lowmask_a0;
	APInt ssa_81_res_z4 = ssa_74_res_z3 | ssa_77_res_zero_low;
	APInt ssa_82_res_o3 = ssa_68_res_o2 | ssa_78_res_one_low;
	APInt ssa_83_res_z5 = ssa_81_res_z4 | ssa_79_prod_zero_low;
	APInt ssa_84_res_o4 = ssa_82_res_o3 | ssa_80_prod_one_low;
	APInt ssa_85_lz_a0(ssa_3_a0z.getBitWidth(), ssa_3_a0z.countl_one());
	APInt ssa_86_lz_prod(ssa_56_prod_z6.getBitWidth(), ssa_56_prod_z6.countl_one());
	APInt ssa_87_lz_min = APIntOps::umin(ssa_85_lz_a0, ssa_86_lz_prod);
	bool ssa_88_lz_min_gt0 = ssa_87_lz_min.ugt(ssa_9_const0);
	APInt ssa_89_lz_sum2 = ssa_87_lz_min - ssa_10_const1;
	APInt ssa_90_lz_sum2_sel = ssa_88_lz_min_gt0 ? ssa_89_lz_sum2 : ssa_9_const0;
	APInt ssa_91_bw_minus_lz2 = ssa_12_bitwidth - ssa_90_lz_sum2_sel;
	APInt ssa_92_highmask_sum;
	if (ssa_91_bw_minus_lz2.uge(ssa_91_bw_minus_lz2.getBitWidth())) {
		ssa_92_highmask_sum = APInt(ssa_11_allones.getBitWidth(), 0);
	} else {
		ssa_92_highmask_sum = ssa_11_allones.shl(ssa_91_bw_minus_lz2);
	}
	APInt ssa_93_res_z6 = ssa_83_res_z5 | ssa_92_highmask_sum;
	bool ssa_94_a1o_zero = ssa_6_a1o.eq(ssa_9_const0);
	APInt ssa_95_a1_unknown = ssa_11_allones ^ ssa_14_a1_or;
	bool ssa_96_a1_unknown_nonzero = ssa_95_a1_unknown.ne(ssa_9_const0);
	APInt ssa_97_a1_unknown_minus1 = ssa_95_a1_unknown - ssa_10_const1;
	APInt ssa_98_a1_unknown_and = ssa_95_a1_unknown & ssa_97_a1_unknown_minus1;
	bool ssa_99_a1_unknown_pow2 = ssa_98_a1_unknown_and.eq(ssa_9_const0);
	bool ssa_100_a1_singlebit0 = ssa_94_a1o_zero && ssa_96_a1_unknown_nonzero;
	bool ssa_101_a1_singlebit = ssa_100_a1_singlebit0 && ssa_99_a1_unknown_pow2;
	bool ssa_102_pair_cond0 = ssa_16_a0_known && ssa_18_a2_known;
	bool ssa_103_pair_cond = ssa_102_pair_cond0 && ssa_101_a1_singlebit;
	APInt ssa_104_a1_shift(ssa_95_a1_unknown.getBitWidth(), ssa_95_a1_unknown.countr_zero());
	APInt ssa_105_a2_shift;
	if (ssa_104_a1_shift.uge(ssa_104_a1_shift.getBitWidth())) {
		ssa_105_a2_shift = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_105_a2_shift = ssa_8_a2o.shl(ssa_104_a1_shift);
	}
	APInt ssa_106_alt_sum = ssa_4_a0o + ssa_105_a2_shift;
	APInt ssa_107_pair_diff = ssa_4_a0o ^ ssa_106_alt_sum;
	APInt ssa_108_pair_same = ssa_11_allones ^ ssa_107_pair_diff;
	APInt ssa_109_pair_one = ssa_108_pair_same & ssa_4_a0o;
	APInt ssa_110_a0_not = ssa_11_allones ^ ssa_4_a0o;
	APInt ssa_111_pair_zero = ssa_108_pair_same & ssa_110_a0_not;
	APInt ssa_112_res_z7 = ssa_103_pair_cond ? ssa_111_pair_zero : ssa_93_res_z6;
	APInt ssa_113_res_o5 = ssa_103_pair_cond ? ssa_109_pair_one : ssa_84_res_o4;
	std::array<APInt, 2> ssa_114_r = std::array<APInt, 2>{ssa_112_res_z7, ssa_113_res_o5};
	return ssa_114_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_12_bitwidth(ssa_3_a0z.getBitWidth(), ssa_3_a0z.getBitWidth());
	bool ssa_13_true = ssa_9_const0.ne(ssa_10_const1);
	APInt ssa_14_a0_or = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_15_a1_or = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_16_a2_or = ssa_7_a2z | ssa_8_a2o;
	bool ssa_17_a0_known = ssa_14_a0_or.eq(ssa_11_allones);
	bool ssa_18_a1_known = ssa_15_a1_or.eq(ssa_11_allones);
	bool ssa_19_a2_known = ssa_16_a2_or.eq(ssa_11_allones);
	bool ssa_20_a1_nonzero = ssa_6_a1o.ne(ssa_9_const0);
	bool ssa_21_a2_nonzero = ssa_8_a2o.ne(ssa_9_const0);
	APInt ssa_22_a1_minus1 = ssa_6_a1o - ssa_10_const1;
	APInt ssa_23_a2_minus1 = ssa_8_a2o - ssa_10_const1;
	APInt ssa_24_a1_and = ssa_6_a1o & ssa_22_a1_minus1;
	APInt ssa_25_a2_and = ssa_8_a2o & ssa_23_a2_minus1;
	bool ssa_26_a1_pow2 = ssa_24_a1_and.eq(ssa_9_const0);
	bool ssa_27_a2_pow2 = ssa_25_a2_and.eq(ssa_9_const0);
	bool ssa_28_a1_pow2_nz = ssa_20_a1_nonzero && ssa_26_a1_pow2;
	bool ssa_29_a2_pow2_nz = ssa_21_a2_nonzero && ssa_27_a2_pow2;
	bool ssa_30_a1_pow2_cond = ssa_18_a1_known && ssa_28_a1_pow2_nz;
	bool ssa_31_a2_pow2_cond = ssa_19_a2_known && ssa_29_a2_pow2_nz;
	APInt ssa_32_a0_max = ssa_11_allones ^ ssa_3_a0z;
	APInt ssa_33_a1_max = ssa_11_allones ^ ssa_5_a1z;
	APInt ssa_34_a2_max = ssa_11_allones ^ ssa_7_a2z;
	bool ssa_35_a2_max_is0 = ssa_34_a2_max.eq(ssa_9_const0);
	APInt ssa_36_a2_max_safe = ssa_35_a2_max_is0 ? ssa_10_const1 : ssa_34_a2_max;
	APInt ssa_37_max_div;
	if (ssa_36_a2_max_safe == 0) {
		ssa_37_max_div = APInt(ssa_11_allones.getBitWidth(), -1);
	} else {
		ssa_37_max_div = ssa_11_allones.udiv(ssa_36_a2_max_safe);
	}
	bool ssa_38_a1_le = ssa_33_a1_max.ule(ssa_37_max_div);
	bool ssa_39_mult_noov = ssa_35_a2_max_is0 || ssa_38_a1_le;
	APInt ssa_40_a1_unknown = ssa_11_allones ^ ssa_15_a1_or;
	APInt ssa_41_a2_unknown = ssa_11_allones ^ ssa_16_a2_or;
	bool ssa_42_a1_unknown_nz = ssa_40_a1_unknown.ne(ssa_9_const0);
	bool ssa_43_a2_unknown_nz = ssa_41_a2_unknown.ne(ssa_9_const0);
	APInt ssa_44_a1_unknown_minus1 = ssa_40_a1_unknown - ssa_10_const1;
	APInt ssa_45_a2_unknown_minus1 = ssa_41_a2_unknown - ssa_10_const1;
	APInt ssa_46_a1_unknown_and = ssa_40_a1_unknown & ssa_44_a1_unknown_minus1;
	APInt ssa_47_a2_unknown_and = ssa_41_a2_unknown & ssa_45_a2_unknown_minus1;
	bool ssa_48_a1_unknown_pow2 = ssa_46_a1_unknown_and.eq(ssa_9_const0);
	bool ssa_49_a2_unknown_pow2 = ssa_47_a2_unknown_and.eq(ssa_9_const0);
	bool ssa_50_a1_single_unknown = ssa_42_a1_unknown_nz && ssa_48_a1_unknown_pow2;
	bool ssa_51_a2_single_unknown = ssa_43_a2_unknown_nz && ssa_49_a2_unknown_pow2;
	bool ssa_52_cond_single1a = ssa_50_a1_single_unknown && ssa_17_a0_known;
	bool ssa_53_cond_single1 = ssa_52_cond_single1a && ssa_19_a2_known;
	APInt ssa_54_shift_single1(ssa_40_a1_unknown.getBitWidth(), ssa_40_a1_unknown.countr_zero());
	APInt ssa_55_one_shift_single1;
	if (ssa_54_shift_single1.uge(ssa_54_shift_single1.getBitWidth())) {
		ssa_55_one_shift_single1 = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_55_one_shift_single1 = ssa_10_const1.shl(ssa_54_shift_single1);
	}
	APInt ssa_56_a1_alt = ssa_6_a1o + ssa_55_one_shift_single1;
	APInt ssa_57_prod_s10 = ssa_6_a1o * ssa_8_a2o;
	APInt ssa_58_prod_s11 = ssa_56_a1_alt * ssa_8_a2o;
	APInt ssa_59_sum_s10 = ssa_57_prod_s10 + ssa_4_a0o;
	APInt ssa_60_sum_s11 = ssa_58_prod_s11 + ssa_4_a0o;
	APInt ssa_61_diff_s1 = ssa_59_sum_s10 ^ ssa_60_sum_s11;
	APInt ssa_62_mask_s1 = ssa_11_allones ^ ssa_61_diff_s1;
	APInt ssa_63_sum_s10_not = ssa_11_allones ^ ssa_59_sum_s10;
	APInt ssa_64_known_one_s1 = ssa_59_sum_s10 & ssa_62_mask_s1;
	APInt ssa_65_known_zero_s1 = ssa_63_sum_s10_not & ssa_62_mask_s1;
	APInt ssa_66_res_z_s1 = ssa_53_cond_single1 ? ssa_65_known_zero_s1 : ssa_9_const0;
	APInt ssa_67_res_o_s1 = ssa_53_cond_single1 ? ssa_64_known_one_s1 : ssa_9_const0;
	bool ssa_68_cond_single2a = ssa_51_a2_single_unknown && ssa_17_a0_known;
	bool ssa_69_cond_single2 = ssa_68_cond_single2a && ssa_18_a1_known;
	APInt ssa_70_shift_single2(ssa_41_a2_unknown.getBitWidth(), ssa_41_a2_unknown.countr_zero());
	APInt ssa_71_one_shift_single2;
	if (ssa_70_shift_single2.uge(ssa_70_shift_single2.getBitWidth())) {
		ssa_71_one_shift_single2 = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_71_one_shift_single2 = ssa_10_const1.shl(ssa_70_shift_single2);
	}
	APInt ssa_72_a2_alt = ssa_8_a2o + ssa_71_one_shift_single2;
	APInt ssa_73_prod_s20 = ssa_6_a1o * ssa_8_a2o;
	APInt ssa_74_prod_s21 = ssa_6_a1o * ssa_72_a2_alt;
	APInt ssa_75_sum_s20 = ssa_73_prod_s20 + ssa_4_a0o;
	APInt ssa_76_sum_s21 = ssa_74_prod_s21 + ssa_4_a0o;
	APInt ssa_77_diff_s2 = ssa_75_sum_s20 ^ ssa_76_sum_s21;
	APInt ssa_78_mask_s2 = ssa_11_allones ^ ssa_77_diff_s2;
	APInt ssa_79_sum_s20_not = ssa_11_allones ^ ssa_75_sum_s20;
	APInt ssa_80_known_one_s2 = ssa_75_sum_s20 & ssa_78_mask_s2;
	APInt ssa_81_known_zero_s2 = ssa_79_sum_s20_not & ssa_78_mask_s2;
	APInt ssa_82_res_z_s2 = ssa_69_cond_single2 ? ssa_81_known_zero_s2 : ssa_9_const0;
	APInt ssa_83_res_o_s2 = ssa_69_cond_single2 ? ssa_80_known_one_s2 : ssa_9_const0;
	APInt ssa_84_a1_unknown_plus1 = ssa_40_a1_unknown + ssa_10_const1;
	APInt ssa_85_a1_unknown_plus1_minus1 = ssa_84_a1_unknown_plus1 - ssa_10_const1;
	APInt ssa_86_a1_unknown_plus1_and = ssa_84_a1_unknown_plus1 & ssa_85_a1_unknown_plus1_minus1;
	bool ssa_87_a1_unknown_lowmask = ssa_86_a1_unknown_plus1_and.eq(ssa_9_const0);
	bool ssa_88_a1_lowmask_cond = ssa_42_a1_unknown_nz && ssa_87_a1_unknown_lowmask;
	bool ssa_89_cond_low1a = ssa_88_a1_lowmask_cond && ssa_17_a0_known;
	bool ssa_90_cond_low1 = ssa_89_cond_low1a && ssa_19_a2_known;
	bool ssa_91_a2o_is0 = ssa_8_a2o.eq(ssa_9_const0);
	APInt ssa_92_a2o_safe = ssa_91_a2o_is0 ? ssa_10_const1 : ssa_8_a2o;
	APInt ssa_93_max_div_low;
	if (ssa_92_a2o_safe == 0) {
		ssa_93_max_div_low = APInt(ssa_11_allones.getBitWidth(), -1);
	} else {
		ssa_93_max_div_low = ssa_11_allones.udiv(ssa_92_a2o_safe);
	}
	bool ssa_94_delta_ok = ssa_40_a1_unknown.ule(ssa_93_max_div_low);
	bool ssa_95_delta_noov = ssa_91_a2o_is0 || ssa_94_delta_ok;
	APInt ssa_96_delta_mul = ssa_40_a1_unknown * ssa_8_a2o;
	APInt ssa_97_prod_low_base = ssa_6_a1o * ssa_8_a2o;
	APInt ssa_98_prod_low_end = ssa_97_prod_low_base + ssa_96_delta_mul;
	bool ssa_99_prod_low_wrap = ssa_98_prod_low_end.ult(ssa_97_prod_low_base);
	bool ssa_100_prod_low_nowrap = ssa_99_prod_low_wrap != ssa_13_true;
	APInt ssa_101_sum_low_base = ssa_97_prod_low_base + ssa_4_a0o;
	APInt ssa_102_sum_low_end = ssa_98_prod_low_end + ssa_4_a0o;
	bool ssa_103_sum_low_wrap = ssa_102_sum_low_end.ult(ssa_101_sum_low_base);
	bool ssa_104_sum_low_nowrap = ssa_103_sum_low_wrap != ssa_13_true;
	bool ssa_105_cond_low1b = ssa_90_cond_low1 && ssa_95_delta_noov;
	bool ssa_106_cond_low1c = ssa_105_cond_low1b && ssa_100_prod_low_nowrap;
	bool ssa_107_cond_low1d = ssa_106_cond_low1c && ssa_104_sum_low_nowrap;
	APInt ssa_108_diff_low1 = ssa_101_sum_low_base ^ ssa_102_sum_low_end;
	APInt ssa_109_lz_low1(ssa_108_diff_low1.getBitWidth(), ssa_108_diff_low1.countl_zero());
	APInt ssa_110_bw_minus_lz_low1 = ssa_12_bitwidth - ssa_109_lz_low1;
	APInt ssa_111_mask_low1;
	if (ssa_110_bw_minus_lz_low1.uge(ssa_110_bw_minus_lz_low1.getBitWidth())) {
		ssa_111_mask_low1 = APInt(ssa_11_allones.getBitWidth(), 0);
	} else {
		ssa_111_mask_low1 = ssa_11_allones.shl(ssa_110_bw_minus_lz_low1);
	}
	APInt ssa_112_sum_low_base_not = ssa_11_allones ^ ssa_101_sum_low_base;
	APInt ssa_113_known_one_low1 = ssa_101_sum_low_base & ssa_111_mask_low1;
	APInt ssa_114_known_zero_low1 = ssa_112_sum_low_base_not & ssa_111_mask_low1;
	APInt ssa_115_res_z_low1 = ssa_107_cond_low1d ? ssa_114_known_zero_low1 : ssa_9_const0;
	APInt ssa_116_res_o_low1 = ssa_107_cond_low1d ? ssa_113_known_one_low1 : ssa_9_const0;
	APInt ssa_117_k2(ssa_8_a2o.getBitWidth(), ssa_8_a2o.countr_zero());
	APInt ssa_118_bw_minus_k2 = ssa_12_bitwidth - ssa_117_k2;
	APInt ssa_119_one_shift_k2;
	if (ssa_118_bw_minus_k2.uge(ssa_118_bw_minus_k2.getBitWidth())) {
		ssa_119_one_shift_k2 = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_119_one_shift_k2 = ssa_10_const1.shl(ssa_118_bw_minus_k2);
	}
	APInt ssa_120_lowmask_k2 = ssa_119_one_shift_k2 - ssa_10_const1;
	APInt ssa_121_a1_low_max = ssa_33_a1_max & ssa_120_lowmask_k2;
	APInt ssa_122_a1_low_min = ssa_6_a1o & ssa_120_lowmask_k2;
	APInt ssa_123_prod_max2;
	if (ssa_117_k2.uge(ssa_117_k2.getBitWidth())) {
		ssa_123_prod_max2 = APInt(ssa_121_a1_low_max.getBitWidth(), 0);
	} else {
		ssa_123_prod_max2 = ssa_121_a1_low_max.shl(ssa_117_k2);
	}
	APInt ssa_124_prod_min2;
	if (ssa_117_k2.uge(ssa_117_k2.getBitWidth())) {
		ssa_124_prod_min2 = APInt(ssa_122_a1_low_min.getBitWidth(), 0);
	} else {
		ssa_124_prod_min2 = ssa_122_a1_low_min.shl(ssa_117_k2);
	}
	APInt ssa_125_sum_min2 = ssa_4_a0o + ssa_124_prod_min2;
	APInt ssa_126_sum_max2 = ssa_32_a0_max + ssa_123_prod_max2;
	bool ssa_127_ov2a = ssa_126_sum_max2.ult(ssa_32_a0_max);
	bool ssa_128_ov2b = ssa_126_sum_max2.ult(ssa_123_prod_max2);
	bool ssa_129_ov2 = ssa_127_ov2a || ssa_128_ov2b;
	bool ssa_130_noov2 = ssa_129_ov2 != ssa_13_true;
	bool ssa_131_cond2a = ssa_31_a2_pow2_cond && ssa_130_noov2;
	APInt ssa_132_diff2 = ssa_125_sum_min2 ^ ssa_126_sum_max2;
	APInt ssa_133_lz2d(ssa_132_diff2.getBitWidth(), ssa_132_diff2.countl_zero());
	APInt ssa_134_bw_minus_lz2 = ssa_12_bitwidth - ssa_133_lz2d;
	APInt ssa_135_mask2;
	if (ssa_134_bw_minus_lz2.uge(ssa_134_bw_minus_lz2.getBitWidth())) {
		ssa_135_mask2 = APInt(ssa_11_allones.getBitWidth(), 0);
	} else {
		ssa_135_mask2 = ssa_11_allones.shl(ssa_134_bw_minus_lz2);
	}
	APInt ssa_136_sum_min2_not = ssa_11_allones ^ ssa_125_sum_min2;
	APInt ssa_137_known_one2 = ssa_125_sum_min2 & ssa_135_mask2;
	APInt ssa_138_known_zero2 = ssa_136_sum_min2_not & ssa_135_mask2;
	APInt ssa_139_res_z2 = ssa_131_cond2a ? ssa_138_known_zero2 : ssa_9_const0;
	APInt ssa_140_res_o2 = ssa_131_cond2a ? ssa_137_known_one2 : ssa_9_const0;
	APInt ssa_141_k1(ssa_6_a1o.getBitWidth(), ssa_6_a1o.countr_zero());
	APInt ssa_142_bw_minus_k1 = ssa_12_bitwidth - ssa_141_k1;
	APInt ssa_143_one_shift_k1;
	if (ssa_142_bw_minus_k1.uge(ssa_142_bw_minus_k1.getBitWidth())) {
		ssa_143_one_shift_k1 = APInt(ssa_10_const1.getBitWidth(), 0);
	} else {
		ssa_143_one_shift_k1 = ssa_10_const1.shl(ssa_142_bw_minus_k1);
	}
	APInt ssa_144_lowmask_k1 = ssa_143_one_shift_k1 - ssa_10_const1;
	APInt ssa_145_a2_low_max = ssa_34_a2_max & ssa_144_lowmask_k1;
	APInt ssa_146_a2_low_min = ssa_8_a2o & ssa_144_lowmask_k1;
	APInt ssa_147_prod_max1;
	if (ssa_141_k1.uge(ssa_141_k1.getBitWidth())) {
		ssa_147_prod_max1 = APInt(ssa_145_a2_low_max.getBitWidth(), 0);
	} else {
		ssa_147_prod_max1 = ssa_145_a2_low_max.shl(ssa_141_k1);
	}
	APInt ssa_148_prod_min1;
	if (ssa_141_k1.uge(ssa_141_k1.getBitWidth())) {
		ssa_148_prod_min1 = APInt(ssa_146_a2_low_min.getBitWidth(), 0);
	} else {
		ssa_148_prod_min1 = ssa_146_a2_low_min.shl(ssa_141_k1);
	}
	APInt ssa_149_sum_min1 = ssa_4_a0o + ssa_148_prod_min1;
	APInt ssa_150_sum_max1 = ssa_32_a0_max + ssa_147_prod_max1;
	bool ssa_151_ov1a = ssa_150_sum_max1.ult(ssa_32_a0_max);
	bool ssa_152_ov1b = ssa_150_sum_max1.ult(ssa_147_prod_max1);
	bool ssa_153_ov1 = ssa_151_ov1a || ssa_152_ov1b;
	bool ssa_154_noov1 = ssa_153_ov1 != ssa_13_true;
	bool ssa_155_cond1a = ssa_30_a1_pow2_cond && ssa_154_noov1;
	APInt ssa_156_diff1 = ssa_149_sum_min1 ^ ssa_150_sum_max1;
	APInt ssa_157_lz1d(ssa_156_diff1.getBitWidth(), ssa_156_diff1.countl_zero());
	APInt ssa_158_bw_minus_lz1 = ssa_12_bitwidth - ssa_157_lz1d;
	APInt ssa_159_mask1;
	if (ssa_158_bw_minus_lz1.uge(ssa_158_bw_minus_lz1.getBitWidth())) {
		ssa_159_mask1 = APInt(ssa_11_allones.getBitWidth(), 0);
	} else {
		ssa_159_mask1 = ssa_11_allones.shl(ssa_158_bw_minus_lz1);
	}
	APInt ssa_160_sum_min1_not = ssa_11_allones ^ ssa_149_sum_min1;
	APInt ssa_161_known_one1 = ssa_149_sum_min1 & ssa_159_mask1;
	APInt ssa_162_known_zero1 = ssa_160_sum_min1_not & ssa_159_mask1;
	APInt ssa_163_res_z1 = ssa_155_cond1a ? ssa_162_known_zero1 : ssa_9_const0;
	APInt ssa_164_res_o1 = ssa_155_cond1a ? ssa_161_known_one1 : ssa_9_const0;
	APInt ssa_165_prod_max3 = ssa_33_a1_max * ssa_34_a2_max;
	APInt ssa_166_prod_min3 = ssa_6_a1o * ssa_8_a2o;
	APInt ssa_167_sum_min3 = ssa_4_a0o + ssa_166_prod_min3;
	APInt ssa_168_sum_max3 = ssa_32_a0_max + ssa_165_prod_max3;
	bool ssa_169_ov3a = ssa_168_sum_max3.ult(ssa_32_a0_max);
	bool ssa_170_ov3b = ssa_168_sum_max3.ult(ssa_165_prod_max3);
	bool ssa_171_ov3 = ssa_169_ov3a || ssa_170_ov3b;
	bool ssa_172_noov3 = ssa_171_ov3 != ssa_13_true;
	bool ssa_173_cond3a = ssa_39_mult_noov && ssa_172_noov3;
	APInt ssa_174_diff3 = ssa_167_sum_min3 ^ ssa_168_sum_max3;
	APInt ssa_175_lz3d(ssa_174_diff3.getBitWidth(), ssa_174_diff3.countl_zero());
	APInt ssa_176_bw_minus_lz3 = ssa_12_bitwidth - ssa_175_lz3d;
	APInt ssa_177_mask3;
	if (ssa_176_bw_minus_lz3.uge(ssa_176_bw_minus_lz3.getBitWidth())) {
		ssa_177_mask3 = APInt(ssa_11_allones.getBitWidth(), 0);
	} else {
		ssa_177_mask3 = ssa_11_allones.shl(ssa_176_bw_minus_lz3);
	}
	APInt ssa_178_sum_min3_not = ssa_11_allones ^ ssa_167_sum_min3;
	APInt ssa_179_known_one3 = ssa_167_sum_min3 & ssa_177_mask3;
	APInt ssa_180_known_zero3 = ssa_178_sum_min3_not & ssa_177_mask3;
	APInt ssa_181_res_z3 = ssa_173_cond3a ? ssa_180_known_zero3 : ssa_9_const0;
	APInt ssa_182_res_o3 = ssa_173_cond3a ? ssa_179_known_one3 : ssa_9_const0;
	bool ssa_183_pow_any = ssa_31_a2_pow2_cond || ssa_30_a1_pow2_cond;
	APInt ssa_184_res_z_pow = ssa_31_a2_pow2_cond ? ssa_139_res_z2 : ssa_163_res_z1;
	APInt ssa_185_res_o_pow = ssa_31_a2_pow2_cond ? ssa_140_res_o2 : ssa_164_res_o1;
	APInt ssa_186_res_z_prev = ssa_183_pow_any ? ssa_184_res_z_pow : ssa_181_res_z3;
	APInt ssa_187_res_o_prev = ssa_183_pow_any ? ssa_185_res_o_pow : ssa_182_res_o3;
	bool ssa_188_single_any = ssa_53_cond_single1 || ssa_69_cond_single2;
	APInt ssa_189_res_z_single = ssa_53_cond_single1 ? ssa_66_res_z_s1 : ssa_82_res_z_s2;
	APInt ssa_190_res_o_single = ssa_53_cond_single1 ? ssa_67_res_o_s1 : ssa_83_res_o_s2;
	APInt ssa_191_res_z_low_sel = ssa_107_cond_low1d ? ssa_115_res_z_low1 : ssa_186_res_z_prev;
	APInt ssa_192_res_o_low_sel = ssa_107_cond_low1d ? ssa_116_res_o_low1 : ssa_187_res_o_prev;
	APInt ssa_193_res_z_mid = ssa_188_single_any ? ssa_189_res_z_single : ssa_191_res_z_low_sel;
	APInt ssa_194_res_o_mid = ssa_188_single_any ? ssa_190_res_o_single : ssa_192_res_o_low_sel;
	std::array<APInt, 2> ssa_195_r = std::array<APInt, 2>{ssa_193_res_z_mid, ssa_194_res_o_mid};
	return ssa_195_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_const1(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_10_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_11_a0_or = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_12_a1_or = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_13_a2_or = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_14_k0(ssa_11_a0_or.getBitWidth(), ssa_11_a0_or.countr_one());
	APInt ssa_15_k1(ssa_12_a1_or.getBitWidth(), ssa_12_a1_or.countr_one());
	APInt ssa_16_k2(ssa_13_a2_or.getBitWidth(), ssa_13_a2_or.countr_one());
	APInt ssa_17_k12 = APIntOps::umin(ssa_15_k1, ssa_16_k2);
	APInt ssa_18_ksum = APIntOps::umin(ssa_14_k0, ssa_17_k12);
	APInt ssa_19_one_shift_prod;
	if (ssa_17_k12.uge(ssa_17_k12.getBitWidth())) {
		ssa_19_one_shift_prod = APInt(ssa_9_const1.getBitWidth(), 0);
	} else {
		ssa_19_one_shift_prod = ssa_9_const1.shl(ssa_17_k12);
	}
	APInt ssa_20_lowmask_prod = ssa_19_one_shift_prod - ssa_9_const1;
	APInt ssa_21_a1_low = ssa_6_a1o & ssa_20_lowmask_prod;
	APInt ssa_22_a2_low = ssa_8_a2o & ssa_20_lowmask_prod;
	APInt ssa_23_prod_low = ssa_21_a1_low * ssa_22_a2_low;
	APInt ssa_24_prod_low_masked = ssa_23_prod_low & ssa_20_lowmask_prod;
	APInt ssa_25_one_shift_sum;
	if (ssa_18_ksum.uge(ssa_18_ksum.getBitWidth())) {
		ssa_25_one_shift_sum = APInt(ssa_9_const1.getBitWidth(), 0);
	} else {
		ssa_25_one_shift_sum = ssa_9_const1.shl(ssa_18_ksum);
	}
	APInt ssa_26_lowmask_sum = ssa_25_one_shift_sum - ssa_9_const1;
	APInt ssa_27_a0_low = ssa_4_a0o & ssa_26_lowmask_sum;
	APInt ssa_28_prod_low_sum = ssa_24_prod_low_masked & ssa_26_lowmask_sum;
	APInt ssa_29_sum_low = ssa_27_a0_low + ssa_28_prod_low_sum;
	APInt ssa_30_sum_low_masked = ssa_29_sum_low & ssa_26_lowmask_sum;
	APInt ssa_31_sum_low_not = ssa_10_allones ^ ssa_30_sum_low_masked;
	APInt ssa_32_res_z = ssa_31_sum_low_not & ssa_26_lowmask_sum;
	APInt ssa_33_res_o = ssa_30_sum_low_masked & ssa_26_lowmask_sum;
	std::array<APInt, 2> ssa_34_r = std::array<APInt, 2>{ssa_32_res_z, ssa_33_res_o};
	return ssa_34_r;
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
