namespace Pattern004 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_sh_z = ssa_0_arg0[0];
	APInt ssa_4_sh_o = ssa_0_arg0[1];
	APInt ssa_5_a1_z = ssa_1_arg1[0];
	APInt ssa_6_a1_o = ssa_1_arg1[1];
	APInt ssa_7_a2_z = ssa_2_arg2[0];
	APInt ssa_8_a2_o = ssa_2_arg2[1];
	APInt ssa_9_ones = APInt::getAllOnes(ssa_3_sh_z.getBitWidth());
	APInt ssa_10_zero(ssa_3_sh_z.getBitWidth(), 0);
	bool ssa_11_true = ssa_10_zero.eq(ssa_10_zero);
	APInt ssa_12_sh_known = ssa_3_sh_z | ssa_4_sh_o;
	APInt ssa_13_a1_known = ssa_5_a1_z | ssa_6_a1_o;
	APInt ssa_14_a2_known = ssa_7_a2_z | ssa_8_a2_o;
	bool ssa_15_sh_full = ssa_12_sh_known.eq(ssa_9_ones);
	bool ssa_16_a1_full = ssa_13_a1_known.eq(ssa_9_ones);
	bool ssa_17_a2_full = ssa_14_a2_known.eq(ssa_9_ones);
	APInt ssa_18_sh_conf = ssa_3_sh_z & ssa_4_sh_o;
	APInt ssa_19_a1_conf = ssa_5_a1_z & ssa_6_a1_o;
	APInt ssa_20_a2_conf = ssa_7_a2_z & ssa_8_a2_o;
	bool ssa_21_sh_nb = ssa_18_sh_conf.eq(ssa_10_zero);
	bool ssa_22_a1_nb = ssa_19_a1_conf.eq(ssa_10_zero);
	bool ssa_23_a2_nb = ssa_20_a2_conf.eq(ssa_10_zero);
	bool ssa_24_all_nb = ssa_21_sh_nb && ssa_22_a1_nb;
	bool ssa_25_all_nb2 = ssa_24_all_nb && ssa_23_a2_nb;
	bool ssa_26_all_full = ssa_15_sh_full && ssa_16_a1_full;
	bool ssa_27_all_full2 = ssa_26_all_full && ssa_17_a2_full;
	bool ssa_28_const_and_valid = ssa_27_all_full2 && ssa_25_all_nb2;
	bool ssa_29_shift_const = ssa_15_sh_full && ssa_21_sh_nb;
	APInt ssa_30_a1_trail(ssa_13_a1_known.getBitWidth(), ssa_13_a1_known.countr_one());
	APInt ssa_31_a2_trail(ssa_14_a2_known.getBitWidth(), ssa_14_a2_known.countr_one());
	APInt ssa_32_min_trail = APIntOps::umin(ssa_30_a1_trail, ssa_31_a2_trail);
	APInt ssa_33_m2 = APIntOps::umin(ssa_32_min_trail, ssa_4_sh_o);
	APInt ssa_34_mask_m2 = ssa_10_zero;
	if (ssa_33_m2.ule(ssa_33_m2.getBitWidth()))
		ssa_34_mask_m2.setLowBits(ssa_33_m2.getZExtValue());
	else
		ssa_34_mask_m2.setLowBits(ssa_33_m2.getBitWidth());
	APInt ssa_35_a1_low_m2 = ssa_6_a1_o & ssa_34_mask_m2;
	APInt ssa_36_a2_low_m2 = ssa_8_a2_o & ssa_34_mask_m2;
	APInt ssa_37_diff_low_m2 = ssa_36_a2_low_m2 - ssa_35_a1_low_m2;
	APInt ssa_38_diff_low_mask = ssa_37_diff_low_m2 & ssa_34_mask_m2;
	bool ssa_39_diff_low_zero = ssa_38_diff_low_mask.eq(ssa_10_zero);
	bool ssa_40_diff_low_nonzero = ssa_39_diff_low_zero != ssa_11_true;
	bool ssa_41_m2_zero = ssa_33_m2.eq(ssa_10_zero);
	bool ssa_42_m2_nonzero = ssa_41_m2_zero != ssa_11_true;
	bool ssa_43_tmp0 = ssa_29_shift_const && ssa_42_m2_nonzero;
	bool ssa_44_impossible_low = ssa_43_tmp0 && ssa_40_diff_low_nonzero;
	APInt ssa_45_x = ssa_8_a2_o - ssa_6_a1_o;
	APInt ssa_46_y;
	if (ssa_4_sh_o.uge(ssa_4_sh_o.getBitWidth()) && ssa_45_x.isSignBitSet()) {
		ssa_46_y = APInt(ssa_45_x.getBitWidth(), -1);
	} else if (ssa_4_sh_o.uge(ssa_4_sh_o.getBitWidth()) && ssa_45_x.isSignBitClear()) {
		ssa_46_y = APInt(ssa_45_x.getBitWidth(), 0);
	} else {
		ssa_46_y = ssa_45_x.ashr(ssa_4_sh_o);
	}
	APInt ssa_47_shl;
	if (ssa_4_sh_o.uge(ssa_4_sh_o.getBitWidth())) {
		ssa_47_shl = APInt(ssa_46_y.getBitWidth(), 0);
	} else {
		ssa_47_shl = ssa_46_y.shl(ssa_4_sh_o);
	}
	bool ssa_48_exact = ssa_47_shl.eq(ssa_45_x);
	APInt ssa_49_bw(ssa_45_x.getBitWidth(), ssa_45_x.getBitWidth());
	bool ssa_50_shift_lt = ssa_4_sh_o.ult(ssa_49_bw);
	bool ssa_51_constraint_ok = ssa_48_exact && ssa_50_shift_lt;
	bool ssa_52_shift_ge_bw = ssa_50_shift_lt != ssa_11_true;
	bool ssa_53_impossible_shift = ssa_29_shift_const && ssa_52_shift_ge_bw;
	bool ssa_54_impossible = ssa_44_impossible_low || ssa_53_impossible_shift;
	bool ssa_55_not_impossible = ssa_54_impossible != ssa_11_true;
	APInt ssa_56_exact_z = ssa_46_y ^ ssa_9_ones;
	APInt ssa_57_res_z_const = ssa_51_constraint_ok ? ssa_56_exact_z : ssa_9_ones;
	APInt ssa_58_res_o_const = ssa_51_constraint_ok ? ssa_46_y : ssa_9_ones;
	APInt ssa_59_res_z_nb = ssa_28_const_and_valid ? ssa_57_res_z_const : ssa_10_zero;
	APInt ssa_60_res_o_nb = ssa_28_const_and_valid ? ssa_58_res_o_const : ssa_10_zero;
	bool ssa_61_cond_ok = ssa_25_all_nb2 && ssa_55_not_impossible;
	APInt ssa_62_res_z = ssa_61_cond_ok ? ssa_59_res_z_nb : ssa_9_ones;
	APInt ssa_63_res_o = ssa_61_cond_ok ? ssa_60_res_o_nb : ssa_9_ones;
	std::array<APInt, 2> ssa_64_r = std::array<APInt, 2>{ssa_62_res_z, ssa_63_res_o};
	return ssa_64_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_sh_z = ssa_0_arg0[0];
	APInt ssa_4_sh_o = ssa_0_arg0[1];
	APInt ssa_5_a1_z = ssa_1_arg1[0];
	APInt ssa_6_a1_o = ssa_1_arg1[1];
	APInt ssa_7_a2_z = ssa_2_arg2[0];
	APInt ssa_8_a2_o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_sh_z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_sh_z.getBitWidth(), 1);
	APInt ssa_11_ones = APInt::getAllOnes(ssa_3_sh_z.getBitWidth());
	bool ssa_12_true = ssa_9_zero.eq(ssa_9_zero);
	APInt ssa_13_sh_known = ssa_3_sh_z | ssa_4_sh_o;
	bool ssa_14_sh_full = ssa_13_sh_known.eq(ssa_11_ones);
	APInt ssa_15_sh_conf = ssa_3_sh_z & ssa_4_sh_o;
	bool ssa_16_sh_nb = ssa_15_sh_conf.eq(ssa_9_zero);
	bool ssa_17_shift_const = ssa_14_sh_full && ssa_16_sh_nb;
	APInt ssa_18_eq_oo = ssa_6_a1_o & ssa_8_a2_o;
	APInt ssa_19_eq_zz = ssa_5_a1_z & ssa_7_a2_z;
	APInt ssa_20_eq_known = ssa_18_eq_oo | ssa_19_eq_zz;
	APInt ssa_21_t(ssa_20_eq_known.getBitWidth(), ssa_20_eq_known.countr_one());
	bool ssa_22_t_ge_k = ssa_21_t.uge(ssa_4_sh_o);
	APInt ssa_23_t_minus_k = ssa_21_t - ssa_4_sh_o;
	APInt ssa_24_low_count = ssa_22_t_ge_k ? ssa_23_t_minus_k : ssa_9_zero;
	APInt ssa_25_low_count2 = ssa_17_shift_const ? ssa_24_low_count : ssa_9_zero;
	APInt ssa_26_mask_low = ssa_9_zero;
	if (ssa_25_low_count2.ule(ssa_25_low_count2.getBitWidth()))
		ssa_26_mask_low.setLowBits(ssa_25_low_count2.getZExtValue());
	else
		ssa_26_mask_low.setLowBits(ssa_25_low_count2.getBitWidth());
	APInt ssa_27_t_plus1 = ssa_21_t + ssa_10_one;
	APInt ssa_28_mask_t1 = ssa_9_zero;
	if (ssa_27_t_plus1.ule(ssa_27_t_plus1.getBitWidth()))
		ssa_28_mask_t1.setLowBits(ssa_27_t_plus1.getZExtValue());
	else
		ssa_28_mask_t1.setLowBits(ssa_27_t_plus1.getBitWidth());
	APInt ssa_29_mask_t0 = ssa_9_zero;
	if (ssa_21_t.ule(ssa_21_t.getBitWidth()))
		ssa_29_mask_t0.setLowBits(ssa_21_t.getZExtValue());
	else
		ssa_29_mask_t0.setLowBits(ssa_21_t.getBitWidth());
	APInt ssa_30_bit_t = ssa_28_mask_t1 ^ ssa_29_mask_t0;
	APInt ssa_31_d1 = ssa_6_a1_o & ssa_7_a2_z;
	APInt ssa_32_d2 = ssa_5_a1_z & ssa_8_a2_o;
	APInt ssa_33_known_diff = ssa_31_d1 | ssa_32_d2;
	APInt ssa_34_diff_at_t = ssa_33_known_diff & ssa_30_bit_t;
	bool ssa_35_diff_at_t_zero = ssa_34_diff_at_t.eq(ssa_9_zero);
	bool ssa_36_diff_at_t_nz = ssa_35_diff_at_t_zero != ssa_12_true;
	bool ssa_37_t_lt_k = ssa_21_t.ult(ssa_4_sh_o);
	bool ssa_38_tmp1 = ssa_17_shift_const && ssa_37_t_lt_k;
	bool ssa_39_impossible = ssa_38_tmp1 && ssa_36_diff_at_t_nz;
	APInt ssa_40_mask_high = ssa_11_ones;
	if (ssa_4_sh_o.ule(ssa_4_sh_o.getBitWidth()))
		ssa_40_mask_high.clearLowBits(ssa_4_sh_o.getZExtValue());
	else
		ssa_40_mask_high.clearLowBits(ssa_4_sh_o.getBitWidth());
	APInt ssa_41_eq_high = ssa_20_eq_known & ssa_40_mask_high;
	bool ssa_42_eq_high_full = ssa_41_eq_high.eq(ssa_40_mask_high);
	bool ssa_43_cond_zero = ssa_17_shift_const && ssa_42_eq_high_full;
	APInt ssa_44_res_z0 = ssa_43_cond_zero ? ssa_11_ones : ssa_26_mask_low;
	APInt ssa_45_res_z = ssa_39_impossible ? ssa_11_ones : ssa_44_res_z0;
	APInt ssa_46_res_o = ssa_39_impossible ? ssa_11_ones : ssa_9_zero;
	std::array<APInt, 2> ssa_47_r = std::array<APInt, 2>{ssa_45_res_z, ssa_46_res_o};
	return ssa_47_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_sh_z = ssa_0_arg0[0];
	APInt ssa_4_sh_o = ssa_0_arg0[1];
	APInt ssa_5_a1_z = ssa_1_arg1[0];
	APInt ssa_6_a1_o = ssa_1_arg1[1];
	APInt ssa_7_a2_z = ssa_2_arg2[0];
	APInt ssa_8_a2_o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_sh_z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_sh_z.getBitWidth(), 1);
	APInt ssa_11_ones = APInt::getAllOnes(ssa_3_sh_z.getBitWidth());
	bool ssa_12_true = ssa_9_zero.eq(ssa_9_zero);
	APInt ssa_13_bw(ssa_3_sh_z.getBitWidth(), ssa_3_sh_z.getBitWidth());
	APInt ssa_14_bw_minus1 = ssa_13_bw - ssa_10_one;
	APInt ssa_15_min_sh = ssa_4_sh_o | ssa_9_zero;
	APInt ssa_16_max_sh0 = ssa_11_ones ^ ssa_3_sh_z;
	APInt ssa_17_max_sh = APIntOps::umin(ssa_16_max_sh0, ssa_14_bw_minus1);
	bool ssa_18_min_sh_lt_bw = ssa_15_min_sh.ult(ssa_13_bw);
	bool ssa_19_min_sh_ge_bw = ssa_18_min_sh_lt_bw != ssa_12_true;
	APInt ssa_20_eq_oo = ssa_6_a1_o & ssa_8_a2_o;
	APInt ssa_21_eq_zz = ssa_5_a1_z & ssa_7_a2_z;
	APInt ssa_22_eq_known = ssa_20_eq_oo | ssa_21_eq_zz;
	APInt ssa_23_diff1 = ssa_6_a1_o & ssa_7_a2_z;
	APInt ssa_24_diff2 = ssa_5_a1_z & ssa_8_a2_o;
	APInt ssa_25_known_diff = ssa_23_diff1 | ssa_24_diff2;
	APInt ssa_26_mask_low_min = ssa_9_zero;
	if (ssa_15_min_sh.ule(ssa_15_min_sh.getBitWidth()))
		ssa_26_mask_low_min.setLowBits(ssa_15_min_sh.getZExtValue());
	else
		ssa_26_mask_low_min.setLowBits(ssa_15_min_sh.getBitWidth());
	APInt ssa_27_diff_low = ssa_25_known_diff & ssa_26_mask_low_min;
	bool ssa_28_diff_low_zero = ssa_27_diff_low.eq(ssa_9_zero);
	bool ssa_29_diff_low_nz = ssa_28_diff_low_zero != ssa_12_true;
	bool ssa_30_impossible = ssa_29_diff_low_nz || ssa_19_min_sh_ge_bw;
	APInt ssa_31_mask_high_min = ssa_11_ones;
	if (ssa_15_min_sh.ule(ssa_15_min_sh.getBitWidth()))
		ssa_31_mask_high_min.clearLowBits(ssa_15_min_sh.getZExtValue());
	else
		ssa_31_mask_high_min.clearLowBits(ssa_15_min_sh.getBitWidth());
	APInt ssa_32_eq_high = ssa_22_eq_known & ssa_31_mask_high_min;
	bool ssa_33_eq_high_full = ssa_32_eq_high.eq(ssa_31_mask_high_min);
	bool ssa_34_cond_zero = ssa_33_eq_high_full && ssa_18_min_sh_lt_bw;
	APInt ssa_35_t(ssa_22_eq_known.getBitWidth(), ssa_22_eq_known.countr_one());
	bool ssa_36_t_ge = ssa_35_t.uge(ssa_17_max_sh);
	APInt ssa_37_t_minus = ssa_35_t - ssa_17_max_sh;
	APInt ssa_38_low_count = ssa_36_t_ge ? ssa_37_t_minus : ssa_9_zero;
	APInt ssa_39_mask_low_zero = ssa_9_zero;
	if (ssa_38_low_count.ule(ssa_38_low_count.getBitWidth()))
		ssa_39_mask_low_zero.setLowBits(ssa_38_low_count.getZExtValue());
	else
		ssa_39_mask_low_zero.setLowBits(ssa_38_low_count.getBitWidth());
	bool ssa_40_min_eq_bw1 = ssa_15_min_sh.eq(ssa_14_bw_minus1);
	bool ssa_41_max_eq_bw1 = ssa_17_max_sh.eq(ssa_14_bw_minus1);
	bool ssa_42_shift_bw1 = ssa_40_min_eq_bw1 && ssa_41_max_eq_bw1;
	APInt ssa_43_sign_mask = ssa_9_zero;
	if (ssa_10_one.ule(ssa_10_one.getBitWidth()))
		ssa_43_sign_mask.setHighBits(ssa_10_one.getZExtValue());
	else
		ssa_43_sign_mask.setHighBits(ssa_10_one.getBitWidth());
	APInt ssa_44_diff_sign = ssa_25_known_diff & ssa_43_sign_mask;
	bool ssa_45_diff_sign_zero = ssa_44_diff_sign.eq(ssa_9_zero);
	bool ssa_46_diff_sign_nz = ssa_45_diff_sign_zero != ssa_12_true;
	bool ssa_47_cond_all_ones0 = ssa_42_shift_bw1 && ssa_28_diff_low_zero;
	bool ssa_48_cond_all_ones = ssa_47_cond_all_ones0 && ssa_46_diff_sign_nz;
	APInt ssa_49_lz1(ssa_5_a1_z.getBitWidth(), ssa_5_a1_z.countl_one());
	APInt ssa_50_lz2(ssa_7_a2_z.getBitWidth(), ssa_7_a2_z.countl_one());
	APInt ssa_51_lz = APIntOps::umin(ssa_49_lz1, ssa_50_lz2);
	bool ssa_52_lz_ge_one = ssa_51_lz.uge(ssa_10_one);
	APInt ssa_53_max_a1 = ssa_11_ones ^ ssa_5_a1_z;
	APInt ssa_54_min_a1 = ssa_6_a1_o | ssa_9_zero;
	APInt ssa_55_min_a2 = ssa_8_a2_o | ssa_9_zero;
	APInt ssa_56_max_a2 = ssa_11_ones ^ ssa_7_a2_z;
	bool ssa_57_a2_ge_a1 = ssa_55_min_a2.uge(ssa_53_max_a1);
	bool ssa_58_cond_nonneg0 = ssa_52_lz_ge_one && ssa_57_a2_ge_a1;
	bool ssa_59_cond_nonneg = ssa_58_cond_nonneg0 && ssa_18_min_sh_lt_bw;
	APInt ssa_60_high_zero_count = ssa_51_lz + ssa_15_min_sh;
	APInt ssa_61_mask_high_zero = ssa_9_zero;
	if (ssa_60_high_zero_count.ule(ssa_60_high_zero_count.getBitWidth()))
		ssa_61_mask_high_zero.setHighBits(ssa_60_high_zero_count.getZExtValue());
	else
		ssa_61_mask_high_zero.setHighBits(ssa_60_high_zero_count.getBitWidth());
	APInt ssa_62_mask_high_sel = ssa_59_cond_nonneg ? ssa_61_mask_high_zero : ssa_9_zero;
	APInt ssa_63_mask_zero_combined = ssa_39_mask_low_zero | ssa_62_mask_high_sel;
	APInt ssa_64_a1_sign1 = ssa_6_a1_o & ssa_43_sign_mask;
	APInt ssa_65_a2_sign1 = ssa_8_a2_o & ssa_43_sign_mask;
	bool ssa_66_a1_sign1_full = ssa_64_a1_sign1.eq(ssa_43_sign_mask);
	bool ssa_67_a2_sign1_full = ssa_65_a2_sign1.eq(ssa_43_sign_mask);
	bool ssa_68_sign_one = ssa_66_a1_sign1_full && ssa_67_a2_sign1_full;
	bool ssa_69_sign_equal = ssa_52_lz_ge_one || ssa_68_sign_one;
	bool ssa_70_a2_lt_a1 = ssa_56_max_a2.ult(ssa_54_min_a1);
	bool ssa_71_cond_neg0 = ssa_69_sign_equal && ssa_70_a2_lt_a1;
	bool ssa_72_cond_neg1 = ssa_71_cond_neg0 && ssa_18_min_sh_lt_bw;
	bool ssa_73_not_zero = ssa_34_cond_zero != ssa_12_true;
	bool ssa_74_cond_neg = ssa_72_cond_neg1 && ssa_73_not_zero;
	APInt ssa_75_mask_high_one = ssa_9_zero;
	if (ssa_15_min_sh.ule(ssa_15_min_sh.getBitWidth()))
		ssa_75_mask_high_one.setHighBits(ssa_15_min_sh.getZExtValue());
	else
		ssa_75_mask_high_one.setHighBits(ssa_15_min_sh.getBitWidth());
	APInt ssa_76_mask_one_sel = ssa_74_cond_neg ? ssa_75_mask_high_one : ssa_9_zero;
	APInt ssa_77_res_z0 = ssa_34_cond_zero ? ssa_11_ones : ssa_63_mask_zero_combined;
	APInt ssa_78_res_z1 = ssa_48_cond_all_ones ? ssa_9_zero : ssa_77_res_z0;
	APInt ssa_79_res_o0 = ssa_48_cond_all_ones ? ssa_11_ones : ssa_76_mask_one_sel;
	APInt ssa_80_res_z = ssa_30_impossible ? ssa_11_ones : ssa_78_res_z1;
	APInt ssa_81_res_o = ssa_30_impossible ? ssa_11_ones : ssa_79_res_o0;
	std::array<APInt, 2> ssa_82_r = std::array<APInt, 2>{ssa_80_res_z, ssa_81_res_o};
	return ssa_82_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_sh_z = ssa_0_arg0[0];
	APInt ssa_4_sh_o = ssa_0_arg0[1];
	APInt ssa_5_a1_z = ssa_1_arg1[0];
	APInt ssa_6_a1_o = ssa_1_arg1[1];
	APInt ssa_7_a2_z = ssa_2_arg2[0];
	APInt ssa_8_a2_o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_sh_z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_sh_z.getBitWidth(), 1);
	APInt ssa_11_ones = APInt::getAllOnes(ssa_3_sh_z.getBitWidth());
	bool ssa_12_true = ssa_9_zero.eq(ssa_9_zero);
	APInt ssa_13_sh_known = ssa_3_sh_z | ssa_4_sh_o;
	bool ssa_14_sh_full = ssa_13_sh_known.eq(ssa_11_ones);
	APInt ssa_15_sh_conf = ssa_3_sh_z & ssa_4_sh_o;
	bool ssa_16_sh_nb = ssa_15_sh_conf.eq(ssa_9_zero);
	bool ssa_17_shift_const = ssa_14_sh_full && ssa_16_sh_nb;
	APInt ssa_18_mask_low = ssa_9_zero;
	if (ssa_4_sh_o.ule(ssa_4_sh_o.getBitWidth()))
		ssa_18_mask_low.setLowBits(ssa_4_sh_o.getZExtValue());
	else
		ssa_18_mask_low.setLowBits(ssa_4_sh_o.getBitWidth());
	APInt ssa_19_diff1 = ssa_6_a1_o & ssa_7_a2_z;
	APInt ssa_20_diff2 = ssa_5_a1_z & ssa_8_a2_o;
	APInt ssa_21_known_diff = ssa_19_diff1 | ssa_20_diff2;
	APInt ssa_22_diff_low = ssa_21_known_diff & ssa_18_mask_low;
	bool ssa_23_diff_low_zero = ssa_22_diff_low.eq(ssa_9_zero);
	bool ssa_24_diff_low_nz = ssa_23_diff_low_zero != ssa_12_true;
	APInt ssa_25_bw(ssa_3_sh_z.getBitWidth(), ssa_3_sh_z.getBitWidth());
	bool ssa_26_shift_lt = ssa_4_sh_o.ult(ssa_25_bw);
	bool ssa_27_shift_ge = ssa_26_shift_lt != ssa_12_true;
	bool ssa_28_impossible0 = ssa_17_shift_const && ssa_27_shift_ge;
	bool ssa_29_impossible1 = ssa_17_shift_const && ssa_24_diff_low_nz;
	bool ssa_30_impossible = ssa_28_impossible0 || ssa_29_impossible1;
	bool ssa_31_not_impossible = ssa_30_impossible != ssa_12_true;
	APInt ssa_32_a2_low_one = ssa_8_a2_o & ssa_18_mask_low;
	APInt ssa_33_a2_low_zero = ssa_7_a2_z & ssa_18_mask_low;
	APInt ssa_34_a1_low_one = ssa_6_a1_o & ssa_18_mask_low;
	APInt ssa_35_a1_low_zero = ssa_5_a1_z & ssa_18_mask_low;
	APInt ssa_36_min_a1 = ssa_6_a1_o | ssa_32_a2_low_one;
	APInt ssa_37_max_a1_base = ssa_11_ones ^ ssa_5_a1_z;
	APInt ssa_38_mask_force0_a1 = ssa_11_ones ^ ssa_33_a2_low_zero;
	APInt ssa_39_max_a1 = ssa_37_max_a1_base & ssa_38_mask_force0_a1;
	APInt ssa_40_min_a2 = ssa_8_a2_o | ssa_34_a1_low_one;
	APInt ssa_41_max_a2_base = ssa_11_ones ^ ssa_7_a2_z;
	APInt ssa_42_mask_force0_a2 = ssa_11_ones ^ ssa_35_a1_low_zero;
	APInt ssa_43_max_a2 = ssa_41_max_a2_base & ssa_42_mask_force0_a2;
	APInt ssa_44_sign_mask = ssa_9_zero;
	if (ssa_10_one.ule(ssa_10_one.getBitWidth()))
		ssa_44_sign_mask.setHighBits(ssa_10_one.getZExtValue());
	else
		ssa_44_sign_mask.setHighBits(ssa_10_one.getBitWidth());
	APInt ssa_45_a1_sign = ssa_5_a1_z & ssa_44_sign_mask;
	APInt ssa_46_a2_sign = ssa_7_a2_z & ssa_44_sign_mask;
	bool ssa_47_a1_sign_zero = ssa_45_a1_sign.eq(ssa_44_sign_mask);
	bool ssa_48_a2_sign_zero = ssa_46_a2_sign.eq(ssa_44_sign_mask);
	bool ssa_49_sign_zero = ssa_47_a1_sign_zero && ssa_48_a2_sign_zero;
	bool ssa_50_a2_ge_a1 = ssa_40_min_a2.uge(ssa_39_max_a1);
	APInt ssa_51_min_hi2;
	if (ssa_4_sh_o.uge(ssa_4_sh_o.getBitWidth())) {
		ssa_51_min_hi2 = APInt(ssa_40_min_a2.getBitWidth(), 0);
	} else {
		ssa_51_min_hi2 = ssa_40_min_a2.lshr(ssa_4_sh_o);
	}
	APInt ssa_52_max_hi1;
	if (ssa_4_sh_o.uge(ssa_4_sh_o.getBitWidth())) {
		ssa_52_max_hi1 = APInt(ssa_39_max_a1.getBitWidth(), 0);
	} else {
		ssa_52_max_hi1 = ssa_39_max_a1.lshr(ssa_4_sh_o);
	}
	bool ssa_53_hi_ge = ssa_51_min_hi2.uge(ssa_52_max_hi1);
	bool ssa_54_ge0 = ssa_50_a2_ge_a1 || ssa_53_hi_ge;
	bool ssa_55_cond0 = ssa_49_sign_zero && ssa_54_ge0;
	bool ssa_56_cond1 = ssa_55_cond0 && ssa_17_shift_const;
	bool ssa_57_cond2 = ssa_56_cond1 && ssa_26_shift_lt;
	bool ssa_58_cond3 = ssa_57_cond2 && ssa_31_not_impossible;
	APInt ssa_59_min_hi1;
	if (ssa_4_sh_o.uge(ssa_4_sh_o.getBitWidth())) {
		ssa_59_min_hi1 = APInt(ssa_36_min_a1.getBitWidth(), 0);
	} else {
		ssa_59_min_hi1 = ssa_36_min_a1.lshr(ssa_4_sh_o);
	}
	APInt ssa_60_max_hi2;
	if (ssa_4_sh_o.uge(ssa_4_sh_o.getBitWidth())) {
		ssa_60_max_hi2 = APInt(ssa_43_max_a2.getBitWidth(), 0);
	} else {
		ssa_60_max_hi2 = ssa_43_max_a2.lshr(ssa_4_sh_o);
	}
	APInt ssa_61_max_res = ssa_60_max_hi2 - ssa_59_min_hi1;
	APInt ssa_62_lz(ssa_61_max_res.getBitWidth(), ssa_61_max_res.countl_zero());
	APInt ssa_63_mask_high_zero = ssa_9_zero;
	if (ssa_62_lz.ule(ssa_62_lz.getBitWidth()))
		ssa_63_mask_high_zero.setHighBits(ssa_62_lz.getZExtValue());
	else
		ssa_63_mask_high_zero.setHighBits(ssa_62_lz.getBitWidth());
	APInt ssa_64_res_z0 = ssa_58_cond3 ? ssa_63_mask_high_zero : ssa_9_zero;
	APInt ssa_65_res_o0 = ssa_9_zero | ssa_9_zero;
	APInt ssa_66_res_z = ssa_30_impossible ? ssa_11_ones : ssa_64_res_z0;
	APInt ssa_67_res_o = ssa_30_impossible ? ssa_11_ones : ssa_65_res_o0;
	std::array<APInt, 2> ssa_68_r = std::array<APInt, 2>{ssa_66_res_z, ssa_67_res_o};
	return ssa_68_r;
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
