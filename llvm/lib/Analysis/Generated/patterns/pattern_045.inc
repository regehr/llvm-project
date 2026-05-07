namespace Pattern045 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_x0 = ssa_0_lhs[0];
	APInt ssa_3_x1 = ssa_0_lhs[1];
	APInt ssa_4_y0 = ssa_1_rhs[0];
	APInt ssa_5_y1 = ssa_1_rhs[1];
	APInt ssa_6_const0(ssa_2_x0.getBitWidth(), 0);
	APInt ssa_7_const1(ssa_2_x0.getBitWidth(), 1);
	APInt ssa_8_const2(ssa_2_x0.getBitWidth(), 2);
	APInt ssa_9_bw(ssa_2_x0.getBitWidth(), ssa_2_x0.getBitWidth());
	APInt ssa_10_all_ones = APInt::getAllOnes(ssa_2_x0.getBitWidth());
	APInt ssa_11_not_bit0 = ssa_10_all_ones ^ ssa_7_const1;
	APInt ssa_12_no_generate = ssa_2_x0 | ssa_4_y0;
	APInt ssa_13_tz(ssa_12_no_generate.getBitWidth(), ssa_12_no_generate.countr_one());
	APInt ssa_14_tz_plus = ssa_13_tz + ssa_7_const1;
	APInt ssa_15_tz_clamp = APIntOps::umin(ssa_14_tz_plus, ssa_9_bw);
	APInt ssa_16_mask_c0_base = ssa_6_const0;
	if (ssa_15_tz_clamp.ule(ssa_15_tz_clamp.getBitWidth()))
		ssa_16_mask_c0_base.setLowBits(ssa_15_tz_clamp.getZExtValue());
	else
		ssa_16_mask_c0_base.setLowBits(ssa_15_tz_clamp.getBitWidth());
	APInt ssa_17_both_one = ssa_3_x1 & ssa_5_y1;
	APInt ssa_18_to(ssa_17_both_one.getBitWidth(), ssa_17_both_one.countr_one());
	APInt ssa_19_to_plus = ssa_18_to + ssa_7_const1;
	APInt ssa_20_to_clamp = APIntOps::umin(ssa_19_to_plus, ssa_9_bw);
	APInt ssa_21_mask_c1_base_all = ssa_6_const0;
	if (ssa_20_to_clamp.ule(ssa_20_to_clamp.getBitWidth()))
		ssa_21_mask_c1_base_all.setLowBits(ssa_20_to_clamp.getZExtValue());
	else
		ssa_21_mask_c1_base_all.setLowBits(ssa_20_to_clamp.getBitWidth());
	APInt ssa_22_mask_c1_base = ssa_21_mask_c1_base_all & ssa_11_not_bit0;
	APInt ssa_23_both_zero = ssa_2_x0 & ssa_4_y0;
	APInt ssa_24_carry1_gen;
	if (ssa_7_const1.uge(ssa_7_const1.getBitWidth())) {
		ssa_24_carry1_gen = APInt(ssa_17_both_one.getBitWidth(), 0);
	} else {
		ssa_24_carry1_gen = ssa_17_both_one.shl(ssa_7_const1);
	}
	APInt ssa_25_carry0_gen;
	if (ssa_7_const1.uge(ssa_7_const1.getBitWidth())) {
		ssa_25_carry0_gen = APInt(ssa_23_both_zero.getBitWidth(), 0);
	} else {
		ssa_25_carry0_gen = ssa_23_both_zero.shl(ssa_7_const1);
	}
	APInt ssa_26_mask_c0_or = ssa_16_mask_c0_base | ssa_25_carry0_gen;
	APInt ssa_27_mask_c1_or = ssa_22_mask_c1_base | ssa_24_carry1_gen;
	APInt ssa_28_prop1 = ssa_3_x1 | ssa_5_y1;
	APInt ssa_29_prop1_shr;
	if (ssa_7_const1.uge(ssa_7_const1.getBitWidth())) {
		ssa_29_prop1_shr = APInt(ssa_28_prop1.getBitWidth(), 0);
	} else {
		ssa_29_prop1_shr = ssa_28_prop1.lshr(ssa_7_const1);
	}
	APInt ssa_30_prop1_cnt(ssa_29_prop1_shr.getBitWidth(), ssa_29_prop1_shr.countr_one());
	APInt ssa_31_prop1_cnt_plus = ssa_30_prop1_cnt + ssa_7_const1;
	APInt ssa_32_prop1_cnt_clamp = APIntOps::umin(ssa_31_prop1_cnt_plus, ssa_9_bw);
	APInt ssa_33_prop1_mask = ssa_6_const0;
	if (ssa_32_prop1_cnt_clamp.ule(ssa_32_prop1_cnt_clamp.getBitWidth()))
		ssa_33_prop1_mask.setLowBits(ssa_32_prop1_cnt_clamp.getZExtValue());
	else
		ssa_33_prop1_mask.setLowBits(ssa_32_prop1_cnt_clamp.getBitWidth());
	APInt ssa_34_prop1_mask_shl;
	if (ssa_7_const1.uge(ssa_7_const1.getBitWidth())) {
		ssa_34_prop1_mask_shl = APInt(ssa_33_prop1_mask.getBitWidth(), 0);
	} else {
		ssa_34_prop1_mask_shl = ssa_33_prop1_mask.shl(ssa_7_const1);
	}
	APInt ssa_35_carry1_bit1 = ssa_24_carry1_gen & ssa_8_const2;
	bool ssa_36_carry1_bit1_cmp = ssa_35_carry1_bit1.eq(ssa_8_const2);
	APInt ssa_37_carry1_from_bit1 = ssa_36_carry1_bit1_cmp ? ssa_34_prop1_mask_shl : ssa_6_const0;
	APInt ssa_38_mask_c1_or2 = ssa_27_mask_c1_or | ssa_37_carry1_from_bit1;
	APInt ssa_39_carry1_prop1 = ssa_38_mask_c1_or2 & ssa_28_prop1;
	APInt ssa_40_carry1_prop1_s;
	if (ssa_7_const1.uge(ssa_7_const1.getBitWidth())) {
		ssa_40_carry1_prop1_s = APInt(ssa_39_carry1_prop1.getBitWidth(), 0);
	} else {
		ssa_40_carry1_prop1_s = ssa_39_carry1_prop1.shl(ssa_7_const1);
	}
	APInt ssa_41_mask_c1_or1 = ssa_38_mask_c1_or2 | ssa_40_carry1_prop1_s;
	APInt ssa_42_carry1_prop2 = ssa_40_carry1_prop1_s & ssa_28_prop1;
	APInt ssa_43_carry1_prop2_s;
	if (ssa_7_const1.uge(ssa_7_const1.getBitWidth())) {
		ssa_43_carry1_prop2_s = APInt(ssa_42_carry1_prop2.getBitWidth(), 0);
	} else {
		ssa_43_carry1_prop2_s = ssa_42_carry1_prop2.shl(ssa_7_const1);
	}
	APInt ssa_44_mask_c1 = ssa_41_mask_c1_or1 | ssa_43_carry1_prop2_s;
	APInt ssa_45_not_carry1 = ssa_44_mask_c1 ^ ssa_10_all_ones;
	APInt ssa_46_mask_c0 = ssa_26_mask_c0_or & ssa_45_not_carry1;
	APInt ssa_47_x1y0 = ssa_3_x1 & ssa_4_y0;
	APInt ssa_48_x0y1 = ssa_2_x0 & ssa_5_y1;
	APInt ssa_49_sum1_c0 = ssa_47_x1y0 | ssa_48_x0y1;
	APInt ssa_50_sum1_c0_m = ssa_49_sum1_c0 & ssa_46_mask_c0;
	APInt ssa_51_sum0_c0 = ssa_23_both_zero | ssa_17_both_one;
	APInt ssa_52_sum0_c0_m = ssa_51_sum0_c0 & ssa_46_mask_c0;
	APInt ssa_53_sum1_c1 = ssa_51_sum0_c0 & ssa_44_mask_c1;
	APInt ssa_54_sum0_c1 = ssa_49_sum1_c0 & ssa_44_mask_c1;
	APInt ssa_55_sum1 = ssa_50_sum1_c0_m | ssa_53_sum1_c1;
	APInt ssa_56_sum0 = ssa_52_sum0_c0_m | ssa_54_sum0_c1;
	APInt ssa_57_z1_approx = ssa_3_x1 & ssa_55_sum1;
	APInt ssa_58_z0_extra_c0 = ssa_5_y1 & ssa_46_mask_c0;
	APInt ssa_59_z0_extra_c1 = ssa_4_y0 & ssa_44_mask_c1;
	APInt ssa_60_z0_extra = ssa_58_z0_extra_c0 | ssa_59_z0_extra_c1;
	APInt ssa_61_z0_approx = ssa_2_x0 | ssa_56_sum0;
	APInt ssa_62_z0_approx2 = ssa_61_z0_approx | ssa_60_z0_extra;
	APInt ssa_63_x_xor = ssa_2_x0 ^ ssa_3_x1;
	APInt ssa_64_y_xor = ssa_4_y0 ^ ssa_5_y1;
	bool ssa_65_x_full = ssa_63_x_xor.eq(ssa_10_all_ones);
	bool ssa_66_y_full = ssa_64_y_xor.eq(ssa_10_all_ones);
	bool ssa_67_both_full = ssa_65_x_full && ssa_66_y_full;
	APInt ssa_68_sum_val = ssa_3_x1 + ssa_5_y1;
	APInt ssa_69_res_val = ssa_3_x1 & ssa_68_sum_val;
	APInt ssa_70_res_zero = ssa_69_res_val ^ ssa_10_all_ones;
	APInt ssa_71_z0 = ssa_67_both_full ? ssa_70_res_zero : ssa_62_z0_approx2;
	APInt ssa_72_z1 = ssa_67_both_full ? ssa_69_res_val : ssa_57_z1_approx;
	std::array<APInt, 2> ssa_73_r = std::array<APInt, 2>{ssa_71_z0, ssa_72_z1};
	return ssa_73_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_x0 = ssa_0_lhs[0];
	APInt ssa_3_x1 = ssa_0_lhs[1];
	APInt ssa_4_y0 = ssa_1_rhs[0];
	APInt ssa_5_y1 = ssa_1_rhs[1];
	APInt ssa_6_const0(ssa_2_x0.getBitWidth(), 0);
	APInt ssa_7_const1(ssa_2_x0.getBitWidth(), 1);
	APInt ssa_8_bw(ssa_2_x0.getBitWidth(), ssa_2_x0.getBitWidth());
	APInt ssa_9_y0_lead(ssa_4_y0.getBitWidth(), ssa_4_y0.countl_one());
	APInt ssa_10_mask_y0_high = ssa_6_const0;
	if (ssa_9_y0_lead.ule(ssa_9_y0_lead.getBitWidth()))
		ssa_10_mask_y0_high.setHighBits(ssa_9_y0_lead.getZExtValue());
	else
		ssa_10_mask_y0_high.setHighBits(ssa_9_y0_lead.getBitWidth());
	APInt ssa_11_both_zero = ssa_2_x0 & ssa_4_y0;
	APInt ssa_12_both_zero_high = ssa_11_both_zero & ssa_10_mask_y0_high;
	APInt ssa_13_tz(ssa_12_both_zero_high.getBitWidth(), ssa_12_both_zero_high.countr_zero());
	APInt ssa_14_tz_plus = ssa_13_tz + ssa_7_const1;
	APInt ssa_15_p1 = APIntOps::umin(ssa_14_tz_plus, ssa_8_bw);
	APInt ssa_16_high_count = ssa_8_bw - ssa_15_p1;
	APInt ssa_17_mask_above = ssa_6_const0;
	if (ssa_16_high_count.ule(ssa_16_high_count.getBitWidth()))
		ssa_17_mask_above.setHighBits(ssa_16_high_count.getZExtValue());
	else
		ssa_17_mask_above.setHighBits(ssa_16_high_count.getBitWidth());
	APInt ssa_18_z1 = ssa_3_x1 & ssa_17_mask_above;
	APInt ssa_19_high_region = ssa_3_x1 & ssa_4_y0;
	APInt ssa_20_prefix_len(ssa_19_high_region.getBitWidth(), ssa_19_high_region.countl_one());
	APInt ssa_21_mask_prefix = ssa_6_const0;
	if (ssa_20_prefix_len.ule(ssa_20_prefix_len.getBitWidth()))
		ssa_21_mask_prefix.setHighBits(ssa_20_prefix_len.getZExtValue());
	else
		ssa_21_mask_prefix.setHighBits(ssa_20_prefix_len.getBitWidth());
	APInt ssa_22_prefix_plus = ssa_20_prefix_len + ssa_7_const1;
	APInt ssa_23_prefix_plus_clamp = APIntOps::umin(ssa_22_prefix_plus, ssa_8_bw);
	APInt ssa_24_mask_prefix_plus = ssa_6_const0;
	if (ssa_23_prefix_plus_clamp.ule(ssa_23_prefix_plus_clamp.getBitWidth()))
		ssa_24_mask_prefix_plus.setHighBits(ssa_23_prefix_plus_clamp.getZExtValue());
	else
		ssa_24_mask_prefix_plus.setHighBits(ssa_23_prefix_plus_clamp.getBitWidth());
	APInt ssa_25_mask_p = ssa_24_mask_prefix_plus ^ ssa_21_mask_prefix;
	APInt ssa_26_both_one = ssa_3_x1 & ssa_5_y1;
	APInt ssa_27_both_one_p = ssa_26_both_one & ssa_25_mask_p;
	bool ssa_28_both_one_p_ne0 = ssa_27_both_one_p.ne(ssa_6_const0);
	APInt ssa_29_z0_extra = ssa_28_both_one_p_ne0 ? ssa_21_mask_prefix : ssa_6_const0;
	APInt ssa_30_z0_base = ssa_2_x0 | ssa_29_z0_extra;
	APInt ssa_31_z0 = ssa_30_z0_base | ssa_6_const0;
	std::array<APInt, 2> ssa_32_r = std::array<APInt, 2>{ssa_31_z0, ssa_18_z1};
	return ssa_32_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_x0 = ssa_0_lhs[0];
	APInt ssa_3_x1 = ssa_0_lhs[1];
	APInt ssa_4_y0 = ssa_1_rhs[0];
	APInt ssa_5_y1 = ssa_1_rhs[1];
	APInt ssa_6_known_x = ssa_2_x0 | ssa_3_x1;
	APInt ssa_7_known_y = ssa_4_y0 | ssa_5_y1;
	APInt ssa_8_tx(ssa_6_known_x.getBitWidth(), ssa_6_known_x.countr_one());
	APInt ssa_9_ty(ssa_7_known_y.getBitWidth(), ssa_7_known_y.countr_one());
	APInt ssa_10_n = APIntOps::umin(ssa_8_tx, ssa_9_ty);
	APInt ssa_11_const0(ssa_2_x0.getBitWidth(), 0);
	APInt ssa_12_low_mask = ssa_11_const0;
	if (ssa_10_n.ule(ssa_10_n.getBitWidth()))
		ssa_12_low_mask.setLowBits(ssa_10_n.getZExtValue());
	else
		ssa_12_low_mask.setLowBits(ssa_10_n.getBitWidth());
	APInt ssa_13_a_low = ssa_3_x1 & ssa_12_low_mask;
	APInt ssa_14_b_low = ssa_5_y1 & ssa_12_low_mask;
	APInt ssa_15_sum_low = ssa_13_a_low + ssa_14_b_low;
	APInt ssa_16_r_low = ssa_13_a_low & ssa_15_sum_low;
	APInt ssa_17_r_low_m = ssa_16_r_low & ssa_12_low_mask;
	APInt ssa_18_z1 = ssa_17_r_low_m | ssa_11_const0;
	APInt ssa_19_z0 = ssa_17_r_low_m ^ ssa_12_low_mask;
	std::array<APInt, 2> ssa_20_r = std::array<APInt, 2>{ssa_19_z0, ssa_18_z1};
	return ssa_20_r;
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
std::array<APInt, 2> solution(std::array<APInt, 2> ssa_0_, std::array<APInt, 2> ssa_1_) {
	std::array<APInt, 2> ssa_2_ = partial_solution_0(ssa_0_, ssa_1_);
	std::array<APInt, 2> ssa_3_ = partial_solution_1(ssa_0_, ssa_1_);
	std::array<APInt, 2> ssa_4_ = partial_solution_2(ssa_0_, ssa_1_);
	std::array<APInt, 2> ssa_5_ = meet(ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_6_ = meet(ssa_5_, ssa_4_);
	return ssa_6_;
}
}
