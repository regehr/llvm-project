namespace Pattern014 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_t1_zero = ssa_5_b0 | ssa_7_c0;
	APInt ssa_10_t1_one = ssa_6_b1 & ssa_8_c1;
	APInt ssa_11_all = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_12_const0(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_13_const1(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_14_maxA = ssa_11_all ^ ssa_3_a0;
	APInt ssa_15_maxB = ssa_11_all ^ ssa_5_b0;
	APInt ssa_16_minSum = ssa_4_a1 + ssa_6_b1;
	APInt ssa_17_maxSum = ssa_14_maxA + ssa_15_maxB;
	bool ssa_18_ovMax1 = ssa_17_maxSum.ult(ssa_14_maxA);
	bool ssa_19_ovMax2 = ssa_17_maxSum.ult(ssa_15_maxB);
	bool ssa_20_ovMax = ssa_18_ovMax1 || ssa_19_ovMax2;
	bool ssa_21_ovMin1 = ssa_16_minSum.ult(ssa_4_a1);
	bool ssa_22_ovMin2 = ssa_16_minSum.ult(ssa_6_b1);
	bool ssa_23_ovMin = ssa_21_ovMin1 || ssa_22_ovMin2;
	bool ssa_24_true = ssa_12_const0.eq(ssa_12_const0);
	bool ssa_25_notOvMin = ssa_23_ovMin != ssa_24_true;
	bool ssa_26_wrap = ssa_20_ovMax && ssa_25_notOvMin;
	APInt ssa_27_diffN = ssa_16_minSum ^ ssa_17_maxSum;
	APInt ssa_28_lzN(ssa_27_diffN.getBitWidth(), ssa_27_diffN.countl_zero());
	APInt ssa_29_bw(ssa_27_diffN.getBitWidth(), ssa_27_diffN.getBitWidth());
	APInt ssa_30_shiftN = ssa_29_bw - ssa_28_lzN;
	APInt ssa_31_maskN;
	if (ssa_30_shiftN.uge(ssa_30_shiftN.getBitWidth())) {
		ssa_31_maskN = APInt(ssa_11_all.getBitWidth(), 0);
	} else {
		ssa_31_maskN = ssa_11_all.shl(ssa_30_shiftN);
	}
	APInt ssa_32_oneN = ssa_31_maskN & ssa_16_minSum;
	APInt ssa_33_not_minSum = ssa_11_all ^ ssa_16_minSum;
	APInt ssa_34_zeroN = ssa_31_maskN & ssa_33_not_minSum;
	APInt ssa_35_diff0 = ssa_12_const0 ^ ssa_17_maxSum;
	APInt ssa_36_lz0(ssa_35_diff0.getBitWidth(), ssa_35_diff0.countl_zero());
	APInt ssa_37_shift0 = ssa_29_bw - ssa_36_lz0;
	APInt ssa_38_mask0;
	if (ssa_37_shift0.uge(ssa_37_shift0.getBitWidth())) {
		ssa_38_mask0 = APInt(ssa_11_all.getBitWidth(), 0);
	} else {
		ssa_38_mask0 = ssa_11_all.shl(ssa_37_shift0);
	}
	APInt ssa_39_one0 = ssa_38_mask0 & ssa_12_const0;
	APInt ssa_40_not_const0 = ssa_11_all ^ ssa_12_const0;
	APInt ssa_41_zero0 = ssa_38_mask0 & ssa_40_not_const0;
	APInt ssa_42_diff1 = ssa_16_minSum ^ ssa_11_all;
	APInt ssa_43_lz1(ssa_42_diff1.getBitWidth(), ssa_42_diff1.countl_zero());
	APInt ssa_44_shift1 = ssa_29_bw - ssa_43_lz1;
	APInt ssa_45_mask1;
	if (ssa_44_shift1.uge(ssa_44_shift1.getBitWidth())) {
		ssa_45_mask1 = APInt(ssa_11_all.getBitWidth(), 0);
	} else {
		ssa_45_mask1 = ssa_11_all.shl(ssa_44_shift1);
	}
	APInt ssa_46_one1 = ssa_45_mask1 & ssa_16_minSum;
	APInt ssa_47_zero1 = ssa_45_mask1 & ssa_33_not_minSum;
	APInt ssa_48_zeroW = ssa_41_zero0 & ssa_47_zero1;
	APInt ssa_49_oneW = ssa_39_one0 & ssa_46_one1;
	APInt ssa_50_sum0 = ssa_26_wrap ? ssa_48_zeroW : ssa_34_zeroN;
	APInt ssa_51_sum1 = ssa_26_wrap ? ssa_49_oneW : ssa_32_oneN;
	APInt ssa_52_t2_zero = ssa_3_a0 | ssa_50_sum0;
	APInt ssa_53_t2_one = ssa_4_a1 & ssa_51_sum1;
	APInt ssa_54_out_zero_pre = ssa_9_t1_zero & ssa_52_t2_zero;
	APInt ssa_55_out_one_pre = ssa_10_t1_one | ssa_53_t2_one;
	APInt ssa_56_x1_y0 = ssa_4_a1 & ssa_5_b0;
	APInt ssa_57_x1_or_y1 = ssa_4_a1 | ssa_6_b1;
	APInt ssa_58_z1_x1ory1 = ssa_8_c1 & ssa_57_x1_or_y1;
	APInt ssa_59_lsb_one_all = ssa_56_x1_y0 | ssa_58_z1_x1ory1;
	APInt ssa_60_lsb_one = ssa_59_lsb_one_all & ssa_13_const1;
	APInt ssa_61_y0_or_z0 = ssa_5_b0 | ssa_7_c0;
	APInt ssa_62_x0_y0orz0 = ssa_3_a0 & ssa_61_y0_or_z0;
	APInt ssa_63_y1_z0 = ssa_6_b1 & ssa_7_c0;
	APInt ssa_64_lsb_zero_all = ssa_62_x0_y0orz0 | ssa_63_y1_z0;
	APInt ssa_65_lsb_zero = ssa_64_lsb_zero_all & ssa_13_const1;
	APInt ssa_66_not_const1 = ssa_11_all ^ ssa_13_const1;
	APInt ssa_67_out_zero_no_lsb = ssa_54_out_zero_pre & ssa_66_not_const1;
	APInt ssa_68_out_one_no_lsb = ssa_55_out_one_pre & ssa_66_not_const1;
	APInt ssa_69_out_zero = ssa_67_out_zero_no_lsb | ssa_65_lsb_zero;
	APInt ssa_70_out_one = ssa_68_out_one_no_lsb | ssa_60_lsb_one;
	APInt ssa_71_overlap = ssa_10_t1_one & ssa_53_t2_one;
	bool ssa_72_overlap_nz = ssa_71_overlap.ne(ssa_12_const0);
	APInt ssa_73_res0 = ssa_72_overlap_nz ? ssa_11_all : ssa_69_out_zero;
	APInt ssa_74_res1 = ssa_72_overlap_nz ? ssa_11_all : ssa_70_out_one;
	std::array<APInt, 2> ssa_75_r = std::array<APInt, 2>{ssa_73_res0, ssa_74_res1};
	return ssa_75_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_const0(ssa_5_b0.getBitWidth(), 0);
	APInt ssa_10_const1(ssa_5_b0.getBitWidth(), 1);
	APInt ssa_11_all = APInt::getAllOnes(ssa_5_b0.getBitWidth());
	APInt ssa_12_t(ssa_5_b0.getBitWidth(), ssa_5_b0.countr_one());
	APInt ssa_13_t1 = ssa_12_t + ssa_10_const1;
	APInt ssa_14_mask_low = ssa_9_const0;
	if (ssa_12_t.ule(ssa_12_t.getBitWidth()))
		ssa_14_mask_low.setLowBits(ssa_12_t.getZExtValue());
	else
		ssa_14_mask_low.setLowBits(ssa_12_t.getBitWidth());
	APInt ssa_15_mask_low_incl = ssa_9_const0;
	if (ssa_13_t1.ule(ssa_13_t1.getBitWidth()))
		ssa_15_mask_low_incl.setLowBits(ssa_13_t1.getZExtValue());
	else
		ssa_15_mask_low_incl.setLowBits(ssa_13_t1.getBitWidth());
	APInt ssa_16_zero_low = ssa_3_a0 & ssa_14_mask_low;
	APInt ssa_17_one_low = ssa_4_a1 & ssa_14_mask_low;
	APInt ssa_18_mask1 = ssa_6_b1 & ssa_15_mask_low_incl;
	APInt ssa_19_zero_bit_t = ssa_18_mask1 & ssa_7_c0;
	APInt ssa_20_zero_ab = ssa_3_a0 & ssa_5_b0;
	APInt ssa_21_zero_ac = ssa_3_a0 & ssa_7_c0;
	APInt ssa_22_zero_mask0 = ssa_16_zero_low | ssa_19_zero_bit_t;
	APInt ssa_23_zero_mask1 = ssa_20_zero_ab | ssa_21_zero_ac;
	APInt ssa_24_zero_mask2 = ssa_22_zero_mask0 | ssa_23_zero_mask1;
	APInt ssa_25_one_bc = ssa_6_b1 & ssa_8_c1;
	APInt ssa_26_ta(ssa_3_a0.getBitWidth(), ssa_3_a0.countr_one());
	APInt ssa_27_ta1 = ssa_26_ta + ssa_10_const1;
	APInt ssa_28_mask_a = ssa_9_const0;
	if (ssa_27_ta1.ule(ssa_27_ta1.getBitWidth()))
		ssa_28_mask_a.setLowBits(ssa_27_ta1.getZExtValue());
	else
		ssa_28_mask_a.setLowBits(ssa_27_ta1.getBitWidth());
	APInt ssa_29_mask_a1 = ssa_4_a1 & ssa_28_mask_a;
	APInt ssa_30_one_from_c = ssa_29_mask_a1 & ssa_8_c1;
	APInt ssa_31_one_from_b0 = ssa_29_mask_a1 & ssa_5_b0;
	APInt ssa_32_one_tmp = ssa_17_one_low | ssa_25_one_bc;
	APInt ssa_33_one_tmp2 = ssa_30_one_from_c | ssa_31_one_from_b0;
	APInt ssa_34_one_mask0 = ssa_32_one_tmp | ssa_33_one_tmp2;
	APInt ssa_35_k(ssa_4_a1.getBitWidth(), ssa_4_a1.countr_one());
	APInt ssa_36_mask_k = ssa_9_const0;
	if (ssa_35_k.ule(ssa_35_k.getBitWidth()))
		ssa_36_mask_k.setLowBits(ssa_35_k.getZExtValue());
	else
		ssa_36_mask_k.setLowBits(ssa_35_k.getBitWidth());
	APInt ssa_37_a1_lsb = ssa_4_a1 & ssa_10_const1;
	APInt ssa_38_b1_lsb = ssa_6_b1 & ssa_10_const1;
	APInt ssa_39_c1_low = ssa_8_c1 & ssa_36_mask_k;
	bool ssa_40_cond_a = ssa_37_a1_lsb.eq(ssa_10_const1);
	bool ssa_41_cond_b = ssa_38_b1_lsb.eq(ssa_10_const1);
	bool ssa_42_cond_c = ssa_39_c1_low.eq(ssa_36_mask_k);
	bool ssa_43_cond_ab = ssa_40_cond_a && ssa_41_cond_b;
	bool ssa_44_cond = ssa_43_cond_ab && ssa_42_cond_c;
	APInt ssa_45_not_const1 = ssa_11_all ^ ssa_10_const1;
	APInt ssa_46_mask_k_no0 = ssa_36_mask_k & ssa_45_not_const1;
	APInt ssa_47_zero_k = ssa_44_cond ? ssa_46_mask_k_no0 : ssa_9_const0;
	APInt ssa_48_one_k = ssa_44_cond ? ssa_10_const1 : ssa_9_const0;
	APInt ssa_49_mask_m0 = ssa_6_b1 & ssa_8_c1;
	APInt ssa_50_mask_m = ssa_49_mask_m0 & ssa_36_mask_k;
	bool ssa_51_mask_m_nz = ssa_50_mask_m.ne(ssa_9_const0);
	APInt ssa_52_ctz(ssa_50_mask_m.getBitWidth(), ssa_50_mask_m.countr_zero());
	APInt ssa_53_low_mask = ssa_9_const0;
	if (ssa_52_ctz.ule(ssa_52_ctz.getBitWidth()))
		ssa_53_low_mask.setLowBits(ssa_52_ctz.getZExtValue());
	else
		ssa_53_low_mask.setLowBits(ssa_52_ctz.getBitWidth());
	APInt ssa_54_low_ones = ssa_51_mask_m_nz ? ssa_53_low_mask : ssa_9_const0;
	APInt ssa_55_bit_t_mask = ssa_15_mask_low_incl ^ ssa_14_mask_low;
	APInt ssa_56_one_t0 = ssa_55_bit_t_mask & ssa_36_mask_k;
	APInt ssa_57_one_t = ssa_56_one_t0 & ssa_8_c1;
	APInt ssa_58_zero_mask = ssa_24_zero_mask2 | ssa_47_zero_k;
	APInt ssa_59_one_mask1 = ssa_34_one_mask0 | ssa_48_one_k;
	APInt ssa_60_one_mask2 = ssa_59_one_mask1 | ssa_54_low_ones;
	APInt ssa_61_one_mask = ssa_60_one_mask2 | ssa_57_one_t;
	std::array<APInt, 2> ssa_62_r = std::array<APInt, 2>{ssa_58_zero_mask, ssa_61_one_mask};
	return ssa_62_r;
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
	APInt ssa_11_tA(ssa_3_a0.getBitWidth(), ssa_3_a0.countr_one());
	APInt ssa_12_tA1 = ssa_11_tA + ssa_10_const1;
	APInt ssa_13_mask_cin0 = ssa_9_const0;
	if (ssa_12_tA1.ule(ssa_12_tA1.getBitWidth()))
		ssa_13_mask_cin0.setLowBits(ssa_12_tA1.getZExtValue());
	else
		ssa_13_mask_cin0.setLowBits(ssa_12_tA1.getBitWidth());
	APInt ssa_14_ab1 = ssa_4_a1 & ssa_6_b1;
	APInt ssa_15_tAB1(ssa_14_ab1.getBitWidth(), ssa_14_ab1.countr_one());
	APInt ssa_16_mask_low_ab = ssa_9_const0;
	if (ssa_15_tAB1.ule(ssa_15_tAB1.getBitWidth()))
		ssa_16_mask_low_ab.setLowBits(ssa_15_tAB1.getZExtValue());
	else
		ssa_16_mask_low_ab.setLowBits(ssa_15_tAB1.getBitWidth());
	APInt ssa_17_mask_cin1;
	if (ssa_10_const1.uge(ssa_10_const1.getBitWidth())) {
		ssa_17_mask_cin1 = APInt(ssa_16_mask_low_ab.getBitWidth(), 0);
	} else {
		ssa_17_mask_cin1 = ssa_16_mask_low_ab.shl(ssa_10_const1);
	}
	APInt ssa_18_b1c0 = ssa_6_b1 & ssa_7_c0;
	APInt ssa_19_zero_extra0 = ssa_18_b1c0 & ssa_13_mask_cin0;
	APInt ssa_20_zero_extra1 = ssa_5_b0 & ssa_17_mask_cin1;
	APInt ssa_21_zero_extra2 = ssa_17_mask_cin1 & ssa_4_a1;
	APInt ssa_22_zero_extra3 = ssa_21_zero_extra2 & ssa_8_c1;
	APInt ssa_23_out_zero0 = ssa_19_zero_extra0 | ssa_20_zero_extra1;
	APInt ssa_24_out_zero = ssa_23_out_zero0 | ssa_22_zero_extra3;
	APInt ssa_25_x1 = ssa_6_b1 & ssa_8_c1;
	APInt ssa_26_y1_extra0 = ssa_13_mask_cin0 & ssa_5_b0;
	APInt ssa_27_y1_extra1 = ssa_17_mask_cin1 & ssa_6_b1;
	APInt ssa_28_y1_extra2 = ssa_26_y1_extra0 & ssa_4_a1;
	APInt ssa_29_y1_extra3 = ssa_27_y1_extra1 & ssa_4_a1;
	APInt ssa_30_out_one0 = ssa_25_x1 | ssa_28_y1_extra2;
	APInt ssa_31_out_one = ssa_30_out_one0 | ssa_29_y1_extra3;
	std::array<APInt, 2> ssa_32_r = std::array<APInt, 2>{ssa_24_out_zero, ssa_31_out_one};
	return ssa_32_r;
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
