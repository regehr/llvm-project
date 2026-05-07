namespace Pattern046 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0_z = ssa_0_arg0[0];
	APInt ssa_4_a0_o = ssa_0_arg0[1];
	APInt ssa_5_a1_z = ssa_1_arg1[0];
	APInt ssa_6_a1_o = ssa_1_arg1[1];
	APInt ssa_7_a2_z = ssa_2_arg2[0];
	APInt ssa_8_a2_o = ssa_2_arg2[1];
	APInt ssa_9_ones = APInt::getAllOnes(ssa_3_a0_z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0_z.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_a0_z.getBitWidth(), 1);
	APInt ssa_12_bw(ssa_9_ones.getBitWidth(), ssa_9_ones.getBitWidth());
	APInt ssa_13_a0_mask = ssa_3_a0_z | ssa_4_a0_o;
	APInt ssa_14_a0_unk = ssa_13_a0_mask ^ ssa_9_ones;
	APInt ssa_15_a0_max = ssa_4_a0_o | ssa_14_a0_unk;
	APInt ssa_16_a1_mask = ssa_5_a1_z | ssa_6_a1_o;
	APInt ssa_17_a1_unk = ssa_16_a1_mask ^ ssa_9_ones;
	APInt ssa_18_a1_max = ssa_6_a1_o | ssa_17_a1_unk;
	APInt ssa_19_a2_mask = ssa_7_a2_z | ssa_8_a2_o;
	APInt ssa_20_a2_unk = ssa_19_a2_mask ^ ssa_9_ones;
	APInt ssa_21_a2_max = ssa_8_a2_o | ssa_20_a2_unk;
	APInt ssa_22_min_sum01_safe = ssa_4_a0_o + ssa_6_a1_o;
	APInt ssa_23_max_sum01_safe = ssa_15_a0_max + ssa_18_a1_max;
	bool ssa_24_sum01_ge_a0 = ssa_23_max_sum01_safe.uge(ssa_15_a0_max);
	bool ssa_25_sum01_ge_a1 = ssa_23_max_sum01_safe.uge(ssa_18_a1_max);
	bool ssa_26_add01_safe = ssa_24_sum01_ge_a0 && ssa_25_sum01_ge_a1;
	APInt ssa_27_min_sum01 = ssa_26_add01_safe ? ssa_22_min_sum01_safe : ssa_10_zero;
	APInt ssa_28_max_sum01 = ssa_26_add01_safe ? ssa_23_max_sum01_safe : ssa_9_ones;
	APInt ssa_29_diff01 = ssa_27_min_sum01 ^ ssa_28_max_sum01;
	APInt ssa_30_lz_diff01(ssa_29_diff01.getBitWidth(), ssa_29_diff01.countl_zero());
	APInt ssa_31_num_low01 = ssa_12_bw - ssa_30_lz_diff01;
	APInt ssa_32_mask01 = ssa_9_ones;
	if (ssa_31_num_low01.ule(ssa_31_num_low01.getBitWidth()))
		ssa_32_mask01.clearLowBits(ssa_31_num_low01.getZExtValue());
	else
		ssa_32_mask01.clearLowBits(ssa_31_num_low01.getBitWidth());
	APInt ssa_33_sum01_one = ssa_27_min_sum01 & ssa_32_mask01;
	APInt ssa_34_sum01_zero = ssa_32_mask01 ^ ssa_33_sum01_one;
	APInt ssa_35_min_sum02_safe = ssa_4_a0_o + ssa_8_a2_o;
	APInt ssa_36_max_sum02_safe = ssa_15_a0_max + ssa_21_a2_max;
	bool ssa_37_sum02_ge_a0 = ssa_36_max_sum02_safe.uge(ssa_15_a0_max);
	bool ssa_38_sum02_ge_a2 = ssa_36_max_sum02_safe.uge(ssa_21_a2_max);
	bool ssa_39_add02_safe = ssa_37_sum02_ge_a0 && ssa_38_sum02_ge_a2;
	APInt ssa_40_min_sum02 = ssa_39_add02_safe ? ssa_35_min_sum02_safe : ssa_10_zero;
	APInt ssa_41_max_sum02 = ssa_39_add02_safe ? ssa_36_max_sum02_safe : ssa_9_ones;
	APInt ssa_42_diff02 = ssa_40_min_sum02 ^ ssa_41_max_sum02;
	APInt ssa_43_lz_diff02(ssa_42_diff02.getBitWidth(), ssa_42_diff02.countl_zero());
	APInt ssa_44_num_low02 = ssa_12_bw - ssa_43_lz_diff02;
	APInt ssa_45_mask02 = ssa_9_ones;
	if (ssa_44_num_low02.ule(ssa_44_num_low02.getBitWidth()))
		ssa_45_mask02.clearLowBits(ssa_44_num_low02.getZExtValue());
	else
		ssa_45_mask02.clearLowBits(ssa_44_num_low02.getBitWidth());
	APInt ssa_46_sum02_one = ssa_40_min_sum02 & ssa_45_mask02;
	APInt ssa_47_sum02_zero = ssa_45_mask02 ^ ssa_46_sum02_one;
	APInt ssa_48_a1_lz(ssa_18_a1_max.getBitWidth(), ssa_18_a1_max.countl_zero());
	APInt ssa_49_a2_lz(ssa_21_a2_max.getBitWidth(), ssa_21_a2_max.countl_zero());
	APInt ssa_50_lz_sum = ssa_48_a1_lz + ssa_49_a2_lz;
	bool ssa_51_mul_safe = ssa_50_lz_sum.uge(ssa_12_bw);
	APInt ssa_52_min_mul_safe = ssa_6_a1_o * ssa_8_a2_o;
	APInt ssa_53_max_mul_safe = ssa_18_a1_max * ssa_21_a2_max;
	APInt ssa_54_min_mul = ssa_51_mul_safe ? ssa_52_min_mul_safe : ssa_10_zero;
	APInt ssa_55_max_mul = ssa_51_mul_safe ? ssa_53_max_mul_safe : ssa_9_ones;
	APInt ssa_56_min_sum_safe = ssa_4_a0_o + ssa_54_min_mul;
	APInt ssa_57_max_sum_safe = ssa_15_a0_max + ssa_55_max_mul;
	bool ssa_58_sum_ge_a0 = ssa_57_max_sum_safe.uge(ssa_15_a0_max);
	bool ssa_59_sum_ge_mul = ssa_57_max_sum_safe.uge(ssa_55_max_mul);
	bool ssa_60_add_safe = ssa_58_sum_ge_a0 && ssa_59_sum_ge_mul;
	APInt ssa_61_min_sum = ssa_60_add_safe ? ssa_56_min_sum_safe : ssa_10_zero;
	APInt ssa_62_max_sum = ssa_60_add_safe ? ssa_57_max_sum_safe : ssa_9_ones;
	APInt ssa_63_diff = ssa_61_min_sum ^ ssa_62_max_sum;
	APInt ssa_64_lz_diff(ssa_63_diff.getBitWidth(), ssa_63_diff.countl_zero());
	APInt ssa_65_num_low = ssa_12_bw - ssa_64_lz_diff;
	APInt ssa_66_mask = ssa_9_ones;
	if (ssa_65_num_low.ule(ssa_65_num_low.getBitWidth()))
		ssa_66_mask.clearLowBits(ssa_65_num_low.getZExtValue());
	else
		ssa_66_mask.clearLowBits(ssa_65_num_low.getBitWidth());
	APInt ssa_67_res_one_gen = ssa_61_min_sum & ssa_66_mask;
	APInt ssa_68_res_zero_gen = ssa_66_mask ^ ssa_67_res_one_gen;
	APInt ssa_69_res_zero_a1 = ssa_3_a0_z & ssa_47_sum02_zero;
	APInt ssa_70_res_one_a1 = ssa_4_a0_o & ssa_46_sum02_one;
	APInt ssa_71_res_zero_a2 = ssa_3_a0_z & ssa_34_sum01_zero;
	APInt ssa_72_res_one_a2 = ssa_4_a0_o & ssa_33_sum01_one;
	APInt ssa_73_res_zero_both = ssa_69_res_zero_a1 & ssa_71_res_zero_a2;
	APInt ssa_74_res_one_both = ssa_70_res_one_a1 & ssa_72_res_one_a2;
	bool ssa_75_a1_le1 = ssa_18_a1_max.ule(ssa_11_one);
	bool ssa_76_a2_le1 = ssa_21_a2_max.ule(ssa_11_one);
	bool ssa_77_true = ssa_10_zero.eq(ssa_10_zero);
	bool ssa_78_not_a1_le1 = ssa_75_a1_le1 != ssa_77_true;
	bool ssa_79_not_a2_le1 = ssa_76_a2_le1 != ssa_77_true;
	bool ssa_80_a1_only = ssa_75_a1_le1 && ssa_79_not_a2_le1;
	bool ssa_81_a2_only = ssa_76_a2_le1 && ssa_78_not_a1_le1;
	bool ssa_82_both_le1 = ssa_75_a1_le1 && ssa_76_a2_le1;
	bool ssa_83_a1_o_eq_ones = ssa_6_a1_o.eq(ssa_9_ones);
	bool ssa_84_a1_z_eq_zero = ssa_5_a1_z.eq(ssa_10_zero);
	bool ssa_85_a1_one = ssa_83_a1_o_eq_ones && ssa_84_a1_z_eq_zero;
	bool ssa_86_a2_o_eq_ones = ssa_8_a2_o.eq(ssa_9_ones);
	bool ssa_87_a2_z_eq_zero = ssa_7_a2_z.eq(ssa_10_zero);
	bool ssa_88_a2_one = ssa_86_a2_o_eq_ones && ssa_87_a2_z_eq_zero;
	bool ssa_89_a1_z_eq_ones = ssa_5_a1_z.eq(ssa_9_ones);
	bool ssa_90_a1_o_eq_zero = ssa_6_a1_o.eq(ssa_10_zero);
	bool ssa_91_a1_zero = ssa_89_a1_z_eq_ones && ssa_90_a1_o_eq_zero;
	bool ssa_92_a2_z_eq_ones = ssa_7_a2_z.eq(ssa_9_ones);
	bool ssa_93_a2_o_eq_zero = ssa_8_a2_o.eq(ssa_10_zero);
	bool ssa_94_a2_zero = ssa_92_a2_z_eq_ones && ssa_93_a2_o_eq_zero;
	bool ssa_95_mul_zero = ssa_91_a1_zero || ssa_94_a2_zero;
	APInt ssa_96_res_zero = ssa_82_both_le1 ? ssa_73_res_zero_both : ssa_68_res_zero_gen;
	APInt ssa_97_res_one = ssa_82_both_le1 ? ssa_74_res_one_both : ssa_67_res_one_gen;
	APInt ssa_98_res_zero = ssa_80_a1_only ? ssa_69_res_zero_a1 : ssa_96_res_zero;
	APInt ssa_99_res_one = ssa_80_a1_only ? ssa_70_res_one_a1 : ssa_97_res_one;
	APInt ssa_100_res_zero = ssa_81_a2_only ? ssa_71_res_zero_a2 : ssa_98_res_zero;
	APInt ssa_101_res_one = ssa_81_a2_only ? ssa_72_res_one_a2 : ssa_99_res_one;
	APInt ssa_102_res_zero = ssa_85_a1_one ? ssa_47_sum02_zero : ssa_100_res_zero;
	APInt ssa_103_res_one = ssa_85_a1_one ? ssa_46_sum02_one : ssa_101_res_one;
	APInt ssa_104_res_zero = ssa_88_a2_one ? ssa_34_sum01_zero : ssa_102_res_zero;
	APInt ssa_105_res_one = ssa_88_a2_one ? ssa_33_sum01_one : ssa_103_res_one;
	APInt ssa_106_res_zero = ssa_95_mul_zero ? ssa_3_a0_z : ssa_104_res_zero;
	APInt ssa_107_res_one = ssa_95_mul_zero ? ssa_4_a0_o : ssa_105_res_one;
	std::array<APInt, 2> ssa_108_r = std::array<APInt, 2>{ssa_106_res_zero, ssa_107_res_one};
	return ssa_108_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0_z = ssa_0_arg0[0];
	APInt ssa_4_a0_o = ssa_0_arg0[1];
	APInt ssa_5_a1_z = ssa_1_arg1[0];
	APInt ssa_6_a1_o = ssa_1_arg1[1];
	APInt ssa_7_a2_z = ssa_2_arg2[0];
	APInt ssa_8_a2_o = ssa_2_arg2[1];
	APInt ssa_9_ones = APInt::getAllOnes(ssa_3_a0_z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0_z.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_a0_z.getBitWidth(), 1);
	APInt ssa_12_two(ssa_3_a0_z.getBitWidth(), 2);
	APInt ssa_13_three(ssa_3_a0_z.getBitWidth(), 3);
	APInt ssa_14_bw(ssa_9_ones.getBitWidth(), ssa_9_ones.getBitWidth());
	APInt ssa_15_bw_minus1 = ssa_14_bw - ssa_11_one;
	APInt ssa_16_sign_mask;
	if (ssa_15_bw_minus1.uge(ssa_15_bw_minus1.getBitWidth())) {
		ssa_16_sign_mask = APInt(ssa_11_one.getBitWidth(), 0);
	} else {
		ssa_16_sign_mask = ssa_11_one.shl(ssa_15_bw_minus1);
	}
	APInt ssa_17_a0_mask = ssa_3_a0_z | ssa_4_a0_o;
	APInt ssa_18_a0_unk = ssa_17_a0_mask ^ ssa_9_ones;
	APInt ssa_19_a0_max = ssa_4_a0_o | ssa_18_a0_unk;
	APInt ssa_20_a2_mask = ssa_7_a2_z | ssa_8_a2_o;
	bool ssa_21_a2_const = ssa_20_a2_mask.eq(ssa_9_ones);
	APInt ssa_22_a2_sign = ssa_8_a2_o & ssa_16_sign_mask;
	bool ssa_23_a2_nonneg = ssa_22_a2_sign.eq(ssa_10_zero);
	APInt ssa_24_max_signed;
	if (ssa_11_one.uge(ssa_11_one.getBitWidth())) {
		ssa_24_max_signed = APInt(ssa_9_ones.getBitWidth(), 0);
	} else {
		ssa_24_max_signed = ssa_9_ones.lshr(ssa_11_one);
	}
	bool ssa_25_a2_zero = ssa_8_a2_o.eq(ssa_10_zero);
	APInt ssa_26_a2_div = ssa_25_a2_zero ? ssa_11_one : ssa_8_a2_o;
	APInt ssa_27_bound_a1;
	if (ssa_26_a2_div == 0) {
		ssa_27_bound_a1 = APInt(ssa_24_max_signed.getBitWidth(), -1);
	} else {
		ssa_27_bound_a1 = ssa_24_max_signed.udiv(ssa_26_a2_div);
	}
	bool ssa_28_bound_le3 = ssa_27_bound_a1.ule(ssa_13_three);
	bool ssa_29_pre0 = ssa_21_a2_const && ssa_28_bound_le3;
	bool ssa_30_pre = ssa_29_pre0 && ssa_23_a2_nonneg;
	APInt ssa_31_not0 = ssa_10_zero ^ ssa_9_ones;
	APInt ssa_32_m0 = ssa_6_a1_o & ssa_31_not0;
	APInt ssa_33_m0 = ssa_5_a1_z & ssa_10_zero;
	APInt ssa_34_m0 = ssa_32_m0 | ssa_33_m0;
	bool ssa_35_p0_raw = ssa_34_m0.eq(ssa_10_zero);
	bool ssa_36_v0_le = ssa_10_zero.ule(ssa_27_bound_a1);
	bool ssa_37_p0 = ssa_35_p0_raw && ssa_36_v0_le;
	APInt ssa_38_not1 = ssa_11_one ^ ssa_9_ones;
	APInt ssa_39_m1 = ssa_6_a1_o & ssa_38_not1;
	APInt ssa_40_m1 = ssa_5_a1_z & ssa_11_one;
	APInt ssa_41_m1 = ssa_39_m1 | ssa_40_m1;
	bool ssa_42_p1_raw = ssa_41_m1.eq(ssa_10_zero);
	bool ssa_43_v1_le = ssa_11_one.ule(ssa_27_bound_a1);
	bool ssa_44_p1 = ssa_42_p1_raw && ssa_43_v1_le;
	APInt ssa_45_not2 = ssa_12_two ^ ssa_9_ones;
	APInt ssa_46_m2 = ssa_6_a1_o & ssa_45_not2;
	APInt ssa_47_m2 = ssa_5_a1_z & ssa_12_two;
	APInt ssa_48_m2 = ssa_46_m2 | ssa_47_m2;
	bool ssa_49_p2_raw = ssa_48_m2.eq(ssa_10_zero);
	bool ssa_50_v2_le = ssa_12_two.ule(ssa_27_bound_a1);
	bool ssa_51_p2 = ssa_49_p2_raw && ssa_50_v2_le;
	APInt ssa_52_not3 = ssa_13_three ^ ssa_9_ones;
	APInt ssa_53_m3 = ssa_6_a1_o & ssa_52_not3;
	APInt ssa_54_m3 = ssa_5_a1_z & ssa_13_three;
	APInt ssa_55_m3 = ssa_53_m3 | ssa_54_m3;
	bool ssa_56_p3_raw = ssa_55_m3.eq(ssa_10_zero);
	bool ssa_57_v3_le = ssa_13_three.ule(ssa_27_bound_a1);
	bool ssa_58_p3 = ssa_56_p3_raw && ssa_57_v3_le;
	APInt ssa_59_prod0 = ssa_8_a2_o * ssa_10_zero;
	APInt ssa_60_bound0 = ssa_9_ones - ssa_59_prod0;
	APInt ssa_61_a0_max0 = APIntOps::umin(ssa_19_a0_max, ssa_60_bound0);
	APInt ssa_62_min_sum0 = ssa_4_a0_o + ssa_59_prod0;
	APInt ssa_63_max_sum0 = ssa_61_a0_max0 + ssa_59_prod0;
	APInt ssa_64_diff0 = ssa_62_min_sum0 ^ ssa_63_max_sum0;
	APInt ssa_65_lz0(ssa_64_diff0.getBitWidth(), ssa_64_diff0.countl_zero());
	APInt ssa_66_num_low0 = ssa_14_bw - ssa_65_lz0;
	APInt ssa_67_mask0 = ssa_9_ones;
	if (ssa_66_num_low0.ule(ssa_66_num_low0.getBitWidth()))
		ssa_67_mask0.clearLowBits(ssa_66_num_low0.getZExtValue());
	else
		ssa_67_mask0.clearLowBits(ssa_66_num_low0.getBitWidth());
	APInt ssa_68_sum0_one = ssa_62_min_sum0 & ssa_67_mask0;
	APInt ssa_69_sum0_zero = ssa_67_mask0 ^ ssa_68_sum0_one;
	APInt ssa_70_prod1 = ssa_8_a2_o * ssa_11_one;
	APInt ssa_71_bound1 = ssa_9_ones - ssa_70_prod1;
	APInt ssa_72_a0_max1 = APIntOps::umin(ssa_19_a0_max, ssa_71_bound1);
	APInt ssa_73_min_sum1 = ssa_4_a0_o + ssa_70_prod1;
	APInt ssa_74_max_sum1 = ssa_72_a0_max1 + ssa_70_prod1;
	APInt ssa_75_diff1 = ssa_73_min_sum1 ^ ssa_74_max_sum1;
	APInt ssa_76_lz1(ssa_75_diff1.getBitWidth(), ssa_75_diff1.countl_zero());
	APInt ssa_77_num_low1 = ssa_14_bw - ssa_76_lz1;
	APInt ssa_78_mask1 = ssa_9_ones;
	if (ssa_77_num_low1.ule(ssa_77_num_low1.getBitWidth()))
		ssa_78_mask1.clearLowBits(ssa_77_num_low1.getZExtValue());
	else
		ssa_78_mask1.clearLowBits(ssa_77_num_low1.getBitWidth());
	APInt ssa_79_sum1_one = ssa_73_min_sum1 & ssa_78_mask1;
	APInt ssa_80_sum1_zero = ssa_78_mask1 ^ ssa_79_sum1_one;
	APInt ssa_81_prod2 = ssa_8_a2_o * ssa_12_two;
	APInt ssa_82_bound2 = ssa_9_ones - ssa_81_prod2;
	APInt ssa_83_a0_max2 = APIntOps::umin(ssa_19_a0_max, ssa_82_bound2);
	APInt ssa_84_min_sum2 = ssa_4_a0_o + ssa_81_prod2;
	APInt ssa_85_max_sum2 = ssa_83_a0_max2 + ssa_81_prod2;
	APInt ssa_86_diff2 = ssa_84_min_sum2 ^ ssa_85_max_sum2;
	APInt ssa_87_lz2(ssa_86_diff2.getBitWidth(), ssa_86_diff2.countl_zero());
	APInt ssa_88_num_low2 = ssa_14_bw - ssa_87_lz2;
	APInt ssa_89_mask2 = ssa_9_ones;
	if (ssa_88_num_low2.ule(ssa_88_num_low2.getBitWidth()))
		ssa_89_mask2.clearLowBits(ssa_88_num_low2.getZExtValue());
	else
		ssa_89_mask2.clearLowBits(ssa_88_num_low2.getBitWidth());
	APInt ssa_90_sum2_one = ssa_84_min_sum2 & ssa_89_mask2;
	APInt ssa_91_sum2_zero = ssa_89_mask2 ^ ssa_90_sum2_one;
	APInt ssa_92_prod3 = ssa_8_a2_o * ssa_13_three;
	APInt ssa_93_bound3 = ssa_9_ones - ssa_92_prod3;
	APInt ssa_94_a0_max3 = APIntOps::umin(ssa_19_a0_max, ssa_93_bound3);
	APInt ssa_95_min_sum3 = ssa_4_a0_o + ssa_92_prod3;
	APInt ssa_96_max_sum3 = ssa_94_a0_max3 + ssa_92_prod3;
	APInt ssa_97_diff3 = ssa_95_min_sum3 ^ ssa_96_max_sum3;
	APInt ssa_98_lz3(ssa_97_diff3.getBitWidth(), ssa_97_diff3.countl_zero());
	APInt ssa_99_num_low3 = ssa_14_bw - ssa_98_lz3;
	APInt ssa_100_mask3 = ssa_9_ones;
	if (ssa_99_num_low3.ule(ssa_99_num_low3.getBitWidth()))
		ssa_100_mask3.clearLowBits(ssa_99_num_low3.getZExtValue());
	else
		ssa_100_mask3.clearLowBits(ssa_99_num_low3.getBitWidth());
	APInt ssa_101_sum3_one = ssa_95_min_sum3 & ssa_100_mask3;
	APInt ssa_102_sum3_zero = ssa_100_mask3 ^ ssa_101_sum3_one;
	APInt ssa_103_t0_one = ssa_37_p0 ? ssa_68_sum0_one : ssa_9_ones;
	APInt ssa_104_t1_one = ssa_44_p1 ? ssa_79_sum1_one : ssa_9_ones;
	APInt ssa_105_t2_one = ssa_51_p2 ? ssa_90_sum2_one : ssa_9_ones;
	APInt ssa_106_t3_one = ssa_58_p3 ? ssa_101_sum3_one : ssa_9_ones;
	APInt ssa_107_t0_zero = ssa_37_p0 ? ssa_69_sum0_zero : ssa_9_ones;
	APInt ssa_108_t1_zero = ssa_44_p1 ? ssa_80_sum1_zero : ssa_9_ones;
	APInt ssa_109_t2_zero = ssa_51_p2 ? ssa_91_sum2_zero : ssa_9_ones;
	APInt ssa_110_t3_zero = ssa_58_p3 ? ssa_102_sum3_zero : ssa_9_ones;
	APInt ssa_111_res_one_a = ssa_103_t0_one & ssa_104_t1_one;
	APInt ssa_112_res_one_b = ssa_105_t2_one & ssa_106_t3_one;
	APInt ssa_113_res_one_enum = ssa_111_res_one_a & ssa_112_res_one_b;
	APInt ssa_114_res_zero_a = ssa_107_t0_zero & ssa_108_t1_zero;
	APInt ssa_115_res_zero_b = ssa_109_t2_zero & ssa_110_t3_zero;
	APInt ssa_116_res_zero_enum = ssa_114_res_zero_a & ssa_115_res_zero_b;
	APInt ssa_117_res_one = ssa_30_pre ? ssa_113_res_one_enum : ssa_10_zero;
	APInt ssa_118_res_zero = ssa_30_pre ? ssa_116_res_zero_enum : ssa_10_zero;
	std::array<APInt, 2> ssa_119_r = std::array<APInt, 2>{ssa_118_res_zero, ssa_117_res_one};
	return ssa_119_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0_z = ssa_0_arg0[0];
	APInt ssa_4_a0_o = ssa_0_arg0[1];
	APInt ssa_5_a1_z = ssa_1_arg1[0];
	APInt ssa_6_a1_o = ssa_1_arg1[1];
	APInt ssa_7_a2_z = ssa_2_arg2[0];
	APInt ssa_8_a2_o = ssa_2_arg2[1];
	APInt ssa_9_ones = APInt::getAllOnes(ssa_3_a0_z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0_z.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_a0_z.getBitWidth(), 1);
	APInt ssa_12_a1_mask = ssa_5_a1_z | ssa_6_a1_o;
	APInt ssa_13_a1_unk = ssa_12_a1_mask ^ ssa_9_ones;
	APInt ssa_14_a1_max = ssa_6_a1_o | ssa_13_a1_unk;
	APInt ssa_15_a2_mask = ssa_7_a2_z | ssa_8_a2_o;
	APInt ssa_16_a2_unk = ssa_15_a2_mask ^ ssa_9_ones;
	APInt ssa_17_a2_max = ssa_8_a2_o | ssa_16_a2_unk;
	bool ssa_18_a1_le1 = ssa_14_a1_max.ule(ssa_11_one);
	bool ssa_19_a2_le1 = ssa_17_a2_max.ule(ssa_11_one);
	APInt ssa_20_a0_known = ssa_3_a0_z | ssa_4_a0_o;
	APInt ssa_21_a1_known = ssa_5_a1_z | ssa_6_a1_o;
	APInt ssa_22_a2_known = ssa_7_a2_z | ssa_8_a2_o;
	APInt ssa_23_k0(ssa_20_a0_known.getBitWidth(), ssa_20_a0_known.countr_one());
	APInt ssa_24_k1(ssa_21_a1_known.getBitWidth(), ssa_21_a1_known.countr_one());
	APInt ssa_25_k2(ssa_22_a2_known.getBitWidth(), ssa_22_a2_known.countr_one());
	APInt ssa_26_k01 = APIntOps::umin(ssa_23_k0, ssa_24_k1);
	APInt ssa_27_k02 = APIntOps::umin(ssa_23_k0, ssa_25_k2);
	APInt ssa_28_mask_low1 = ssa_10_zero;
	if (ssa_27_k02.ule(ssa_27_k02.getBitWidth()))
		ssa_28_mask_low1.setLowBits(ssa_27_k02.getZExtValue());
	else
		ssa_28_mask_low1.setLowBits(ssa_27_k02.getBitWidth());
	APInt ssa_29_mask_low2 = ssa_10_zero;
	if (ssa_26_k01.ule(ssa_26_k01.getBitWidth()))
		ssa_29_mask_low2.setLowBits(ssa_26_k01.getZExtValue());
	else
		ssa_29_mask_low2.setLowBits(ssa_26_k01.getBitWidth());
	APInt ssa_30_a0_low1 = ssa_4_a0_o & ssa_28_mask_low1;
	APInt ssa_31_a2_low = ssa_8_a2_o & ssa_28_mask_low1;
	APInt ssa_32_low_sum1 = ssa_30_a0_low1 + ssa_31_a2_low;
	APInt ssa_33_low_sum1_m = ssa_32_low_sum1 & ssa_28_mask_low1;
	APInt ssa_34_sum1_zero = ssa_28_mask_low1 ^ ssa_33_low_sum1_m;
	APInt ssa_35_res1_zero = ssa_3_a0_z & ssa_34_sum1_zero;
	APInt ssa_36_res1_one = ssa_4_a0_o & ssa_33_low_sum1_m;
	APInt ssa_37_res1_zero_s = ssa_18_a1_le1 ? ssa_35_res1_zero : ssa_10_zero;
	APInt ssa_38_res1_one_s = ssa_18_a1_le1 ? ssa_36_res1_one : ssa_10_zero;
	APInt ssa_39_a0_low2 = ssa_4_a0_o & ssa_29_mask_low2;
	APInt ssa_40_a1_low = ssa_6_a1_o & ssa_29_mask_low2;
	APInt ssa_41_low_sum2 = ssa_39_a0_low2 + ssa_40_a1_low;
	APInt ssa_42_low_sum2_m = ssa_41_low_sum2 & ssa_29_mask_low2;
	APInt ssa_43_sum2_zero = ssa_29_mask_low2 ^ ssa_42_low_sum2_m;
	APInt ssa_44_res2_zero = ssa_3_a0_z & ssa_43_sum2_zero;
	APInt ssa_45_res2_one = ssa_4_a0_o & ssa_42_low_sum2_m;
	APInt ssa_46_res2_zero_s = ssa_19_a2_le1 ? ssa_44_res2_zero : ssa_10_zero;
	APInt ssa_47_res2_one_s = ssa_19_a2_le1 ? ssa_45_res2_one : ssa_10_zero;
	APInt ssa_48_res_zero0 = ssa_37_res1_zero_s | ssa_46_res2_zero_s;
	APInt ssa_49_res_one0 = ssa_38_res1_one_s | ssa_47_res2_one_s;
	APInt ssa_50_a0_lsb_zero = ssa_3_a0_z & ssa_11_one;
	APInt ssa_51_a0_lsb_one = ssa_4_a0_o & ssa_11_one;
	APInt ssa_52_a1_lsb_zero = ssa_5_a1_z & ssa_11_one;
	APInt ssa_53_a1_lsb_one = ssa_6_a1_o & ssa_11_one;
	APInt ssa_54_a2_lsb_zero = ssa_7_a2_z & ssa_11_one;
	APInt ssa_55_a2_lsb_one = ssa_8_a2_o & ssa_11_one;
	APInt ssa_56_prod_lsb_zero = ssa_52_a1_lsb_zero | ssa_54_a2_lsb_zero;
	APInt ssa_57_prod_lsb_one = ssa_53_a1_lsb_one & ssa_55_a2_lsb_one;
	APInt ssa_58_res_lsb_one_a = ssa_51_a0_lsb_one & ssa_56_prod_lsb_zero;
	APInt ssa_59_res_lsb_one_b = ssa_50_a0_lsb_zero & ssa_57_prod_lsb_one;
	APInt ssa_60_res_lsb_one = ssa_58_res_lsb_one_a | ssa_59_res_lsb_one_b;
	APInt ssa_61_res_lsb_zero_a = ssa_50_a0_lsb_zero & ssa_56_prod_lsb_zero;
	APInt ssa_62_res_lsb_zero_b = ssa_51_a0_lsb_one & ssa_57_prod_lsb_one;
	APInt ssa_63_res_lsb_zero = ssa_61_res_lsb_zero_a | ssa_62_res_lsb_zero_b;
	APInt ssa_64_res_zero = ssa_48_res_zero0 | ssa_63_res_lsb_zero;
	APInt ssa_65_res_one = ssa_49_res_one0 | ssa_60_res_lsb_one;
	std::array<APInt, 2> ssa_66_r = std::array<APInt, 2>{ssa_64_res_zero, ssa_65_res_one};
	return ssa_66_r;
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
