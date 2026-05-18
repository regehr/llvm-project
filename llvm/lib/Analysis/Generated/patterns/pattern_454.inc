namespace Pattern454 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0_kz = ssa_0_arg0[0];
	APInt ssa_4_arg0_ko = ssa_0_arg0[1];
	APInt ssa_5_arg1_kz = ssa_1_arg1[0];
	APInt ssa_6_arg1_ko = ssa_1_arg1[1];
	APInt ssa_7_arg2_kz = ssa_2_arg2[0];
	APInt ssa_8_arg2_ko = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_arg0_kz.getBitWidth(), 0);
	APInt ssa_10_ones = APInt::getAllOnes(ssa_3_arg0_kz.getBitWidth());
	APInt ssa_11_bw(ssa_3_arg0_kz.getBitWidth(), ssa_3_arg0_kz.getBitWidth());
	APInt ssa_12_shift_bits(ssa_11_bw.getBitWidth(), ssa_11_bw.countr_zero());
	APInt ssa_13_high_count = ssa_11_bw - ssa_12_shift_bits;
	APInt ssa_14_constraint_highmask = ssa_9_zero;
	if (ssa_13_high_count.ule(ssa_13_high_count.getBitWidth()))
		ssa_14_constraint_highmask.setHighBits(ssa_13_high_count.getZExtValue());
	else
		ssa_14_constraint_highmask.setHighBits(ssa_13_high_count.getBitWidth());
	APInt ssa_15_arg0_kz_c = ssa_3_arg0_kz | ssa_14_constraint_highmask;
	APInt ssa_16_arg2_kz_c = ssa_7_arg2_kz | ssa_14_constraint_highmask;
	APInt ssa_17_arg0_kz_or_ko = ssa_15_arg0_kz_c | ssa_4_arg0_ko;
	bool ssa_18_arg0_all_known = ssa_17_arg0_kz_or_ko.eq(ssa_10_ones);
	APInt ssa_19_arg0_overlap = ssa_15_arg0_kz_c & ssa_4_arg0_ko;
	bool ssa_20_arg0_no_overlap = ssa_19_arg0_overlap.eq(ssa_9_zero);
	bool ssa_21_arg0_exact = ssa_18_arg0_all_known && ssa_20_arg0_no_overlap;
	APInt ssa_22_arg2_kz_or_ko = ssa_16_arg2_kz_c | ssa_8_arg2_ko;
	bool ssa_23_arg2_all_known = ssa_22_arg2_kz_or_ko.eq(ssa_10_ones);
	APInt ssa_24_arg2_overlap = ssa_16_arg2_kz_c & ssa_8_arg2_ko;
	bool ssa_25_arg2_no_overlap = ssa_24_arg2_overlap.eq(ssa_9_zero);
	bool ssa_26_arg2_exact = ssa_23_arg2_all_known && ssa_25_arg2_no_overlap;
	bool ssa_27_arg0_lb_ge_bw = ssa_4_arg0_ko.uge(ssa_11_bw);
	bool ssa_28_arg2_lb_ge_bw = ssa_8_arg2_ko.uge(ssa_11_bw);
	bool ssa_29_constraint_unsat = ssa_27_arg0_lb_ge_bw || ssa_28_arg2_lb_ge_bw;
	bool ssa_30_arg1_kz_all = ssa_5_arg1_kz.eq(ssa_10_ones);
	bool ssa_31_arg1_ko_zero = ssa_6_arg1_ko.eq(ssa_9_zero);
	bool ssa_32_arg1_zero = ssa_30_arg1_kz_all && ssa_31_arg1_ko_zero;
	bool ssa_33_arg2_ko_zero = ssa_8_arg2_ko.eq(ssa_9_zero);
	bool ssa_34_arg2_zero = ssa_26_arg2_exact && ssa_33_arg2_ko_zero;
	bool ssa_35_arg0_arg2_zero = ssa_21_arg0_exact && ssa_34_arg2_zero;
	APInt ssa_36_arg1_max = ssa_10_ones ^ ssa_5_arg1_kz;
	APInt ssa_37_arg2_max = ssa_10_ones ^ ssa_16_arg2_kz_c;
	APInt ssa_38_sum_max = ssa_36_arg1_max + ssa_37_arg2_max;
	bool ssa_39_sum_ov;
	ssa_36_arg1_max.uadd_ov(ssa_37_arg2_max, ssa_39_sum_ov);
	APInt ssa_40_sum_lz(ssa_38_sum_max.getBitWidth(), ssa_38_sum_max.countl_zero());
	APInt ssa_41_sum_kz_raw = ssa_9_zero;
	if (ssa_40_sum_lz.ule(ssa_40_sum_lz.getBitWidth()))
		ssa_41_sum_kz_raw.setHighBits(ssa_40_sum_lz.getZExtValue());
	else
		ssa_41_sum_kz_raw.setHighBits(ssa_40_sum_lz.getBitWidth());
	APInt ssa_42_sum_kz = ssa_39_sum_ov ? ssa_9_zero : ssa_41_sum_kz_raw;
	APInt ssa_43_sum_kz_lshr;
	if (ssa_8_arg2_ko.uge(ssa_8_arg2_ko.getBitWidth())) {
		ssa_43_sum_kz_lshr = APInt(ssa_42_sum_kz.getBitWidth(), 0);
	} else {
		ssa_43_sum_kz_lshr = ssa_42_sum_kz.lshr(ssa_8_arg2_ko);
	}
	APInt ssa_44_lshr_highmask = ssa_9_zero;
	if (ssa_8_arg2_ko.ule(ssa_8_arg2_ko.getBitWidth()))
		ssa_44_lshr_highmask.setHighBits(ssa_8_arg2_ko.getZExtValue());
	else
		ssa_44_lshr_highmask.setHighBits(ssa_8_arg2_ko.getBitWidth());
	APInt ssa_45_lshr_kz_exact = ssa_43_sum_kz_lshr | ssa_44_lshr_highmask;
	APInt ssa_46_lshr_kz = ssa_26_arg2_exact ? ssa_45_lshr_kz_exact : ssa_44_lshr_highmask;
	bool ssa_47_lshr_zero_kz = ssa_46_lshr_kz.eq(ssa_10_ones);
	bool ssa_48_lshr_zero = ssa_47_lshr_zero_kz || ssa_32_arg1_zero;
	APInt ssa_49_lshr_kz_shl;
	if (ssa_4_arg0_ko.uge(ssa_4_arg0_ko.getBitWidth())) {
		ssa_49_lshr_kz_shl = APInt(ssa_46_lshr_kz.getBitWidth(), 0);
	} else {
		ssa_49_lshr_kz_shl = ssa_46_lshr_kz.shl(ssa_4_arg0_ko);
	}
	APInt ssa_50_lowmask = ssa_9_zero;
	if (ssa_4_arg0_ko.ule(ssa_4_arg0_ko.getBitWidth()))
		ssa_50_lowmask.setLowBits(ssa_4_arg0_ko.getZExtValue());
	else
		ssa_50_lowmask.setLowBits(ssa_4_arg0_ko.getBitWidth());
	APInt ssa_51_res_kz_exact = ssa_49_lshr_kz_shl | ssa_50_lowmask;
	APInt ssa_52_res_kz = ssa_21_arg0_exact ? ssa_51_res_kz_exact : ssa_50_lowmask;
	APInt ssa_53_arg1_kz_shl;
	if (ssa_4_arg0_ko.uge(ssa_4_arg0_ko.getBitWidth())) {
		ssa_53_arg1_kz_shl = APInt(ssa_5_arg1_kz.getBitWidth(), 0);
	} else {
		ssa_53_arg1_kz_shl = ssa_5_arg1_kz.shl(ssa_4_arg0_ko);
	}
	APInt ssa_54_arg1_ko_shl;
	if (ssa_4_arg0_ko.uge(ssa_4_arg0_ko.getBitWidth())) {
		ssa_54_arg1_ko_shl = APInt(ssa_6_arg1_ko.getBitWidth(), 0);
	} else {
		ssa_54_arg1_ko_shl = ssa_6_arg1_ko.shl(ssa_4_arg0_ko);
	}
	APInt ssa_55_arg1_kz_shl_or = ssa_53_arg1_kz_shl | ssa_50_lowmask;
	APInt ssa_56_res_kz_arg1 = ssa_35_arg0_arg2_zero ? ssa_55_arg1_kz_shl_or : ssa_52_res_kz;
	APInt ssa_57_res_ko_arg1 = ssa_35_arg0_arg2_zero ? ssa_54_arg1_ko_shl : ssa_9_zero;
	APInt ssa_58_lshr_max;
	if (ssa_8_arg2_ko.uge(ssa_8_arg2_ko.getBitWidth())) {
		ssa_58_lshr_max = APInt(ssa_38_sum_max.getBitWidth(), 0);
	} else {
		ssa_58_lshr_max = ssa_38_sum_max.lshr(ssa_8_arg2_ko);
	}
	APInt ssa_59_shl_max;
	if (ssa_4_arg0_ko.uge(ssa_4_arg0_ko.getBitWidth())) {
		ssa_59_shl_max = APInt(ssa_58_lshr_max.getBitWidth(), 0);
	} else {
		ssa_59_shl_max = ssa_58_lshr_max.shl(ssa_4_arg0_ko);
	}
	bool ssa_60_shl_ov;
	ssa_58_lshr_max.ushl_ov(ssa_4_arg0_ko, ssa_60_shl_ov);
	APInt ssa_61_bound_lz(ssa_59_shl_max.getBitWidth(), ssa_59_shl_max.countl_zero());
	APInt ssa_62_bound_kz_raw = ssa_9_zero;
	if (ssa_61_bound_lz.ule(ssa_61_bound_lz.getBitWidth()))
		ssa_62_bound_kz_raw.setHighBits(ssa_61_bound_lz.getZExtValue());
	else
		ssa_62_bound_kz_raw.setHighBits(ssa_61_bound_lz.getBitWidth());
	APInt ssa_63_bound_kz_ov = ssa_39_sum_ov ? ssa_9_zero : ssa_62_bound_kz_raw;
	APInt ssa_64_bound_kz_shl = ssa_60_shl_ov ? ssa_9_zero : ssa_63_bound_kz_ov;
	APInt ssa_65_bound_kz = ssa_21_arg0_exact ? ssa_64_bound_kz_shl : ssa_9_zero;
	APInt ssa_66_res_kz_bound = ssa_56_res_kz_arg1 | ssa_65_bound_kz;
	APInt ssa_67_res_kz_zero = ssa_48_lshr_zero ? ssa_10_ones : ssa_66_res_kz_bound;
	APInt ssa_68_res_ko_zero = ssa_48_lshr_zero ? ssa_9_zero : ssa_57_res_ko_arg1;
	APInt ssa_69_out_kz = ssa_29_constraint_unsat ? ssa_10_ones : ssa_67_res_kz_zero;
	APInt ssa_70_out_ko = ssa_29_constraint_unsat ? ssa_10_ones : ssa_68_res_ko_zero;
	std::array<APInt, 2> ssa_71_r = std::array<APInt, 2>{ssa_69_out_kz, ssa_70_out_ko};
	return ssa_71_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0_kz = ssa_0_arg0[0];
	APInt ssa_4_arg0_ko = ssa_0_arg0[1];
	APInt ssa_5_arg1_kz = ssa_1_arg1[0];
	APInt ssa_6_arg1_ko = ssa_1_arg1[1];
	APInt ssa_7_arg2_kz = ssa_2_arg2[0];
	APInt ssa_8_arg2_ko = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_arg0_kz.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_arg0_kz.getBitWidth(), 1);
	APInt ssa_11_c2(ssa_3_arg0_kz.getBitWidth(), 2);
	APInt ssa_12_c3(ssa_3_arg0_kz.getBitWidth(), 3);
	APInt ssa_13_c4(ssa_3_arg0_kz.getBitWidth(), 4);
	APInt ssa_14_ones = APInt::getAllOnes(ssa_3_arg0_kz.getBitWidth());
	APInt ssa_15_bw(ssa_3_arg0_kz.getBitWidth(), ssa_3_arg0_kz.getBitWidth());
	APInt ssa_16_shift_bits(ssa_15_bw.getBitWidth(), ssa_15_bw.countr_zero());
	APInt ssa_17_high_count = ssa_15_bw - ssa_16_shift_bits;
	APInt ssa_18_constraint_highmask = ssa_9_zero;
	if (ssa_17_high_count.ule(ssa_17_high_count.getBitWidth()))
		ssa_18_constraint_highmask.setHighBits(ssa_17_high_count.getZExtValue());
	else
		ssa_18_constraint_highmask.setHighBits(ssa_17_high_count.getBitWidth());
	APInt ssa_19_arg0_kz_c = ssa_3_arg0_kz | ssa_18_constraint_highmask;
	APInt ssa_20_arg2_kz_c = ssa_7_arg2_kz | ssa_18_constraint_highmask;
	APInt ssa_21_max_c = ssa_14_ones ^ ssa_19_arg0_kz_c;
	APInt ssa_22_max_a = ssa_14_ones ^ ssa_5_arg1_kz;
	APInt ssa_23_arg0_kz_or_ko = ssa_19_arg0_kz_c | ssa_4_arg0_ko;
	bool ssa_24_arg0_all_known = ssa_23_arg0_kz_or_ko.eq(ssa_14_ones);
	APInt ssa_25_arg0_overlap = ssa_19_arg0_kz_c & ssa_4_arg0_ko;
	bool ssa_26_arg0_no_overlap = ssa_25_arg0_overlap.eq(ssa_9_zero);
	bool ssa_27_arg0_exact = ssa_24_arg0_all_known && ssa_26_arg0_no_overlap;
	APInt ssa_28_arg1_kz_or_ko = ssa_5_arg1_kz | ssa_6_arg1_ko;
	bool ssa_29_arg1_all_known = ssa_28_arg1_kz_or_ko.eq(ssa_14_ones);
	APInt ssa_30_arg1_overlap = ssa_5_arg1_kz & ssa_6_arg1_ko;
	bool ssa_31_arg1_no_overlap = ssa_30_arg1_overlap.eq(ssa_9_zero);
	bool ssa_32_arg1_exact = ssa_29_arg1_all_known && ssa_31_arg1_no_overlap;
	APInt ssa_33_arg2_kz_or_ko = ssa_20_arg2_kz_c | ssa_8_arg2_ko;
	bool ssa_34_arg2_all_known = ssa_33_arg2_kz_or_ko.eq(ssa_14_ones);
	APInt ssa_35_arg2_overlap = ssa_20_arg2_kz_c & ssa_8_arg2_ko;
	bool ssa_36_arg2_no_overlap = ssa_35_arg2_overlap.eq(ssa_9_zero);
	bool ssa_37_arg2_exact = ssa_34_arg2_all_known && ssa_36_arg2_no_overlap;
	APInt ssa_38_sum_max = ssa_22_max_a + ssa_8_arg2_ko;
	bool ssa_39_sum_ov;
	ssa_22_max_a.uadd_ov(ssa_8_arg2_ko, ssa_39_sum_ov);
	APInt ssa_40_sum_mod = ssa_39_sum_ov ? ssa_14_ones : ssa_38_sum_max;
	APInt ssa_41_u_max_b;
	if (ssa_8_arg2_ko.uge(ssa_8_arg2_ko.getBitWidth())) {
		ssa_41_u_max_b = APInt(ssa_40_sum_mod.getBitWidth(), 0);
	} else {
		ssa_41_u_max_b = ssa_40_sum_mod.lshr(ssa_8_arg2_ko);
	}
	APInt ssa_42_u_max = ssa_37_arg2_exact ? ssa_41_u_max_b : ssa_22_max_a;
	APInt ssa_43_lz_u(ssa_42_u_max.getBitWidth(), ssa_42_u_max.countl_zero());
	APInt ssa_44_u_kz = ssa_9_zero;
	if (ssa_43_lz_u.ule(ssa_43_lz_u.getBitWidth()))
		ssa_44_u_kz.setHighBits(ssa_43_lz_u.getZExtValue());
	else
		ssa_44_u_kz.setHighBits(ssa_43_lz_u.getBitWidth());
	APInt ssa_45_tz_b(ssa_7_arg2_kz.getBitWidth(), ssa_7_arg2_kz.countr_one());
	APInt ssa_46_min_pos_raw;
	if (ssa_45_tz_b.uge(ssa_45_tz_b.getBitWidth())) {
		ssa_46_min_pos_raw = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_46_min_pos_raw = ssa_10_one.shl(ssa_45_tz_b);
	}
	bool ssa_47_tzb_eq_bw = ssa_45_tz_b.eq(ssa_15_bw);
	APInt ssa_48_min_pos = ssa_47_tzb_eq_bw ? ssa_15_bw : ssa_46_min_pos_raw;
	APInt ssa_49_sum_pos_max = ssa_22_max_a + ssa_48_min_pos;
	bool ssa_50_sum_pos_ov;
	ssa_22_max_a.uadd_ov(ssa_48_min_pos, ssa_50_sum_pos_ov);
	APInt ssa_51_sum_pos_mod = ssa_50_sum_pos_ov ? ssa_14_ones : ssa_49_sum_pos_max;
	APInt ssa_52_u_max_pos;
	if (ssa_48_min_pos.uge(ssa_48_min_pos.getBitWidth())) {
		ssa_52_u_max_pos = APInt(ssa_51_sum_pos_mod.getBitWidth(), 0);
	} else {
		ssa_52_u_max_pos = ssa_51_sum_pos_mod.lshr(ssa_48_min_pos);
	}
	APInt ssa_53_lz_u_pos(ssa_52_u_max_pos.getBitWidth(), ssa_52_u_max_pos.countl_zero());
	APInt ssa_54_u_kz_pos = ssa_9_zero;
	if (ssa_53_lz_u_pos.ule(ssa_53_lz_u_pos.getBitWidth()))
		ssa_54_u_kz_pos.setHighBits(ssa_53_lz_u_pos.getZExtValue());
	else
		ssa_54_u_kz_pos.setHighBits(ssa_53_lz_u_pos.getBitWidth());
	APInt ssa_55_u_kz_common = ssa_5_arg1_kz & ssa_54_u_kz_pos;
	bool ssa_56_arg2_ko_zero = ssa_8_arg2_ko.eq(ssa_9_zero);
	bool ssa_57_arg2_zero = ssa_37_arg2_exact && ssa_56_arg2_ko_zero;
	APInt ssa_58_u_kz_maybe0 = ssa_56_arg2_ko_zero ? ssa_55_u_kz_common : ssa_54_u_kz_pos;
	APInt ssa_59_u_kz_sel = ssa_57_arg2_zero ? ssa_5_arg1_kz : ssa_58_u_kz_maybe0;
	APInt ssa_60_u_kz_combined = ssa_44_u_kz | ssa_59_u_kz_sel;
	bool ssa_61_lz_gt_maxc = ssa_43_lz_u.ugt(ssa_21_max_c);
	APInt ssa_62_lz_diff = ssa_43_lz_u - ssa_21_max_c;
	APInt ssa_63_lz_res = ssa_61_lz_gt_maxc ? ssa_62_lz_diff : ssa_9_zero;
	APInt ssa_64_highmask = ssa_9_zero;
	if (ssa_63_lz_res.ule(ssa_63_lz_res.getBitWidth()))
		ssa_64_highmask.setHighBits(ssa_63_lz_res.getZExtValue());
	else
		ssa_64_highmask.setHighBits(ssa_63_lz_res.getBitWidth());
	APInt ssa_65_tz_c(ssa_3_arg0_kz.getBitWidth(), ssa_3_arg0_kz.countr_one());
	APInt ssa_66_low_count_raw;
	if (ssa_65_tz_c.uge(ssa_65_tz_c.getBitWidth())) {
		ssa_66_low_count_raw = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_66_low_count_raw = ssa_10_one.shl(ssa_65_tz_c);
	}
	bool ssa_67_tz_eq_bw = ssa_65_tz_c.eq(ssa_15_bw);
	APInt ssa_68_low_count = ssa_67_tz_eq_bw ? ssa_15_bw : ssa_66_low_count_raw;
	APInt ssa_69_lowmask_shift = ssa_9_zero;
	if (ssa_68_low_count.ule(ssa_68_low_count.getBitWidth()))
		ssa_69_lowmask_shift.setLowBits(ssa_68_low_count.getZExtValue());
	else
		ssa_69_lowmask_shift.setLowBits(ssa_68_low_count.getBitWidth());
	APInt ssa_70_u_kz_low = ssa_60_u_kz_combined & ssa_69_lowmask_shift;
	APInt ssa_71_lowmask = ssa_9_zero;
	if (ssa_4_arg0_ko.ule(ssa_4_arg0_ko.getBitWidth()))
		ssa_71_lowmask.setLowBits(ssa_4_arg0_ko.getZExtValue());
	else
		ssa_71_lowmask.setLowBits(ssa_4_arg0_ko.getBitWidth());
	APInt ssa_72_arg1_tz(ssa_5_arg1_kz.getBitWidth(), ssa_5_arg1_kz.countr_one());
	APInt ssa_73_lowmask_arg1 = ssa_9_zero;
	if (ssa_72_arg1_tz.ule(ssa_72_arg1_tz.getBitWidth()))
		ssa_73_lowmask_arg1.setLowBits(ssa_72_arg1_tz.getZExtValue());
	else
		ssa_73_lowmask_arg1.setLowBits(ssa_72_arg1_tz.getBitWidth());
	APInt ssa_74_lowmask_arg1_sel = ssa_57_arg2_zero ? ssa_73_lowmask_arg1 : ssa_9_zero;
	APInt ssa_75_res_kz = ssa_64_highmask | ssa_71_lowmask;
	APInt ssa_76_res_kz = ssa_75_res_kz | ssa_74_lowmask_arg1_sel;
	APInt ssa_77_res_kz = ssa_76_res_kz | ssa_70_u_kz_low;
	APInt ssa_78_u_kz_shl;
	if (ssa_4_arg0_ko.uge(ssa_4_arg0_ko.getBitWidth())) {
		ssa_78_u_kz_shl = APInt(ssa_60_u_kz_combined.getBitWidth(), 0);
	} else {
		ssa_78_u_kz_shl = ssa_60_u_kz_combined.shl(ssa_4_arg0_ko);
	}
	APInt ssa_79_u_kz_shl_sel = ssa_27_arg0_exact ? ssa_78_u_kz_shl : ssa_9_zero;
	APInt ssa_80_res_kz = ssa_77_res_kz | ssa_79_u_kz_shl_sel;
	APInt ssa_81_sum_exact = ssa_6_arg1_ko + ssa_8_arg2_ko;
	APInt ssa_82_lshr_exact;
	if (ssa_8_arg2_ko.uge(ssa_8_arg2_ko.getBitWidth())) {
		ssa_82_lshr_exact = APInt(ssa_81_sum_exact.getBitWidth(), 0);
	} else {
		ssa_82_lshr_exact = ssa_81_sum_exact.lshr(ssa_8_arg2_ko);
	}
	APInt ssa_83_res_exact;
	if (ssa_4_arg0_ko.uge(ssa_4_arg0_ko.getBitWidth())) {
		ssa_83_res_exact = APInt(ssa_82_lshr_exact.getBitWidth(), 0);
	} else {
		ssa_83_res_exact = ssa_82_lshr_exact.shl(ssa_4_arg0_ko);
	}
	APInt ssa_84_res_kz_exact = ssa_14_ones ^ ssa_83_res_exact;
	APInt ssa_85_res_ko_exact = ssa_83_res_exact | ssa_9_zero;
	bool ssa_86_arg01_exact = ssa_27_arg0_exact && ssa_32_arg1_exact;
	bool ssa_87_all_exact = ssa_86_arg01_exact && ssa_37_arg2_exact;
	APInt ssa_88_res_kz_sel = ssa_87_all_exact ? ssa_84_res_kz_exact : ssa_80_res_kz;
	APInt ssa_89_res_ko_sel = ssa_87_all_exact ? ssa_85_res_ko_exact : ssa_9_zero;
	APInt ssa_90_arg0_unk = ssa_14_ones ^ ssa_23_arg0_kz_or_ko;
	APInt ssa_91_u_possible = ssa_14_ones ^ ssa_60_u_kz_combined;
	bool ssa_92_u_exact_cond = ssa_32_arg1_exact && ssa_37_arg2_exact;
	APInt ssa_93_u_base = ssa_92_u_exact_cond ? ssa_82_lshr_exact : ssa_91_u_possible;
	APInt ssa_94_mask0;
	if (ssa_4_arg0_ko.uge(ssa_4_arg0_ko.getBitWidth())) {
		ssa_94_mask0 = APInt(ssa_93_u_base.getBitWidth(), 0);
	} else {
		ssa_94_mask0 = ssa_93_u_base.shl(ssa_4_arg0_ko);
	}
	APInt ssa_95_unk0 = ssa_90_arg0_unk & ssa_10_one;
	bool ssa_96_unk0_nz = ssa_95_unk0.ne(ssa_9_zero);
	APInt ssa_97_mask0_shl;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_97_mask0_shl = APInt(ssa_94_mask0.getBitWidth(), 0);
	} else {
		ssa_97_mask0_shl = ssa_94_mask0.shl(ssa_10_one);
	}
	APInt ssa_98_mask0_or = ssa_94_mask0 | ssa_97_mask0_shl;
	APInt ssa_99_mask1 = ssa_96_unk0_nz ? ssa_98_mask0_or : ssa_94_mask0;
	APInt ssa_100_unk1 = ssa_90_arg0_unk & ssa_11_c2;
	bool ssa_101_unk1_nz = ssa_100_unk1.ne(ssa_9_zero);
	APInt ssa_102_mask1_shl;
	if (ssa_11_c2.uge(ssa_11_c2.getBitWidth())) {
		ssa_102_mask1_shl = APInt(ssa_99_mask1.getBitWidth(), 0);
	} else {
		ssa_102_mask1_shl = ssa_99_mask1.shl(ssa_11_c2);
	}
	APInt ssa_103_mask1_or = ssa_99_mask1 | ssa_102_mask1_shl;
	APInt ssa_104_mask2 = ssa_101_unk1_nz ? ssa_103_mask1_or : ssa_99_mask1;
	APInt ssa_105_unk2 = ssa_90_arg0_unk & ssa_13_c4;
	bool ssa_106_unk2_nz = ssa_105_unk2.ne(ssa_9_zero);
	APInt ssa_107_mask2_shl;
	if (ssa_13_c4.uge(ssa_13_c4.getBitWidth())) {
		ssa_107_mask2_shl = APInt(ssa_104_mask2.getBitWidth(), 0);
	} else {
		ssa_107_mask2_shl = ssa_104_mask2.shl(ssa_13_c4);
	}
	APInt ssa_108_mask2_or = ssa_104_mask2 | ssa_107_mask2_shl;
	APInt ssa_109_mask3 = ssa_106_unk2_nz ? ssa_108_mask2_or : ssa_104_mask2;
	APInt ssa_110_low3_mask = ssa_9_zero;
	if (ssa_12_c3.ule(ssa_12_c3.getBitWidth()))
		ssa_110_low3_mask.setLowBits(ssa_12_c3.getZExtValue());
	else
		ssa_110_low3_mask.setLowBits(ssa_12_c3.getBitWidth());
	APInt ssa_111_high_mask = ssa_14_ones ^ ssa_110_low3_mask;
	APInt ssa_112_high_unk = ssa_90_arg0_unk & ssa_111_high_mask;
	bool ssa_113_high_unk_nz = ssa_112_high_unk.ne(ssa_9_zero);
	APInt ssa_114_mask_used = ssa_113_high_unk_nz ? ssa_14_ones : ssa_109_mask3;
	APInt ssa_115_known_zero_mask = ssa_14_ones ^ ssa_114_mask_used;
	APInt ssa_116_res_kz = ssa_88_res_kz_sel | ssa_115_known_zero_mask;
	bool ssa_117_arg1_kz_all = ssa_5_arg1_kz.eq(ssa_14_ones);
	bool ssa_118_arg1_ko_zero = ssa_6_arg1_ko.eq(ssa_9_zero);
	bool ssa_119_arg1_zero = ssa_117_arg1_kz_all && ssa_118_arg1_ko_zero;
	APInt ssa_120_res_kz = ssa_119_arg1_zero ? ssa_14_ones : ssa_116_res_kz;
	bool ssa_121_arg0_lb_ge_bw = ssa_4_arg0_ko.uge(ssa_15_bw);
	bool ssa_122_arg2_lb_ge_bw = ssa_8_arg2_ko.uge(ssa_15_bw);
	bool ssa_123_constraint_unsat = ssa_121_arg0_lb_ge_bw || ssa_122_arg2_lb_ge_bw;
	APInt ssa_124_out_kz = ssa_123_constraint_unsat ? ssa_14_ones : ssa_120_res_kz;
	APInt ssa_125_out_ko = ssa_123_constraint_unsat ? ssa_14_ones : ssa_89_res_ko_sel;
	std::array<APInt, 2> ssa_126_r = std::array<APInt, 2>{ssa_124_out_kz, ssa_125_out_ko};
	return ssa_126_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0_kz = ssa_0_arg0[0];
	APInt ssa_4_arg0_ko = ssa_0_arg0[1];
	APInt ssa_5_arg1_kz = ssa_1_arg1[0];
	APInt ssa_6_arg1_ko = ssa_1_arg1[1];
	APInt ssa_7_arg2_kz = ssa_2_arg2[0];
	APInt ssa_8_arg2_ko = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_arg0_kz.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_arg0_kz.getBitWidth(), 1);
	APInt ssa_11_c2(ssa_3_arg0_kz.getBitWidth(), 2);
	APInt ssa_12_c3(ssa_3_arg0_kz.getBitWidth(), 3);
	APInt ssa_13_c4(ssa_3_arg0_kz.getBitWidth(), 4);
	APInt ssa_14_c8(ssa_3_arg0_kz.getBitWidth(), 8);
	APInt ssa_15_ones = APInt::getAllOnes(ssa_3_arg0_kz.getBitWidth());
	APInt ssa_16_arg1_kz_or_ko = ssa_5_arg1_kz | ssa_6_arg1_ko;
	bool ssa_17_arg1_all_known = ssa_16_arg1_kz_or_ko.eq(ssa_15_ones);
	APInt ssa_18_arg1_overlap = ssa_5_arg1_kz & ssa_6_arg1_ko;
	bool ssa_19_arg1_no_overlap = ssa_18_arg1_overlap.eq(ssa_9_zero);
	bool ssa_20_arg1_exact = ssa_17_arg1_all_known && ssa_19_arg1_no_overlap;
	APInt ssa_21_arg2_kz_or_ko = ssa_7_arg2_kz | ssa_8_arg2_ko;
	bool ssa_22_arg2_all_known = ssa_21_arg2_kz_or_ko.eq(ssa_15_ones);
	APInt ssa_23_arg2_overlap = ssa_7_arg2_kz & ssa_8_arg2_ko;
	bool ssa_24_arg2_no_overlap = ssa_23_arg2_overlap.eq(ssa_9_zero);
	bool ssa_25_arg2_exact = ssa_22_arg2_all_known && ssa_24_arg2_no_overlap;
	bool ssa_26_both_exact = ssa_20_arg1_exact && ssa_25_arg2_exact;
	APInt ssa_27_sum = ssa_6_arg1_ko + ssa_8_arg2_ko;
	APInt ssa_28_base;
	if (ssa_8_arg2_ko.uge(ssa_8_arg2_ko.getBitWidth())) {
		ssa_28_base = APInt(ssa_27_sum.getBitWidth(), 0);
	} else {
		ssa_28_base = ssa_27_sum.lshr(ssa_8_arg2_ko);
	}
	bool ssa_29_base_nonzero = ssa_28_base.ne(ssa_9_zero);
	APInt ssa_30_base_minus1 = ssa_28_base - ssa_10_one;
	APInt ssa_31_base_and = ssa_28_base & ssa_30_base_minus1;
	bool ssa_32_base_pow2 = ssa_31_base_and.eq(ssa_9_zero);
	bool ssa_33_base_onehot = ssa_32_base_pow2 && ssa_29_base_nonzero;
	bool ssa_34_cond0 = ssa_26_both_exact && ssa_33_base_onehot;
	APInt ssa_35_known_mask = ssa_3_arg0_kz | ssa_4_arg0_ko;
	APInt ssa_36_low3_mask = ssa_9_zero;
	if (ssa_12_c3.ule(ssa_12_c3.getBitWidth()))
		ssa_36_low3_mask.setLowBits(ssa_12_c3.getZExtValue());
	else
		ssa_36_low3_mask.setLowBits(ssa_12_c3.getBitWidth());
	APInt ssa_37_known_mask_low = ssa_35_known_mask & ssa_36_low3_mask;
	APInt ssa_38_unknown_low = ssa_36_low3_mask ^ ssa_37_known_mask_low;
	APInt ssa_39_known_one_low = ssa_4_arg0_ko & ssa_36_low3_mask;
	APInt ssa_40_mask0;
	if (ssa_39_known_one_low.uge(ssa_39_known_one_low.getBitWidth())) {
		ssa_40_mask0 = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_40_mask0 = ssa_10_one.shl(ssa_39_known_one_low);
	}
	APInt ssa_41_unk0 = ssa_38_unknown_low & ssa_10_one;
	bool ssa_42_unk0_nz = ssa_41_unk0.ne(ssa_9_zero);
	APInt ssa_43_mask0_shl;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_43_mask0_shl = APInt(ssa_40_mask0.getBitWidth(), 0);
	} else {
		ssa_43_mask0_shl = ssa_40_mask0.shl(ssa_10_one);
	}
	APInt ssa_44_mask0_or = ssa_40_mask0 | ssa_43_mask0_shl;
	APInt ssa_45_mask1 = ssa_42_unk0_nz ? ssa_44_mask0_or : ssa_40_mask0;
	APInt ssa_46_unk1 = ssa_38_unknown_low & ssa_11_c2;
	bool ssa_47_unk1_nz = ssa_46_unk1.ne(ssa_9_zero);
	APInt ssa_48_mask1_shl;
	if (ssa_11_c2.uge(ssa_11_c2.getBitWidth())) {
		ssa_48_mask1_shl = APInt(ssa_45_mask1.getBitWidth(), 0);
	} else {
		ssa_48_mask1_shl = ssa_45_mask1.shl(ssa_11_c2);
	}
	APInt ssa_49_mask1_or = ssa_45_mask1 | ssa_48_mask1_shl;
	APInt ssa_50_mask2 = ssa_47_unk1_nz ? ssa_49_mask1_or : ssa_45_mask1;
	APInt ssa_51_unk2 = ssa_38_unknown_low & ssa_13_c4;
	bool ssa_52_unk2_nz = ssa_51_unk2.ne(ssa_9_zero);
	APInt ssa_53_mask2_shl;
	if (ssa_13_c4.uge(ssa_13_c4.getBitWidth())) {
		ssa_53_mask2_shl = APInt(ssa_50_mask2.getBitWidth(), 0);
	} else {
		ssa_53_mask2_shl = ssa_50_mask2.shl(ssa_13_c4);
	}
	APInt ssa_54_mask2_or = ssa_50_mask2 | ssa_53_mask2_shl;
	APInt ssa_55_mask3 = ssa_52_unk2_nz ? ssa_54_mask2_or : ssa_50_mask2;
	APInt ssa_56_tz(ssa_28_base.getBitWidth(), ssa_28_base.countr_zero());
	APInt ssa_57_tz_low3 = ssa_56_tz & ssa_36_low3_mask;
	APInt ssa_58_low8_mask = ssa_9_zero;
	if (ssa_14_c8.ule(ssa_14_c8.getBitWidth()))
		ssa_58_low8_mask.setLowBits(ssa_14_c8.getZExtValue());
	else
		ssa_58_low8_mask.setLowBits(ssa_14_c8.getBitWidth());
	APInt ssa_59_mask_shiftl;
	if (ssa_57_tz_low3.uge(ssa_57_tz_low3.getBitWidth())) {
		ssa_59_mask_shiftl = APInt(ssa_55_mask3.getBitWidth(), 0);
	} else {
		ssa_59_mask_shiftl = ssa_55_mask3.shl(ssa_57_tz_low3);
	}
	APInt ssa_60_shift_amt_r = ssa_14_c8 - ssa_57_tz_low3;
	APInt ssa_61_mask_shiftr;
	if (ssa_60_shift_amt_r.uge(ssa_60_shift_amt_r.getBitWidth())) {
		ssa_61_mask_shiftr = APInt(ssa_55_mask3.getBitWidth(), 0);
	} else {
		ssa_61_mask_shiftr = ssa_55_mask3.lshr(ssa_60_shift_amt_r);
	}
	APInt ssa_62_mask_or = ssa_59_mask_shiftl | ssa_61_mask_shiftr;
	APInt ssa_63_mask_rot = ssa_62_mask_or & ssa_58_low8_mask;
	APInt ssa_64_pow8;
	if (ssa_14_c8.uge(ssa_14_c8.getBitWidth())) {
		ssa_64_pow8 = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_64_pow8 = ssa_10_one.shl(ssa_14_c8);
	}
	APInt ssa_65_divisor = ssa_64_pow8 - ssa_10_one;
	APInt ssa_66_pattern;
	if (ssa_65_divisor == 0) {
		ssa_66_pattern = APInt(ssa_15_ones.getBitWidth(), -1);
	} else {
		ssa_66_pattern = ssa_15_ones.udiv(ssa_65_divisor);
	}
	APInt ssa_67_mask_periodic = ssa_63_mask_rot * ssa_66_pattern;
	APInt ssa_68_known_zero_mask = ssa_15_ones ^ ssa_67_mask_periodic;
	APInt ssa_69_res_kz = ssa_34_cond0 ? ssa_68_known_zero_mask : ssa_9_zero;
	APInt ssa_70_res_ko = ssa_34_cond0 ? ssa_9_zero : ssa_9_zero;
	std::array<APInt, 2> ssa_71_r = std::array<APInt, 2>{ssa_69_res_kz, ssa_70_res_ko};
	return ssa_71_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_arg0_kz = ssa_0_arg0[0];
	APInt ssa_4_arg0_ko = ssa_0_arg0[1];
	APInt ssa_5_arg1_kz = ssa_1_arg1[0];
	APInt ssa_6_arg1_ko = ssa_1_arg1[1];
	APInt ssa_7_arg2_kz = ssa_2_arg2[0];
	APInt ssa_8_arg2_ko = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_arg0_kz.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_arg0_kz.getBitWidth(), 1);
	APInt ssa_11_c3(ssa_3_arg0_kz.getBitWidth(), 3);
	APInt ssa_12_c8(ssa_3_arg0_kz.getBitWidth(), 8);
	APInt ssa_13_ones = APInt::getAllOnes(ssa_3_arg0_kz.getBitWidth());
	APInt ssa_14_bw(ssa_3_arg0_kz.getBitWidth(), ssa_3_arg0_kz.getBitWidth());
	bool ssa_15_true = ssa_9_zero.eq(ssa_9_zero);
	APInt ssa_16_arg1_kz_or_ko = ssa_5_arg1_kz | ssa_6_arg1_ko;
	bool ssa_17_arg1_all_known = ssa_16_arg1_kz_or_ko.eq(ssa_13_ones);
	APInt ssa_18_arg1_overlap = ssa_5_arg1_kz & ssa_6_arg1_ko;
	bool ssa_19_arg1_no_overlap = ssa_18_arg1_overlap.eq(ssa_9_zero);
	bool ssa_20_arg1_exact = ssa_17_arg1_all_known && ssa_19_arg1_no_overlap;
	APInt ssa_21_a = ssa_6_arg1_ko | ssa_9_zero;
	APInt ssa_22_b_tz(ssa_7_arg2_kz.getBitWidth(), ssa_7_arg2_kz.countr_one());
	APInt ssa_23_b_min;
	if (ssa_22_b_tz.uge(ssa_22_b_tz.getBitWidth())) {
		ssa_23_b_min = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_23_b_min = ssa_10_one.shl(ssa_22_b_tz);
	}
	bool ssa_24_b_min_ge_bw = ssa_23_b_min.uge(ssa_14_bw);
	APInt ssa_25_sum = ssa_21_a + ssa_23_b_min;
	bool ssa_26_sum_ov;
	ssa_21_a.uadd_ov(ssa_23_b_min, ssa_26_sum_ov);
	bool ssa_27_sum_no_ov = ssa_26_sum_ov != ssa_15_true;
	APInt ssa_28_pow_bmin;
	if (ssa_23_b_min.uge(ssa_23_b_min.getBitWidth())) {
		ssa_28_pow_bmin = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_28_pow_bmin = ssa_10_one.shl(ssa_23_b_min);
	}
	bool ssa_29_sum_lt = ssa_25_sum.ult(ssa_28_pow_bmin);
	bool ssa_30_sum_cond = ssa_27_sum_no_ov && ssa_29_sum_lt;
	bool ssa_31_cond_b = ssa_30_sum_cond || ssa_24_b_min_ge_bw;
	APInt ssa_32_low3_mask = ssa_9_zero;
	if (ssa_11_c3.ule(ssa_11_c3.getBitWidth()))
		ssa_32_low3_mask.setLowBits(ssa_11_c3.getZExtValue());
	else
		ssa_32_low3_mask.setLowBits(ssa_11_c3.getBitWidth());
	APInt ssa_33_arg0_kz_low = ssa_3_arg0_kz & ssa_32_low3_mask;
	bool ssa_34_arg0_low3 = ssa_33_arg0_kz_low.eq(ssa_32_low3_mask);
	bool ssa_35_bw_ge = ssa_14_bw.uge(ssa_12_c8);
	APInt ssa_36_clz(ssa_21_a.getBitWidth(), ssa_21_a.countl_zero());
	APInt ssa_37_bw_minus = ssa_14_bw - ssa_12_c8;
	bool ssa_38_lz_ge = ssa_36_clz.uge(ssa_37_bw_minus);
	bool ssa_39_cond0 = ssa_20_arg1_exact && ssa_31_cond_b;
	bool ssa_40_cond1 = ssa_39_cond0 && ssa_34_arg0_low3;
	bool ssa_41_cond2 = ssa_40_cond1 && ssa_35_bw_ge;
	bool ssa_42_cond = ssa_41_cond2 && ssa_38_lz_ge;
	APInt ssa_43_low8_mask = ssa_9_zero;
	if (ssa_12_c8.ule(ssa_12_c8.getBitWidth()))
		ssa_43_low8_mask.setLowBits(ssa_12_c8.getZExtValue());
	else
		ssa_43_low8_mask.setLowBits(ssa_12_c8.getBitWidth());
	APInt ssa_44_a_low8 = ssa_21_a & ssa_43_low8_mask;
	APInt ssa_45_pow8;
	if (ssa_12_c8.uge(ssa_12_c8.getBitWidth())) {
		ssa_45_pow8 = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_45_pow8 = ssa_10_one.shl(ssa_12_c8);
	}
	APInt ssa_46_divisor = ssa_45_pow8 - ssa_10_one;
	APInt ssa_47_pattern;
	if (ssa_46_divisor == 0) {
		ssa_47_pattern = APInt(ssa_13_ones.getBitWidth(), -1);
	} else {
		ssa_47_pattern = ssa_13_ones.udiv(ssa_46_divisor);
	}
	APInt ssa_48_mask_periodic = ssa_44_a_low8 * ssa_47_pattern;
	APInt ssa_49_known_zero_mask = ssa_13_ones ^ ssa_48_mask_periodic;
	APInt ssa_50_out_kz = ssa_42_cond ? ssa_49_known_zero_mask : ssa_9_zero;
	APInt ssa_51_out_ko = ssa_9_zero | ssa_9_zero;
	std::array<APInt, 2> ssa_52_r = std::array<APInt, 2>{ssa_50_out_kz, ssa_51_out_ko};
	return ssa_52_r;
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
