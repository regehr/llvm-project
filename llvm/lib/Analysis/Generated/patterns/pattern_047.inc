namespace Pattern047 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0_kz = ssa_0_arg0[0];
	APInt ssa_4_a0_ko = ssa_0_arg0[1];
	APInt ssa_5_a1_kz = ssa_1_arg1[0];
	APInt ssa_6_a1_ko = ssa_1_arg1[1];
	APInt ssa_7_a2_kz = ssa_2_arg2[0];
	APInt ssa_8_a2_ko = ssa_2_arg2[1];
	APInt ssa_9_a0_mask = ssa_3_a0_kz | ssa_4_a0_ko;
	APInt ssa_10_a1_mask = ssa_5_a1_kz | ssa_6_a1_ko;
	APInt ssa_11_a2_mask = ssa_7_a2_kz | ssa_8_a2_ko;
	APInt ssa_12_tz0(ssa_9_a0_mask.getBitWidth(), ssa_9_a0_mask.countr_one());
	APInt ssa_13_tz1(ssa_10_a1_mask.getBitWidth(), ssa_10_a1_mask.countr_one());
	APInt ssa_14_tz2(ssa_11_a2_mask.getBitWidth(), ssa_11_a2_mask.countr_one());
	APInt ssa_15_k01 = APIntOps::umin(ssa_12_tz0, ssa_13_tz1);
	APInt ssa_16_k = APIntOps::umin(ssa_15_k01, ssa_14_tz2);
	APInt ssa_17_zero(ssa_3_a0_kz.getBitWidth(), 0);
	APInt ssa_18_maskLow = ssa_17_zero;
	if (ssa_16_k.ule(ssa_16_k.getBitWidth()))
		ssa_18_maskLow.setLowBits(ssa_16_k.getZExtValue());
	else
		ssa_18_maskLow.setLowBits(ssa_16_k.getBitWidth());
	APInt ssa_19_a0_val_low = ssa_4_a0_ko & ssa_18_maskLow;
	APInt ssa_20_a1_val_low = ssa_6_a1_ko & ssa_18_maskLow;
	APInt ssa_21_a2_val_low = ssa_8_a2_ko & ssa_18_maskLow;
	APInt ssa_22_sum_low = ssa_20_a1_val_low + ssa_21_a2_val_low;
	APInt ssa_23_res_low = ssa_22_sum_low - ssa_19_a0_val_low;
	APInt ssa_24_res_low_masked = ssa_23_res_low & ssa_18_maskLow;
	APInt ssa_25_out_k0 = ssa_24_res_low_masked ^ ssa_18_maskLow;
	APInt ssa_26_out_k1 = ssa_24_res_low_masked | ssa_17_zero;
	std::array<APInt, 2> ssa_27_r = std::array<APInt, 2>{ssa_25_out_k0, ssa_26_out_k1};
	return ssa_27_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0_kz = ssa_0_arg0[0];
	APInt ssa_4_a0_ko = ssa_0_arg0[1];
	APInt ssa_5_a1_kz = ssa_1_arg1[0];
	APInt ssa_6_a1_ko = ssa_1_arg1[1];
	APInt ssa_7_a2_kz = ssa_2_arg2[0];
	APInt ssa_8_a2_ko = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0_kz.getBitWidth(), 0);
	APInt ssa_10_allones = APInt::getAllOnes(ssa_3_a0_kz.getBitWidth());
	APInt ssa_11_a0_mask = ssa_3_a0_kz | ssa_4_a0_ko;
	APInt ssa_12_a1_mask = ssa_5_a1_kz | ssa_6_a1_ko;
	APInt ssa_13_a2_mask = ssa_7_a2_kz | ssa_8_a2_ko;
	APInt ssa_14_tz0(ssa_11_a0_mask.getBitWidth(), ssa_11_a0_mask.countr_one());
	APInt ssa_15_tz1(ssa_12_a1_mask.getBitWidth(), ssa_12_a1_mask.countr_one());
	APInt ssa_16_tz2(ssa_13_a2_mask.getBitWidth(), ssa_13_a2_mask.countr_one());
	APInt ssa_17_k01 = APIntOps::umin(ssa_14_tz0, ssa_15_tz1);
	APInt ssa_18_k = APIntOps::umin(ssa_17_k01, ssa_16_tz2);
	APInt ssa_19_maskLow = ssa_9_zero;
	if (ssa_18_k.ule(ssa_18_k.getBitWidth()))
		ssa_19_maskLow.setLowBits(ssa_18_k.getZExtValue());
	else
		ssa_19_maskLow.setLowBits(ssa_18_k.getBitWidth());
	APInt ssa_20_a0_val_low = ssa_4_a0_ko & ssa_19_maskLow;
	APInt ssa_21_a1_val_low = ssa_6_a1_ko & ssa_19_maskLow;
	APInt ssa_22_a2_val_low = ssa_8_a2_ko & ssa_19_maskLow;
	APInt ssa_23_sum_low = ssa_21_a1_val_low + ssa_22_a2_val_low;
	APInt ssa_24_res_low = ssa_23_sum_low - ssa_20_a0_val_low;
	APInt ssa_25_res_low_masked = ssa_24_res_low & ssa_19_maskLow;
	APInt ssa_26_low_k0 = ssa_25_res_low_masked ^ ssa_19_maskLow;
	APInt ssa_27_low_k1 = ssa_25_res_low_masked | ssa_9_zero;
	APInt ssa_28_a0_unknown = ssa_11_a0_mask ^ ssa_10_allones;
	APInt ssa_29_a1_unknown = ssa_12_a1_mask ^ ssa_10_allones;
	APInt ssa_30_a2_unknown = ssa_13_a2_mask ^ ssa_10_allones;
	APInt ssa_31_a0_max = ssa_4_a0_ko | ssa_28_a0_unknown;
	APInt ssa_32_a1_max = ssa_6_a1_ko | ssa_29_a1_unknown;
	APInt ssa_33_a2_max = ssa_8_a2_ko | ssa_30_a2_unknown;
	APInt ssa_34_minsum = ssa_6_a1_ko + ssa_8_a2_ko;
	APInt ssa_35_maxsum = ssa_32_a1_max + ssa_33_a2_max;
	APInt ssa_36_res_min = ssa_34_minsum - ssa_31_a0_max;
	APInt ssa_37_res_max = ssa_35_maxsum - ssa_4_a0_ko;
	bool ssa_38_ov = ssa_35_maxsum.ult(ssa_32_a1_max);
	bool ssa_39_under_ok = ssa_34_minsum.uge(ssa_31_a0_max);
	bool ssa_40_true = ssa_9_zero.eq(ssa_9_zero);
	bool ssa_41_not_ov = ssa_38_ov != ssa_40_true;
	bool ssa_42_nowrap = ssa_41_not_ov && ssa_39_under_ok;
	APInt ssa_43_lz(ssa_37_res_max.getBitWidth(), ssa_37_res_max.countl_zero());
	APInt ssa_44_maskHigh0 = ssa_9_zero;
	if (ssa_43_lz.ule(ssa_43_lz.getBitWidth()))
		ssa_44_maskHigh0.setHighBits(ssa_43_lz.getZExtValue());
	else
		ssa_44_maskHigh0.setHighBits(ssa_43_lz.getBitWidth());
	APInt ssa_45_l1_min(ssa_36_res_min.getBitWidth(), ssa_36_res_min.countl_one());
	APInt ssa_46_l1_max(ssa_37_res_max.getBitWidth(), ssa_37_res_max.countl_one());
	APInt ssa_47_l1 = APIntOps::umin(ssa_45_l1_min, ssa_46_l1_max);
	APInt ssa_48_maskHigh1 = ssa_9_zero;
	if (ssa_47_l1.ule(ssa_47_l1.getBitWidth()))
		ssa_48_maskHigh1.setHighBits(ssa_47_l1.getZExtValue());
	else
		ssa_48_maskHigh1.setHighBits(ssa_47_l1.getBitWidth());
	APInt ssa_49_range_k0 = ssa_42_nowrap ? ssa_44_maskHigh0 : ssa_9_zero;
	APInt ssa_50_range_k1 = ssa_42_nowrap ? ssa_48_maskHigh1 : ssa_9_zero;
	APInt ssa_51_out_k0 = ssa_26_low_k0 | ssa_49_range_k0;
	APInt ssa_52_out_k1 = ssa_27_low_k1 | ssa_50_range_k1;
	std::array<APInt, 2> ssa_53_r = std::array<APInt, 2>{ssa_51_out_k0, ssa_52_out_k1};
	return ssa_53_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0_kz = ssa_0_arg0[0];
	APInt ssa_4_a0_ko = ssa_0_arg0[1];
	APInt ssa_5_a1_kz = ssa_1_arg1[0];
	APInt ssa_6_a1_ko = ssa_1_arg1[1];
	APInt ssa_7_a2_kz = ssa_2_arg2[0];
	APInt ssa_8_a2_ko = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0_kz.getBitWidth(), 0);
	APInt ssa_10_allones = APInt::getAllOnes(ssa_3_a0_kz.getBitWidth());
	APInt ssa_11_a0_mask = ssa_3_a0_kz | ssa_4_a0_ko;
	APInt ssa_12_a1_mask = ssa_5_a1_kz | ssa_6_a1_ko;
	APInt ssa_13_a2_mask = ssa_7_a2_kz | ssa_8_a2_ko;
	APInt ssa_14_a0_unknown = ssa_11_a0_mask ^ ssa_10_allones;
	APInt ssa_15_a1_unknown = ssa_12_a1_mask ^ ssa_10_allones;
	APInt ssa_16_a2_unknown = ssa_13_a2_mask ^ ssa_10_allones;
	APInt ssa_17_a0_max = ssa_4_a0_ko | ssa_14_a0_unknown;
	APInt ssa_18_a1_max = ssa_6_a1_ko | ssa_15_a1_unknown;
	APInt ssa_19_a2_max = ssa_8_a2_ko | ssa_16_a2_unknown;
	APInt ssa_20_minsum = ssa_6_a1_ko + ssa_8_a2_ko;
	APInt ssa_21_maxsum = ssa_18_a1_max + ssa_19_a2_max;
	APInt ssa_22_res_min = ssa_20_minsum - ssa_17_a0_max;
	APInt ssa_23_res_max = ssa_21_maxsum - ssa_4_a0_ko;
	bool ssa_24_ov = ssa_21_maxsum.ult(ssa_18_a1_max);
	bool ssa_25_true = ssa_9_zero.eq(ssa_9_zero);
	bool ssa_26_not_ov = ssa_24_ov != ssa_25_true;
	bool ssa_27_under_always = ssa_21_maxsum.ult(ssa_4_a0_ko);
	bool ssa_28_cond = ssa_26_not_ov && ssa_27_under_always;
	APInt ssa_29_l1_min(ssa_22_res_min.getBitWidth(), ssa_22_res_min.countl_one());
	APInt ssa_30_l1_max(ssa_23_res_max.getBitWidth(), ssa_23_res_max.countl_one());
	APInt ssa_31_l1 = APIntOps::umin(ssa_29_l1_min, ssa_30_l1_max);
	APInt ssa_32_maskHigh1 = ssa_9_zero;
	if (ssa_31_l1.ule(ssa_31_l1.getBitWidth()))
		ssa_32_maskHigh1.setHighBits(ssa_31_l1.getZExtValue());
	else
		ssa_32_maskHigh1.setHighBits(ssa_31_l1.getBitWidth());
	APInt ssa_33_out_k1 = ssa_28_cond ? ssa_32_maskHigh1 : ssa_9_zero;
	APInt ssa_34_out_k0(ssa_3_a0_kz.getBitWidth(), 0);
	std::array<APInt, 2> ssa_35_r = std::array<APInt, 2>{ssa_34_out_k0, ssa_33_out_k1};
	return ssa_35_r;
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
