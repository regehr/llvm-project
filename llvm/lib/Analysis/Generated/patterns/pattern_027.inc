namespace Pattern027 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0_z = ssa_0_arg0[0];
	APInt ssa_4_arg0_o = ssa_0_arg0[1];
	APInt ssa_5_arg1_z = ssa_1_arg1[0];
	APInt ssa_6_arg1_o = ssa_1_arg1[1];
	APInt ssa_7_arg2_z = ssa_2_arg2[0];
	APInt ssa_8_arg2_o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_arg0_z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_arg0_z.getBitWidth(), 1);
	APInt ssa_11_allones = APInt::getAllOnes(ssa_3_arg0_z.getBitWidth());
	APInt ssa_12_bitwidth(ssa_3_arg0_z.getBitWidth(), ssa_3_arg0_z.getBitWidth());
	bool ssa_13_shift_lb_ge_bw = ssa_4_arg0_o.uge(ssa_12_bitwidth);
	APInt ssa_14_tz1(ssa_5_arg1_z.getBitWidth(), ssa_5_arg1_z.countr_one());
	APInt ssa_15_tz2(ssa_7_arg2_z.getBitWidth(), ssa_7_arg2_z.countr_one());
	APInt ssa_16_mask_t1 = ssa_9_zero;
	if (ssa_14_tz1.ule(ssa_14_tz1.getBitWidth()))
		ssa_16_mask_t1.setLowBits(ssa_14_tz1.getZExtValue());
	else
		ssa_16_mask_t1.setLowBits(ssa_14_tz1.getBitWidth());
	APInt ssa_17_mask_t2 = ssa_9_zero;
	if (ssa_15_tz2.ule(ssa_15_tz2.getBitWidth()))
		ssa_17_mask_t2.setLowBits(ssa_15_tz2.getZExtValue());
	else
		ssa_17_mask_t2.setLowBits(ssa_15_tz2.getBitWidth());
	APInt ssa_18_k1_from1 = ssa_8_arg2_o & ssa_16_mask_t1;
	APInt ssa_19_k1_from2 = ssa_6_arg1_o & ssa_17_mask_t2;
	APInt ssa_20_sum_k1_base = ssa_18_k1_from1 | ssa_19_k1_from2;
	APInt ssa_21_kz_from1 = ssa_7_arg2_z & ssa_16_mask_t1;
	APInt ssa_22_kz_from2 = ssa_5_arg1_z & ssa_17_mask_t2;
	APInt ssa_23_sum_kz_base = ssa_21_kz_from1 | ssa_22_kz_from2;
	APInt ssa_24_mask1 = ssa_5_arg1_z | ssa_6_arg1_o;
	APInt ssa_25_mask2 = ssa_7_arg2_z | ssa_8_arg2_o;
	bool ssa_26_exact1 = ssa_24_mask1.eq(ssa_11_allones);
	bool ssa_27_exact2 = ssa_25_mask2.eq(ssa_11_allones);
	bool ssa_28_both_exact = ssa_26_exact1 && ssa_27_exact2;
	APInt ssa_29_sum_val = ssa_6_arg1_o + ssa_8_arg2_o;
	APInt ssa_30_sum_kz_exact = ssa_11_allones ^ ssa_29_sum_val;
	bool ssa_31_a1z_eq = ssa_5_arg1_z.eq(ssa_11_allones);
	bool ssa_32_a1o_eq = ssa_6_arg1_o.eq(ssa_9_zero);
	bool ssa_33_a1_is_zero = ssa_31_a1z_eq && ssa_32_a1o_eq;
	bool ssa_34_a2z_eq = ssa_7_arg2_z.eq(ssa_11_allones);
	bool ssa_35_a2o_eq = ssa_8_arg2_o.eq(ssa_9_zero);
	bool ssa_36_a2_is_zero = ssa_34_a2z_eq && ssa_35_a2o_eq;
	APInt ssa_37_sum_kz = ssa_28_both_exact ? ssa_30_sum_kz_exact : ssa_23_sum_kz_base;
	APInt ssa_38_sum_k1 = ssa_28_both_exact ? ssa_29_sum_val : ssa_20_sum_k1_base;
	APInt ssa_39_sum_kz = ssa_33_a1_is_zero ? ssa_7_arg2_z : ssa_37_sum_kz;
	APInt ssa_40_sum_k1 = ssa_33_a1_is_zero ? ssa_8_arg2_o : ssa_38_sum_k1;
	APInt ssa_41_sum_kz = ssa_36_a2_is_zero ? ssa_5_arg1_z : ssa_39_sum_kz;
	APInt ssa_42_sum_k1 = ssa_36_a2_is_zero ? ssa_6_arg1_o : ssa_40_sum_k1;
	APInt ssa_43_shift_mask = ssa_3_arg0_z | ssa_4_arg0_o;
	bool ssa_44_shift_exact = ssa_43_shift_mask.eq(ssa_11_allones);
	APInt ssa_45_high_mask_exact = ssa_9_zero;
	if (ssa_4_arg0_o.ule(ssa_4_arg0_o.getBitWidth()))
		ssa_45_high_mask_exact.setHighBits(ssa_4_arg0_o.getZExtValue());
	else
		ssa_45_high_mask_exact.setHighBits(ssa_4_arg0_o.getBitWidth());
	APInt ssa_46_sum_kz_shift;
	if (ssa_4_arg0_o.uge(ssa_4_arg0_o.getBitWidth())) {
		ssa_46_sum_kz_shift = APInt(ssa_41_sum_kz.getBitWidth(), 0);
	} else {
		ssa_46_sum_kz_shift = ssa_41_sum_kz.lshr(ssa_4_arg0_o);
	}
	APInt ssa_47_sum_k1_shift;
	if (ssa_4_arg0_o.uge(ssa_4_arg0_o.getBitWidth())) {
		ssa_47_sum_k1_shift = APInt(ssa_42_sum_k1.getBitWidth(), 0);
	} else {
		ssa_47_sum_k1_shift = ssa_42_sum_k1.lshr(ssa_4_arg0_o);
	}
	APInt ssa_48_res_kz_exact_base = ssa_46_sum_kz_shift | ssa_45_high_mask_exact;
	APInt ssa_49_max1 = ssa_11_allones ^ ssa_5_arg1_z;
	APInt ssa_50_max2 = ssa_11_allones ^ ssa_7_arg2_z;
	APInt ssa_51_sum_max = ssa_49_max1 + ssa_50_max2;
	bool ssa_52_no_ov1 = ssa_51_sum_max.uge(ssa_49_max1);
	bool ssa_53_no_ov2 = ssa_51_sum_max.uge(ssa_50_max2);
	bool ssa_54_no_ov = ssa_52_no_ov1 && ssa_53_no_ov2;
	APInt ssa_55_lz_sum(ssa_51_sum_max.getBitWidth(), ssa_51_sum_max.countl_zero());
	APInt ssa_56_lz_res = ssa_55_lz_sum + ssa_4_arg0_o;
	APInt ssa_57_high_mask_bound = ssa_9_zero;
	if (ssa_56_lz_res.ule(ssa_56_lz_res.getBitWidth()))
		ssa_57_high_mask_bound.setHighBits(ssa_56_lz_res.getZExtValue());
	else
		ssa_57_high_mask_bound.setHighBits(ssa_56_lz_res.getBitWidth());
	APInt ssa_58_res_kz_bound = ssa_46_sum_kz_shift | ssa_57_high_mask_bound;
	APInt ssa_59_res_kz_exact = ssa_54_no_ov ? ssa_58_res_kz_bound : ssa_48_res_kz_exact_base;
	APInt ssa_60_high_mask_min = ssa_9_zero;
	if (ssa_4_arg0_o.ule(ssa_4_arg0_o.getBitWidth()))
		ssa_60_high_mask_min.setHighBits(ssa_4_arg0_o.getZExtValue());
	else
		ssa_60_high_mask_min.setHighBits(ssa_4_arg0_o.getBitWidth());
	APInt ssa_61_res_kz_base = ssa_54_no_ov ? ssa_57_high_mask_bound : ssa_60_high_mask_min;
	APInt ssa_62_res_k1_base = ssa_9_zero | ssa_9_zero;
	APInt ssa_63_res_kz = ssa_44_shift_exact ? ssa_59_res_kz_exact : ssa_61_res_kz_base;
	APInt ssa_64_res_k1 = ssa_44_shift_exact ? ssa_47_sum_k1_shift : ssa_62_res_k1_base;
	APInt ssa_65_t_sum(ssa_41_sum_kz.getBitWidth(), ssa_41_sum_kz.countr_one());
	APInt ssa_66_s_max = ssa_11_allones ^ ssa_3_arg0_z;
	bool ssa_67_t_ge = ssa_65_t_sum.uge(ssa_66_s_max);
	APInt ssa_68_diff_raw = ssa_65_t_sum - ssa_66_s_max;
	APInt ssa_69_diff = ssa_67_t_ge ? ssa_68_diff_raw : ssa_9_zero;
	APInt ssa_70_low_zero_mask = ssa_9_zero;
	if (ssa_69_diff.ule(ssa_69_diff.getBitWidth()))
		ssa_70_low_zero_mask.setLowBits(ssa_69_diff.getZExtValue());
	else
		ssa_70_low_zero_mask.setLowBits(ssa_69_diff.getBitWidth());
	APInt ssa_71_res_kz = ssa_63_res_kz | ssa_70_low_zero_mask;
	bool ssa_72_sum_kz_eq = ssa_41_sum_kz.eq(ssa_11_allones);
	bool ssa_73_sum_k1_eq = ssa_42_sum_k1.eq(ssa_9_zero);
	bool ssa_74_sum_is_zero = ssa_72_sum_kz_eq && ssa_73_sum_k1_eq;
	APInt ssa_75_res_kz = ssa_74_sum_is_zero ? ssa_11_allones : ssa_71_res_kz;
	APInt ssa_76_res_k1 = ssa_74_sum_is_zero ? ssa_9_zero : ssa_64_res_k1;
	APInt ssa_77_res_kz = ssa_13_shift_lb_ge_bw ? ssa_11_allones : ssa_75_res_kz;
	APInt ssa_78_res_k1 = ssa_13_shift_lb_ge_bw ? ssa_11_allones : ssa_76_res_k1;
	std::array<APInt, 2> ssa_79_r = std::array<APInt, 2>{ssa_77_res_kz, ssa_78_res_k1};
	return ssa_79_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0_z = ssa_0_arg0[0];
	APInt ssa_4_arg0_o = ssa_0_arg0[1];
	APInt ssa_5_arg1_z = ssa_1_arg1[0];
	APInt ssa_6_arg1_o = ssa_1_arg1[1];
	APInt ssa_7_arg2_z = ssa_2_arg2[0];
	APInt ssa_8_arg2_o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_arg0_z.getBitWidth(), 0);
	APInt ssa_10_allones = APInt::getAllOnes(ssa_3_arg0_z.getBitWidth());
	APInt ssa_11_bitwidth(ssa_3_arg0_z.getBitWidth(), ssa_3_arg0_z.getBitWidth());
	bool ssa_12_shift_lb_ge_bw = ssa_4_arg0_o.uge(ssa_11_bitwidth);
	APInt ssa_13_shift_mask = ssa_3_arg0_z | ssa_4_arg0_o;
	bool ssa_14_shift_exact = ssa_13_shift_mask.eq(ssa_10_allones);
	APInt ssa_15_tz2(ssa_7_arg2_z.getBitWidth(), ssa_7_arg2_z.countr_one());
	APInt ssa_16_arg1_o_sh2;
	if (ssa_15_tz2.uge(ssa_15_tz2.getBitWidth())) {
		ssa_16_arg1_o_sh2 = APInt(ssa_6_arg1_o.getBitWidth(), 0);
	} else {
		ssa_16_arg1_o_sh2 = ssa_6_arg1_o.lshr(ssa_15_tz2);
	}
	APInt ssa_17_arg1_z_sh2;
	if (ssa_15_tz2.uge(ssa_15_tz2.getBitWidth())) {
		ssa_17_arg1_z_sh2 = APInt(ssa_5_arg1_z.getBitWidth(), 0);
	} else {
		ssa_17_arg1_z_sh2 = ssa_5_arg1_z.lshr(ssa_15_tz2);
	}
	APInt ssa_18_arg2_o_sh2;
	if (ssa_15_tz2.uge(ssa_15_tz2.getBitWidth())) {
		ssa_18_arg2_o_sh2 = APInt(ssa_8_arg2_o.getBitWidth(), 0);
	} else {
		ssa_18_arg2_o_sh2 = ssa_8_arg2_o.lshr(ssa_15_tz2);
	}
	APInt ssa_19_arg2_z_sh2;
	if (ssa_15_tz2.uge(ssa_15_tz2.getBitWidth())) {
		ssa_19_arg2_z_sh2 = APInt(ssa_7_arg2_z.getBitWidth(), 0);
	} else {
		ssa_19_arg2_z_sh2 = ssa_7_arg2_z.lshr(ssa_15_tz2);
	}
	APInt ssa_20_mask1_sh2 = ssa_16_arg1_o_sh2 | ssa_17_arg1_z_sh2;
	APInt ssa_21_mask2_sh2 = ssa_18_arg2_o_sh2 | ssa_19_arg2_z_sh2;
	APInt ssa_22_t1_sh2(ssa_20_mask1_sh2.getBitWidth(), ssa_20_mask1_sh2.countr_one());
	APInt ssa_23_t2_sh2(ssa_21_mask2_sh2.getBitWidth(), ssa_21_mask2_sh2.countr_one());
	APInt ssa_24_t_low2 = APIntOps::umin(ssa_22_t1_sh2, ssa_23_t2_sh2);
	APInt ssa_25_mask_low2 = ssa_9_zero;
	if (ssa_24_t_low2.ule(ssa_24_t_low2.getBitWidth()))
		ssa_25_mask_low2.setLowBits(ssa_24_t_low2.getZExtValue());
	else
		ssa_25_mask_low2.setLowBits(ssa_24_t_low2.getBitWidth());
	APInt ssa_26_arg1_low2 = ssa_16_arg1_o_sh2 & ssa_25_mask_low2;
	APInt ssa_27_arg2_low2 = ssa_18_arg2_o_sh2 & ssa_25_mask_low2;
	APInt ssa_28_sum_low2 = ssa_26_arg1_low2 + ssa_27_arg2_low2;
	APInt ssa_29_sum_low_bits2 = ssa_28_sum_low2 & ssa_25_mask_low2;
	APInt ssa_30_sum_k1_low2 = ssa_29_sum_low_bits2 | ssa_9_zero;
	APInt ssa_31_sum_kz_low2 = ssa_29_sum_low_bits2 ^ ssa_25_mask_low2;
	APInt ssa_32_sum_k1;
	if (ssa_15_tz2.uge(ssa_15_tz2.getBitWidth())) {
		ssa_32_sum_k1 = APInt(ssa_30_sum_k1_low2.getBitWidth(), 0);
	} else {
		ssa_32_sum_k1 = ssa_30_sum_k1_low2.shl(ssa_15_tz2);
	}
	APInt ssa_33_sum_kz;
	if (ssa_15_tz2.uge(ssa_15_tz2.getBitWidth())) {
		ssa_33_sum_kz = APInt(ssa_31_sum_kz_low2.getBitWidth(), 0);
	} else {
		ssa_33_sum_kz = ssa_31_sum_kz_low2.shl(ssa_15_tz2);
	}
	APInt ssa_34_res_k1_shift;
	if (ssa_4_arg0_o.uge(ssa_4_arg0_o.getBitWidth())) {
		ssa_34_res_k1_shift = APInt(ssa_32_sum_k1.getBitWidth(), 0);
	} else {
		ssa_34_res_k1_shift = ssa_32_sum_k1.lshr(ssa_4_arg0_o);
	}
	APInt ssa_35_res_kz_shift;
	if (ssa_4_arg0_o.uge(ssa_4_arg0_o.getBitWidth())) {
		ssa_35_res_kz_shift = APInt(ssa_33_sum_kz.getBitWidth(), 0);
	} else {
		ssa_35_res_kz_shift = ssa_33_sum_kz.lshr(ssa_4_arg0_o);
	}
	bool ssa_36_arg1_o_eq_zero = ssa_6_arg1_o.eq(ssa_9_zero);
	APInt ssa_37_arg2_mask = ssa_7_arg2_z | ssa_8_arg2_o;
	bool ssa_38_arg2_exact = ssa_37_arg2_mask.eq(ssa_10_allones);
	APInt ssa_39_lz1(ssa_5_arg1_z.getBitWidth(), ssa_5_arg1_z.countl_one());
	APInt ssa_40_arg1_highmask = ssa_9_zero;
	if (ssa_39_lz1.ule(ssa_39_lz1.getBitWidth()))
		ssa_40_arg1_highmask.setHighBits(ssa_39_lz1.getZExtValue());
	else
		ssa_40_arg1_highmask.setHighBits(ssa_39_lz1.getBitWidth());
	bool ssa_41_arg1_contig = ssa_40_arg1_highmask.eq(ssa_5_arg1_z);
	APInt ssa_42_arg1_max = ssa_10_allones ^ ssa_5_arg1_z;
	APInt ssa_43_sum_max1 = ssa_8_arg2_o + ssa_42_arg1_max;
	bool ssa_44_no_ov1 = ssa_43_sum_max1.uge(ssa_8_arg2_o);
	APInt ssa_45_min_out1;
	if (ssa_4_arg0_o.uge(ssa_4_arg0_o.getBitWidth())) {
		ssa_45_min_out1 = APInt(ssa_8_arg2_o.getBitWidth(), 0);
	} else {
		ssa_45_min_out1 = ssa_8_arg2_o.lshr(ssa_4_arg0_o);
	}
	APInt ssa_46_max_out1;
	if (ssa_4_arg0_o.uge(ssa_4_arg0_o.getBitWidth())) {
		ssa_46_max_out1 = APInt(ssa_43_sum_max1.getBitWidth(), 0);
	} else {
		ssa_46_max_out1 = ssa_43_sum_max1.lshr(ssa_4_arg0_o);
	}
	APInt ssa_47_diff1 = ssa_45_min_out1 ^ ssa_46_max_out1;
	APInt ssa_48_lz_diff1(ssa_47_diff1.getBitWidth(), ssa_47_diff1.countl_zero());
	APInt ssa_49_mask_high1 = ssa_9_zero;
	if (ssa_48_lz_diff1.ule(ssa_48_lz_diff1.getBitWidth()))
		ssa_49_mask_high1.setHighBits(ssa_48_lz_diff1.getZExtValue());
	else
		ssa_49_mask_high1.setHighBits(ssa_48_lz_diff1.getBitWidth());
	APInt ssa_50_res_k1_r1 = ssa_45_min_out1 & ssa_49_mask_high1;
	APInt ssa_51_min_out1_not = ssa_45_min_out1 ^ ssa_10_allones;
	APInt ssa_52_res_kz_r1 = ssa_51_min_out1_not & ssa_49_mask_high1;
	bool ssa_53_cond1_a = ssa_14_shift_exact && ssa_38_arg2_exact;
	bool ssa_54_cond1_b = ssa_36_arg1_o_eq_zero && ssa_41_arg1_contig;
	bool ssa_55_cond1_c = ssa_53_cond1_a && ssa_54_cond1_b;
	bool ssa_56_cond1 = ssa_55_cond1_c && ssa_44_no_ov1;
	APInt ssa_57_res_k1_r1_sel = ssa_56_cond1 ? ssa_50_res_k1_r1 : ssa_9_zero;
	APInt ssa_58_res_kz_r1_sel = ssa_56_cond1 ? ssa_52_res_kz_r1 : ssa_9_zero;
	APInt ssa_59_res_k1_tmp = ssa_34_res_k1_shift | ssa_57_res_k1_r1_sel;
	APInt ssa_60_res_kz_tmp = ssa_35_res_kz_shift | ssa_58_res_kz_r1_sel;
	APInt ssa_61_res_k1 = ssa_14_shift_exact ? ssa_59_res_k1_tmp : ssa_9_zero;
	APInt ssa_62_res_kz = ssa_14_shift_exact ? ssa_60_res_kz_tmp : ssa_9_zero;
	APInt ssa_63_res_k1 = ssa_12_shift_lb_ge_bw ? ssa_10_allones : ssa_61_res_k1;
	APInt ssa_64_res_kz = ssa_12_shift_lb_ge_bw ? ssa_10_allones : ssa_62_res_kz;
	std::array<APInt, 2> ssa_65_r = std::array<APInt, 2>{ssa_64_res_kz, ssa_63_res_k1};
	return ssa_65_r;
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
	std::array<APInt, 2> ssa_5_ = meet(ssa_3_, ssa_4_);
	return ssa_5_;
}
}
