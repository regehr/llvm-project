namespace Pattern004 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_allones = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_12_u0 = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_13_u1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_14_u2 = ssa_7_a2z | ssa_8_a2o;
	bool ssa_15_e0 = ssa_12_u0.eq(ssa_9_allones);
	bool ssa_16_e1 = ssa_13_u1.eq(ssa_9_allones);
	bool ssa_17_e2 = ssa_14_u2.eq(ssa_9_allones);
	bool ssa_18_e12 = ssa_16_e1 && ssa_17_e2;
	bool ssa_19_e012 = ssa_18_e12 && ssa_15_e0;
	APInt ssa_20_x = ssa_8_a2o - ssa_6_a1o;
	APInt ssa_21_y;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_20_x.isSignBitSet()) {
		ssa_21_y = APInt(ssa_20_x.getBitWidth(), -1);
	} else if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_20_x.isSignBitClear()) {
		ssa_21_y = APInt(ssa_20_x.getBitWidth(), 0);
	} else {
		ssa_21_y = ssa_20_x.ashr(ssa_4_a0o);
	}
	APInt ssa_22_shl;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_22_shl = APInt(ssa_21_y.getBitWidth(), 0);
	} else {
		ssa_22_shl = ssa_21_y.shl(ssa_4_a0o);
	}
	bool ssa_23_exact = ssa_22_shl.eq(ssa_20_x);
	APInt ssa_24_bw(ssa_20_x.getBitWidth(), ssa_20_x.getBitWidth());
	bool ssa_25_lt = ssa_4_a0o.ult(ssa_24_bw);
	bool ssa_26_ok = ssa_23_exact && ssa_25_lt;
	APInt ssa_27_y0 = ssa_21_y ^ ssa_9_allones;
	APInt ssa_28_exr0 = ssa_26_ok ? ssa_27_y0 : ssa_9_allones;
	APInt ssa_29_exr1 = ssa_26_ok ? ssa_21_y : ssa_9_allones;
	bool ssa_30_ge = ssa_4_a0o.uge(ssa_24_bw);
	bool ssa_31_unsat_shift = ssa_30_ge && ssa_30_ge;
	APInt ssa_32_common = ssa_13_u1 & ssa_14_u2;
	APInt ssa_33_trail(ssa_32_common.getBitWidth(), ssa_32_common.countr_one());
	APInt ssa_34_n = APIntOps::umin(ssa_33_trail, ssa_4_a0o);
	APInt ssa_35_lowmask = ssa_10_zero;
	if (ssa_34_n.ule(ssa_34_n.getBitWidth()))
		ssa_35_lowmask.setLowBits(ssa_34_n.getZExtValue());
	else
		ssa_35_lowmask.setLowBits(ssa_34_n.getBitWidth());
	APInt ssa_36_a1low = ssa_6_a1o & ssa_35_lowmask;
	APInt ssa_37_a2low = ssa_8_a2o & ssa_35_lowmask;
	APInt ssa_38_xlow = ssa_37_a2low - ssa_36_a1low;
	APInt ssa_39_xlowm = ssa_38_xlow & ssa_35_lowmask;
	bool ssa_40_lnz = ssa_39_xlowm.ne(ssa_10_zero);
	bool ssa_41_unsat = ssa_31_unsat_shift || ssa_40_lnz;
	bool ssa_42_xzero = ssa_20_x.eq(ssa_10_zero);
	bool ssa_43_xones = ssa_20_x.eq(ssa_9_allones);
	bool ssa_44_cz = ssa_18_e12 && ssa_42_xzero;
	bool ssa_45_co = ssa_18_e12 && ssa_43_xones;
	APInt ssa_46_highmask = ssa_9_allones;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_46_highmask.clearLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_46_highmask.clearLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_47_a1h = ssa_5_a1z & ssa_46_highmask;
	APInt ssa_48_a2h = ssa_7_a2z & ssa_46_highmask;
	bool ssa_49_s1 = ssa_47_a1h.eq(ssa_46_highmask);
	bool ssa_50_s2 = ssa_48_a2h.eq(ssa_46_highmask);
	bool ssa_51_sz = ssa_49_s1 && ssa_50_s2;
	APInt ssa_52_bwm1 = ssa_24_bw - ssa_11_one;
	bool ssa_53_shmax = ssa_4_a0o.eq(ssa_52_bwm1);
	APInt ssa_54_d1 = ssa_6_a1o & ssa_7_a2z;
	APInt ssa_55_d2 = ssa_5_a1z & ssa_8_a2o;
	APInt ssa_56_d = ssa_54_d1 | ssa_55_d2;
	bool ssa_57_dnz = ssa_56_d.ne(ssa_10_zero);
	bool ssa_58_cm = ssa_53_shmax && ssa_57_dnz;
	APInt ssa_59_r0 = ssa_44_cz ? ssa_9_allones : ssa_10_zero;
	APInt ssa_60_r1 = ssa_44_cz ? ssa_10_zero : ssa_10_zero;
	APInt ssa_61_r0a = ssa_45_co ? ssa_10_zero : ssa_59_r0;
	APInt ssa_62_r1a = ssa_45_co ? ssa_9_allones : ssa_60_r1;
	APInt ssa_63_r0b = ssa_58_cm ? ssa_10_zero : ssa_61_r0a;
	APInt ssa_64_r1b = ssa_58_cm ? ssa_9_allones : ssa_62_r1a;
	APInt ssa_65_r0c = ssa_51_sz ? ssa_9_allones : ssa_63_r0b;
	APInt ssa_66_r1c = ssa_51_sz ? ssa_10_zero : ssa_64_r1b;
	APInt ssa_67_r0d = ssa_19_e012 ? ssa_28_exr0 : ssa_65_r0c;
	APInt ssa_68_r1d = ssa_19_e012 ? ssa_29_exr1 : ssa_66_r1c;
	APInt ssa_69_r0e = ssa_41_unsat ? ssa_9_allones : ssa_67_r0d;
	APInt ssa_70_r1e = ssa_41_unsat ? ssa_9_allones : ssa_68_r1d;
	std::array<APInt, 2> ssa_71_r = std::array<APInt, 2>{ssa_69_r0e, ssa_70_r1e};
	return ssa_71_r;
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
	APInt ssa_11_u0 = ssa_3_a0z | ssa_4_a0o;
	bool ssa_12_e0 = ssa_11_u0.eq(ssa_9_allones);
	APInt ssa_13_signmask = ssa_10_zero;
	ssa_13_signmask.setSignBit();
	APInt ssa_14_highmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_14_highmask.setHighBits(ssa_4_a0o.getZExtValue());
	else
		ssa_14_highmask.setHighBits(ssa_4_a0o.getBitWidth());
	APInt ssa_15_a1o_sign = ssa_6_a1o & ssa_13_signmask;
	APInt ssa_16_a1z_sign = ssa_5_a1z & ssa_13_signmask;
	bool ssa_17_a1_sign_one = ssa_15_a1o_sign.eq(ssa_13_signmask);
	bool ssa_18_a1_sign_zero = ssa_16_a1z_sign.eq(ssa_13_signmask);
	APInt ssa_19_a1o_sh;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_19_a1o_sh = APInt(ssa_6_a1o.getBitWidth(), 0);
	} else {
		ssa_19_a1o_sh = ssa_6_a1o.lshr(ssa_4_a0o);
	}
	APInt ssa_20_a1z_sh;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_20_a1z_sh = APInt(ssa_5_a1z.getBitWidth(), 0);
	} else {
		ssa_20_a1z_sh = ssa_5_a1z.lshr(ssa_4_a0o);
	}
	APInt ssa_21_a1o_high = ssa_17_a1_sign_one ? ssa_14_highmask : ssa_10_zero;
	APInt ssa_22_a1z_high = ssa_18_a1_sign_zero ? ssa_14_highmask : ssa_10_zero;
	APInt ssa_23_a1o_sh2 = ssa_19_a1o_sh | ssa_21_a1o_high;
	APInt ssa_24_a1z_sh2 = ssa_20_a1z_sh | ssa_22_a1z_high;
	APInt ssa_25_a1u_sh = ssa_23_a1o_sh2 | ssa_24_a1z_sh2;
	bool ssa_26_a1_known = ssa_25_a1u_sh.eq(ssa_9_allones);
	APInt ssa_27_a2o_sign = ssa_8_a2o & ssa_13_signmask;
	APInt ssa_28_a2z_sign = ssa_7_a2z & ssa_13_signmask;
	bool ssa_29_a2_sign_one = ssa_27_a2o_sign.eq(ssa_13_signmask);
	bool ssa_30_a2_sign_zero = ssa_28_a2z_sign.eq(ssa_13_signmask);
	APInt ssa_31_a2o_sh;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_31_a2o_sh = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_31_a2o_sh = ssa_8_a2o.lshr(ssa_4_a0o);
	}
	APInt ssa_32_a2z_sh;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_32_a2z_sh = APInt(ssa_7_a2z.getBitWidth(), 0);
	} else {
		ssa_32_a2z_sh = ssa_7_a2z.lshr(ssa_4_a0o);
	}
	APInt ssa_33_a2o_high = ssa_29_a2_sign_one ? ssa_14_highmask : ssa_10_zero;
	APInt ssa_34_a2z_high = ssa_30_a2_sign_zero ? ssa_14_highmask : ssa_10_zero;
	APInt ssa_35_a2o_sh2 = ssa_31_a2o_sh | ssa_33_a2o_high;
	APInt ssa_36_a2z_sh2 = ssa_32_a2z_sh | ssa_34_a2z_high;
	APInt ssa_37_a2u_sh = ssa_35_a2o_sh2 | ssa_36_a2z_sh2;
	bool ssa_38_a2_known = ssa_37_a2u_sh.eq(ssa_9_allones);
	bool ssa_39_e01 = ssa_12_e0 && ssa_26_a1_known;
	bool ssa_40_cond = ssa_39_e01 && ssa_38_a2_known;
	APInt ssa_41_a1v;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_6_a1o.isSignBitSet()) {
		ssa_41_a1v = APInt(ssa_6_a1o.getBitWidth(), -1);
	} else if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_6_a1o.isSignBitClear()) {
		ssa_41_a1v = APInt(ssa_6_a1o.getBitWidth(), 0);
	} else {
		ssa_41_a1v = ssa_6_a1o.ashr(ssa_4_a0o);
	}
	APInt ssa_42_a2v;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitSet()) {
		ssa_42_a2v = APInt(ssa_8_a2o.getBitWidth(), -1);
	} else if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth()) && ssa_8_a2o.isSignBitClear()) {
		ssa_42_a2v = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_42_a2v = ssa_8_a2o.ashr(ssa_4_a0o);
	}
	APInt ssa_43_rfull = ssa_42_a2v - ssa_41_a1v;
	APInt ssa_44_rlow = ssa_43_rfull;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_44_rlow.clearHighBits(ssa_4_a0o.getZExtValue());
	else
		ssa_44_rlow.clearHighBits(ssa_4_a0o.getBitWidth());
	APInt ssa_45_signmask_k;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_45_signmask_k = APInt(ssa_13_signmask.getBitWidth(), 0);
	} else {
		ssa_45_signmask_k = ssa_13_signmask.lshr(ssa_4_a0o);
	}
	APInt ssa_46_rsign = ssa_43_rfull & ssa_45_signmask_k;
	bool ssa_47_rsign_one = ssa_46_rsign.eq(ssa_45_signmask_k);
	APInt ssa_48_rhigh = ssa_47_rsign_one ? ssa_14_highmask : ssa_10_zero;
	APInt ssa_49_y = ssa_44_rlow | ssa_48_rhigh;
	APInt ssa_50_y0 = ssa_49_y ^ ssa_9_allones;
	APInt ssa_51_r0 = ssa_40_cond ? ssa_50_y0 : ssa_10_zero;
	APInt ssa_52_r1 = ssa_40_cond ? ssa_49_y : ssa_10_zero;
	std::array<APInt, 2> ssa_53_r = std::array<APInt, 2>{ssa_51_r0, ssa_52_r1};
	return ssa_53_r;
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
	APInt ssa_11_u0 = ssa_3_a0z | ssa_4_a0o;
	bool ssa_12_e0 = ssa_11_u0.eq(ssa_9_allones);
	APInt ssa_13_u1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_14_u2 = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_15_common = ssa_13_u1 & ssa_14_u2;
	APInt ssa_16_common_sh;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_16_common_sh = APInt(ssa_15_common.getBitWidth(), 0);
	} else {
		ssa_16_common_sh = ssa_15_common.lshr(ssa_4_a0o);
	}
	APInt ssa_17_k(ssa_16_common_sh.getBitWidth(), ssa_16_common_sh.countr_one());
	APInt ssa_18_lowmask = ssa_10_zero;
	if (ssa_17_k.ule(ssa_17_k.getBitWidth()))
		ssa_18_lowmask.setLowBits(ssa_17_k.getZExtValue());
	else
		ssa_18_lowmask.setLowBits(ssa_17_k.getBitWidth());
	APInt ssa_19_lowmask_sh;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_19_lowmask_sh = APInt(ssa_18_lowmask.getBitWidth(), 0);
	} else {
		ssa_19_lowmask_sh = ssa_18_lowmask.shl(ssa_4_a0o);
	}
	APInt ssa_20_a1v = ssa_6_a1o & ssa_19_lowmask_sh;
	APInt ssa_21_a2v = ssa_8_a2o & ssa_19_lowmask_sh;
	APInt ssa_22_x = ssa_21_a2v - ssa_20_a1v;
	APInt ssa_23_xlow = ssa_22_x & ssa_19_lowmask_sh;
	APInt ssa_24_y;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_24_y = APInt(ssa_23_xlow.getBitWidth(), 0);
	} else {
		ssa_24_y = ssa_23_xlow.lshr(ssa_4_a0o);
	}
	APInt ssa_25_r1 = ssa_12_e0 ? ssa_24_y : ssa_10_zero;
	APInt ssa_26_r0v = ssa_24_y ^ ssa_18_lowmask;
	APInt ssa_27_r0 = ssa_12_e0 ? ssa_26_r0v : ssa_10_zero;
	std::array<APInt, 2> ssa_28_r = std::array<APInt, 2>{ssa_27_r0, ssa_25_r1};
	return ssa_28_r;
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
	APInt ssa_11_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_12_u0 = ssa_3_a0z | ssa_4_a0o;
	bool ssa_13_e0 = ssa_12_u0.eq(ssa_9_allones);
	APInt ssa_14_u1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_15_unk1 = ssa_9_allones ^ ssa_14_u1;
	APInt ssa_16_max1 = ssa_6_a1o | ssa_15_unk1;
	APInt ssa_17_u2 = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_18_unk2 = ssa_9_allones ^ ssa_17_u2;
	APInt ssa_19_max2 = ssa_8_a2o | ssa_18_unk2;
	APInt ssa_20_min2 = ssa_8_a2o & ssa_9_allones;
	APInt ssa_21_signmask = ssa_10_zero;
	ssa_21_signmask.setSignBit();
	APInt ssa_22_a1z_sign = ssa_5_a1z & ssa_21_signmask;
	APInt ssa_23_a2z_sign = ssa_7_a2z & ssa_21_signmask;
	APInt ssa_24_a1o_sign = ssa_6_a1o & ssa_21_signmask;
	bool ssa_25_a1_sign_zero = ssa_22_a1z_sign.eq(ssa_21_signmask);
	bool ssa_26_a2_sign_zero = ssa_23_a2z_sign.eq(ssa_21_signmask);
	bool ssa_27_a1_sign_one = ssa_24_a1o_sign.eq(ssa_21_signmask);
	APInt ssa_28_pow;
	if (ssa_4_a0o.uge(ssa_4_a0o.getBitWidth())) {
		ssa_28_pow = APInt(ssa_11_one.getBitWidth(), 0);
	} else {
		ssa_28_pow = ssa_11_one.shl(ssa_4_a0o);
	}
	bool ssa_29_borrow;
	ssa_16_max1.usub_ov(ssa_20_min2, ssa_29_borrow);
	APInt ssa_30_diff = ssa_16_max1 - ssa_20_min2;
	bool ssa_31_diff_lt = ssa_30_diff.ult(ssa_28_pow);
	bool ssa_32_cond1 = ssa_29_borrow || ssa_31_diff_lt;
	bool ssa_33_c01 = ssa_13_e0 && ssa_25_a1_sign_zero;
	bool ssa_34_c02 = ssa_33_c01 && ssa_26_a2_sign_zero;
	bool ssa_35_cond_pos = ssa_34_c02 && ssa_32_cond1;
	APInt ssa_36_lz2(ssa_19_max2.getBitWidth(), ssa_19_max2.countl_zero());
	APInt ssa_37_Lpos = ssa_36_lz2 + ssa_4_a0o;
	APInt ssa_38_r0v_pos = ssa_10_zero;
	if (ssa_37_Lpos.ule(ssa_37_Lpos.getBitWidth()))
		ssa_38_r0v_pos.setHighBits(ssa_37_Lpos.getZExtValue());
	else
		ssa_38_r0v_pos.setHighBits(ssa_37_Lpos.getBitWidth());
	bool ssa_39_a2_zero = ssa_7_a2z.eq(ssa_9_allones);
	APInt ssa_40_l1(ssa_6_a1o.getBitWidth(), ssa_6_a1o.countl_one());
	bool ssa_41_l1_gt1 = ssa_40_l1.ugt(ssa_11_one);
	bool ssa_42_c11 = ssa_13_e0 && ssa_27_a1_sign_one;
	bool ssa_43_c12 = ssa_42_c11 && ssa_39_a2_zero;
	bool ssa_44_cond_neg = ssa_43_c12 && ssa_41_l1_gt1;
	APInt ssa_45_l1m1 = ssa_40_l1 - ssa_11_one;
	APInt ssa_46_Lneg = ssa_45_l1m1 + ssa_4_a0o;
	APInt ssa_47_r0v_neg = ssa_10_zero;
	if (ssa_46_Lneg.ule(ssa_46_Lneg.getBitWidth()))
		ssa_47_r0v_neg.setHighBits(ssa_46_Lneg.getZExtValue());
	else
		ssa_47_r0v_neg.setHighBits(ssa_46_Lneg.getBitWidth());
	APInt ssa_48_r0c = ssa_35_cond_pos ? ssa_38_r0v_pos : ssa_10_zero;
	APInt ssa_49_r0d = ssa_44_cond_neg ? ssa_47_r0v_neg : ssa_48_r0c;
	APInt ssa_50_r1c = ssa_35_cond_pos ? ssa_10_zero : ssa_10_zero;
	APInt ssa_51_r1d = ssa_44_cond_neg ? ssa_10_zero : ssa_50_r1c;
	APInt ssa_52_d1 = ssa_6_a1o & ssa_7_a2z;
	APInt ssa_53_d2 = ssa_5_a1z & ssa_8_a2o;
	APInt ssa_54_d = ssa_52_d1 | ssa_53_d2;
	APInt ssa_55_lowmask = ssa_10_zero;
	if (ssa_4_a0o.ule(ssa_4_a0o.getBitWidth()))
		ssa_55_lowmask.setLowBits(ssa_4_a0o.getZExtValue());
	else
		ssa_55_lowmask.setLowBits(ssa_4_a0o.getBitWidth());
	APInt ssa_56_dlow = ssa_54_d & ssa_55_lowmask;
	bool ssa_57_dnonzero = ssa_56_dlow.ne(ssa_10_zero);
	bool ssa_58_dunsat = ssa_13_e0 && ssa_57_dnonzero;
	APInt ssa_59_bw(ssa_3_a0z.getBitWidth(), ssa_3_a0z.getBitWidth());
	bool ssa_60_ge = ssa_4_a0o.uge(ssa_59_bw);
	bool ssa_61_unsat_shift = ssa_13_e0 && ssa_60_ge;
	bool ssa_62_unsat = ssa_58_dunsat || ssa_61_unsat_shift;
	APInt ssa_63_r0 = ssa_62_unsat ? ssa_9_allones : ssa_49_r0d;
	APInt ssa_64_r1 = ssa_62_unsat ? ssa_9_allones : ssa_51_r1d;
	std::array<APInt, 2> ssa_65_r = std::array<APInt, 2>{ssa_63_r0, ssa_64_r1};
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
	std::array<APInt, 2> ssa_5_ = partial_solution_2(ssa_0_, ssa_1_, ssa_2_);
	std::array<APInt, 2> ssa_6_ = partial_solution_3(ssa_0_, ssa_1_, ssa_2_);
	std::array<APInt, 2> ssa_7_ = meet(ssa_3_, ssa_4_);
	std::array<APInt, 2> ssa_8_ = meet(ssa_7_, ssa_5_);
	std::array<APInt, 2> ssa_9_ = meet(ssa_8_, ssa_6_);
	return ssa_9_;
}
}
