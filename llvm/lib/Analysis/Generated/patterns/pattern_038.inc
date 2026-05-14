namespace Pattern038 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_11_sign_mask = ssa_10_zero;
	ssa_11_sign_mask.setSignBit();
	APInt ssa_12_known_b = ssa_5_b0 | ssa_6_b1;
	bool ssa_13_cond_b_full = ssa_12_known_b.eq(ssa_9_allones);
	APInt ssa_14_kz_shifted;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_7_c0.isSignBitSet()) {
		ssa_14_kz_shifted = APInt(ssa_7_c0.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_7_c0.isSignBitClear()) {
		ssa_14_kz_shifted = APInt(ssa_7_c0.getBitWidth(), 0);
	} else {
		ssa_14_kz_shifted = ssa_7_c0.ashr(ssa_6_b1);
	}
	APInt ssa_15_ko_shifted;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_8_c1.isSignBitSet()) {
		ssa_15_ko_shifted = APInt(ssa_8_c1.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_8_c1.isSignBitClear()) {
		ssa_15_ko_shifted = APInt(ssa_8_c1.getBitWidth(), 0);
	} else {
		ssa_15_ko_shifted = ssa_8_c1.ashr(ssa_6_b1);
	}
	APInt ssa_16_c0_sign = ssa_7_c0 & ssa_11_sign_mask;
	APInt ssa_17_c1_sign = ssa_8_c1 & ssa_11_sign_mask;
	APInt ssa_18_kz_d = ssa_13_cond_b_full ? ssa_14_kz_shifted : ssa_16_c0_sign;
	APInt ssa_19_ko_d = ssa_13_cond_b_full ? ssa_15_ko_shifted : ssa_17_c1_sign;
	APInt ssa_20_known_d = ssa_18_kz_d | ssa_19_ko_d;
	bool ssa_21_cond_d_full = ssa_20_known_d.eq(ssa_9_allones);
	APInt ssa_22_known_a = ssa_3_a0 | ssa_4_a1;
	bool ssa_23_cond_a_full = ssa_22_known_a.eq(ssa_9_allones);
	bool ssa_24_a_zero_val = ssa_4_a1.eq(ssa_10_zero);
	bool ssa_25_cond_a_zero = ssa_23_cond_a_full && ssa_24_a_zero_val;
	bool ssa_26_d_zero_val = ssa_19_ko_d.eq(ssa_10_zero);
	bool ssa_27_cond_d_zero = ssa_21_cond_d_full && ssa_26_d_zero_val;
	bool ssa_28_cond_full_sum = ssa_23_cond_a_full && ssa_21_cond_d_full;
	APInt ssa_29_sum_val = ssa_4_a1 + ssa_19_ko_d;
	APInt ssa_30_kz_full = ssa_29_sum_val ^ ssa_9_allones;
	APInt ssa_31_ko_full = ssa_29_sum_val | ssa_10_zero;
	APInt ssa_32_a_sign1 = ssa_4_a1 & ssa_11_sign_mask;
	bool ssa_33_a_sign1_cond = ssa_32_a_sign1.eq(ssa_11_sign_mask);
	APInt ssa_34_a_sign0 = ssa_3_a0 & ssa_11_sign_mask;
	bool ssa_35_a_sign0_cond = ssa_34_a_sign0.eq(ssa_11_sign_mask);
	APInt ssa_36_d_sign1 = ssa_19_ko_d & ssa_11_sign_mask;
	bool ssa_37_d_sign1_cond = ssa_36_d_sign1.eq(ssa_11_sign_mask);
	APInt ssa_38_d_sign0 = ssa_18_kz_d & ssa_11_sign_mask;
	bool ssa_39_d_sign0_cond = ssa_38_d_sign0.eq(ssa_11_sign_mask);
	bool ssa_40_cond_same_sign1 = ssa_33_a_sign1_cond && ssa_37_d_sign1_cond;
	bool ssa_41_cond_same_sign0 = ssa_35_a_sign0_cond && ssa_39_d_sign0_cond;
	APInt ssa_42_kz_sign = ssa_41_cond_same_sign0 ? ssa_11_sign_mask : ssa_10_zero;
	APInt ssa_43_ko_sign = ssa_40_cond_same_sign1 ? ssa_11_sign_mask : ssa_10_zero;
	APInt ssa_44_tza(ssa_3_a0.getBitWidth(), ssa_3_a0.countr_one());
	APInt ssa_45_tzd(ssa_18_kz_d.getBitWidth(), ssa_18_kz_d.countr_one());
	APInt ssa_46_k = APIntOps::umin(ssa_44_tza, ssa_45_tzd);
	APInt ssa_47_lowmask = ssa_10_zero;
	if (ssa_46_k.ule(ssa_46_k.getBitWidth()))
		ssa_47_lowmask.setLowBits(ssa_46_k.getZExtValue());
	else
		ssa_47_lowmask.setLowBits(ssa_46_k.getBitWidth());
	APInt ssa_48_kz_base = ssa_42_kz_sign | ssa_47_lowmask;
	APInt ssa_49_known_dmask = ssa_18_kz_d | ssa_19_ko_d;
	APInt ssa_50_nd(ssa_49_known_dmask.getBitWidth(), ssa_49_known_dmask.countr_one());
	APInt ssa_51_mask_low_d = ssa_10_zero;
	if (ssa_50_nd.ule(ssa_50_nd.getBitWidth()))
		ssa_51_mask_low_d.setLowBits(ssa_50_nd.getZExtValue());
	else
		ssa_51_mask_low_d.setLowBits(ssa_50_nd.getBitWidth());
	APInt ssa_52_low_d = ssa_19_ko_d & ssa_51_mask_low_d;
	APInt ssa_53_low_a_const = ssa_4_a1 & ssa_51_mask_low_d;
	APInt ssa_54_sum_low_d = ssa_52_low_d + ssa_53_low_a_const;
	APInt ssa_55_sum_low_bits_d = ssa_54_sum_low_d & ssa_51_mask_low_d;
	APInt ssa_56_kz_low_d = ssa_55_sum_low_bits_d ^ ssa_51_mask_low_d;
	APInt ssa_57_ko_low_d = ssa_55_sum_low_bits_d | ssa_10_zero;
	APInt ssa_58_known_amask = ssa_3_a0 | ssa_4_a1;
	APInt ssa_59_na(ssa_58_known_amask.getBitWidth(), ssa_58_known_amask.countr_one());
	APInt ssa_60_mask_low_a = ssa_10_zero;
	if (ssa_59_na.ule(ssa_59_na.getBitWidth()))
		ssa_60_mask_low_a.setLowBits(ssa_59_na.getZExtValue());
	else
		ssa_60_mask_low_a.setLowBits(ssa_59_na.getBitWidth());
	APInt ssa_61_low_a = ssa_4_a1 & ssa_60_mask_low_a;
	APInt ssa_62_low_d_const = ssa_19_ko_d & ssa_60_mask_low_a;
	APInt ssa_63_sum_low_a = ssa_61_low_a + ssa_62_low_d_const;
	APInt ssa_64_sum_low_bits_a = ssa_63_sum_low_a & ssa_60_mask_low_a;
	APInt ssa_65_kz_low_a = ssa_64_sum_low_bits_a ^ ssa_60_mask_low_a;
	APInt ssa_66_ko_low_a = ssa_64_sum_low_bits_a | ssa_10_zero;
	APInt ssa_67_kz_const_a = ssa_42_kz_sign | ssa_56_kz_low_d;
	APInt ssa_68_ko_const_a = ssa_43_ko_sign | ssa_57_ko_low_d;
	APInt ssa_69_kz_const_d = ssa_42_kz_sign | ssa_65_kz_low_a;
	APInt ssa_70_ko_const_d = ssa_43_ko_sign | ssa_66_ko_low_a;
	APInt ssa_71_res_kz0 = ssa_23_cond_a_full ? ssa_67_kz_const_a : ssa_48_kz_base;
	APInt ssa_72_res_ko0 = ssa_23_cond_a_full ? ssa_68_ko_const_a : ssa_43_ko_sign;
	APInt ssa_73_res_kz1 = ssa_21_cond_d_full ? ssa_69_kz_const_d : ssa_71_res_kz0;
	APInt ssa_74_res_ko1 = ssa_21_cond_d_full ? ssa_70_ko_const_d : ssa_72_res_ko0;
	APInt ssa_75_res_kz2 = ssa_27_cond_d_zero ? ssa_3_a0 : ssa_73_res_kz1;
	APInt ssa_76_res_ko2 = ssa_27_cond_d_zero ? ssa_4_a1 : ssa_74_res_ko1;
	APInt ssa_77_res_kz3 = ssa_25_cond_a_zero ? ssa_18_kz_d : ssa_75_res_kz2;
	APInt ssa_78_res_ko3 = ssa_25_cond_a_zero ? ssa_19_ko_d : ssa_76_res_ko2;
	APInt ssa_79_res_kz4 = ssa_28_cond_full_sum ? ssa_30_kz_full : ssa_77_res_kz3;
	APInt ssa_80_res_ko4 = ssa_28_cond_full_sum ? ssa_31_ko_full : ssa_78_res_ko3;
	std::array<APInt, 2> ssa_81_r = std::array<APInt, 2>{ssa_79_res_kz4, ssa_80_res_ko4};
	return ssa_81_r;
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
	APInt ssa_11_one(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_12_sign_mask = ssa_10_zero;
	ssa_12_sign_mask.setSignBit();
	APInt ssa_13_known_b = ssa_5_b0 | ssa_6_b1;
	bool ssa_14_cond_b_full = ssa_13_known_b.eq(ssa_9_allones);
	APInt ssa_15_kz_shifted;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_7_c0.isSignBitSet()) {
		ssa_15_kz_shifted = APInt(ssa_7_c0.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_7_c0.isSignBitClear()) {
		ssa_15_kz_shifted = APInt(ssa_7_c0.getBitWidth(), 0);
	} else {
		ssa_15_kz_shifted = ssa_7_c0.ashr(ssa_6_b1);
	}
	APInt ssa_16_ko_shifted;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_8_c1.isSignBitSet()) {
		ssa_16_ko_shifted = APInt(ssa_8_c1.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_8_c1.isSignBitClear()) {
		ssa_16_ko_shifted = APInt(ssa_8_c1.getBitWidth(), 0);
	} else {
		ssa_16_ko_shifted = ssa_8_c1.ashr(ssa_6_b1);
	}
	APInt ssa_17_c0_sign = ssa_7_c0 & ssa_12_sign_mask;
	APInt ssa_18_c1_sign = ssa_8_c1 & ssa_12_sign_mask;
	APInt ssa_19_kz_d = ssa_14_cond_b_full ? ssa_15_kz_shifted : ssa_17_c0_sign;
	APInt ssa_20_ko_d = ssa_14_cond_b_full ? ssa_16_ko_shifted : ssa_18_c1_sign;
	APInt ssa_21_known_d = ssa_19_kz_d | ssa_20_ko_d;
	bool ssa_22_cond_d_full = ssa_21_known_d.eq(ssa_9_allones);
	APInt ssa_23_known_a = ssa_3_a0 | ssa_4_a1;
	bool ssa_24_cond_a_full = ssa_23_known_a.eq(ssa_9_allones);
	bool ssa_25_a_zero_val = ssa_4_a1.eq(ssa_10_zero);
	bool ssa_26_cond_a_zero = ssa_24_cond_a_full && ssa_25_a_zero_val;
	bool ssa_27_d_zero_val = ssa_20_ko_d.eq(ssa_10_zero);
	bool ssa_28_cond_d_zero = ssa_22_cond_d_full && ssa_27_d_zero_val;
	bool ssa_29_cond_full_sum = ssa_24_cond_a_full && ssa_22_cond_d_full;
	APInt ssa_30_sum_val = ssa_4_a1 + ssa_20_ko_d;
	APInt ssa_31_kz_full = ssa_30_sum_val ^ ssa_9_allones;
	APInt ssa_32_ko_full = ssa_30_sum_val | ssa_10_zero;
	APInt ssa_33_a_sign1 = ssa_4_a1 & ssa_12_sign_mask;
	bool ssa_34_a_sign1_cond = ssa_33_a_sign1.eq(ssa_12_sign_mask);
	APInt ssa_35_a_sign0 = ssa_3_a0 & ssa_12_sign_mask;
	bool ssa_36_a_sign0_cond = ssa_35_a_sign0.eq(ssa_12_sign_mask);
	APInt ssa_37_d_sign1 = ssa_20_ko_d & ssa_12_sign_mask;
	bool ssa_38_d_sign1_cond = ssa_37_d_sign1.eq(ssa_12_sign_mask);
	APInt ssa_39_d_sign0 = ssa_19_kz_d & ssa_12_sign_mask;
	bool ssa_40_d_sign0_cond = ssa_39_d_sign0.eq(ssa_12_sign_mask);
	bool ssa_41_cond_same_sign1 = ssa_34_a_sign1_cond && ssa_38_d_sign1_cond;
	bool ssa_42_cond_same_sign0 = ssa_36_a_sign0_cond && ssa_40_d_sign0_cond;
	APInt ssa_43_kz_sign = ssa_42_cond_same_sign0 ? ssa_12_sign_mask : ssa_10_zero;
	APInt ssa_44_ko_sign = ssa_41_cond_same_sign1 ? ssa_12_sign_mask : ssa_10_zero;
	APInt ssa_45_tza(ssa_3_a0.getBitWidth(), ssa_3_a0.countr_one());
	APInt ssa_46_tzd(ssa_19_kz_d.getBitWidth(), ssa_19_kz_d.countr_one());
	APInt ssa_47_k = APIntOps::umin(ssa_45_tza, ssa_46_tzd);
	APInt ssa_48_lowmask = ssa_10_zero;
	if (ssa_47_k.ule(ssa_47_k.getBitWidth()))
		ssa_48_lowmask.setLowBits(ssa_47_k.getZExtValue());
	else
		ssa_48_lowmask.setLowBits(ssa_47_k.getBitWidth());
	APInt ssa_49_kz_base = ssa_43_kz_sign | ssa_48_lowmask;
	APInt ssa_50_known_dmask = ssa_19_kz_d | ssa_20_ko_d;
	APInt ssa_51_nd(ssa_50_known_dmask.getBitWidth(), ssa_50_known_dmask.countr_one());
	APInt ssa_52_mask_low_d = ssa_10_zero;
	if (ssa_51_nd.ule(ssa_51_nd.getBitWidth()))
		ssa_52_mask_low_d.setLowBits(ssa_51_nd.getZExtValue());
	else
		ssa_52_mask_low_d.setLowBits(ssa_51_nd.getBitWidth());
	APInt ssa_53_low_d = ssa_20_ko_d & ssa_52_mask_low_d;
	APInt ssa_54_low_a_const = ssa_4_a1 & ssa_52_mask_low_d;
	APInt ssa_55_sum_low_d = ssa_53_low_d + ssa_54_low_a_const;
	APInt ssa_56_sum_low_bits_d = ssa_55_sum_low_d & ssa_52_mask_low_d;
	APInt ssa_57_kz_low_d = ssa_56_sum_low_bits_d ^ ssa_52_mask_low_d;
	APInt ssa_58_ko_low_d = ssa_56_sum_low_bits_d | ssa_10_zero;
	APInt ssa_59_known_amask = ssa_3_a0 | ssa_4_a1;
	APInt ssa_60_na(ssa_59_known_amask.getBitWidth(), ssa_59_known_amask.countr_one());
	APInt ssa_61_mask_low_a = ssa_10_zero;
	if (ssa_60_na.ule(ssa_60_na.getBitWidth()))
		ssa_61_mask_low_a.setLowBits(ssa_60_na.getZExtValue());
	else
		ssa_61_mask_low_a.setLowBits(ssa_60_na.getBitWidth());
	APInt ssa_62_low_a = ssa_4_a1 & ssa_61_mask_low_a;
	APInt ssa_63_low_d_const = ssa_20_ko_d & ssa_61_mask_low_a;
	APInt ssa_64_sum_low_a = ssa_62_low_a + ssa_63_low_d_const;
	APInt ssa_65_sum_low_bits_a = ssa_64_sum_low_a & ssa_61_mask_low_a;
	APInt ssa_66_kz_low_a = ssa_65_sum_low_bits_a ^ ssa_61_mask_low_a;
	APInt ssa_67_ko_low_a = ssa_65_sum_low_bits_a | ssa_10_zero;
	APInt ssa_68_kz_const_a = ssa_43_kz_sign | ssa_57_kz_low_d;
	APInt ssa_69_ko_const_a = ssa_44_ko_sign | ssa_58_ko_low_d;
	APInt ssa_70_kz_const_d = ssa_43_kz_sign | ssa_66_kz_low_a;
	APInt ssa_71_ko_const_d = ssa_44_ko_sign | ssa_67_ko_low_a;
	APInt ssa_72_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_73_ldz_d(ssa_19_kz_d.getBitWidth(), ssa_19_kz_d.countl_one());
	APInt ssa_74_mbits_d = ssa_72_bw - ssa_73_ldz_d;
	APInt ssa_75_a_high;
	if (ssa_74_mbits_d.uge(ssa_74_mbits_d.getBitWidth())) {
		ssa_75_a_high = APInt(ssa_4_a1.getBitWidth(), 0);
	} else {
		ssa_75_a_high = ssa_4_a1.lshr(ssa_74_mbits_d);
	}
	APInt ssa_76_t1_a_high(ssa_75_a_high.getBitWidth(), ssa_75_a_high.countr_one());
	APInt ssa_77_t1_a_high_p1 = ssa_76_t1_a_high + ssa_11_one;
	APInt ssa_78_mask_high_a = ssa_9_allones;
	if (ssa_77_t1_a_high_p1.ule(ssa_77_t1_a_high_p1.getBitWidth()))
		ssa_78_mask_high_a.clearLowBits(ssa_77_t1_a_high_p1.getZExtValue());
	else
		ssa_78_mask_high_a.clearLowBits(ssa_77_t1_a_high_p1.getBitWidth());
	APInt ssa_79_inv_a_high = ssa_75_a_high ^ ssa_9_allones;
	APInt ssa_80_high_kz_a_part = ssa_79_inv_a_high & ssa_78_mask_high_a;
	APInt ssa_81_high_ko_a_part = ssa_75_a_high & ssa_78_mask_high_a;
	APInt ssa_82_high_kz_a;
	if (ssa_74_mbits_d.uge(ssa_74_mbits_d.getBitWidth())) {
		ssa_82_high_kz_a = APInt(ssa_80_high_kz_a_part.getBitWidth(), 0);
	} else {
		ssa_82_high_kz_a = ssa_80_high_kz_a_part.shl(ssa_74_mbits_d);
	}
	APInt ssa_83_high_ko_a;
	if (ssa_74_mbits_d.uge(ssa_74_mbits_d.getBitWidth())) {
		ssa_83_high_ko_a = APInt(ssa_81_high_ko_a_part.getBitWidth(), 0);
	} else {
		ssa_83_high_ko_a = ssa_81_high_ko_a_part.shl(ssa_74_mbits_d);
	}
	APInt ssa_84_ldz_a(ssa_3_a0.getBitWidth(), ssa_3_a0.countl_one());
	APInt ssa_85_mbits_a = ssa_72_bw - ssa_84_ldz_a;
	APInt ssa_86_d_high;
	if (ssa_85_mbits_a.uge(ssa_85_mbits_a.getBitWidth())) {
		ssa_86_d_high = APInt(ssa_20_ko_d.getBitWidth(), 0);
	} else {
		ssa_86_d_high = ssa_20_ko_d.lshr(ssa_85_mbits_a);
	}
	APInt ssa_87_t1_d_high(ssa_86_d_high.getBitWidth(), ssa_86_d_high.countr_one());
	APInt ssa_88_t1_d_high_p1 = ssa_87_t1_d_high + ssa_11_one;
	APInt ssa_89_mask_high_d = ssa_9_allones;
	if (ssa_88_t1_d_high_p1.ule(ssa_88_t1_d_high_p1.getBitWidth()))
		ssa_89_mask_high_d.clearLowBits(ssa_88_t1_d_high_p1.getZExtValue());
	else
		ssa_89_mask_high_d.clearLowBits(ssa_88_t1_d_high_p1.getBitWidth());
	APInt ssa_90_inv_d_high = ssa_86_d_high ^ ssa_9_allones;
	APInt ssa_91_high_kz_d_part = ssa_90_inv_d_high & ssa_89_mask_high_d;
	APInt ssa_92_high_ko_d_part = ssa_86_d_high & ssa_89_mask_high_d;
	APInt ssa_93_high_kz_d;
	if (ssa_85_mbits_a.uge(ssa_85_mbits_a.getBitWidth())) {
		ssa_93_high_kz_d = APInt(ssa_91_high_kz_d_part.getBitWidth(), 0);
	} else {
		ssa_93_high_kz_d = ssa_91_high_kz_d_part.shl(ssa_85_mbits_a);
	}
	APInt ssa_94_high_ko_d;
	if (ssa_85_mbits_a.uge(ssa_85_mbits_a.getBitWidth())) {
		ssa_94_high_ko_d = APInt(ssa_92_high_ko_d_part.getBitWidth(), 0);
	} else {
		ssa_94_high_ko_d = ssa_92_high_ko_d_part.shl(ssa_85_mbits_a);
	}
	APInt ssa_95_unk_d = ssa_50_known_dmask ^ ssa_9_allones;
	APInt ssa_96_max_d = ssa_20_ko_d | ssa_95_unk_d;
	APInt ssa_97_min_d = ssa_20_ko_d | ssa_10_zero;
	APInt ssa_98_max_sum = ssa_4_a1 + ssa_96_max_d;
	APInt ssa_99_min_sum = ssa_4_a1 + ssa_97_min_d;
	bool ssa_100_uov_max;
	ssa_4_a1.uadd_ov(ssa_96_max_d, ssa_100_uov_max);
	bool ssa_101_true = -1;
	bool ssa_102_no_uov_max = ssa_100_uov_max != ssa_101_true;
	APInt ssa_103_lz_max(ssa_98_max_sum.getBitWidth(), ssa_98_max_sum.countl_zero());
	APInt ssa_104_kz_high_bound = ssa_10_zero;
	if (ssa_103_lz_max.ule(ssa_103_lz_max.getBitWidth()))
		ssa_104_kz_high_bound.setHighBits(ssa_103_lz_max.getZExtValue());
	else
		ssa_104_kz_high_bound.setHighBits(ssa_103_lz_max.getBitWidth());
	APInt ssa_105_lo_min(ssa_99_min_sum.getBitWidth(), ssa_99_min_sum.countl_one());
	APInt ssa_106_ko_high_bound = ssa_10_zero;
	if (ssa_105_lo_min.ule(ssa_105_lo_min.getBitWidth()))
		ssa_106_ko_high_bound.setHighBits(ssa_105_lo_min.getZExtValue());
	else
		ssa_106_ko_high_bound.setHighBits(ssa_105_lo_min.getBitWidth());
	APInt ssa_107_kz_high_bound_sel = ssa_102_no_uov_max ? ssa_104_kz_high_bound : ssa_10_zero;
	APInt ssa_108_ko_high_bound_sel = ssa_102_no_uov_max ? ssa_106_ko_high_bound : ssa_10_zero;
	bool ssa_109_min_nonneg = ssa_99_min_sum.sge(ssa_10_zero);
	bool ssa_110_max_nonneg = ssa_98_max_sum.sge(ssa_10_zero);
	bool ssa_111_cond_all_nonneg = ssa_109_min_nonneg && ssa_110_max_nonneg;
	bool ssa_112_min_neg = ssa_99_min_sum.slt(ssa_10_zero);
	bool ssa_113_max_neg = ssa_98_max_sum.slt(ssa_10_zero);
	bool ssa_114_cond_all_neg = ssa_112_min_neg && ssa_113_max_neg;
	bool ssa_115_cond_all_nonneg_d = ssa_111_cond_all_nonneg && ssa_40_d_sign0_cond;
	bool ssa_116_cond_all_neg_d = ssa_114_cond_all_neg && ssa_40_d_sign0_cond;
	APInt ssa_117_kz_nonneg = ssa_10_zero;
	if (ssa_103_lz_max.ule(ssa_103_lz_max.getBitWidth()))
		ssa_117_kz_nonneg.setHighBits(ssa_103_lz_max.getZExtValue());
	else
		ssa_117_kz_nonneg.setHighBits(ssa_103_lz_max.getBitWidth());
	APInt ssa_118_lo_min_neg(ssa_99_min_sum.getBitWidth(), ssa_99_min_sum.countl_one());
	APInt ssa_119_ko_neg = ssa_10_zero;
	if (ssa_118_lo_min_neg.ule(ssa_118_lo_min_neg.getBitWidth()))
		ssa_119_ko_neg.setHighBits(ssa_118_lo_min_neg.getZExtValue());
	else
		ssa_119_ko_neg.setHighBits(ssa_118_lo_min_neg.getBitWidth());
	APInt ssa_120_kz_nonneg_sel = ssa_115_cond_all_nonneg_d ? ssa_117_kz_nonneg : ssa_10_zero;
	APInt ssa_121_ko_neg_sel = ssa_116_cond_all_neg_d ? ssa_119_ko_neg : ssa_10_zero;
	APInt ssa_122_kz_const_a2 = ssa_68_kz_const_a | ssa_82_high_kz_a;
	APInt ssa_123_ko_const_a2 = ssa_69_ko_const_a | ssa_83_high_ko_a;
	APInt ssa_124_kz_const_d2 = ssa_70_kz_const_d | ssa_93_high_kz_d;
	APInt ssa_125_ko_const_d2 = ssa_71_ko_const_d | ssa_94_high_ko_d;
	APInt ssa_126_kz_const_a3 = ssa_122_kz_const_a2 | ssa_107_kz_high_bound_sel;
	APInt ssa_127_ko_const_a3 = ssa_123_ko_const_a2 | ssa_108_ko_high_bound_sel;
	APInt ssa_128_kz_const_a4 = ssa_126_kz_const_a3 | ssa_120_kz_nonneg_sel;
	APInt ssa_129_ko_const_a4 = ssa_127_ko_const_a3 | ssa_121_ko_neg_sel;
	APInt ssa_130_res_kz0 = ssa_24_cond_a_full ? ssa_128_kz_const_a4 : ssa_49_kz_base;
	APInt ssa_131_res_ko0 = ssa_24_cond_a_full ? ssa_129_ko_const_a4 : ssa_44_ko_sign;
	APInt ssa_132_res_kz1 = ssa_22_cond_d_full ? ssa_124_kz_const_d2 : ssa_130_res_kz0;
	APInt ssa_133_res_ko1 = ssa_22_cond_d_full ? ssa_125_ko_const_d2 : ssa_131_res_ko0;
	APInt ssa_134_res_kz2 = ssa_28_cond_d_zero ? ssa_3_a0 : ssa_132_res_kz1;
	APInt ssa_135_res_ko2 = ssa_28_cond_d_zero ? ssa_4_a1 : ssa_133_res_ko1;
	APInt ssa_136_res_kz3 = ssa_26_cond_a_zero ? ssa_19_kz_d : ssa_134_res_kz2;
	APInt ssa_137_res_ko3 = ssa_26_cond_a_zero ? ssa_20_ko_d : ssa_135_res_ko2;
	APInt ssa_138_res_kz4 = ssa_29_cond_full_sum ? ssa_31_kz_full : ssa_136_res_kz3;
	APInt ssa_139_res_ko4 = ssa_29_cond_full_sum ? ssa_32_ko_full : ssa_137_res_ko3;
	std::array<APInt, 2> ssa_140_r = std::array<APInt, 2>{ssa_138_res_kz4, ssa_139_res_ko4};
	return ssa_140_r;
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
	APInt ssa_11_one(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_12_sign_mask = ssa_10_zero;
	ssa_12_sign_mask.setSignBit();
	APInt ssa_13_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_14_max_b = ssa_13_bw - ssa_11_one;
	APInt ssa_15_lz_max_b(ssa_14_max_b.getBitWidth(), ssa_14_max_b.countl_zero());
	APInt ssa_16_bits_b = ssa_13_bw - ssa_15_lz_max_b;
	APInt ssa_17_mask_low_b = ssa_10_zero;
	if (ssa_16_bits_b.ule(ssa_16_bits_b.getBitWidth()))
		ssa_17_mask_low_b.setLowBits(ssa_16_bits_b.getZExtValue());
	else
		ssa_17_mask_low_b.setLowBits(ssa_16_bits_b.getBitWidth());
	APInt ssa_18_mask_high_b = ssa_17_mask_low_b ^ ssa_9_allones;
	APInt ssa_19_b0_ref = ssa_5_b0 | ssa_18_mask_high_b;
	APInt ssa_20_known_b = ssa_19_b0_ref | ssa_6_b1;
	bool ssa_21_cond_b_full = ssa_20_known_b.eq(ssa_9_allones);
	APInt ssa_22_kz_shifted;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_7_c0.isSignBitSet()) {
		ssa_22_kz_shifted = APInt(ssa_7_c0.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_7_c0.isSignBitClear()) {
		ssa_22_kz_shifted = APInt(ssa_7_c0.getBitWidth(), 0);
	} else {
		ssa_22_kz_shifted = ssa_7_c0.ashr(ssa_6_b1);
	}
	APInt ssa_23_ko_shifted;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_8_c1.isSignBitSet()) {
		ssa_23_ko_shifted = APInt(ssa_8_c1.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_8_c1.isSignBitClear()) {
		ssa_23_ko_shifted = APInt(ssa_8_c1.getBitWidth(), 0);
	} else {
		ssa_23_ko_shifted = ssa_8_c1.ashr(ssa_6_b1);
	}
	APInt ssa_24_c0_sign = ssa_7_c0 & ssa_12_sign_mask;
	APInt ssa_25_c1_sign = ssa_8_c1 & ssa_12_sign_mask;
	APInt ssa_26_kz_d = ssa_21_cond_b_full ? ssa_22_kz_shifted : ssa_24_c0_sign;
	APInt ssa_27_ko_d = ssa_21_cond_b_full ? ssa_23_ko_shifted : ssa_25_c1_sign;
	APInt ssa_28_known_d = ssa_26_kz_d | ssa_27_ko_d;
	bool ssa_29_cond_d_full = ssa_28_known_d.eq(ssa_9_allones);
	APInt ssa_30_known_a = ssa_3_a0 | ssa_4_a1;
	bool ssa_31_cond_a_full = ssa_30_known_a.eq(ssa_9_allones);
	bool ssa_32_a_zero_val = ssa_4_a1.eq(ssa_10_zero);
	bool ssa_33_cond_a_zero = ssa_31_cond_a_full && ssa_32_a_zero_val;
	bool ssa_34_d_zero_val = ssa_27_ko_d.eq(ssa_10_zero);
	bool ssa_35_cond_d_zero = ssa_29_cond_d_full && ssa_34_d_zero_val;
	bool ssa_36_cond_full_sum = ssa_31_cond_a_full && ssa_29_cond_d_full;
	APInt ssa_37_sum_val = ssa_4_a1 + ssa_27_ko_d;
	APInt ssa_38_kz_full = ssa_37_sum_val ^ ssa_9_allones;
	APInt ssa_39_ko_full = ssa_37_sum_val | ssa_10_zero;
	APInt ssa_40_a_sign1 = ssa_4_a1 & ssa_12_sign_mask;
	bool ssa_41_a_sign1_cond = ssa_40_a_sign1.eq(ssa_12_sign_mask);
	APInt ssa_42_a_sign0 = ssa_3_a0 & ssa_12_sign_mask;
	bool ssa_43_a_sign0_cond = ssa_42_a_sign0.eq(ssa_12_sign_mask);
	APInt ssa_44_d_sign1 = ssa_27_ko_d & ssa_12_sign_mask;
	bool ssa_45_d_sign1_cond = ssa_44_d_sign1.eq(ssa_12_sign_mask);
	APInt ssa_46_d_sign0 = ssa_26_kz_d & ssa_12_sign_mask;
	bool ssa_47_d_sign0_cond = ssa_46_d_sign0.eq(ssa_12_sign_mask);
	bool ssa_48_cond_same_sign1 = ssa_41_a_sign1_cond && ssa_45_d_sign1_cond;
	bool ssa_49_cond_same_sign0 = ssa_43_a_sign0_cond && ssa_47_d_sign0_cond;
	APInt ssa_50_kz_sign = ssa_49_cond_same_sign0 ? ssa_12_sign_mask : ssa_10_zero;
	APInt ssa_51_ko_sign = ssa_48_cond_same_sign1 ? ssa_12_sign_mask : ssa_10_zero;
	APInt ssa_52_tza(ssa_3_a0.getBitWidth(), ssa_3_a0.countr_one());
	APInt ssa_53_tzd(ssa_26_kz_d.getBitWidth(), ssa_26_kz_d.countr_one());
	APInt ssa_54_k = APIntOps::umin(ssa_52_tza, ssa_53_tzd);
	APInt ssa_55_lowmask = ssa_10_zero;
	if (ssa_54_k.ule(ssa_54_k.getBitWidth()))
		ssa_55_lowmask.setLowBits(ssa_54_k.getZExtValue());
	else
		ssa_55_lowmask.setLowBits(ssa_54_k.getBitWidth());
	APInt ssa_56_kz_base = ssa_50_kz_sign | ssa_55_lowmask;
	APInt ssa_57_known_dmask = ssa_26_kz_d | ssa_27_ko_d;
	APInt ssa_58_nd(ssa_57_known_dmask.getBitWidth(), ssa_57_known_dmask.countr_one());
	APInt ssa_59_mask_low_d = ssa_10_zero;
	if (ssa_58_nd.ule(ssa_58_nd.getBitWidth()))
		ssa_59_mask_low_d.setLowBits(ssa_58_nd.getZExtValue());
	else
		ssa_59_mask_low_d.setLowBits(ssa_58_nd.getBitWidth());
	APInt ssa_60_low_d = ssa_27_ko_d & ssa_59_mask_low_d;
	APInt ssa_61_low_a_const = ssa_4_a1 & ssa_59_mask_low_d;
	APInt ssa_62_sum_low_d = ssa_60_low_d + ssa_61_low_a_const;
	APInt ssa_63_sum_low_bits_d = ssa_62_sum_low_d & ssa_59_mask_low_d;
	APInt ssa_64_kz_low_d = ssa_63_sum_low_bits_d ^ ssa_59_mask_low_d;
	APInt ssa_65_ko_low_d = ssa_63_sum_low_bits_d | ssa_10_zero;
	APInt ssa_66_known_amask = ssa_3_a0 | ssa_4_a1;
	APInt ssa_67_na(ssa_66_known_amask.getBitWidth(), ssa_66_known_amask.countr_one());
	APInt ssa_68_mask_low_a = ssa_10_zero;
	if (ssa_67_na.ule(ssa_67_na.getBitWidth()))
		ssa_68_mask_low_a.setLowBits(ssa_67_na.getZExtValue());
	else
		ssa_68_mask_low_a.setLowBits(ssa_67_na.getBitWidth());
	APInt ssa_69_low_a = ssa_4_a1 & ssa_68_mask_low_a;
	APInt ssa_70_low_d_const = ssa_27_ko_d & ssa_68_mask_low_a;
	APInt ssa_71_sum_low_a = ssa_69_low_a + ssa_70_low_d_const;
	APInt ssa_72_sum_low_bits_a = ssa_71_sum_low_a & ssa_68_mask_low_a;
	APInt ssa_73_kz_low_a = ssa_72_sum_low_bits_a ^ ssa_68_mask_low_a;
	APInt ssa_74_ko_low_a = ssa_72_sum_low_bits_a | ssa_10_zero;
	APInt ssa_75_kz_const_a = ssa_50_kz_sign | ssa_64_kz_low_d;
	APInt ssa_76_ko_const_a = ssa_51_ko_sign | ssa_65_ko_low_d;
	APInt ssa_77_kz_const_d = ssa_50_kz_sign | ssa_73_kz_low_a;
	APInt ssa_78_ko_const_d = ssa_51_ko_sign | ssa_74_ko_low_a;
	APInt ssa_79_ldz_d(ssa_26_kz_d.getBitWidth(), ssa_26_kz_d.countl_one());
	APInt ssa_80_mbits_d = ssa_13_bw - ssa_79_ldz_d;
	APInt ssa_81_a_high;
	if (ssa_80_mbits_d.uge(ssa_80_mbits_d.getBitWidth())) {
		ssa_81_a_high = APInt(ssa_4_a1.getBitWidth(), 0);
	} else {
		ssa_81_a_high = ssa_4_a1.lshr(ssa_80_mbits_d);
	}
	APInt ssa_82_t1_a_high(ssa_81_a_high.getBitWidth(), ssa_81_a_high.countr_one());
	APInt ssa_83_t1_a_high_p1 = ssa_82_t1_a_high + ssa_11_one;
	APInt ssa_84_mask_high_a = ssa_9_allones;
	if (ssa_83_t1_a_high_p1.ule(ssa_83_t1_a_high_p1.getBitWidth()))
		ssa_84_mask_high_a.clearLowBits(ssa_83_t1_a_high_p1.getZExtValue());
	else
		ssa_84_mask_high_a.clearLowBits(ssa_83_t1_a_high_p1.getBitWidth());
	APInt ssa_85_inv_a_high = ssa_81_a_high ^ ssa_9_allones;
	APInt ssa_86_high_kz_a_part = ssa_85_inv_a_high & ssa_84_mask_high_a;
	APInt ssa_87_high_ko_a_part = ssa_81_a_high & ssa_84_mask_high_a;
	APInt ssa_88_high_kz_a;
	if (ssa_80_mbits_d.uge(ssa_80_mbits_d.getBitWidth())) {
		ssa_88_high_kz_a = APInt(ssa_86_high_kz_a_part.getBitWidth(), 0);
	} else {
		ssa_88_high_kz_a = ssa_86_high_kz_a_part.shl(ssa_80_mbits_d);
	}
	APInt ssa_89_high_ko_a;
	if (ssa_80_mbits_d.uge(ssa_80_mbits_d.getBitWidth())) {
		ssa_89_high_ko_a = APInt(ssa_87_high_ko_a_part.getBitWidth(), 0);
	} else {
		ssa_89_high_ko_a = ssa_87_high_ko_a_part.shl(ssa_80_mbits_d);
	}
	APInt ssa_90_ldz_a(ssa_3_a0.getBitWidth(), ssa_3_a0.countl_one());
	APInt ssa_91_mbits_a = ssa_13_bw - ssa_90_ldz_a;
	APInt ssa_92_d_high;
	if (ssa_91_mbits_a.uge(ssa_91_mbits_a.getBitWidth())) {
		ssa_92_d_high = APInt(ssa_27_ko_d.getBitWidth(), 0);
	} else {
		ssa_92_d_high = ssa_27_ko_d.lshr(ssa_91_mbits_a);
	}
	APInt ssa_93_t1_d_high(ssa_92_d_high.getBitWidth(), ssa_92_d_high.countr_one());
	APInt ssa_94_t1_d_high_p1 = ssa_93_t1_d_high + ssa_11_one;
	APInt ssa_95_mask_high_d = ssa_9_allones;
	if (ssa_94_t1_d_high_p1.ule(ssa_94_t1_d_high_p1.getBitWidth()))
		ssa_95_mask_high_d.clearLowBits(ssa_94_t1_d_high_p1.getZExtValue());
	else
		ssa_95_mask_high_d.clearLowBits(ssa_94_t1_d_high_p1.getBitWidth());
	APInt ssa_96_inv_d_high = ssa_92_d_high ^ ssa_9_allones;
	APInt ssa_97_high_kz_d_part = ssa_96_inv_d_high & ssa_95_mask_high_d;
	APInt ssa_98_high_ko_d_part = ssa_92_d_high & ssa_95_mask_high_d;
	APInt ssa_99_high_kz_d;
	if (ssa_91_mbits_a.uge(ssa_91_mbits_a.getBitWidth())) {
		ssa_99_high_kz_d = APInt(ssa_97_high_kz_d_part.getBitWidth(), 0);
	} else {
		ssa_99_high_kz_d = ssa_97_high_kz_d_part.shl(ssa_91_mbits_a);
	}
	APInt ssa_100_high_ko_d;
	if (ssa_91_mbits_a.uge(ssa_91_mbits_a.getBitWidth())) {
		ssa_100_high_ko_d = APInt(ssa_98_high_ko_d_part.getBitWidth(), 0);
	} else {
		ssa_100_high_ko_d = ssa_98_high_ko_d_part.shl(ssa_91_mbits_a);
	}
	APInt ssa_101_unk_d = ssa_57_known_dmask ^ ssa_9_allones;
	APInt ssa_102_max_d = ssa_27_ko_d | ssa_101_unk_d;
	APInt ssa_103_min_d = ssa_27_ko_d | ssa_10_zero;
	APInt ssa_104_max_sum = ssa_4_a1 + ssa_102_max_d;
	APInt ssa_105_min_sum = ssa_4_a1 + ssa_103_min_d;
	bool ssa_106_uov_max;
	ssa_4_a1.uadd_ov(ssa_102_max_d, ssa_106_uov_max);
	bool ssa_107_true = -1;
	bool ssa_108_no_uov_max = ssa_106_uov_max != ssa_107_true;
	APInt ssa_109_lz_max(ssa_104_max_sum.getBitWidth(), ssa_104_max_sum.countl_zero());
	APInt ssa_110_kz_high_bound = ssa_10_zero;
	if (ssa_109_lz_max.ule(ssa_109_lz_max.getBitWidth()))
		ssa_110_kz_high_bound.setHighBits(ssa_109_lz_max.getZExtValue());
	else
		ssa_110_kz_high_bound.setHighBits(ssa_109_lz_max.getBitWidth());
	APInt ssa_111_lo_min(ssa_105_min_sum.getBitWidth(), ssa_105_min_sum.countl_one());
	APInt ssa_112_ko_high_bound = ssa_10_zero;
	if (ssa_111_lo_min.ule(ssa_111_lo_min.getBitWidth()))
		ssa_112_ko_high_bound.setHighBits(ssa_111_lo_min.getZExtValue());
	else
		ssa_112_ko_high_bound.setHighBits(ssa_111_lo_min.getBitWidth());
	APInt ssa_113_kz_high_bound_sel = ssa_108_no_uov_max ? ssa_110_kz_high_bound : ssa_10_zero;
	APInt ssa_114_ko_high_bound_sel = ssa_108_no_uov_max ? ssa_112_ko_high_bound : ssa_10_zero;
	bool ssa_115_min_nonneg = ssa_105_min_sum.sge(ssa_10_zero);
	bool ssa_116_max_nonneg = ssa_104_max_sum.sge(ssa_10_zero);
	bool ssa_117_cond_all_nonneg = ssa_115_min_nonneg && ssa_116_max_nonneg;
	bool ssa_118_min_neg = ssa_105_min_sum.slt(ssa_10_zero);
	bool ssa_119_max_neg = ssa_104_max_sum.slt(ssa_10_zero);
	bool ssa_120_cond_all_neg = ssa_118_min_neg && ssa_119_max_neg;
	bool ssa_121_cond_all_nonneg_d = ssa_117_cond_all_nonneg && ssa_47_d_sign0_cond;
	bool ssa_122_cond_all_neg_d = ssa_120_cond_all_neg && ssa_47_d_sign0_cond;
	APInt ssa_123_kz_nonneg = ssa_10_zero;
	if (ssa_109_lz_max.ule(ssa_109_lz_max.getBitWidth()))
		ssa_123_kz_nonneg.setHighBits(ssa_109_lz_max.getZExtValue());
	else
		ssa_123_kz_nonneg.setHighBits(ssa_109_lz_max.getBitWidth());
	APInt ssa_124_lo_min_neg(ssa_105_min_sum.getBitWidth(), ssa_105_min_sum.countl_one());
	APInt ssa_125_ko_neg = ssa_10_zero;
	if (ssa_124_lo_min_neg.ule(ssa_124_lo_min_neg.getBitWidth()))
		ssa_125_ko_neg.setHighBits(ssa_124_lo_min_neg.getZExtValue());
	else
		ssa_125_ko_neg.setHighBits(ssa_124_lo_min_neg.getBitWidth());
	APInt ssa_126_kz_nonneg_sel = ssa_121_cond_all_nonneg_d ? ssa_123_kz_nonneg : ssa_10_zero;
	APInt ssa_127_ko_neg_sel = ssa_122_cond_all_neg_d ? ssa_125_ko_neg : ssa_10_zero;
	APInt ssa_128_kz_const_a2 = ssa_75_kz_const_a | ssa_88_high_kz_a;
	APInt ssa_129_ko_const_a2 = ssa_76_ko_const_a | ssa_89_high_ko_a;
	APInt ssa_130_kz_const_d2 = ssa_77_kz_const_d | ssa_99_high_kz_d;
	APInt ssa_131_ko_const_d2 = ssa_78_ko_const_d | ssa_100_high_ko_d;
	APInt ssa_132_kz_const_a3 = ssa_128_kz_const_a2 | ssa_113_kz_high_bound_sel;
	APInt ssa_133_ko_const_a3 = ssa_129_ko_const_a2 | ssa_114_ko_high_bound_sel;
	APInt ssa_134_kz_const_a4 = ssa_132_kz_const_a3 | ssa_126_kz_nonneg_sel;
	APInt ssa_135_ko_const_a4 = ssa_133_ko_const_a3 | ssa_127_ko_neg_sel;
	APInt ssa_136_res_kz0 = ssa_31_cond_a_full ? ssa_134_kz_const_a4 : ssa_56_kz_base;
	APInt ssa_137_res_ko0 = ssa_31_cond_a_full ? ssa_135_ko_const_a4 : ssa_51_ko_sign;
	APInt ssa_138_res_kz1 = ssa_29_cond_d_full ? ssa_130_kz_const_d2 : ssa_136_res_kz0;
	APInt ssa_139_res_ko1 = ssa_29_cond_d_full ? ssa_131_ko_const_d2 : ssa_137_res_ko0;
	APInt ssa_140_res_kz2 = ssa_35_cond_d_zero ? ssa_3_a0 : ssa_138_res_kz1;
	APInt ssa_141_res_ko2 = ssa_35_cond_d_zero ? ssa_4_a1 : ssa_139_res_ko1;
	APInt ssa_142_res_kz3 = ssa_33_cond_a_zero ? ssa_26_kz_d : ssa_140_res_kz2;
	APInt ssa_143_res_ko3 = ssa_33_cond_a_zero ? ssa_27_ko_d : ssa_141_res_ko2;
	APInt ssa_144_res_kz4 = ssa_36_cond_full_sum ? ssa_38_kz_full : ssa_142_res_kz3;
	APInt ssa_145_res_ko4 = ssa_36_cond_full_sum ? ssa_39_ko_full : ssa_143_res_ko3;
	bool ssa_146_cond_ab_full = ssa_31_cond_a_full && ssa_21_cond_b_full;
	APInt ssa_147_smin_base = APInt::getSignedMinValue(ssa_3_a0.getBitWidth());
	APInt ssa_148_smax_base = APInt::getSignedMaxValue(ssa_3_a0.getBitWidth());
	APInt ssa_149_shift_min;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_147_smin_base.isSignBitSet()) {
		ssa_149_shift_min = APInt(ssa_147_smin_base.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_147_smin_base.isSignBitClear()) {
		ssa_149_shift_min = APInt(ssa_147_smin_base.getBitWidth(), 0);
	} else {
		ssa_149_shift_min = ssa_147_smin_base.ashr(ssa_6_b1);
	}
	APInt ssa_150_shift_max;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_148_smax_base.isSignBitSet()) {
		ssa_150_shift_max = APInt(ssa_148_smax_base.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_148_smax_base.isSignBitClear()) {
		ssa_150_shift_max = APInt(ssa_148_smax_base.getBitWidth(), 0);
	} else {
		ssa_150_shift_max = ssa_148_smax_base.ashr(ssa_6_b1);
	}
	bool ssa_151_ov_min;
	ssa_147_smin_base.ssub_ov(ssa_4_a1, ssa_151_ov_min);
	bool ssa_152_ov_max;
	ssa_148_smax_base.ssub_ov(ssa_4_a1, ssa_152_ov_max);
	bool ssa_153_no_ov_min = ssa_151_ov_min != ssa_107_true;
	bool ssa_154_no_ov_max2 = ssa_152_ov_max != ssa_107_true;
	APInt ssa_155_d_min_ov_raw = ssa_147_smin_base - ssa_4_a1;
	APInt ssa_156_d_max_ov_raw = ssa_148_smax_base - ssa_4_a1;
	APInt ssa_157_d_min_ov = ssa_153_no_ov_min ? ssa_155_d_min_ov_raw : ssa_147_smin_base;
	APInt ssa_158_d_max_ov = ssa_154_no_ov_max2 ? ssa_156_d_max_ov_raw : ssa_148_smax_base;
	APInt ssa_159_d_min_int = APIntOps::smax(ssa_149_shift_min, ssa_157_d_min_ov);
	APInt ssa_160_d_max_int = APIntOps::smin(ssa_150_shift_max, ssa_158_d_max_ov);
	APInt ssa_161_sum_min2 = ssa_4_a1 + ssa_159_d_min_int;
	APInt ssa_162_sum_max2 = ssa_4_a1 + ssa_160_d_max_int;
	APInt ssa_163_xor_sum = ssa_161_sum_min2 ^ ssa_162_sum_max2;
	APInt ssa_164_lz_sum(ssa_163_xor_sum.getBitWidth(), ssa_163_xor_sum.countl_zero());
	APInt ssa_165_mask_high_sum = ssa_10_zero;
	if (ssa_164_lz_sum.ule(ssa_164_lz_sum.getBitWidth()))
		ssa_165_mask_high_sum.setHighBits(ssa_164_lz_sum.getZExtValue());
	else
		ssa_165_mask_high_sum.setHighBits(ssa_164_lz_sum.getBitWidth());
	APInt ssa_166_ko_pref = ssa_161_sum_min2 & ssa_165_mask_high_sum;
	APInt ssa_167_inv_sum_min = ssa_161_sum_min2 ^ ssa_9_allones;
	APInt ssa_168_kz_pref = ssa_167_inv_sum_min & ssa_165_mask_high_sum;
	APInt ssa_169_kz_extra = ssa_146_cond_ab_full ? ssa_168_kz_pref : ssa_10_zero;
	APInt ssa_170_ko_extra = ssa_146_cond_ab_full ? ssa_166_ko_pref : ssa_10_zero;
	APInt ssa_171_res_kz5 = ssa_144_res_kz4 | ssa_169_kz_extra;
	APInt ssa_172_res_ko5 = ssa_145_res_ko4 | ssa_170_ko_extra;
	std::array<APInt, 2> ssa_173_r = std::array<APInt, 2>{ssa_171_res_kz5, ssa_172_res_ko5};
	return ssa_173_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_11_sign_mask = ssa_10_zero;
	ssa_11_sign_mask.setSignBit();
	APInt ssa_12_known_b = ssa_5_b0 | ssa_6_b1;
	bool ssa_13_cond_b_full = ssa_12_known_b.eq(ssa_9_allones);
	APInt ssa_14_min_b = ssa_6_b1 | ssa_10_zero;
	APInt ssa_15_sign_high_mask = ssa_10_zero;
	if (ssa_14_min_b.ule(ssa_14_min_b.getBitWidth()))
		ssa_15_sign_high_mask.setHighBits(ssa_14_min_b.getZExtValue());
	else
		ssa_15_sign_high_mask.setHighBits(ssa_14_min_b.getBitWidth());
	APInt ssa_16_c_sign1 = ssa_8_c1 & ssa_11_sign_mask;
	bool ssa_17_c_sign1_cond = ssa_16_c_sign1.eq(ssa_11_sign_mask);
	APInt ssa_18_c_sign0 = ssa_7_c0 & ssa_11_sign_mask;
	bool ssa_19_c_sign0_cond = ssa_18_c_sign0.eq(ssa_11_sign_mask);
	APInt ssa_20_kz_sign_high = ssa_19_c_sign0_cond ? ssa_15_sign_high_mask : ssa_10_zero;
	APInt ssa_21_ko_sign_high = ssa_17_c_sign1_cond ? ssa_15_sign_high_mask : ssa_10_zero;
	APInt ssa_22_kz_shifted;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_7_c0.isSignBitSet()) {
		ssa_22_kz_shifted = APInt(ssa_7_c0.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_7_c0.isSignBitClear()) {
		ssa_22_kz_shifted = APInt(ssa_7_c0.getBitWidth(), 0);
	} else {
		ssa_22_kz_shifted = ssa_7_c0.ashr(ssa_6_b1);
	}
	APInt ssa_23_ko_shifted;
	if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_8_c1.isSignBitSet()) {
		ssa_23_ko_shifted = APInt(ssa_8_c1.getBitWidth(), -1);
	} else if (ssa_6_b1.uge(ssa_6_b1.getBitWidth()) && ssa_8_c1.isSignBitClear()) {
		ssa_23_ko_shifted = APInt(ssa_8_c1.getBitWidth(), 0);
	} else {
		ssa_23_ko_shifted = ssa_8_c1.ashr(ssa_6_b1);
	}
	APInt ssa_24_d_kz = ssa_13_cond_b_full ? ssa_22_kz_shifted : ssa_20_kz_sign_high;
	APInt ssa_25_d_ko = ssa_13_cond_b_full ? ssa_23_ko_shifted : ssa_21_ko_sign_high;
	APInt ssa_26_known_a = ssa_3_a0 | ssa_4_a1;
	bool ssa_27_cond_a_full = ssa_26_known_a.eq(ssa_9_allones);
	APInt ssa_28_known_dmask = ssa_24_d_kz | ssa_25_d_ko;
	APInt ssa_29_unk_d = ssa_28_known_dmask ^ ssa_9_allones;
	APInt ssa_30_max_d = ssa_25_d_ko | ssa_29_unk_d;
	APInt ssa_31_min_d = ssa_25_d_ko | ssa_10_zero;
	APInt ssa_32_max_sum = ssa_4_a1 + ssa_30_max_d;
	APInt ssa_33_min_sum = ssa_4_a1 + ssa_31_min_d;
	bool ssa_34_uov_max;
	ssa_4_a1.uadd_ov(ssa_30_max_d, ssa_34_uov_max);
	bool ssa_35_true = -1;
	bool ssa_36_no_uov_max = ssa_34_uov_max != ssa_35_true;
	APInt ssa_37_xor_sum = ssa_33_min_sum ^ ssa_32_max_sum;
	APInt ssa_38_lz_sum(ssa_37_xor_sum.getBitWidth(), ssa_37_xor_sum.countl_zero());
	APInt ssa_39_mask_high_sum = ssa_10_zero;
	if (ssa_38_lz_sum.ule(ssa_38_lz_sum.getBitWidth()))
		ssa_39_mask_high_sum.setHighBits(ssa_38_lz_sum.getZExtValue());
	else
		ssa_39_mask_high_sum.setHighBits(ssa_38_lz_sum.getBitWidth());
	APInt ssa_40_ko_high = ssa_33_min_sum & ssa_39_mask_high_sum;
	APInt ssa_41_inv_min_sum = ssa_33_min_sum ^ ssa_9_allones;
	APInt ssa_42_kz_high = ssa_41_inv_min_sum & ssa_39_mask_high_sum;
	bool ssa_43_cond_range = ssa_27_cond_a_full && ssa_36_no_uov_max;
	APInt ssa_44_res_kz = ssa_43_cond_range ? ssa_42_kz_high : ssa_10_zero;
	APInt ssa_45_res_ko = ssa_43_cond_range ? ssa_40_ko_high : ssa_10_zero;
	std::array<APInt, 2> ssa_46_r = std::array<APInt, 2>{ssa_44_res_kz, ssa_45_res_ko};
	return ssa_46_r;
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
