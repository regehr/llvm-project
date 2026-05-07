namespace Pattern022 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_known1 = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_10_ones1 = APInt::getAllOnes(ssa_5_a1z.getBitWidth());
	bool ssa_11_isconst = ssa_9_known1.eq(ssa_10_ones1);
	APInt ssa_12_bw(ssa_5_a1z.getBitWidth(), ssa_5_a1z.getBitWidth());
	bool ssa_13_shamt_lt_bw = ssa_6_a1o.ult(ssa_12_bw);
	bool ssa_14_invalid = ssa_6_a1o.uge(ssa_12_bw);
	bool ssa_15_valid_const = ssa_11_isconst && ssa_13_shamt_lt_bw;
	APInt ssa_16_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_17_one(ssa_3_a0z.getBitWidth(), 1);
	bool ssa_18_true = ssa_16_zero.eq(ssa_16_zero);
	bool ssa_19_not_isconst = ssa_11_isconst != ssa_18_true;
	APInt ssa_20_a0_known = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_21_a2_known = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_22_ones0 = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_23_ones2 = APInt::getAllOnes(ssa_7_a2z.getBitWidth());
	bool ssa_24_a0_const = ssa_20_a0_known.eq(ssa_22_ones0);
	bool ssa_25_a2_const = ssa_21_a2_known.eq(ssa_23_ones2);
	bool ssa_26_const_ops = ssa_24_a0_const && ssa_25_a2_const;
	APInt ssa_27_lowmask_lb = ssa_16_zero;
	if (ssa_6_a1o.ule(ssa_6_a1o.getBitWidth()))
		ssa_27_lowmask_lb.setLowBits(ssa_6_a1o.getZExtValue());
	else
		ssa_27_lowmask_lb.setLowBits(ssa_6_a1o.getBitWidth());
	APInt ssa_28_res0_lb = ssa_3_a0z & ssa_27_lowmask_lb;
	APInt ssa_29_res1_lb = ssa_4_a0o & ssa_27_lowmask_lb;
	APInt ssa_30_shl_z_raw;
	if (ssa_6_a1o.uge(ssa_6_a1o.getBitWidth())) {
		ssa_30_shl_z_raw = APInt(ssa_7_a2z.getBitWidth(), 0);
	} else {
		ssa_30_shl_z_raw = ssa_7_a2z.shl(ssa_6_a1o);
	}
	APInt ssa_31_shl_o;
	if (ssa_6_a1o.uge(ssa_6_a1o.getBitWidth())) {
		ssa_31_shl_o = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_31_shl_o = ssa_8_a2o.shl(ssa_6_a1o);
	}
	APInt ssa_32_shl_z = ssa_30_shl_z_raw | ssa_27_lowmask_lb;
	APInt ssa_33_s1 = ssa_6_a1o + ssa_17_one;
	APInt ssa_34_lowmask1 = ssa_16_zero;
	if (ssa_33_s1.ule(ssa_33_s1.getBitWidth()))
		ssa_34_lowmask1.setLowBits(ssa_33_s1.getZExtValue());
	else
		ssa_34_lowmask1.setLowBits(ssa_33_s1.getBitWidth());
	APInt ssa_35_a0z_low = ssa_3_a0z & ssa_34_lowmask1;
	APInt ssa_36_a0o_low = ssa_4_a0o & ssa_34_lowmask1;
	APInt ssa_37_shl_z_low = ssa_32_shl_z & ssa_34_lowmask1;
	APInt ssa_38_shl_o_low = ssa_31_shl_o & ssa_34_lowmask1;
	APInt ssa_39_t0 = ssa_35_a0z_low & ssa_37_shl_z_low;
	APInt ssa_40_t1 = ssa_36_a0o_low & ssa_38_shl_o_low;
	APInt ssa_41_res0_const = ssa_39_t0 | ssa_40_t1;
	APInt ssa_42_t2 = ssa_35_a0z_low & ssa_38_shl_o_low;
	APInt ssa_43_t3 = ssa_36_a0o_low & ssa_37_shl_z_low;
	APInt ssa_44_res1_const = ssa_42_t2 | ssa_43_t3;
	APInt ssa_45_shl_z_raw_s1;
	if (ssa_33_s1.uge(ssa_33_s1.getBitWidth())) {
		ssa_45_shl_z_raw_s1 = APInt(ssa_7_a2z.getBitWidth(), 0);
	} else {
		ssa_45_shl_z_raw_s1 = ssa_7_a2z.shl(ssa_33_s1);
	}
	APInt ssa_46_shl_o_s1;
	if (ssa_33_s1.uge(ssa_33_s1.getBitWidth())) {
		ssa_46_shl_o_s1 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_46_shl_o_s1 = ssa_8_a2o.shl(ssa_33_s1);
	}
	APInt ssa_47_lowmask_lb_s1 = ssa_16_zero;
	if (ssa_33_s1.ule(ssa_33_s1.getBitWidth()))
		ssa_47_lowmask_lb_s1.setLowBits(ssa_33_s1.getZExtValue());
	else
		ssa_47_lowmask_lb_s1.setLowBits(ssa_33_s1.getBitWidth());
	APInt ssa_48_shl_z_s1 = ssa_45_shl_z_raw_s1 | ssa_47_lowmask_lb_s1;
	APInt ssa_49_s2 = ssa_33_s1 + ssa_17_one;
	APInt ssa_50_lowmask2 = ssa_16_zero;
	if (ssa_49_s2.ule(ssa_49_s2.getBitWidth()))
		ssa_50_lowmask2.setLowBits(ssa_49_s2.getZExtValue());
	else
		ssa_50_lowmask2.setLowBits(ssa_49_s2.getBitWidth());
	APInt ssa_51_a0z_low_s1 = ssa_3_a0z & ssa_50_lowmask2;
	APInt ssa_52_a0o_low_s1 = ssa_4_a0o & ssa_50_lowmask2;
	APInt ssa_53_shl_z_low_s1 = ssa_48_shl_z_s1 & ssa_50_lowmask2;
	APInt ssa_54_shl_o_low_s1 = ssa_46_shl_o_s1 & ssa_50_lowmask2;
	APInt ssa_55_u0 = ssa_51_a0z_low_s1 & ssa_53_shl_z_low_s1;
	APInt ssa_56_u1 = ssa_52_a0o_low_s1 & ssa_54_shl_o_low_s1;
	APInt ssa_57_res0_const_s1 = ssa_55_u0 | ssa_56_u1;
	APInt ssa_58_u2 = ssa_51_a0z_low_s1 & ssa_54_shl_o_low_s1;
	APInt ssa_59_u3 = ssa_52_a0o_low_s1 & ssa_53_shl_z_low_s1;
	APInt ssa_60_res1_const_s1 = ssa_58_u2 | ssa_59_u3;
	APInt ssa_61_res0_meet = ssa_41_res0_const & ssa_57_res0_const_s1;
	APInt ssa_62_res1_meet = ssa_44_res1_const & ssa_60_res1_const_s1;
	APInt ssa_63_shl_a2;
	if (ssa_6_a1o.uge(ssa_6_a1o.getBitWidth())) {
		ssa_63_shl_a2 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_63_shl_a2 = ssa_8_a2o.shl(ssa_6_a1o);
	}
	APInt ssa_64_res_val = ssa_4_a0o + ssa_63_shl_a2;
	APInt ssa_65_res0_exact = ssa_64_res_val ^ ssa_22_ones0;
	APInt ssa_66_res1_exact = ssa_64_res_val | ssa_16_zero;
	APInt ssa_67_shl_a2_s1;
	if (ssa_33_s1.uge(ssa_33_s1.getBitWidth())) {
		ssa_67_shl_a2_s1 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_67_shl_a2_s1 = ssa_8_a2o.shl(ssa_33_s1);
	}
	APInt ssa_68_res_val_s1 = ssa_4_a0o + ssa_67_shl_a2_s1;
	APInt ssa_69_res0_exact_s1 = ssa_68_res_val_s1 ^ ssa_22_ones0;
	APInt ssa_70_res1_exact_s1 = ssa_68_res_val_s1 | ssa_16_zero;
	APInt ssa_71_res0_exact_meet = ssa_65_res0_exact & ssa_69_res0_exact_s1;
	APInt ssa_72_res1_exact_meet = ssa_66_res1_exact & ssa_70_res1_exact_s1;
	APInt ssa_73_res0_const_use = ssa_26_const_ops ? ssa_65_res0_exact : ssa_41_res0_const;
	APInt ssa_74_res1_const_use = ssa_26_const_ops ? ssa_66_res1_exact : ssa_44_res1_const;
	APInt ssa_75_res0_const_s1_use = ssa_26_const_ops ? ssa_69_res0_exact_s1 : ssa_57_res0_const_s1;
	APInt ssa_76_res1_const_s1_use = ssa_26_const_ops ? ssa_70_res1_exact_s1 : ssa_60_res1_const_s1;
	APInt ssa_77_res0_meet_use = ssa_26_const_ops ? ssa_71_res0_exact_meet : ssa_61_res0_meet;
	APInt ssa_78_res1_meet_use = ssa_26_const_ops ? ssa_72_res1_exact_meet : ssa_62_res1_meet;
	APInt ssa_79_known1_or1 = ssa_9_known1 | ssa_17_one;
	bool ssa_80_cond_low1 = ssa_79_known1_or1.eq(ssa_10_ones1);
	bool ssa_81_s1_lt_bw = ssa_33_s1.ult(ssa_12_bw);
	bool ssa_82_not_s1_lt_bw = ssa_81_s1_lt_bw != ssa_18_true;
	bool ssa_83_cond_pair_tmp = ssa_80_cond_low1 && ssa_81_s1_lt_bw;
	bool ssa_84_cond_pair = ssa_83_cond_pair_tmp && ssa_19_not_isconst;
	bool ssa_85_cond_force_tmp = ssa_80_cond_low1 && ssa_82_not_s1_lt_bw;
	bool ssa_86_cond_force = ssa_85_cond_force_tmp && ssa_19_not_isconst;
	APInt ssa_87_res0_tmp1 = ssa_84_cond_pair ? ssa_77_res0_meet_use : ssa_28_res0_lb;
	APInt ssa_88_res1_tmp1 = ssa_84_cond_pair ? ssa_78_res1_meet_use : ssa_29_res1_lb;
	APInt ssa_89_res0_tmp2 = ssa_86_cond_force ? ssa_73_res0_const_use : ssa_87_res0_tmp1;
	APInt ssa_90_res1_tmp2 = ssa_86_cond_force ? ssa_74_res1_const_use : ssa_88_res1_tmp1;
	APInt ssa_91_res0_pre = ssa_15_valid_const ? ssa_73_res0_const_use : ssa_89_res0_tmp2;
	APInt ssa_92_res1_pre = ssa_15_valid_const ? ssa_74_res1_const_use : ssa_90_res1_tmp2;
	APInt ssa_93_all = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_94_res0 = ssa_14_invalid ? ssa_93_all : ssa_91_res0_pre;
	APInt ssa_95_res1 = ssa_14_invalid ? ssa_93_all : ssa_92_res1_pre;
	std::array<APInt, 2> ssa_96_r = std::array<APInt, 2>{ssa_94_res0, ssa_95_res1};
	return ssa_96_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_a2_known = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_11_ones2 = APInt::getAllOnes(ssa_7_a2z.getBitWidth());
	bool ssa_12_a2_const = ssa_10_a2_known.eq(ssa_11_ones2);
	bool ssa_13_a2_is_zero = ssa_8_a2o.eq(ssa_9_zero);
	bool ssa_14_a2_zero_const = ssa_12_a2_const && ssa_13_a2_is_zero;
	APInt ssa_15_a0_known = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_16_ones0 = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	bool ssa_17_a0_const = ssa_15_a0_known.eq(ssa_16_ones0);
	bool ssa_18_a0_is_zero = ssa_4_a0o.eq(ssa_9_zero);
	bool ssa_19_a0_zero_const = ssa_17_a0_const && ssa_18_a0_is_zero;
	APInt ssa_20_a1_known = ssa_5_a1z | ssa_6_a1o;
	APInt ssa_21_ones1 = APInt::getAllOnes(ssa_5_a1z.getBitWidth());
	bool ssa_22_a1_const = ssa_20_a1_known.eq(ssa_21_ones1);
	bool ssa_23_cond_shl = ssa_19_a0_zero_const && ssa_22_a1_const;
	APInt ssa_24_lowmask = ssa_9_zero;
	if (ssa_6_a1o.ule(ssa_6_a1o.getBitWidth()))
		ssa_24_lowmask.setLowBits(ssa_6_a1o.getZExtValue());
	else
		ssa_24_lowmask.setLowBits(ssa_6_a1o.getBitWidth());
	APInt ssa_25_shl_z_raw;
	if (ssa_6_a1o.uge(ssa_6_a1o.getBitWidth())) {
		ssa_25_shl_z_raw = APInt(ssa_7_a2z.getBitWidth(), 0);
	} else {
		ssa_25_shl_z_raw = ssa_7_a2z.shl(ssa_6_a1o);
	}
	APInt ssa_26_shl_o;
	if (ssa_6_a1o.uge(ssa_6_a1o.getBitWidth())) {
		ssa_26_shl_o = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_26_shl_o = ssa_8_a2o.shl(ssa_6_a1o);
	}
	APInt ssa_27_shl_z = ssa_25_shl_z_raw | ssa_24_lowmask;
	APInt ssa_28_ctz_a0z(ssa_3_a0z.getBitWidth(), ssa_3_a0z.countr_one());
	APInt ssa_29_clz_a2z(ssa_7_a2z.getBitWidth(), ssa_7_a2z.countl_one());
	APInt ssa_30_bw(ssa_3_a0z.getBitWidth(), ssa_3_a0z.getBitWidth());
	APInt ssa_31_tmp0 = ssa_30_bw - ssa_29_clz_a2z;
	APInt ssa_32_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_33_max_bit = ssa_31_tmp0 - ssa_32_one;
	APInt ssa_34_max_bit_sh = ssa_33_max_bit + ssa_6_a1o;
	bool ssa_35_cond_range0 = ssa_34_max_bit_sh.ult(ssa_28_ctz_a0z);
	bool ssa_36_cond_range = ssa_35_cond_range0 && ssa_22_a1_const;
	APInt ssa_37_highmask = ssa_16_ones0;
	if (ssa_28_ctz_a0z.ule(ssa_28_ctz_a0z.getBitWidth()))
		ssa_37_highmask.clearLowBits(ssa_28_ctz_a0z.getZExtValue());
	else
		ssa_37_highmask.clearLowBits(ssa_28_ctz_a0z.getBitWidth());
	APInt ssa_38_res0_high = ssa_3_a0z & ssa_37_highmask;
	APInt ssa_39_res1_high = ssa_4_a0o & ssa_37_highmask;
	APInt ssa_40_res0_base = ssa_36_cond_range ? ssa_38_res0_high : ssa_9_zero;
	APInt ssa_41_res1_base = ssa_36_cond_range ? ssa_39_res1_high : ssa_9_zero;
	APInt ssa_42_res0_shl = ssa_23_cond_shl ? ssa_27_shl_z : ssa_40_res0_base;
	APInt ssa_43_res1_shl = ssa_23_cond_shl ? ssa_26_shl_o : ssa_41_res1_base;
	APInt ssa_44_res0 = ssa_14_a2_zero_const ? ssa_3_a0z : ssa_42_res0_shl;
	APInt ssa_45_res1 = ssa_14_a2_zero_const ? ssa_4_a0o : ssa_43_res1_shl;
	std::array<APInt, 2> ssa_46_r = std::array<APInt, 2>{ssa_44_res0, ssa_45_res1};
	return ssa_46_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_11_bw(ssa_5_a1z.getBitWidth(), ssa_5_a1z.getBitWidth());
	APInt ssa_12_bw_m1 = ssa_11_bw - ssa_10_one;
	APInt ssa_13_clz_bw_m1(ssa_12_bw_m1.getBitWidth(), ssa_12_bw_m1.countl_zero());
	APInt ssa_14_num_bits = ssa_11_bw - ssa_13_clz_bw_m1;
	APInt ssa_15_lowmask_sh = ssa_9_zero;
	if (ssa_14_num_bits.ule(ssa_14_num_bits.getBitWidth()))
		ssa_15_lowmask_sh.setLowBits(ssa_14_num_bits.getZExtValue());
	else
		ssa_15_lowmask_sh.setLowBits(ssa_14_num_bits.getBitWidth());
	APInt ssa_16_all1 = APInt::getAllOnes(ssa_5_a1z.getBitWidth());
	APInt ssa_17_highmask_sh = ssa_16_all1;
	if (ssa_14_num_bits.ule(ssa_14_num_bits.getBitWidth()))
		ssa_17_highmask_sh.clearLowBits(ssa_14_num_bits.getZExtValue());
	else
		ssa_17_highmask_sh.clearLowBits(ssa_14_num_bits.getBitWidth());
	APInt ssa_18_a1o_high = ssa_6_a1o & ssa_17_highmask_sh;
	bool ssa_19_a1o_high_zero = ssa_18_a1o_high.eq(ssa_9_zero);
	APInt ssa_20_a1o_low = ssa_6_a1o & ssa_15_lowmask_sh;
	APInt ssa_21_a1z_ref = ssa_5_a1z | ssa_17_highmask_sh;
	APInt ssa_22_known1 = ssa_21_a1z_ref | ssa_20_a1o_low;
	bool ssa_23_isconst = ssa_22_known1.eq(ssa_16_all1);
	bool ssa_24_shamt_lt_bw = ssa_20_a1o_low.ult(ssa_11_bw);
	bool ssa_25_invalid = ssa_20_a1o_low.uge(ssa_11_bw);
	bool ssa_26_valid_const = ssa_23_isconst && ssa_24_shamt_lt_bw;
	bool ssa_27_true = ssa_9_zero.eq(ssa_9_zero);
	bool ssa_28_not_isconst = ssa_23_isconst != ssa_27_true;
	APInt ssa_29_a0_known = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_30_a2_known = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_31_ones0 = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_32_ones2 = APInt::getAllOnes(ssa_7_a2z.getBitWidth());
	bool ssa_33_a0_const = ssa_29_a0_known.eq(ssa_31_ones0);
	bool ssa_34_a2_const = ssa_30_a2_known.eq(ssa_32_ones2);
	bool ssa_35_const_ops = ssa_33_a0_const && ssa_34_a2_const;
	APInt ssa_36_lowmask_lb = ssa_9_zero;
	if (ssa_20_a1o_low.ule(ssa_20_a1o_low.getBitWidth()))
		ssa_36_lowmask_lb.setLowBits(ssa_20_a1o_low.getZExtValue());
	else
		ssa_36_lowmask_lb.setLowBits(ssa_20_a1o_low.getBitWidth());
	APInt ssa_37_res0_lb = ssa_3_a0z & ssa_36_lowmask_lb;
	APInt ssa_38_res1_lb = ssa_4_a0o & ssa_36_lowmask_lb;
	APInt ssa_39_shl_z_raw;
	if (ssa_20_a1o_low.uge(ssa_20_a1o_low.getBitWidth())) {
		ssa_39_shl_z_raw = APInt(ssa_7_a2z.getBitWidth(), 0);
	} else {
		ssa_39_shl_z_raw = ssa_7_a2z.shl(ssa_20_a1o_low);
	}
	APInt ssa_40_shl_o;
	if (ssa_20_a1o_low.uge(ssa_20_a1o_low.getBitWidth())) {
		ssa_40_shl_o = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_40_shl_o = ssa_8_a2o.shl(ssa_20_a1o_low);
	}
	APInt ssa_41_shl_z = ssa_39_shl_z_raw | ssa_36_lowmask_lb;
	APInt ssa_42_mask_carry0 = ssa_3_a0z | ssa_41_shl_z;
	APInt ssa_43_p(ssa_42_mask_carry0.getBitWidth(), ssa_42_mask_carry0.countr_one());
	APInt ssa_44_lowmask_p = ssa_9_zero;
	if (ssa_43_p.ule(ssa_43_p.getBitWidth()))
		ssa_44_lowmask_p.setLowBits(ssa_43_p.getZExtValue());
	else
		ssa_44_lowmask_p.setLowBits(ssa_43_p.getBitWidth());
	APInt ssa_45_a0z_p = ssa_3_a0z & ssa_44_lowmask_p;
	APInt ssa_46_a0o_p = ssa_4_a0o & ssa_44_lowmask_p;
	APInt ssa_47_shl_z_p = ssa_41_shl_z & ssa_44_lowmask_p;
	APInt ssa_48_shl_o_p = ssa_40_shl_o & ssa_44_lowmask_p;
	APInt ssa_49_k0 = ssa_45_a0z_p & ssa_47_shl_z_p;
	APInt ssa_50_k1 = ssa_46_a0o_p & ssa_48_shl_o_p;
	APInt ssa_51_res0_pref = ssa_49_k0 | ssa_50_k1;
	APInt ssa_52_k2 = ssa_45_a0z_p & ssa_48_shl_o_p;
	APInt ssa_53_k3 = ssa_46_a0o_p & ssa_47_shl_z_p;
	APInt ssa_54_res1_pref = ssa_52_k2 | ssa_53_k3;
	APInt ssa_55_s1 = ssa_20_a1o_low + ssa_10_one;
	APInt ssa_56_lowmask1 = ssa_9_zero;
	if (ssa_55_s1.ule(ssa_55_s1.getBitWidth()))
		ssa_56_lowmask1.setLowBits(ssa_55_s1.getZExtValue());
	else
		ssa_56_lowmask1.setLowBits(ssa_55_s1.getBitWidth());
	APInt ssa_57_a0z_low = ssa_3_a0z & ssa_56_lowmask1;
	APInt ssa_58_a0o_low = ssa_4_a0o & ssa_56_lowmask1;
	APInt ssa_59_shl_z_low = ssa_41_shl_z & ssa_56_lowmask1;
	APInt ssa_60_shl_o_low = ssa_40_shl_o & ssa_56_lowmask1;
	APInt ssa_61_t0 = ssa_57_a0z_low & ssa_59_shl_z_low;
	APInt ssa_62_t1 = ssa_58_a0o_low & ssa_60_shl_o_low;
	APInt ssa_63_res0_const = ssa_61_t0 | ssa_62_t1;
	APInt ssa_64_t2 = ssa_57_a0z_low & ssa_60_shl_o_low;
	APInt ssa_65_t3 = ssa_58_a0o_low & ssa_59_shl_z_low;
	APInt ssa_66_res1_const = ssa_64_t2 | ssa_65_t3;
	APInt ssa_67_shl_z_raw_s1;
	if (ssa_55_s1.uge(ssa_55_s1.getBitWidth())) {
		ssa_67_shl_z_raw_s1 = APInt(ssa_7_a2z.getBitWidth(), 0);
	} else {
		ssa_67_shl_z_raw_s1 = ssa_7_a2z.shl(ssa_55_s1);
	}
	APInt ssa_68_shl_o_s1;
	if (ssa_55_s1.uge(ssa_55_s1.getBitWidth())) {
		ssa_68_shl_o_s1 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_68_shl_o_s1 = ssa_8_a2o.shl(ssa_55_s1);
	}
	APInt ssa_69_lowmask_lb_s1 = ssa_9_zero;
	if (ssa_55_s1.ule(ssa_55_s1.getBitWidth()))
		ssa_69_lowmask_lb_s1.setLowBits(ssa_55_s1.getZExtValue());
	else
		ssa_69_lowmask_lb_s1.setLowBits(ssa_55_s1.getBitWidth());
	APInt ssa_70_shl_z_s1 = ssa_67_shl_z_raw_s1 | ssa_69_lowmask_lb_s1;
	APInt ssa_71_s2 = ssa_55_s1 + ssa_10_one;
	APInt ssa_72_lowmask2 = ssa_9_zero;
	if (ssa_71_s2.ule(ssa_71_s2.getBitWidth()))
		ssa_72_lowmask2.setLowBits(ssa_71_s2.getZExtValue());
	else
		ssa_72_lowmask2.setLowBits(ssa_71_s2.getBitWidth());
	APInt ssa_73_a0z_low_s1 = ssa_3_a0z & ssa_72_lowmask2;
	APInt ssa_74_a0o_low_s1 = ssa_4_a0o & ssa_72_lowmask2;
	APInt ssa_75_shl_z_low_s1 = ssa_70_shl_z_s1 & ssa_72_lowmask2;
	APInt ssa_76_shl_o_low_s1 = ssa_68_shl_o_s1 & ssa_72_lowmask2;
	APInt ssa_77_u0 = ssa_73_a0z_low_s1 & ssa_75_shl_z_low_s1;
	APInt ssa_78_u1 = ssa_74_a0o_low_s1 & ssa_76_shl_o_low_s1;
	APInt ssa_79_res0_const_s1 = ssa_77_u0 | ssa_78_u1;
	APInt ssa_80_u2 = ssa_73_a0z_low_s1 & ssa_76_shl_o_low_s1;
	APInt ssa_81_u3 = ssa_74_a0o_low_s1 & ssa_75_shl_z_low_s1;
	APInt ssa_82_res1_const_s1 = ssa_80_u2 | ssa_81_u3;
	APInt ssa_83_res0_meet = ssa_63_res0_const & ssa_79_res0_const_s1;
	APInt ssa_84_res1_meet = ssa_66_res1_const & ssa_82_res1_const_s1;
	APInt ssa_85_shl_a2;
	if (ssa_20_a1o_low.uge(ssa_20_a1o_low.getBitWidth())) {
		ssa_85_shl_a2 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_85_shl_a2 = ssa_8_a2o.shl(ssa_20_a1o_low);
	}
	APInt ssa_86_res_val = ssa_4_a0o + ssa_85_shl_a2;
	APInt ssa_87_res0_exact = ssa_86_res_val ^ ssa_31_ones0;
	APInt ssa_88_res1_exact = ssa_86_res_val | ssa_9_zero;
	APInt ssa_89_shl_a2_s1;
	if (ssa_55_s1.uge(ssa_55_s1.getBitWidth())) {
		ssa_89_shl_a2_s1 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_89_shl_a2_s1 = ssa_8_a2o.shl(ssa_55_s1);
	}
	APInt ssa_90_res_val_s1 = ssa_4_a0o + ssa_89_shl_a2_s1;
	APInt ssa_91_res0_exact_s1 = ssa_90_res_val_s1 ^ ssa_31_ones0;
	APInt ssa_92_res1_exact_s1 = ssa_90_res_val_s1 | ssa_9_zero;
	APInt ssa_93_res0_exact_meet = ssa_87_res0_exact & ssa_91_res0_exact_s1;
	APInt ssa_94_res1_exact_meet = ssa_88_res1_exact & ssa_92_res1_exact_s1;
	APInt ssa_95_res0_const_use = ssa_35_const_ops ? ssa_87_res0_exact : ssa_63_res0_const;
	APInt ssa_96_res1_const_use = ssa_35_const_ops ? ssa_88_res1_exact : ssa_66_res1_const;
	APInt ssa_97_res0_const_s1_use = ssa_35_const_ops ? ssa_91_res0_exact_s1 : ssa_79_res0_const_s1;
	APInt ssa_98_res1_const_s1_use = ssa_35_const_ops ? ssa_92_res1_exact_s1 : ssa_82_res1_const_s1;
	APInt ssa_99_res0_meet_use = ssa_35_const_ops ? ssa_93_res0_exact_meet : ssa_83_res0_meet;
	APInt ssa_100_res1_meet_use = ssa_35_const_ops ? ssa_94_res1_exact_meet : ssa_84_res1_meet;
	APInt ssa_101_known1_or1 = ssa_22_known1 | ssa_10_one;
	bool ssa_102_cond_low1 = ssa_101_known1_or1.eq(ssa_16_all1);
	bool ssa_103_s1_lt_bw = ssa_55_s1.ult(ssa_11_bw);
	bool ssa_104_not_s1_lt_bw = ssa_103_s1_lt_bw != ssa_27_true;
	bool ssa_105_cond_pair_tmp = ssa_102_cond_low1 && ssa_103_s1_lt_bw;
	bool ssa_106_cond_pair = ssa_105_cond_pair_tmp && ssa_28_not_isconst;
	bool ssa_107_cond_force_tmp = ssa_102_cond_low1 && ssa_104_not_s1_lt_bw;
	bool ssa_108_cond_force = ssa_107_cond_force_tmp && ssa_28_not_isconst;
	APInt ssa_109_res0_tmp1 = ssa_106_cond_pair ? ssa_99_res0_meet_use : ssa_37_res0_lb;
	APInt ssa_110_res1_tmp1 = ssa_106_cond_pair ? ssa_100_res1_meet_use : ssa_38_res1_lb;
	APInt ssa_111_res0_tmp2 = ssa_108_cond_force ? ssa_95_res0_const_use : ssa_109_res0_tmp1;
	APInt ssa_112_res1_tmp2 = ssa_108_cond_force ? ssa_96_res1_const_use : ssa_110_res1_tmp1;
	APInt ssa_113_res0_pre = ssa_26_valid_const ? ssa_95_res0_const_use : ssa_111_res0_tmp2;
	APInt ssa_114_res1_pre = ssa_26_valid_const ? ssa_96_res1_const_use : ssa_112_res1_tmp2;
	APInt ssa_115_all0 = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	APInt ssa_116_res0 = ssa_25_invalid ? ssa_115_all0 : ssa_113_res0_pre;
	APInt ssa_117_res1 = ssa_25_invalid ? ssa_115_all0 : ssa_114_res1_pre;
	APInt ssa_118_res0_base = ssa_116_res0 | ssa_37_res0_lb;
	APInt ssa_119_res1_base = ssa_117_res1 | ssa_38_res1_lb;
	APInt ssa_120_res0_add = ssa_26_valid_const ? ssa_51_res0_pref : ssa_9_zero;
	APInt ssa_121_res1_add = ssa_26_valid_const ? ssa_54_res1_pref : ssa_9_zero;
	APInt ssa_122_res0_or = ssa_118_res0_base | ssa_120_res0_add;
	APInt ssa_123_res1_or = ssa_119_res1_base | ssa_121_res1_add;
	APInt ssa_124_res0_final = ssa_19_a1o_high_zero ? ssa_122_res0_or : ssa_115_all0;
	APInt ssa_125_res1_final = ssa_19_a1o_high_zero ? ssa_123_res1_or : ssa_115_all0;
	std::array<APInt, 2> ssa_126_r = std::array<APInt, 2>{ssa_124_res0_final, ssa_125_res1_final};
	return ssa_126_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0z = ssa_0_arg0[0];
	APInt ssa_4_a0o = ssa_0_arg0[1];
	APInt ssa_5_a1z = ssa_1_arg1[0];
	APInt ssa_6_a1o = ssa_1_arg1[1];
	APInt ssa_7_a2z = ssa_2_arg2[0];
	APInt ssa_8_a2o = ssa_2_arg2[1];
	APInt ssa_9_zero(ssa_3_a0z.getBitWidth(), 0);
	APInt ssa_10_one(ssa_3_a0z.getBitWidth(), 1);
	APInt ssa_11_two(ssa_3_a0z.getBitWidth(), 2);
	APInt ssa_12_three(ssa_3_a0z.getBitWidth(), 3);
	APInt ssa_13_four(ssa_3_a0z.getBitWidth(), 4);
	APInt ssa_14_six(ssa_3_a0z.getBitWidth(), 6);
	APInt ssa_15_a0_known = ssa_3_a0z | ssa_4_a0o;
	APInt ssa_16_ones0 = APInt::getAllOnes(ssa_3_a0z.getBitWidth());
	bool ssa_17_a0_const = ssa_15_a0_known.eq(ssa_16_ones0);
	APInt ssa_18_a2_known = ssa_7_a2z | ssa_8_a2o;
	APInt ssa_19_ones2 = APInt::getAllOnes(ssa_7_a2z.getBitWidth());
	bool ssa_20_a2_const = ssa_18_a2_known.eq(ssa_19_ones2);
	bool ssa_21_const_ops = ssa_17_a0_const && ssa_20_a2_const;
	APInt ssa_22_bw(ssa_5_a1z.getBitWidth(), ssa_5_a1z.getBitWidth());
	APInt ssa_23_bw_m1 = ssa_22_bw - ssa_10_one;
	APInt ssa_24_clz_bw_m1(ssa_23_bw_m1.getBitWidth(), ssa_23_bw_m1.countl_zero());
	APInt ssa_25_num_bits = ssa_22_bw - ssa_24_clz_bw_m1;
	APInt ssa_26_lowmask_sh = ssa_9_zero;
	if (ssa_25_num_bits.ule(ssa_25_num_bits.getBitWidth()))
		ssa_26_lowmask_sh.setLowBits(ssa_25_num_bits.getZExtValue());
	else
		ssa_26_lowmask_sh.setLowBits(ssa_25_num_bits.getBitWidth());
	APInt ssa_27_all1 = APInt::getAllOnes(ssa_5_a1z.getBitWidth());
	APInt ssa_28_highmask_sh = ssa_27_all1;
	if (ssa_25_num_bits.ule(ssa_25_num_bits.getBitWidth()))
		ssa_28_highmask_sh.clearLowBits(ssa_25_num_bits.getZExtValue());
	else
		ssa_28_highmask_sh.clearLowBits(ssa_25_num_bits.getBitWidth());
	APInt ssa_29_a1o_low = ssa_6_a1o & ssa_26_lowmask_sh;
	APInt ssa_30_a1z_ref = ssa_5_a1z | ssa_28_highmask_sh;
	APInt ssa_31_known1 = ssa_30_a1z_ref | ssa_29_a1o_low;
	bool ssa_32_a1_const = ssa_31_known1.eq(ssa_27_all1);
	APInt ssa_33_known1_or3 = ssa_31_known1 | ssa_12_three;
	bool ssa_34_cond_quad3 = ssa_33_known1_or3.eq(ssa_27_all1);
	APInt ssa_35_known1_or6 = ssa_31_known1 | ssa_14_six;
	bool ssa_36_cond_quad6 = ssa_35_known1_or6.eq(ssa_27_all1);
	APInt ssa_37_known1_low2 = ssa_29_a1o_low & ssa_12_three;
	APInt ssa_38_known0_low2 = ssa_30_a1z_ref & ssa_12_three;
	APInt ssa_39_union_low2 = ssa_37_known1_low2 | ssa_38_known0_low2;
	APInt ssa_40_unknown_low2 = ssa_39_union_low2 ^ ssa_12_three;
	APInt ssa_41_unk_bit0 = ssa_40_unknown_low2 & ssa_10_one;
	APInt ssa_42_unk_bit1 = ssa_40_unknown_low2 & ssa_11_two;
	APInt ssa_43_s1 = ssa_29_a1o_low | ssa_41_unk_bit0;
	APInt ssa_44_s2 = ssa_29_a1o_low | ssa_42_unk_bit1;
	APInt ssa_45_s3 = ssa_29_a1o_low | ssa_40_unknown_low2;
	APInt ssa_46_known1_low6 = ssa_29_a1o_low & ssa_14_six;
	APInt ssa_47_known0_low6 = ssa_30_a1z_ref & ssa_14_six;
	APInt ssa_48_union_low6 = ssa_46_known1_low6 | ssa_47_known0_low6;
	APInt ssa_49_unknown_low6 = ssa_48_union_low6 ^ ssa_14_six;
	APInt ssa_50_unk_bit1 = ssa_49_unknown_low6 & ssa_11_two;
	APInt ssa_51_unk_bit2 = ssa_49_unknown_low6 & ssa_13_four;
	APInt ssa_52_s1 = ssa_29_a1o_low | ssa_50_unk_bit1;
	APInt ssa_53_s2 = ssa_29_a1o_low | ssa_51_unk_bit2;
	APInt ssa_54_s3 = ssa_29_a1o_low | ssa_49_unknown_low6;
	APInt ssa_55_a2_known_or1 = ssa_18_a2_known | ssa_10_one;
	bool ssa_56_a2_bit0_maybe = ssa_55_a2_known_or1.eq(ssa_19_ones2);
	APInt ssa_57_known1_bit0_a2 = ssa_8_a2o & ssa_10_one;
	APInt ssa_58_known0_bit0_a2 = ssa_7_a2z & ssa_10_one;
	APInt ssa_59_union_bit0_a2 = ssa_57_known1_bit0_a2 | ssa_58_known0_bit0_a2;
	APInt ssa_60_unknown_bit0_a2 = ssa_59_union_bit0_a2 ^ ssa_10_one;
	APInt ssa_61_a2_val1 = ssa_8_a2o | ssa_60_unknown_bit0_a2;
	APInt ssa_62_shl0;
	if (ssa_29_a1o_low.uge(ssa_29_a1o_low.getBitWidth())) {
		ssa_62_shl0 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_62_shl0 = ssa_8_a2o.shl(ssa_29_a1o_low);
	}
	APInt ssa_63_shl1;
	if (ssa_43_s1.uge(ssa_43_s1.getBitWidth())) {
		ssa_63_shl1 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_63_shl1 = ssa_8_a2o.shl(ssa_43_s1);
	}
	APInt ssa_64_shl2;
	if (ssa_44_s2.uge(ssa_44_s2.getBitWidth())) {
		ssa_64_shl2 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_64_shl2 = ssa_8_a2o.shl(ssa_44_s2);
	}
	APInt ssa_65_shl3;
	if (ssa_45_s3.uge(ssa_45_s3.getBitWidth())) {
		ssa_65_shl3 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_65_shl3 = ssa_8_a2o.shl(ssa_45_s3);
	}
	APInt ssa_66_shl1;
	if (ssa_52_s1.uge(ssa_52_s1.getBitWidth())) {
		ssa_66_shl1 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_66_shl1 = ssa_8_a2o.shl(ssa_52_s1);
	}
	APInt ssa_67_shl2;
	if (ssa_53_s2.uge(ssa_53_s2.getBitWidth())) {
		ssa_67_shl2 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_67_shl2 = ssa_8_a2o.shl(ssa_53_s2);
	}
	APInt ssa_68_shl3;
	if (ssa_54_s3.uge(ssa_54_s3.getBitWidth())) {
		ssa_68_shl3 = APInt(ssa_8_a2o.getBitWidth(), 0);
	} else {
		ssa_68_shl3 = ssa_8_a2o.shl(ssa_54_s3);
	}
	APInt ssa_69_shl_a2;
	if (ssa_29_a1o_low.uge(ssa_29_a1o_low.getBitWidth())) {
		ssa_69_shl_a2 = APInt(ssa_61_a2_val1.getBitWidth(), 0);
	} else {
		ssa_69_shl_a2 = ssa_61_a2_val1.shl(ssa_29_a1o_low);
	}
	APInt ssa_70_res_val0 = ssa_4_a0o + ssa_62_shl0;
	APInt ssa_71_res_val1 = ssa_4_a0o + ssa_63_shl1;
	APInt ssa_72_res_val2 = ssa_4_a0o + ssa_64_shl2;
	APInt ssa_73_res_val3 = ssa_4_a0o + ssa_65_shl3;
	APInt ssa_74_res_val1 = ssa_4_a0o + ssa_66_shl1;
	APInt ssa_75_res_val2 = ssa_4_a0o + ssa_67_shl2;
	APInt ssa_76_res_val3 = ssa_4_a0o + ssa_68_shl3;
	APInt ssa_77_res_val_a2 = ssa_4_a0o + ssa_69_shl_a2;
	APInt ssa_78_res0 = ssa_70_res_val0 ^ ssa_16_ones0;
	APInt ssa_79_res0 = ssa_71_res_val1 ^ ssa_16_ones0;
	APInt ssa_80_res0 = ssa_72_res_val2 ^ ssa_16_ones0;
	APInt ssa_81_res0 = ssa_73_res_val3 ^ ssa_16_ones0;
	APInt ssa_82_res0 = ssa_74_res_val1 ^ ssa_16_ones0;
	APInt ssa_83_res0 = ssa_75_res_val2 ^ ssa_16_ones0;
	APInt ssa_84_res0 = ssa_76_res_val3 ^ ssa_16_ones0;
	APInt ssa_85_res0_a2 = ssa_77_res_val_a2 ^ ssa_16_ones0;
	APInt ssa_86_res0_meet01 = ssa_78_res0 & ssa_79_res0;
	APInt ssa_87_res0_meet23 = ssa_80_res0 & ssa_81_res0;
	APInt ssa_88_res0_meet3 = ssa_86_res0_meet01 & ssa_87_res0_meet23;
	APInt ssa_89_res0_meet01 = ssa_78_res0 & ssa_82_res0;
	APInt ssa_90_res0_meet23 = ssa_83_res0 & ssa_84_res0;
	APInt ssa_91_res0_meet6 = ssa_89_res0_meet01 & ssa_90_res0_meet23;
	APInt ssa_92_res0_meet_bit0 = ssa_78_res0 & ssa_85_res0_a2;
	APInt ssa_93_res1_meet01 = ssa_70_res_val0 & ssa_71_res_val1;
	APInt ssa_94_res1_meet23 = ssa_72_res_val2 & ssa_73_res_val3;
	APInt ssa_95_res1_meet3 = ssa_93_res1_meet01 & ssa_94_res1_meet23;
	APInt ssa_96_res1_meet01 = ssa_70_res_val0 & ssa_74_res_val1;
	APInt ssa_97_res1_meet23 = ssa_75_res_val2 & ssa_76_res_val3;
	APInt ssa_98_res1_meet6 = ssa_96_res1_meet01 & ssa_97_res1_meet23;
	APInt ssa_99_res1_meet_bit0 = ssa_70_res_val0 & ssa_77_res_val_a2;
	bool ssa_100_cond3 = ssa_34_cond_quad3 && ssa_21_const_ops;
	bool ssa_101_cond6 = ssa_36_cond_quad6 && ssa_21_const_ops;
	bool ssa_102_cond_bit0 = ssa_17_a0_const && ssa_32_a1_const;
	bool ssa_103_cond_bit0 = ssa_102_cond_bit0 && ssa_56_a2_bit0_maybe;
	APInt ssa_104_res0_case3 = ssa_100_cond3 ? ssa_88_res0_meet3 : ssa_9_zero;
	APInt ssa_105_res1_case3 = ssa_100_cond3 ? ssa_95_res1_meet3 : ssa_9_zero;
	APInt ssa_106_res0_case6 = ssa_101_cond6 ? ssa_91_res0_meet6 : ssa_9_zero;
	APInt ssa_107_res1_case6 = ssa_101_cond6 ? ssa_98_res1_meet6 : ssa_9_zero;
	APInt ssa_108_res0_case_bit0 = ssa_103_cond_bit0 ? ssa_92_res0_meet_bit0 : ssa_9_zero;
	APInt ssa_109_res1_case_bit0 = ssa_103_cond_bit0 ? ssa_99_res1_meet_bit0 : ssa_9_zero;
	APInt ssa_110_res0_tmp = ssa_104_res0_case3 | ssa_106_res0_case6;
	APInt ssa_111_res1_tmp = ssa_105_res1_case3 | ssa_107_res1_case6;
	APInt ssa_112_res0 = ssa_110_res0_tmp | ssa_108_res0_case_bit0;
	APInt ssa_113_res1 = ssa_111_res1_tmp | ssa_109_res1_case_bit0;
	std::array<APInt, 2> ssa_114_r = std::array<APInt, 2>{ssa_112_res0, ssa_113_res1};
	return ssa_114_r;
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
