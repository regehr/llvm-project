namespace Pattern025 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_kz0 = ssa_0_arg0[0];
	APInt ssa_5_ko0 = ssa_0_arg0[1];
	APInt ssa_6_kz1 = ssa_1_arg1[0];
	APInt ssa_7_ko1 = ssa_1_arg1[1];
	APInt ssa_8_kz2 = ssa_2_arg2[0];
	APInt ssa_9_ko2 = ssa_2_arg2[1];
	APInt ssa_10_kz3 = ssa_3_arg3[0];
	APInt ssa_11_ko3 = ssa_3_arg3[1];
	APInt ssa_12_ones = APInt::getAllOnes(ssa_4_kz0.getBitWidth());
	APInt ssa_13_or0 = ssa_4_kz0 | ssa_5_ko0;
	APInt ssa_14_or1 = ssa_6_kz1 | ssa_7_ko1;
	APInt ssa_15_or2 = ssa_8_kz2 | ssa_9_ko2;
	APInt ssa_16_or3 = ssa_10_kz3 | ssa_11_ko3;
	bool ssa_17_full0 = ssa_13_or0.eq(ssa_12_ones);
	bool ssa_18_full1 = ssa_14_or1.eq(ssa_12_ones);
	bool ssa_19_full2 = ssa_15_or2.eq(ssa_12_ones);
	bool ssa_20_full3 = ssa_16_or3.eq(ssa_12_ones);
	bool ssa_21_cond01 = ssa_17_full0 && ssa_18_full1;
	bool ssa_22_cond23 = ssa_19_full2 && ssa_20_full3;
	bool ssa_23_cond = ssa_21_cond01 && ssa_22_cond23;
	APInt ssa_24_sum01 = ssa_5_ko0 + ssa_7_ko1;
	APInt ssa_25_sum23 = ssa_9_ko2 + ssa_11_ko3;
	APInt ssa_26_sum_val = ssa_24_sum01 + ssa_25_sum23;
	APInt ssa_27_not_sum = ssa_26_sum_val ^ ssa_12_ones;
	APInt ssa_28_mask0 = ssa_4_kz0 | ssa_5_ko0;
	APInt ssa_29_mask1 = ssa_6_kz1 | ssa_7_ko1;
	APInt ssa_30_mask2 = ssa_8_kz2 | ssa_9_ko2;
	APInt ssa_31_mask3 = ssa_10_kz3 | ssa_11_ko3;
	APInt ssa_32_k0(ssa_28_mask0.getBitWidth(), ssa_28_mask0.countr_one());
	APInt ssa_33_k1(ssa_29_mask1.getBitWidth(), ssa_29_mask1.countr_one());
	APInt ssa_34_k2(ssa_30_mask2.getBitWidth(), ssa_30_mask2.countr_one());
	APInt ssa_35_k3(ssa_31_mask3.getBitWidth(), ssa_31_mask3.countr_one());
	APInt ssa_36_k01 = APIntOps::umin(ssa_32_k0, ssa_33_k1);
	APInt ssa_37_k23 = APIntOps::umin(ssa_34_k2, ssa_35_k3);
	APInt ssa_38_k = APIntOps::umin(ssa_36_k01, ssa_37_k23);
	APInt ssa_39_zero(ssa_4_kz0.getBitWidth(), 0);
	APInt ssa_40_mask_low = ssa_39_zero;
	if (ssa_38_k.ule(ssa_38_k.getBitWidth()))
		ssa_40_mask_low.setLowBits(ssa_38_k.getZExtValue());
	else
		ssa_40_mask_low.setLowBits(ssa_38_k.getBitWidth());
	APInt ssa_41_val0 = ssa_5_ko0 & ssa_40_mask_low;
	APInt ssa_42_val1 = ssa_7_ko1 & ssa_40_mask_low;
	APInt ssa_43_val2 = ssa_9_ko2 & ssa_40_mask_low;
	APInt ssa_44_val3 = ssa_11_ko3 & ssa_40_mask_low;
	APInt ssa_45_sum01_low = ssa_41_val0 + ssa_42_val1;
	APInt ssa_46_sum23_low = ssa_43_val2 + ssa_44_val3;
	APInt ssa_47_sum_low = ssa_45_sum01_low + ssa_46_sum23_low;
	APInt ssa_48_knownOne_low = ssa_47_sum_low & ssa_40_mask_low;
	APInt ssa_49_not_sum_low = ssa_47_sum_low ^ ssa_40_mask_low;
	APInt ssa_50_knownZero_low = ssa_49_not_sum_low & ssa_40_mask_low;
	APInt ssa_51_resOne = ssa_23_cond ? ssa_26_sum_val : ssa_48_knownOne_low;
	APInt ssa_52_resZero = ssa_23_cond ? ssa_27_not_sum : ssa_50_knownZero_low;
	std::array<APInt, 2> ssa_53_r = std::array<APInt, 2>{ssa_52_resZero, ssa_51_resOne};
	return ssa_53_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_kz0 = ssa_0_arg0[0];
	APInt ssa_5_ko0 = ssa_0_arg0[1];
	APInt ssa_6_kz1 = ssa_1_arg1[0];
	APInt ssa_7_ko1 = ssa_1_arg1[1];
	APInt ssa_8_kz2 = ssa_2_arg2[0];
	APInt ssa_9_ko2 = ssa_2_arg2[1];
	APInt ssa_10_kz3 = ssa_3_arg3[0];
	APInt ssa_11_ko3 = ssa_3_arg3[1];
	APInt ssa_12_ones = APInt::getAllOnes(ssa_4_kz0.getBitWidth());
	APInt ssa_13_zero(ssa_4_kz0.getBitWidth(), 0);
	APInt ssa_14_mask0 = ssa_4_kz0 | ssa_5_ko0;
	APInt ssa_15_mask1 = ssa_6_kz1 | ssa_7_ko1;
	APInt ssa_16_mask2 = ssa_8_kz2 | ssa_9_ko2;
	APInt ssa_17_mask3 = ssa_10_kz3 | ssa_11_ko3;
	APInt ssa_18_lead0(ssa_14_mask0.getBitWidth(), ssa_14_mask0.countl_one());
	APInt ssa_19_lead1(ssa_15_mask1.getBitWidth(), ssa_15_mask1.countl_one());
	APInt ssa_20_lead2(ssa_16_mask2.getBitWidth(), ssa_16_mask2.countl_one());
	APInt ssa_21_lead3(ssa_17_mask3.getBitWidth(), ssa_17_mask3.countl_one());
	APInt ssa_22_mask_lead0 = ssa_13_zero;
	if (ssa_18_lead0.ule(ssa_18_lead0.getBitWidth()))
		ssa_22_mask_lead0.setHighBits(ssa_18_lead0.getZExtValue());
	else
		ssa_22_mask_lead0.setHighBits(ssa_18_lead0.getBitWidth());
	APInt ssa_23_mask_lead1 = ssa_13_zero;
	if (ssa_19_lead1.ule(ssa_19_lead1.getBitWidth()))
		ssa_23_mask_lead1.setHighBits(ssa_19_lead1.getZExtValue());
	else
		ssa_23_mask_lead1.setHighBits(ssa_19_lead1.getBitWidth());
	APInt ssa_24_mask_lead2 = ssa_13_zero;
	if (ssa_20_lead2.ule(ssa_20_lead2.getBitWidth()))
		ssa_24_mask_lead2.setHighBits(ssa_20_lead2.getZExtValue());
	else
		ssa_24_mask_lead2.setHighBits(ssa_20_lead2.getBitWidth());
	APInt ssa_25_mask_lead3 = ssa_13_zero;
	if (ssa_21_lead3.ule(ssa_21_lead3.getBitWidth()))
		ssa_25_mask_lead3.setHighBits(ssa_21_lead3.getZExtValue());
	else
		ssa_25_mask_lead3.setHighBits(ssa_21_lead3.getBitWidth());
	bool ssa_26_ok0 = ssa_14_mask0.eq(ssa_22_mask_lead0);
	bool ssa_27_ok1 = ssa_15_mask1.eq(ssa_23_mask_lead1);
	bool ssa_28_ok2 = ssa_16_mask2.eq(ssa_24_mask_lead2);
	bool ssa_29_ok3 = ssa_17_mask3.eq(ssa_25_mask_lead3);
	bool ssa_30_ok01 = ssa_26_ok0 && ssa_27_ok1;
	bool ssa_31_ok23 = ssa_28_ok2 && ssa_29_ok3;
	bool ssa_32_ok = ssa_30_ok01 && ssa_31_ok23;
	APInt ssa_33_unk0 = ssa_14_mask0 ^ ssa_12_ones;
	APInt ssa_34_unk1 = ssa_15_mask1 ^ ssa_12_ones;
	APInt ssa_35_unk2 = ssa_16_mask2 ^ ssa_12_ones;
	APInt ssa_36_unk3 = ssa_17_mask3 ^ ssa_12_ones;
	APInt ssa_37_max0 = ssa_5_ko0 | ssa_33_unk0;
	APInt ssa_38_max1 = ssa_7_ko1 | ssa_34_unk1;
	APInt ssa_39_max2 = ssa_9_ko2 | ssa_35_unk2;
	APInt ssa_40_max3 = ssa_11_ko3 | ssa_36_unk3;
	APInt ssa_41_sum_min01 = ssa_5_ko0 + ssa_7_ko1;
	APInt ssa_42_sum_min23 = ssa_9_ko2 + ssa_11_ko3;
	APInt ssa_43_sum_min = ssa_41_sum_min01 + ssa_42_sum_min23;
	APInt ssa_44_sum_max01 = ssa_37_max0 + ssa_38_max1;
	APInt ssa_45_sum_max23 = ssa_39_max2 + ssa_40_max3;
	APInt ssa_46_sum_max = ssa_44_sum_max01 + ssa_45_sum_max23;
	bool ssa_47_wrap = ssa_46_sum_max.ult(ssa_43_sum_min);
	APInt ssa_48_diff = ssa_43_sum_min ^ ssa_46_sum_max;
	APInt ssa_49_lead(ssa_48_diff.getBitWidth(), ssa_48_diff.countl_zero());
	APInt ssa_50_mask_high_raw = ssa_13_zero;
	if (ssa_49_lead.ule(ssa_49_lead.getBitWidth()))
		ssa_50_mask_high_raw.setHighBits(ssa_49_lead.getZExtValue());
	else
		ssa_50_mask_high_raw.setHighBits(ssa_49_lead.getBitWidth());
	APInt ssa_51_mask_high_ok = ssa_32_ok ? ssa_50_mask_high_raw : ssa_13_zero;
	APInt ssa_52_mask_high = ssa_47_wrap ? ssa_13_zero : ssa_51_mask_high_ok;
	APInt ssa_53_sum_min_not = ssa_43_sum_min ^ ssa_12_ones;
	APInt ssa_54_knownOne_high = ssa_43_sum_min & ssa_52_mask_high;
	APInt ssa_55_knownZero_high = ssa_53_sum_min_not & ssa_52_mask_high;
	APInt ssa_56_k0(ssa_14_mask0.getBitWidth(), ssa_14_mask0.countr_one());
	APInt ssa_57_k1(ssa_15_mask1.getBitWidth(), ssa_15_mask1.countr_one());
	APInt ssa_58_k2(ssa_16_mask2.getBitWidth(), ssa_16_mask2.countr_one());
	APInt ssa_59_k3(ssa_17_mask3.getBitWidth(), ssa_17_mask3.countr_one());
	APInt ssa_60_k01 = APIntOps::umin(ssa_56_k0, ssa_57_k1);
	APInt ssa_61_k23 = APIntOps::umin(ssa_58_k2, ssa_59_k3);
	APInt ssa_62_k = APIntOps::umin(ssa_60_k01, ssa_61_k23);
	APInt ssa_63_mask_low = ssa_13_zero;
	if (ssa_62_k.ule(ssa_62_k.getBitWidth()))
		ssa_63_mask_low.setLowBits(ssa_62_k.getZExtValue());
	else
		ssa_63_mask_low.setLowBits(ssa_62_k.getBitWidth());
	APInt ssa_64_val0 = ssa_5_ko0 & ssa_63_mask_low;
	APInt ssa_65_val1 = ssa_7_ko1 & ssa_63_mask_low;
	APInt ssa_66_val2 = ssa_9_ko2 & ssa_63_mask_low;
	APInt ssa_67_val3 = ssa_11_ko3 & ssa_63_mask_low;
	APInt ssa_68_sum_low01 = ssa_64_val0 + ssa_65_val1;
	APInt ssa_69_sum_low23 = ssa_66_val2 + ssa_67_val3;
	APInt ssa_70_sum_low = ssa_68_sum_low01 + ssa_69_sum_low23;
	APInt ssa_71_knownOne_low = ssa_70_sum_low & ssa_63_mask_low;
	APInt ssa_72_sum_low_not = ssa_70_sum_low ^ ssa_63_mask_low;
	APInt ssa_73_knownZero_low = ssa_72_sum_low_not & ssa_63_mask_low;
	APInt ssa_74_knownOne = ssa_71_knownOne_low | ssa_54_knownOne_high;
	APInt ssa_75_knownZero = ssa_73_knownZero_low | ssa_55_knownZero_high;
	std::array<APInt, 2> ssa_76_r = std::array<APInt, 2>{ssa_75_knownZero, ssa_74_knownOne};
	return ssa_76_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_kz0 = ssa_0_arg0[0];
	APInt ssa_5_ko0 = ssa_0_arg0[1];
	APInt ssa_6_kz1 = ssa_1_arg1[0];
	APInt ssa_7_ko1 = ssa_1_arg1[1];
	APInt ssa_8_kz2 = ssa_2_arg2[0];
	APInt ssa_9_ko2 = ssa_2_arg2[1];
	APInt ssa_10_kz3 = ssa_3_arg3[0];
	APInt ssa_11_ko3 = ssa_3_arg3[1];
	APInt ssa_12_ones = APInt::getAllOnes(ssa_4_kz0.getBitWidth());
	APInt ssa_13_zero(ssa_4_kz0.getBitWidth(), 0);
	APInt ssa_14_mask0 = ssa_4_kz0 | ssa_5_ko0;
	APInt ssa_15_mask1 = ssa_6_kz1 | ssa_7_ko1;
	APInt ssa_16_mask2 = ssa_8_kz2 | ssa_9_ko2;
	APInt ssa_17_mask3 = ssa_10_kz3 | ssa_11_ko3;
	APInt ssa_18_unk0 = ssa_14_mask0 ^ ssa_12_ones;
	APInt ssa_19_unk1 = ssa_15_mask1 ^ ssa_12_ones;
	APInt ssa_20_unk2 = ssa_16_mask2 ^ ssa_12_ones;
	APInt ssa_21_unk3 = ssa_17_mask3 ^ ssa_12_ones;
	APInt ssa_22_max0 = ssa_5_ko0 | ssa_18_unk0;
	APInt ssa_23_max1 = ssa_7_ko1 | ssa_19_unk1;
	APInt ssa_24_max2 = ssa_9_ko2 | ssa_20_unk2;
	APInt ssa_25_max3 = ssa_11_ko3 | ssa_21_unk3;
	APInt ssa_26_sum_max01 = ssa_22_max0 + ssa_23_max1;
	bool ssa_27_ovf01 = ssa_26_sum_max01.ult(ssa_22_max0);
	APInt ssa_28_sum_max23 = ssa_24_max2 + ssa_25_max3;
	bool ssa_29_ovf23 = ssa_28_sum_max23.ult(ssa_24_max2);
	APInt ssa_30_sum_max = ssa_26_sum_max01 + ssa_28_sum_max23;
	bool ssa_31_ovf0123 = ssa_30_sum_max.ult(ssa_26_sum_max01);
	bool ssa_32_ovf01 = ssa_27_ovf01 || ssa_29_ovf23;
	bool ssa_33_wrap = ssa_32_ovf01 || ssa_31_ovf0123;
	APInt ssa_34_sum_min01 = ssa_5_ko0 + ssa_7_ko1;
	APInt ssa_35_sum_min23 = ssa_9_ko2 + ssa_11_ko3;
	APInt ssa_36_sum_min = ssa_34_sum_min01 + ssa_35_sum_min23;
	APInt ssa_37_diff = ssa_36_sum_min ^ ssa_30_sum_max;
	APInt ssa_38_lead(ssa_37_diff.getBitWidth(), ssa_37_diff.countl_zero());
	APInt ssa_39_mask_high_raw = ssa_13_zero;
	if (ssa_38_lead.ule(ssa_38_lead.getBitWidth()))
		ssa_39_mask_high_raw.setHighBits(ssa_38_lead.getZExtValue());
	else
		ssa_39_mask_high_raw.setHighBits(ssa_38_lead.getBitWidth());
	APInt ssa_40_mask_high = ssa_33_wrap ? ssa_13_zero : ssa_39_mask_high_raw;
	APInt ssa_41_sum_min_not = ssa_36_sum_min ^ ssa_12_ones;
	APInt ssa_42_knownOne_high = ssa_36_sum_min & ssa_40_mask_high;
	APInt ssa_43_knownZero_high = ssa_41_sum_min_not & ssa_40_mask_high;
	APInt ssa_44_k0(ssa_14_mask0.getBitWidth(), ssa_14_mask0.countr_one());
	APInt ssa_45_k1(ssa_15_mask1.getBitWidth(), ssa_15_mask1.countr_one());
	APInt ssa_46_k2(ssa_16_mask2.getBitWidth(), ssa_16_mask2.countr_one());
	APInt ssa_47_k3(ssa_17_mask3.getBitWidth(), ssa_17_mask3.countr_one());
	APInt ssa_48_k01 = APIntOps::umin(ssa_44_k0, ssa_45_k1);
	APInt ssa_49_k23 = APIntOps::umin(ssa_46_k2, ssa_47_k3);
	APInt ssa_50_k = APIntOps::umin(ssa_48_k01, ssa_49_k23);
	APInt ssa_51_mask_low = ssa_13_zero;
	if (ssa_50_k.ule(ssa_50_k.getBitWidth()))
		ssa_51_mask_low.setLowBits(ssa_50_k.getZExtValue());
	else
		ssa_51_mask_low.setLowBits(ssa_50_k.getBitWidth());
	APInt ssa_52_val0 = ssa_5_ko0 & ssa_51_mask_low;
	APInt ssa_53_val1 = ssa_7_ko1 & ssa_51_mask_low;
	APInt ssa_54_val2 = ssa_9_ko2 & ssa_51_mask_low;
	APInt ssa_55_val3 = ssa_11_ko3 & ssa_51_mask_low;
	APInt ssa_56_sum_low01 = ssa_52_val0 + ssa_53_val1;
	APInt ssa_57_sum_low23 = ssa_54_val2 + ssa_55_val3;
	APInt ssa_58_sum_low = ssa_56_sum_low01 + ssa_57_sum_low23;
	APInt ssa_59_knownOne_low = ssa_58_sum_low & ssa_51_mask_low;
	APInt ssa_60_sum_low_not = ssa_58_sum_low ^ ssa_51_mask_low;
	APInt ssa_61_knownZero_low = ssa_60_sum_low_not & ssa_51_mask_low;
	APInt ssa_62_knownOne = ssa_59_knownOne_low | ssa_42_knownOne_high;
	APInt ssa_63_knownZero = ssa_61_knownZero_low | ssa_43_knownZero_high;
	std::array<APInt, 2> ssa_64_r = std::array<APInt, 2>{ssa_63_knownZero, ssa_62_knownOne};
	return ssa_64_r;
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
