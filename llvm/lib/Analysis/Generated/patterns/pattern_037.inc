namespace Pattern037 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_k0 = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_12_k1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_13_k2 = ssa_7_a2z | ssa_8_a2o;
	bool ssa_14_all0 = ssa_11_k0.eq(ssa_9_allones);
	bool ssa_15_all1 = ssa_12_k1.eq(ssa_9_allones);
	bool ssa_16_all2 = ssa_13_k2.eq(ssa_9_allones);
	bool ssa_17_and01 = ssa_14_all0 && ssa_15_all1;
	bool ssa_18_all = ssa_17_and01 && ssa_16_all2;
	APInt ssa_19_ashr;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitSet()) {
		ssa_19_ashr = APInt(ssa_8_a2o.getBitWidth(), -1);
	} else if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitClear()) {
		ssa_19_ashr = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_19_ashr = ssa_8_a2o.ashr(ssa_4_a0o);
	}
	APInt ssa_20_add = ssa_6_a1o + ssa_19_ashr;
	APInt ssa_21_kz_exact = ssa_20_add ^ ssa_9_allones;
	bool ssa_22_a1o_zero = ssa_6_a1o.eq(ssa_10_zero);
	bool ssa_23_a1z_all = ssa_5_a1z.eq(ssa_9_allones);
	bool ssa_24_arg1_zero = ssa_22_a1o_zero && ssa_23_a1z_all;
	bool ssa_25_case2 = ssa_14_all0 && ssa_24_arg1_zero;
	APInt ssa_26_lowmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_26_lowmask.setLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_26_lowmask.setLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_27_a2z2 = ssa_7_a2z | ssa_26_lowmask;
	APInt ssa_28_kz_shifted;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_28_kz_shifted = APInt(ssa_27_a2z2.getBitWidth(), 0);
	} else {
		ssa_28_kz_shifted = ssa_27_a2z2.lshr(ssa_4_a0o);
	}
	APInt ssa_29_ko_shifted;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_29_ko_shifted = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_29_ko_shifted = ssa_8_a2o.lshr(ssa_4_a0o);
	}
	APInt ssa_30_signmask = ssa_10_zero;
	ssa_30_signmask.setSignBit();
	APInt ssa_31_sign_o = ssa_8_a2o & ssa_30_signmask;
	APInt ssa_32_sign_z = ssa_7_a2z & ssa_30_signmask;
	bool ssa_33_sign_one = ssa_31_sign_o.eq(ssa_30_signmask);
	bool ssa_34_sign_zero = ssa_32_sign_z.eq(ssa_30_signmask);
	APInt ssa_35_topmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_35_topmask.setHighBits(ssa_4_a0o.getZExtValue());
	else
		ssa_35_topmask.setHighBits(ssa_4_a0o.getBitWidth());
	APInt ssa_36_kz_top = ssa_28_kz_shifted | ssa_35_topmask;
	APInt ssa_37_ko_top = ssa_29_ko_shifted | ssa_35_topmask;
	APInt ssa_38_ashr_kz = ssa_34_sign_zero ? ssa_36_kz_top : ssa_28_kz_shifted;
	APInt ssa_39_ashr_ko = ssa_33_sign_one ? ssa_37_ko_top : ssa_29_ko_shifted;
	bool ssa_40_a2o_zero = ssa_8_a2o.eq(ssa_10_zero);
	bool ssa_41_a2z_all = ssa_7_a2z.eq(ssa_9_allones);
	bool ssa_42_arg2_zero = ssa_40_a2o_zero && ssa_41_a2z_all;
	APInt ssa_43_a1_sign_o = ssa_6_a1o & ssa_30_signmask;
	APInt ssa_44_a1_sign_z = ssa_5_a1z & ssa_30_signmask;
	bool ssa_45_a1_sign_one = ssa_43_a1_sign_o.eq(ssa_30_signmask);
	bool ssa_46_a1_sign_zero = ssa_44_a1_sign_z.eq(ssa_30_signmask);
	bool ssa_47_sign_same_one = ssa_45_a1_sign_one && ssa_33_sign_one;
	bool ssa_48_sign_same_zero = ssa_46_a1_sign_zero && ssa_34_sign_zero;
	APInt ssa_49_sign_one_mask = ssa_47_sign_same_one ? ssa_30_signmask : ssa_10_zero;
	APInt ssa_50_sign_zero_mask = ssa_48_sign_same_zero ? ssa_30_signmask : ssa_10_zero;
	APInt ssa_51_tz1(ssa_5_a1z.getBitWidth(), ssa_5_a1z.countr_one());
	APInt ssa_52_tz2(ssa_38_ashr_kz.getBitWidth(), ssa_38_ashr_kz.countr_one());
	APInt ssa_53_tz_min = APIntOps::umin(ssa_51_tz1, ssa_52_tz2);
	APInt ssa_54_lowmask_add = ssa_10_zero;
	if (ssa_53_tz_min.ule(ssa_53_tz_min.getBitWidth()))
		ssa_54_lowmask_add.setLowBits(ssa_53_tz_min.getZExtValue());
	else
		ssa_54_lowmask_add.setLowBits(ssa_53_tz_min.getBitWidth());
	APInt ssa_55_lowmask_add_sel = ssa_14_all0 ? ssa_54_lowmask_add : ssa_10_zero;
	APInt ssa_56_base_zero = ssa_50_sign_zero_mask | ssa_55_lowmask_add_sel;
	APInt ssa_57_res0_case2 = ssa_25_case2 ? ssa_38_ashr_kz : ssa_56_base_zero;
	APInt ssa_58_res1_case2 = ssa_25_case2 ? ssa_39_ashr_ko : ssa_49_sign_one_mask;
	APInt ssa_59_res0_case3 = ssa_42_arg2_zero ? ssa_5_a1z : ssa_57_res0_case2;
	APInt ssa_60_res1_case3 = ssa_42_arg2_zero ? ssa_6_a1o : ssa_58_res1_case2;
	APInt ssa_61_res0 = ssa_18_all ? ssa_21_kz_exact : ssa_59_res0_case3;
	APInt ssa_62_res1 = ssa_18_all ? ssa_20_add : ssa_60_res1_case3;
	std::array<APInt, 2> ssa_63_r = std::array<APInt, 2>{ssa_61_res0, ssa_62_res1};
	return ssa_63_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_k0 = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_12_k1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_13_k2 = ssa_7_a2z | ssa_8_a2o;
	bool ssa_14_all0 = ssa_11_k0.eq(ssa_9_allones);
	bool ssa_15_all1 = ssa_12_k1.eq(ssa_9_allones);
	bool ssa_16_all2 = ssa_13_k2.eq(ssa_9_allones);
	bool ssa_17_and01 = ssa_14_all0 && ssa_15_all1;
	bool ssa_18_all = ssa_17_and01 && ssa_16_all2;
	APInt ssa_19_ashr;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitSet()) {
		ssa_19_ashr = APInt(ssa_8_a2o.getBitWidth(), -1);
	} else if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitClear()) {
		ssa_19_ashr = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_19_ashr = ssa_8_a2o.ashr(ssa_4_a0o);
	}
	APInt ssa_20_add = ssa_6_a1o + ssa_19_ashr;
	APInt ssa_21_kz_exact = ssa_20_add ^ ssa_9_allones;
	bool ssa_22_a1o_zero = ssa_6_a1o.eq(ssa_10_zero);
	bool ssa_23_a1z_all = ssa_5_a1z.eq(ssa_9_allones);
	bool ssa_24_arg1_zero = ssa_22_a1o_zero && ssa_23_a1z_all;
	bool ssa_25_case2 = ssa_14_all0 && ssa_24_arg1_zero;
	APInt ssa_26_lowmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_26_lowmask.setLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_26_lowmask.setLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_27_a2z2 = ssa_7_a2z | ssa_26_lowmask;
	APInt ssa_28_kz_shifted;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_28_kz_shifted = APInt(ssa_27_a2z2.getBitWidth(), 0);
	} else {
		ssa_28_kz_shifted = ssa_27_a2z2.lshr(ssa_4_a0o);
	}
	APInt ssa_29_ko_shifted;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_29_ko_shifted = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_29_ko_shifted = ssa_8_a2o.lshr(ssa_4_a0o);
	}
	APInt ssa_30_signmask = ssa_10_zero;
	ssa_30_signmask.setSignBit();
	APInt ssa_31_sign_o = ssa_8_a2o & ssa_30_signmask;
	APInt ssa_32_sign_z = ssa_7_a2z & ssa_30_signmask;
	bool ssa_33_sign_one = ssa_31_sign_o.eq(ssa_30_signmask);
	bool ssa_34_sign_zero = ssa_32_sign_z.eq(ssa_30_signmask);
	APInt ssa_35_topmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_35_topmask.setHighBits(ssa_4_a0o.getZExtValue());
	else
		ssa_35_topmask.setHighBits(ssa_4_a0o.getBitWidth());
	APInt ssa_36_kz_top = ssa_28_kz_shifted | ssa_35_topmask;
	APInt ssa_37_ko_top = ssa_29_ko_shifted | ssa_35_topmask;
	APInt ssa_38_ashr_kz = ssa_34_sign_zero ? ssa_36_kz_top : ssa_28_kz_shifted;
	APInt ssa_39_ashr_ko = ssa_33_sign_one ? ssa_37_ko_top : ssa_29_ko_shifted;
	bool ssa_40_a2o_zero = ssa_8_a2o.eq(ssa_10_zero);
	bool ssa_41_a2z_all = ssa_7_a2z.eq(ssa_9_allones);
	bool ssa_42_arg2_zero = ssa_40_a2o_zero && ssa_41_a2z_all;
	APInt ssa_43_a1_sign_o = ssa_6_a1o & ssa_30_signmask;
	APInt ssa_44_a1_sign_z = ssa_5_a1z & ssa_30_signmask;
	bool ssa_45_a1_sign_one = ssa_43_a1_sign_o.eq(ssa_30_signmask);
	bool ssa_46_a1_sign_zero = ssa_44_a1_sign_z.eq(ssa_30_signmask);
	bool ssa_47_sign_same_one = ssa_45_a1_sign_one && ssa_33_sign_one;
	bool ssa_48_sign_same_zero = ssa_46_a1_sign_zero && ssa_34_sign_zero;
	APInt ssa_49_sign_one_mask = ssa_47_sign_same_one ? ssa_30_signmask : ssa_10_zero;
	APInt ssa_50_sign_zero_mask = ssa_48_sign_same_zero ? ssa_30_signmask : ssa_10_zero;
	APInt ssa_51_tz1(ssa_5_a1z.getBitWidth(), ssa_5_a1z.countr_one());
	APInt ssa_52_tz2(ssa_38_ashr_kz.getBitWidth(), ssa_38_ashr_kz.countr_one());
	APInt ssa_53_tz_min = APIntOps::umin(ssa_51_tz1, ssa_52_tz2);
	APInt ssa_54_lowmask_add = ssa_10_zero;
	if (ssa_53_tz_min.ule(ssa_53_tz_min.getBitWidth()))
		ssa_54_lowmask_add.setLowBits(ssa_53_tz_min.getZExtValue());
	else
		ssa_54_lowmask_add.setLowBits(ssa_53_tz_min.getBitWidth());
	APInt ssa_55_lowmask_add_sel = ssa_14_all0 ? ssa_54_lowmask_add : ssa_10_zero;
	APInt ssa_56_to1(ssa_6_a1o.getBitWidth(), ssa_6_a1o.countr_one());
	APInt ssa_57_to2(ssa_39_ashr_ko.getBitWidth(), ssa_39_ashr_ko.countr_one());
	APInt ssa_58_oz_min = APIntOps::umin(ssa_56_to1, ssa_52_tz2);
	APInt ssa_59_zo_min = APIntOps::umin(ssa_51_tz1, ssa_57_to2);
	APInt ssa_60_lowmask_oz = ssa_10_zero;
	if (ssa_58_oz_min.ule(ssa_58_oz_min.getBitWidth()))
		ssa_60_lowmask_oz.setLowBits(ssa_58_oz_min.getZExtValue());
	else
		ssa_60_lowmask_oz.setLowBits(ssa_58_oz_min.getBitWidth());
	APInt ssa_61_lowmask_zo = ssa_10_zero;
	if (ssa_59_zo_min.ule(ssa_59_zo_min.getBitWidth()))
		ssa_61_lowmask_zo.setLowBits(ssa_59_zo_min.getZExtValue());
	else
		ssa_61_lowmask_zo.setLowBits(ssa_59_zo_min.getBitWidth());
	APInt ssa_62_lowmask_one = ssa_60_lowmask_oz | ssa_61_lowmask_zo;
	APInt ssa_63_lowmask_one_sel = ssa_14_all0 ? ssa_62_lowmask_one : ssa_10_zero;
	APInt ssa_64_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_65_lsb_mask = ssa_10_zero;
	if (ssa_64_one.ule(ssa_64_one.getBitWidth()))
		ssa_65_lsb_mask.setLowBits(ssa_64_one.getZExtValue());
	else
		ssa_65_lsb_mask.setLowBits(ssa_64_one.getBitWidth());
	APInt ssa_66_a1_lsb_o = ssa_6_a1o & ssa_65_lsb_mask;
	bool ssa_67_a1_lsb_one = ssa_66_a1_lsb_o.eq(ssa_65_lsb_mask);
	APInt ssa_68_ashr_lsb_o = ssa_39_ashr_ko & ssa_65_lsb_mask;
	bool ssa_69_ashr_lsb_one = ssa_68_ashr_lsb_o.eq(ssa_65_lsb_mask);
	bool ssa_70_lsb_both_one = ssa_67_a1_lsb_one && ssa_69_ashr_lsb_one;
	APInt ssa_71_lsb_zero_mask = ssa_70_lsb_both_one ? ssa_65_lsb_mask : ssa_10_zero;
	APInt ssa_72_lsb_zero_sel = ssa_14_all0 ? ssa_71_lsb_zero_mask : ssa_10_zero;
	APInt ssa_73_base_zero = ssa_50_sign_zero_mask | ssa_55_lowmask_add_sel;
	APInt ssa_74_base_zero2 = ssa_73_base_zero | ssa_72_lsb_zero_sel;
	APInt ssa_75_base_one = ssa_49_sign_one_mask | ssa_63_lowmask_one_sel;
	APInt ssa_76_res0_case2 = ssa_25_case2 ? ssa_38_ashr_kz : ssa_74_base_zero2;
	APInt ssa_77_res1_case2 = ssa_25_case2 ? ssa_39_ashr_ko : ssa_75_base_one;
	APInt ssa_78_res0_case3 = ssa_42_arg2_zero ? ssa_5_a1z : ssa_76_res0_case2;
	APInt ssa_79_res1_case3 = ssa_42_arg2_zero ? ssa_6_a1o : ssa_77_res1_case2;
	APInt ssa_80_res0 = ssa_18_all ? ssa_21_kz_exact : ssa_78_res0_case3;
	APInt ssa_81_res1 = ssa_18_all ? ssa_20_add : ssa_79_res1_case3;
	std::array<APInt, 2> ssa_82_r = std::array<APInt, 2>{ssa_80_res0, ssa_81_res1};
	return ssa_82_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_k0 = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_12_k1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_13_k2 = ssa_7_a2z | ssa_8_a2o;
	bool ssa_14_all0 = ssa_11_k0.eq(ssa_9_allones);
	bool ssa_15_all1 = ssa_12_k1.eq(ssa_9_allones);
	bool ssa_16_all2 = ssa_13_k2.eq(ssa_9_allones);
	bool ssa_17_and01 = ssa_14_all0 && ssa_15_all1;
	bool ssa_18_all = ssa_17_and01 && ssa_16_all2;
	APInt ssa_19_bw(ssa_3_a0z.getBitWidth(), ssa_3_a0z.getBitWidth());
	bool ssa_20_shift_lt = ssa_4_a0o.ult(ssa_19_bw);
	bool ssa_21_true = -1;
	bool ssa_22_not_shift_lt = ssa_20_shift_lt != ssa_21_true;
	bool ssa_23_unsat_shift = ssa_14_all0 && ssa_22_not_shift_lt;
	APInt ssa_24_lowmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_24_lowmask.setLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_24_lowmask.setLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_25_a2_low = ssa_8_a2o & ssa_24_lowmask;
	bool ssa_26_a2_low_zero = ssa_25_a2_low.eq(ssa_10_zero);
	bool ssa_27_not_a2_low_zero = ssa_26_a2_low_zero != ssa_21_true;
	bool ssa_28_unsat_low = ssa_14_all0 && ssa_27_not_a2_low_zero;
	APInt ssa_29_ashr;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitSet()) {
		ssa_29_ashr = APInt(ssa_8_a2o.getBitWidth(), -1);
	} else if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitClear()) {
		ssa_29_ashr = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_29_ashr = ssa_8_a2o.ashr(ssa_4_a0o);
	}
	bool ssa_30_sadd_ov;
	ssa_6_a1o.sadd_ov(ssa_29_ashr, ssa_30_sadd_ov);
	bool ssa_31_unsat_ov = ssa_18_all && ssa_30_sadd_ov;
	bool ssa_32_unsat0 = ssa_23_unsat_shift || ssa_28_unsat_low;
	bool ssa_33_unsat = ssa_32_unsat0 || ssa_31_unsat_ov;
	APInt ssa_34_res0 = ssa_33_unsat ? ssa_9_allones : ssa_10_zero;
	APInt ssa_35_res1 = ssa_33_unsat ? ssa_9_allones : ssa_10_zero;
	std::array<APInt, 2> ssa_36_r = std::array<APInt, 2>{ssa_34_res0, ssa_35_res1};
	return ssa_36_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_k0 = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_12_k1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_13_k2 = ssa_7_a2z | ssa_8_a2o;
	bool ssa_14_all0 = ssa_11_k0.eq(ssa_9_allones);
	bool ssa_15_all1 = ssa_12_k1.eq(ssa_9_allones);
	bool ssa_16_all2 = ssa_13_k2.eq(ssa_9_allones);
	bool ssa_17_and01 = ssa_14_all0 && ssa_15_all1;
	bool ssa_18_all = ssa_17_and01 && ssa_16_all2;
	APInt ssa_19_ashr;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitSet()) {
		ssa_19_ashr = APInt(ssa_8_a2o.getBitWidth(), -1);
	} else if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitClear()) {
		ssa_19_ashr = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_19_ashr = ssa_8_a2o.ashr(ssa_4_a0o);
	}
	APInt ssa_20_add = ssa_6_a1o + ssa_19_ashr;
	APInt ssa_21_kz_exact = ssa_20_add ^ ssa_9_allones;
	bool ssa_22_a1o_zero = ssa_6_a1o.eq(ssa_10_zero);
	bool ssa_23_a1z_all = ssa_5_a1z.eq(ssa_9_allones);
	bool ssa_24_arg1_zero = ssa_22_a1o_zero && ssa_23_a1z_all;
	bool ssa_25_case2 = ssa_14_all0 && ssa_24_arg1_zero;
	APInt ssa_26_lowmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_26_lowmask.setLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_26_lowmask.setLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_27_a2z2 = ssa_7_a2z | ssa_26_lowmask;
	APInt ssa_28_kz_shifted;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_28_kz_shifted = APInt(ssa_27_a2z2.getBitWidth(), 0);
	} else {
		ssa_28_kz_shifted = ssa_27_a2z2.lshr(ssa_4_a0o);
	}
	APInt ssa_29_ko_shifted;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_29_ko_shifted = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_29_ko_shifted = ssa_8_a2o.lshr(ssa_4_a0o);
	}
	APInt ssa_30_signmask = ssa_10_zero;
	ssa_30_signmask.setSignBit();
	APInt ssa_31_sign_o = ssa_8_a2o & ssa_30_signmask;
	APInt ssa_32_sign_z = ssa_7_a2z & ssa_30_signmask;
	bool ssa_33_sign_one = ssa_31_sign_o.eq(ssa_30_signmask);
	bool ssa_34_sign_zero = ssa_32_sign_z.eq(ssa_30_signmask);
	APInt ssa_35_topmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_35_topmask.setHighBits(ssa_4_a0o.getZExtValue());
	else
		ssa_35_topmask.setHighBits(ssa_4_a0o.getBitWidth());
	APInt ssa_36_kz_top = ssa_28_kz_shifted | ssa_35_topmask;
	APInt ssa_37_ko_top = ssa_29_ko_shifted | ssa_35_topmask;
	APInt ssa_38_ashr_kz = ssa_34_sign_zero ? ssa_36_kz_top : ssa_28_kz_shifted;
	APInt ssa_39_ashr_ko = ssa_33_sign_one ? ssa_37_ko_top : ssa_29_ko_shifted;
	bool ssa_40_a2o_zero = ssa_8_a2o.eq(ssa_10_zero);
	bool ssa_41_a2z_all = ssa_7_a2z.eq(ssa_9_allones);
	bool ssa_42_arg2_zero = ssa_40_a2o_zero && ssa_41_a2z_all;
	APInt ssa_43_a1_sign_o = ssa_6_a1o & ssa_30_signmask;
	APInt ssa_44_a1_sign_z = ssa_5_a1z & ssa_30_signmask;
	bool ssa_45_a1_sign_one = ssa_43_a1_sign_o.eq(ssa_30_signmask);
	bool ssa_46_a1_sign_zero = ssa_44_a1_sign_z.eq(ssa_30_signmask);
	bool ssa_47_sign_same_one = ssa_45_a1_sign_one && ssa_33_sign_one;
	bool ssa_48_sign_same_zero = ssa_46_a1_sign_zero && ssa_34_sign_zero;
	APInt ssa_49_sign_one_mask = ssa_47_sign_same_one ? ssa_30_signmask : ssa_10_zero;
	APInt ssa_50_sign_zero_mask = ssa_48_sign_same_zero ? ssa_30_signmask : ssa_10_zero;
	APInt ssa_51_tz1(ssa_5_a1z.getBitWidth(), ssa_5_a1z.countr_one());
	APInt ssa_52_tz2(ssa_38_ashr_kz.getBitWidth(), ssa_38_ashr_kz.countr_one());
	APInt ssa_53_tz_min = APIntOps::umin(ssa_51_tz1, ssa_52_tz2);
	APInt ssa_54_lowmask_add = ssa_10_zero;
	if (ssa_53_tz_min.ule(ssa_53_tz_min.getBitWidth()))
		ssa_54_lowmask_add.setLowBits(ssa_53_tz_min.getZExtValue());
	else
		ssa_54_lowmask_add.setLowBits(ssa_53_tz_min.getBitWidth());
	APInt ssa_55_lowmask_add_sel = ssa_14_all0 ? ssa_54_lowmask_add : ssa_10_zero;
	APInt ssa_56_to1(ssa_6_a1o.getBitWidth(), ssa_6_a1o.countr_one());
	APInt ssa_57_to2(ssa_39_ashr_ko.getBitWidth(), ssa_39_ashr_ko.countr_one());
	APInt ssa_58_oz_min = APIntOps::umin(ssa_56_to1, ssa_52_tz2);
	APInt ssa_59_zo_min = APIntOps::umin(ssa_51_tz1, ssa_57_to2);
	APInt ssa_60_lowmask_oz = ssa_10_zero;
	if (ssa_58_oz_min.ule(ssa_58_oz_min.getBitWidth()))
		ssa_60_lowmask_oz.setLowBits(ssa_58_oz_min.getZExtValue());
	else
		ssa_60_lowmask_oz.setLowBits(ssa_58_oz_min.getBitWidth());
	APInt ssa_61_lowmask_zo = ssa_10_zero;
	if (ssa_59_zo_min.ule(ssa_59_zo_min.getBitWidth()))
		ssa_61_lowmask_zo.setLowBits(ssa_59_zo_min.getZExtValue());
	else
		ssa_61_lowmask_zo.setLowBits(ssa_59_zo_min.getBitWidth());
	APInt ssa_62_lowmask_one = ssa_60_lowmask_oz | ssa_61_lowmask_zo;
	APInt ssa_63_lowmask_one_sel = ssa_14_all0 ? ssa_62_lowmask_one : ssa_10_zero;
	APInt ssa_64_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_65_lsb_mask = ssa_10_zero;
	if (ssa_64_one.ule(ssa_64_one.getBitWidth()))
		ssa_65_lsb_mask.setLowBits(ssa_64_one.getZExtValue());
	else
		ssa_65_lsb_mask.setLowBits(ssa_64_one.getBitWidth());
	APInt ssa_66_a1_lsb_o = ssa_6_a1o & ssa_65_lsb_mask;
	bool ssa_67_a1_lsb_one = ssa_66_a1_lsb_o.eq(ssa_65_lsb_mask);
	APInt ssa_68_ashr_lsb_o = ssa_39_ashr_ko & ssa_65_lsb_mask;
	bool ssa_69_ashr_lsb_one = ssa_68_ashr_lsb_o.eq(ssa_65_lsb_mask);
	bool ssa_70_lsb_both_one = ssa_67_a1_lsb_one && ssa_69_ashr_lsb_one;
	APInt ssa_71_lsb_zero_mask = ssa_70_lsb_both_one ? ssa_65_lsb_mask : ssa_10_zero;
	APInt ssa_72_lsb_zero_sel = ssa_14_all0 ? ssa_71_lsb_zero_mask : ssa_10_zero;
	APInt ssa_73_base_zero = ssa_50_sign_zero_mask | ssa_55_lowmask_add_sel;
	APInt ssa_74_base_zero2 = ssa_73_base_zero | ssa_72_lsb_zero_sel;
	APInt ssa_75_base_one = ssa_49_sign_one_mask | ssa_63_lowmask_one_sel;
	APInt ssa_76_lz1(ssa_5_a1z.getBitWidth(), ssa_5_a1z.countl_one());
	APInt ssa_77_lz2(ssa_38_ashr_kz.getBitWidth(), ssa_38_ashr_kz.countl_one());
	APInt ssa_78_lz_min = APIntOps::umin(ssa_76_lz1, ssa_77_lz2);
	APInt ssa_79_lz_min_sub = ssa_78_lz_min - ssa_64_one;
	bool ssa_80_lz_min_gt0 = ssa_78_lz_min.ugt(ssa_10_zero);
	APInt ssa_81_lz_cnt = ssa_80_lz_min_gt0 ? ssa_79_lz_min_sub : ssa_10_zero;
	APInt ssa_82_highmask_zero = ssa_10_zero;
	if (ssa_81_lz_cnt.ule(ssa_81_lz_cnt.getBitWidth()))
		ssa_82_highmask_zero.setHighBits(ssa_81_lz_cnt.getZExtValue());
	else
		ssa_82_highmask_zero.setHighBits(ssa_81_lz_cnt.getBitWidth());
	bool ssa_83_cond_zero_hi0 = ssa_14_all0 && ssa_48_sign_same_zero;
	APInt ssa_84_highmask_zero_sel = ssa_83_cond_zero_hi0 ? ssa_82_highmask_zero : ssa_10_zero;
	APInt ssa_85_base_zero3 = ssa_74_base_zero2 | ssa_84_highmask_zero_sel;
	APInt ssa_86_lo1(ssa_6_a1o.getBitWidth(), ssa_6_a1o.countl_one());
	APInt ssa_87_lo2(ssa_39_ashr_ko.getBitWidth(), ssa_39_ashr_ko.countl_one());
	APInt ssa_88_lo_min = APIntOps::umin(ssa_86_lo1, ssa_87_lo2);
	APInt ssa_89_lo_min_sub = ssa_88_lo_min - ssa_64_one;
	bool ssa_90_lo_min_gt0 = ssa_88_lo_min.ugt(ssa_10_zero);
	APInt ssa_91_lo_cnt = ssa_90_lo_min_gt0 ? ssa_89_lo_min_sub : ssa_10_zero;
	APInt ssa_92_highmask_one = ssa_10_zero;
	if (ssa_91_lo_cnt.ule(ssa_91_lo_cnt.getBitWidth()))
		ssa_92_highmask_one.setHighBits(ssa_91_lo_cnt.getZExtValue());
	else
		ssa_92_highmask_one.setHighBits(ssa_91_lo_cnt.getBitWidth());
	bool ssa_93_cond_one_hi0 = ssa_14_all0 && ssa_47_sign_same_one;
	APInt ssa_94_highmask_one_sel = ssa_93_cond_one_hi0 ? ssa_92_highmask_one : ssa_10_zero;
	APInt ssa_95_base_one2 = ssa_75_base_one | ssa_94_highmask_one_sel;
	APInt ssa_96_res0_case2 = ssa_25_case2 ? ssa_38_ashr_kz : ssa_85_base_zero3;
	APInt ssa_97_res1_case2 = ssa_25_case2 ? ssa_39_ashr_ko : ssa_95_base_one2;
	APInt ssa_98_res0_case3 = ssa_42_arg2_zero ? ssa_5_a1z : ssa_96_res0_case2;
	APInt ssa_99_res1_case3 = ssa_42_arg2_zero ? ssa_6_a1o : ssa_97_res1_case2;
	APInt ssa_100_res0 = ssa_18_all ? ssa_21_kz_exact : ssa_98_res0_case3;
	APInt ssa_101_res1 = ssa_18_all ? ssa_20_add : ssa_99_res1_case3;
	std::array<APInt, 2> ssa_102_r = std::array<APInt, 2>{ssa_100_res0, ssa_101_res1};
	return ssa_102_r;
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
