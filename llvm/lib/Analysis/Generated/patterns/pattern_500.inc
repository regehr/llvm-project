namespace Pattern500 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_a0_z = ssa_0_arg0[0];
	APInt ssa_5_a0_o = ssa_0_arg0[1];
	APInt ssa_6_a1_z = ssa_1_arg1[0];
	APInt ssa_7_a1_o = ssa_1_arg1[1];
	APInt ssa_8_a2_z = ssa_2_arg2[0];
	APInt ssa_9_a2_o = ssa_2_arg2[1];
	APInt ssa_10_a3_z = ssa_3_arg3[0];
	APInt ssa_11_a3_o = ssa_3_arg3[1];
	APInt ssa_12_zero(ssa_4_a0_z.getBitWidth(), 0);
	APInt ssa_13_a0_conf = ssa_4_a0_z & ssa_5_a0_o;
	APInt ssa_14_a1_conf = ssa_6_a1_z & ssa_7_a1_o;
	APInt ssa_15_a2_conf = ssa_8_a2_z & ssa_9_a2_o;
	APInt ssa_16_a3_conf = ssa_10_a3_z & ssa_11_a3_o;
	bool ssa_17_a0_bot = ssa_13_a0_conf.ne(ssa_12_zero);
	bool ssa_18_a1_bot = ssa_14_a1_conf.ne(ssa_12_zero);
	bool ssa_19_a2_bot = ssa_15_a2_conf.ne(ssa_12_zero);
	bool ssa_20_a3_bot = ssa_16_a3_conf.ne(ssa_12_zero);
	APInt ssa_21_bw(ssa_4_a0_z.getBitWidth(), ssa_4_a0_z.getBitWidth());
	bool ssa_22_a1_uge_bw = ssa_7_a1_o.uge(ssa_21_bw);
	bool ssa_23_a3_uge_bw = ssa_11_a3_o.uge(ssa_21_bw);
	bool ssa_24_bot01 = ssa_17_a0_bot || ssa_18_a1_bot;
	bool ssa_25_bot23 = ssa_19_a2_bot || ssa_20_a3_bot;
	bool ssa_26_bot0123 = ssa_24_bot01 || ssa_25_bot23;
	bool ssa_27_bot_shift1 = ssa_26_bot0123 || ssa_22_a1_uge_bw;
	bool ssa_28_bot_all = ssa_27_bot_shift1 || ssa_23_a3_uge_bw;
	APInt ssa_29_allones = APInt::getAllOnes(ssa_4_a0_z.getBitWidth());
	APInt ssa_30_a0_known = ssa_4_a0_z | ssa_5_a0_o;
	APInt ssa_31_a0_unknown = ssa_29_allones ^ ssa_30_a0_known;
	APInt ssa_32_low_mask = ssa_12_zero;
	if (ssa_7_a1_o.ule(ssa_7_a1_o.getBitWidth()))
		ssa_32_low_mask.setLowBits(ssa_7_a1_o.getZExtValue());
	else
		ssa_32_low_mask.setLowBits(ssa_7_a1_o.getBitWidth());
	APInt ssa_33_unknown_low = ssa_31_a0_unknown & ssa_32_low_mask;
	bool ssa_34_low_all_known = ssa_33_unknown_low.eq(ssa_12_zero);
	APInt ssa_35_masked_z = ssa_4_a0_z & ssa_32_low_mask;
	APInt ssa_36_masked_o = ssa_5_a0_o & ssa_32_low_mask;
	APInt ssa_37_low_z = ssa_34_low_all_known ? ssa_35_masked_z : ssa_12_zero;
	APInt ssa_38_low_o = ssa_34_low_all_known ? ssa_36_masked_o : ssa_12_zero;
	APInt ssa_39_a2_notz = ssa_29_allones ^ ssa_8_a2_z;
	APInt ssa_40_a3_notz = ssa_29_allones ^ ssa_10_a3_z;
	APInt ssa_41_a2_max = ssa_9_a2_o | ssa_39_a2_notz;
	APInt ssa_42_a3_max = ssa_11_a3_o | ssa_40_a3_notz;
	bool ssa_43_sum_ov;
	ssa_41_a2_max.uadd_ov(ssa_42_a3_max, ssa_43_sum_ov);
	APInt ssa_44_sum_max = ssa_41_a2_max + ssa_42_a3_max;
	APInt ssa_45_one(ssa_4_a0_z.getBitWidth(), 1);
	APInt ssa_46_threshold;
	if (ssa_11_a3_o.uge(ssa_11_a3_o.getBitWidth())) {
		ssa_46_threshold = APInt(ssa_45_one.getBitWidth(), 0);
	} else {
		ssa_46_threshold = ssa_45_one.shl(ssa_11_a3_o);
	}
	bool ssa_47_sum_lt = ssa_44_sum_max.ult(ssa_46_threshold);
	bool ssa_48_true = ssa_12_zero.eq(ssa_12_zero);
	bool ssa_49_no_ov = ssa_43_sum_ov != ssa_48_true;
	bool ssa_50_t1_zero = ssa_49_no_ov && ssa_47_sum_lt;
	bool ssa_51_a3_is_zero = ssa_10_a3_z.eq(ssa_29_allones);
	APInt ssa_52_bw_minus_s1 = ssa_21_bw - ssa_7_a1_o;
	APInt ssa_53_mask_bw_s1 = ssa_12_zero;
	if (ssa_52_bw_minus_s1.ule(ssa_52_bw_minus_s1.getBitWidth()))
		ssa_53_mask_bw_s1.setLowBits(ssa_52_bw_minus_s1.getZExtValue());
	else
		ssa_53_mask_bw_s1.setLowBits(ssa_52_bw_minus_s1.getBitWidth());
	APInt ssa_54_a2_missing = ssa_39_a2_notz & ssa_53_mask_bw_s1;
	bool ssa_55_a2_low_zero = ssa_54_a2_missing.eq(ssa_12_zero);
	bool ssa_56_t2_zero_alt = ssa_51_a3_is_zero && ssa_55_a2_low_zero;
	bool ssa_57_t2_zero = ssa_50_t1_zero || ssa_56_t2_zero_alt;
	APInt ssa_58_res0_mid = ssa_57_t2_zero ? ssa_4_a0_z : ssa_37_low_z;
	APInt ssa_59_res1_mid = ssa_57_t2_zero ? ssa_5_a0_o : ssa_38_low_o;
	APInt ssa_60_res0 = ssa_28_bot_all ? ssa_29_allones : ssa_58_res0_mid;
	APInt ssa_61_res1 = ssa_28_bot_all ? ssa_29_allones : ssa_59_res1_mid;
	std::array<APInt, 2> ssa_62_r = std::array<APInt, 2>{ssa_60_res0, ssa_61_res1};
	return ssa_62_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_a0_z = ssa_0_arg0[0];
	APInt ssa_5_a0_o = ssa_0_arg0[1];
	APInt ssa_6_a1_z = ssa_1_arg1[0];
	APInt ssa_7_a1_o = ssa_1_arg1[1];
	APInt ssa_8_a2_z = ssa_2_arg2[0];
	APInt ssa_9_a2_o = ssa_2_arg2[1];
	APInt ssa_10_a3_z = ssa_3_arg3[0];
	APInt ssa_11_a3_o = ssa_3_arg3[1];
	APInt ssa_12_zero(ssa_4_a0_z.getBitWidth(), 0);
	bool ssa_13_true = ssa_12_zero.eq(ssa_12_zero);
	APInt ssa_14_a0_conf = ssa_4_a0_z & ssa_5_a0_o;
	APInt ssa_15_a1_conf = ssa_6_a1_z & ssa_7_a1_o;
	APInt ssa_16_a2_conf = ssa_8_a2_z & ssa_9_a2_o;
	APInt ssa_17_a3_conf = ssa_10_a3_z & ssa_11_a3_o;
	bool ssa_18_a0_bot = ssa_14_a0_conf.ne(ssa_12_zero);
	bool ssa_19_a1_bot = ssa_15_a1_conf.ne(ssa_12_zero);
	bool ssa_20_a2_bot = ssa_16_a2_conf.ne(ssa_12_zero);
	bool ssa_21_a3_bot = ssa_17_a3_conf.ne(ssa_12_zero);
	APInt ssa_22_bw(ssa_4_a0_z.getBitWidth(), ssa_4_a0_z.getBitWidth());
	bool ssa_23_a1_uge_bw = ssa_7_a1_o.uge(ssa_22_bw);
	bool ssa_24_a3_uge_bw = ssa_11_a3_o.uge(ssa_22_bw);
	bool ssa_25_bot01 = ssa_18_a0_bot || ssa_19_a1_bot;
	bool ssa_26_bot23 = ssa_20_a2_bot || ssa_21_a3_bot;
	bool ssa_27_bot0123 = ssa_25_bot01 || ssa_26_bot23;
	bool ssa_28_bot_shift1 = ssa_27_bot0123 || ssa_23_a1_uge_bw;
	APInt ssa_29_allones = APInt::getAllOnes(ssa_4_a0_z.getBitWidth());
	APInt ssa_30_low_mask = ssa_12_zero;
	if (ssa_7_a1_o.ule(ssa_7_a1_o.getBitWidth()))
		ssa_30_low_mask.setLowBits(ssa_7_a1_o.getZExtValue());
	else
		ssa_30_low_mask.setLowBits(ssa_7_a1_o.getBitWidth());
	APInt ssa_31_low_z = ssa_4_a0_z & ssa_30_low_mask;
	APInt ssa_32_low_o = ssa_5_a0_o & ssa_30_low_mask;
	APInt ssa_33_a2_notz = ssa_29_allones ^ ssa_8_a2_z;
	APInt ssa_34_a3_notz = ssa_29_allones ^ ssa_10_a3_z;
	APInt ssa_35_a2_max = ssa_9_a2_o | ssa_33_a2_notz;
	APInt ssa_36_a3_max = ssa_11_a3_o | ssa_34_a3_notz;
	bool ssa_37_sum_ov;
	ssa_35_a2_max.uadd_ov(ssa_36_a3_max, ssa_37_sum_ov);
	APInt ssa_38_sum_max = ssa_35_a2_max + ssa_36_a3_max;
	APInt ssa_39_one(ssa_4_a0_z.getBitWidth(), 1);
	APInt ssa_40_a3_trail_zero(ssa_10_a3_z.getBitWidth(), ssa_10_a3_z.countr_one());
	APInt ssa_41_pow_k;
	if (ssa_40_a3_trail_zero.uge(ssa_40_a3_trail_zero.getBitWidth())) {
		ssa_41_pow_k = APInt(ssa_39_one.getBitWidth(), 0);
	} else {
		ssa_41_pow_k = ssa_39_one.shl(ssa_40_a3_trail_zero);
	}
	bool ssa_42_k_eq_bw = ssa_40_a3_trail_zero.eq(ssa_22_bw);
	bool ssa_43_pow_ge_bw = ssa_41_pow_k.uge(ssa_22_bw);
	bool ssa_44_a3_bound = ssa_42_k_eq_bw || ssa_43_pow_ge_bw;
	bool ssa_45_a3_o_is_zero = ssa_11_a3_o.eq(ssa_12_zero);
	bool ssa_46_a3_must_zero = ssa_44_a3_bound && ssa_45_a3_o_is_zero;
	bool ssa_47_not_a3_o_is_zero = ssa_45_a3_o_is_zero != ssa_13_true;
	bool ssa_48_a3_invalid = ssa_44_a3_bound && ssa_47_not_a3_o_is_zero;
	bool ssa_49_bot_shift2 = ssa_28_bot_shift1 || ssa_24_a3_uge_bw;
	bool ssa_50_bot_all = ssa_49_bot_shift2 || ssa_48_a3_invalid;
	APInt ssa_51_threshold;
	if (ssa_11_a3_o.uge(ssa_11_a3_o.getBitWidth())) {
		ssa_51_threshold = APInt(ssa_39_one.getBitWidth(), 0);
	} else {
		ssa_51_threshold = ssa_39_one.shl(ssa_11_a3_o);
	}
	bool ssa_52_sum_lt = ssa_38_sum_max.ult(ssa_51_threshold);
	bool ssa_53_no_ov = ssa_37_sum_ov != ssa_13_true;
	bool ssa_54_t1_zero = ssa_53_no_ov && ssa_52_sum_lt;
	bool ssa_55_a3_is_zero = ssa_10_a3_z.eq(ssa_29_allones);
	bool ssa_56_a3_zero_cond = ssa_55_a3_is_zero || ssa_46_a3_must_zero;
	APInt ssa_57_bw_minus_s1 = ssa_22_bw - ssa_7_a1_o;
	APInt ssa_58_mask_bw_s1 = ssa_12_zero;
	if (ssa_57_bw_minus_s1.ule(ssa_57_bw_minus_s1.getBitWidth()))
		ssa_58_mask_bw_s1.setLowBits(ssa_57_bw_minus_s1.getZExtValue());
	else
		ssa_58_mask_bw_s1.setLowBits(ssa_57_bw_minus_s1.getBitWidth());
	APInt ssa_59_a2_missing = ssa_33_a2_notz & ssa_58_mask_bw_s1;
	bool ssa_60_a2_low_zero = ssa_59_a2_missing.eq(ssa_12_zero);
	bool ssa_61_t2_zero_alt = ssa_56_a3_zero_cond && ssa_60_a2_low_zero;
	bool ssa_62_t2_zero = ssa_54_t1_zero || ssa_61_t2_zero_alt;
	APInt ssa_63_res0_mid = ssa_62_t2_zero ? ssa_4_a0_z : ssa_31_low_z;
	APInt ssa_64_res1_mid = ssa_62_t2_zero ? ssa_5_a0_o : ssa_32_low_o;
	APInt ssa_65_res0 = ssa_50_bot_all ? ssa_29_allones : ssa_63_res0_mid;
	APInt ssa_66_res1 = ssa_50_bot_all ? ssa_29_allones : ssa_64_res1_mid;
	std::array<APInt, 2> ssa_67_r = std::array<APInt, 2>{ssa_65_res0, ssa_66_res1};
	return ssa_67_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_a0_z = ssa_0_arg0[0];
	APInt ssa_5_a0_o = ssa_0_arg0[1];
	APInt ssa_6_a1_z = ssa_1_arg1[0];
	APInt ssa_7_a1_o = ssa_1_arg1[1];
	APInt ssa_8_a2_z = ssa_2_arg2[0];
	APInt ssa_9_a2_o = ssa_2_arg2[1];
	APInt ssa_10_a3_z = ssa_3_arg3[0];
	APInt ssa_11_a3_o = ssa_3_arg3[1];
	APInt ssa_12_zero(ssa_4_a0_z.getBitWidth(), 0);
	APInt ssa_13_allones = APInt::getAllOnes(ssa_4_a0_z.getBitWidth());
	APInt ssa_14_a0_known = ssa_4_a0_z | ssa_5_a0_o;
	APInt ssa_15_a1_known = ssa_6_a1_z | ssa_7_a1_o;
	APInt ssa_16_a2_known = ssa_8_a2_z | ssa_9_a2_o;
	APInt ssa_17_a3_known = ssa_10_a3_z | ssa_11_a3_o;
	bool ssa_18_a0_full = ssa_14_a0_known.eq(ssa_13_allones);
	bool ssa_19_a1_full = ssa_15_a1_known.eq(ssa_13_allones);
	bool ssa_20_a2_full = ssa_16_a2_known.eq(ssa_13_allones);
	bool ssa_21_a3_full = ssa_17_a3_known.eq(ssa_13_allones);
	bool ssa_22_full01 = ssa_18_a0_full && ssa_19_a1_full;
	bool ssa_23_full23 = ssa_20_a2_full && ssa_21_a3_full;
	bool ssa_24_all_full = ssa_22_full01 && ssa_23_full23;
	APInt ssa_25_bw(ssa_4_a0_z.getBitWidth(), ssa_4_a0_z.getBitWidth());
	bool ssa_26_a1_lt_bw = ssa_7_a1_o.ult(ssa_25_bw);
	bool ssa_27_a3_lt_bw = ssa_11_a3_o.ult(ssa_25_bw);
	bool ssa_28_shift_ok = ssa_26_a1_lt_bw && ssa_27_a3_lt_bw;
	APInt ssa_29_t0 = ssa_9_a2_o + ssa_11_a3_o;
	APInt ssa_30_t1;
	if (ssa_11_a3_o.uge(ssa_11_a3_o.getBitWidth())) {
		ssa_30_t1 = APInt(ssa_29_t0.getBitWidth(), 0);
	} else {
		ssa_30_t1 = ssa_29_t0.lshr(ssa_11_a3_o);
	}
	APInt ssa_31_t2;
	if (ssa_7_a1_o.uge(ssa_7_a1_o.getBitWidth())) {
		ssa_31_t2 = APInt(ssa_30_t1.getBitWidth(), 0);
	} else {
		ssa_31_t2 = ssa_30_t1.shl(ssa_7_a1_o);
	}
	APInt ssa_32_t3 = ssa_5_a0_o + ssa_31_t2;
	APInt ssa_33_res0_exact = ssa_13_allones ^ ssa_32_t3;
	APInt ssa_34_res0_full = ssa_28_shift_ok ? ssa_33_res0_exact : ssa_13_allones;
	APInt ssa_35_res1_full = ssa_28_shift_ok ? ssa_32_t3 : ssa_13_allones;
	APInt ssa_36_res0_base = ssa_24_all_full ? ssa_34_res0_full : ssa_12_zero;
	APInt ssa_37_res1_base = ssa_24_all_full ? ssa_35_res1_full : ssa_12_zero;
	bool ssa_38_a2_is_zero = ssa_8_a2_z.eq(ssa_13_allones);
	APInt ssa_39_res0_a2 = ssa_38_a2_is_zero ? ssa_4_a0_z : ssa_12_zero;
	APInt ssa_40_res1_a2 = ssa_38_a2_is_zero ? ssa_5_a0_o : ssa_12_zero;
	APInt ssa_41_res0 = ssa_36_res0_base | ssa_39_res0_a2;
	APInt ssa_42_res1 = ssa_37_res1_base | ssa_40_res1_a2;
	std::array<APInt, 2> ssa_43_r = std::array<APInt, 2>{ssa_41_res0, ssa_42_res1};
	return ssa_43_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_a0_z = ssa_0_arg0[0];
	APInt ssa_5_a0_o = ssa_0_arg0[1];
	APInt ssa_6_a1_z = ssa_1_arg1[0];
	APInt ssa_7_a1_o = ssa_1_arg1[1];
	APInt ssa_8_a2_z = ssa_2_arg2[0];
	APInt ssa_9_a2_o = ssa_2_arg2[1];
	APInt ssa_10_a3_z = ssa_3_arg3[0];
	APInt ssa_11_a3_o = ssa_3_arg3[1];
	APInt ssa_12_zero(ssa_4_a0_z.getBitWidth(), 0);
	bool ssa_13_true = ssa_12_zero.eq(ssa_12_zero);
	APInt ssa_14_allones = APInt::getAllOnes(ssa_4_a0_z.getBitWidth());
	APInt ssa_15_a0_notz = ssa_14_allones ^ ssa_4_a0_z;
	APInt ssa_16_a0_max = ssa_5_a0_o | ssa_15_a0_notz;
	APInt ssa_17_a1_notz = ssa_14_allones ^ ssa_6_a1_z;
	APInt ssa_18_a2_notz = ssa_14_allones ^ ssa_8_a2_z;
	APInt ssa_19_a3_notz = ssa_14_allones ^ ssa_10_a3_z;
	APInt ssa_20_a1_max_raw = ssa_7_a1_o | ssa_17_a1_notz;
	APInt ssa_21_a2_max = ssa_9_a2_o | ssa_18_a2_notz;
	APInt ssa_22_a3_max_raw = ssa_11_a3_o | ssa_19_a3_notz;
	APInt ssa_23_one(ssa_4_a0_z.getBitWidth(), 1);
	APInt ssa_24_bw(ssa_4_a0_z.getBitWidth(), ssa_4_a0_z.getBitWidth());
	APInt ssa_25_bw_minus_one = ssa_24_bw - ssa_23_one;
	bool ssa_26_a1_max_ge_bw = ssa_20_a1_max_raw.uge(ssa_24_bw);
	APInt ssa_27_a1_max = ssa_26_a1_max_ge_bw ? ssa_25_bw_minus_one : ssa_20_a1_max_raw;
	bool ssa_28_a3_max_ge_bw = ssa_22_a3_max_raw.uge(ssa_24_bw);
	APInt ssa_29_a3_max = ssa_28_a3_max_ge_bw ? ssa_25_bw_minus_one : ssa_22_a3_max_raw;
	APInt ssa_30_sum_max = ssa_21_a2_max + ssa_29_a3_max;
	bool ssa_31_sum_ov;
	ssa_21_a2_max.uadd_ov(ssa_29_a3_max, ssa_31_sum_ov);
	bool ssa_32_a1_le_a3 = ssa_27_a1_max.ule(ssa_11_a3_o);
	bool ssa_33_no_sum_ov = ssa_31_sum_ov != ssa_13_true;
	bool ssa_34_sum_ok = ssa_33_no_sum_ov && ssa_32_a1_le_a3;
	APInt ssa_35_max_res = ssa_16_a0_max + ssa_30_sum_max;
	bool ssa_36_max_res_ov;
	ssa_16_a0_max.uadd_ov(ssa_30_sum_max, ssa_36_max_res_ov);
	bool ssa_37_no_max_ov = ssa_36_max_res_ov != ssa_13_true;
	bool ssa_38_prefix_ok = ssa_34_sum_ok && ssa_37_no_max_ov;
	APInt ssa_39_msb_mask = ssa_12_zero;
	ssa_39_msb_mask.setSignBit();
	bool ssa_40_max_lt_msb = ssa_35_max_res.ult(ssa_39_msb_mask);
	bool ssa_41_min_ge_msb = ssa_5_a0_o.uge(ssa_39_msb_mask);
	bool ssa_42_msb_zero_cond = ssa_38_prefix_ok && ssa_40_max_lt_msb;
	bool ssa_43_msb_one_cond = ssa_38_prefix_ok && ssa_41_min_ge_msb;
	APInt ssa_44_msb_zero = ssa_42_msb_zero_cond ? ssa_39_msb_mask : ssa_12_zero;
	APInt ssa_45_msb_one = ssa_43_msb_one_cond ? ssa_39_msb_mask : ssa_12_zero;
	std::array<APInt, 2> ssa_46_r = std::array<APInt, 2>{ssa_44_msb_zero, ssa_45_msb_one};
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
std::array<APInt, 2> solution(std::array<APInt, 2> ssa_0_, std::array<APInt, 2> ssa_1_, std::array<APInt, 2> ssa_2_, std::array<APInt, 2> ssa_3_) {
	std::array<APInt, 2> ssa_4_ = partial_solution_0(ssa_0_, ssa_1_, ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_5_ = partial_solution_1(ssa_0_, ssa_1_, ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_6_ = partial_solution_2(ssa_0_, ssa_1_, ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_7_ = partial_solution_3(ssa_0_, ssa_1_, ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_8_ = meet(ssa_4_, ssa_5_);
	std::array<APInt, 2> ssa_9_ = meet(ssa_8_, ssa_6_);
	std::array<APInt, 2> ssa_10_ = meet(ssa_9_, ssa_7_);
	return ssa_10_;
}
}
