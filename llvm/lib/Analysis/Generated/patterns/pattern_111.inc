namespace Pattern111 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_a0z = ssa_0_arg0[0];
	APInt ssa_5_a0o = ssa_0_arg0[1];
	APInt ssa_6_a1z = ssa_1_arg1[0];
	APInt ssa_7_a1o = ssa_1_arg1[1];
	APInt ssa_8_a2z = ssa_2_arg2[0];
	APInt ssa_9_a2o = ssa_2_arg2[1];
	APInt ssa_10_a3z = ssa_3_arg3[0];
	APInt ssa_11_a3o = ssa_3_arg3[1];
	APInt ssa_12_const0(ssa_4_a0z.getBitWidth(), 0);
	APInt ssa_13_all_ones = APInt::getAllOnes(ssa_4_a0z.getBitWidth());
	APInt ssa_14_bw(ssa_4_a0z.getBitWidth(), ssa_4_a0z.getBitWidth());
	APInt ssa_15_ov0 = ssa_4_a0z & ssa_5_a0o;
	APInt ssa_16_ov1 = ssa_6_a1z & ssa_7_a1o;
	APInt ssa_17_ov2 = ssa_8_a2z & ssa_9_a2o;
	APInt ssa_18_ov3 = ssa_10_a3z & ssa_11_a3o;
	bool ssa_19_ov0z = ssa_15_ov0.eq(ssa_12_const0);
	bool ssa_20_ov1z = ssa_16_ov1.eq(ssa_12_const0);
	bool ssa_21_ov2z = ssa_17_ov2.eq(ssa_12_const0);
	bool ssa_22_ov3z = ssa_18_ov3.eq(ssa_12_const0);
	bool ssa_23_ov0nz = ssa_15_ov0.ne(ssa_12_const0);
	bool ssa_24_ov1nz = ssa_16_ov1.ne(ssa_12_const0);
	bool ssa_25_ov2nz = ssa_17_ov2.ne(ssa_12_const0);
	bool ssa_26_ov3nz = ssa_18_ov3.ne(ssa_12_const0);
	APInt ssa_27_full0 = ssa_4_a0z | ssa_5_a0o;
	APInt ssa_28_full1 = ssa_6_a1z | ssa_7_a1o;
	APInt ssa_29_full2 = ssa_8_a2z | ssa_9_a2o;
	APInt ssa_30_full3 = ssa_10_a3z | ssa_11_a3o;
	bool ssa_31_full0c = ssa_27_full0.eq(ssa_13_all_ones);
	bool ssa_32_full1c = ssa_28_full1.eq(ssa_13_all_ones);
	bool ssa_33_full2c = ssa_29_full2.eq(ssa_13_all_ones);
	bool ssa_34_full3c = ssa_30_full3.eq(ssa_13_all_ones);
	bool ssa_35_const0c = ssa_31_full0c && ssa_19_ov0z;
	bool ssa_36_const1c = ssa_32_full1c && ssa_20_ov1z;
	bool ssa_37_const2c = ssa_33_full2c && ssa_21_ov2z;
	bool ssa_38_const3c = ssa_34_full3c && ssa_22_ov3z;
	bool ssa_39_const01 = ssa_35_const0c && ssa_36_const1c;
	bool ssa_40_const23 = ssa_37_const2c && ssa_38_const3c;
	bool ssa_41_all_const = ssa_39_const01 && ssa_40_const23;
	bool ssa_42_ov01 = ssa_23_ov0nz || ssa_24_ov1nz;
	bool ssa_43_ov23 = ssa_25_ov2nz || ssa_26_ov3nz;
	bool ssa_44_any_ov = ssa_42_ov01 || ssa_43_ov23;
	APInt ssa_45_sub0 = ssa_11_a3o - ssa_9_a2o;
	APInt ssa_46_ashr0;
	if (ssa_7_a1o.uge(ssa_7_a1o.getBitWidth()) && ssa_45_sub0.isSignBitSet()) {
		ssa_46_ashr0 = APInt(ssa_45_sub0.getBitWidth(), -1);
	} else if (ssa_7_a1o.uge(ssa_7_a1o.getBitWidth()) && ssa_45_sub0.isSignBitClear()) {
		ssa_46_ashr0 = APInt(ssa_45_sub0.getBitWidth(), 0);
	} else {
		ssa_46_ashr0 = ssa_45_sub0.ashr(ssa_7_a1o);
	}
	APInt ssa_47_sub1 = ssa_5_a0o - ssa_46_ashr0;
	APInt ssa_48_res0_exact = ssa_47_sub1 ^ ssa_13_all_ones;
	APInt ssa_49_res1_exact = ssa_47_sub1 | ssa_12_const0;
	bool ssa_50_ashr0_zero = ssa_46_ashr0.eq(ssa_12_const0);
	bool ssa_51_const123 = ssa_36_const1c && ssa_37_const2c;
	bool ssa_52_const123b = ssa_51_const123 && ssa_38_const3c;
	bool ssa_53_pass1 = ssa_52_const123b && ssa_50_ashr0_zero;
	bool ssa_54_a2_zero = ssa_9_a2o.eq(ssa_12_const0);
	bool ssa_55_a2_zero_c = ssa_37_const2c && ssa_54_a2_zero;
	APInt ssa_56_bw_minus_k = ssa_14_bw - ssa_7_a1o;
	APInt ssa_57_high_mask = ssa_12_const0;
	if (ssa_56_bw_minus_k.ule(ssa_56_bw_minus_k.getBitWidth()))
		ssa_57_high_mask.setHighBits(ssa_56_bw_minus_k.getZExtValue());
	else
		ssa_57_high_mask.setHighBits(ssa_56_bw_minus_k.getBitWidth());
	APInt ssa_58_a3_high_z = ssa_10_a3z & ssa_57_high_mask;
	bool ssa_59_a3_high_zc = ssa_58_a3_high_z.eq(ssa_57_high_mask);
	bool ssa_60_k_lt_bw = ssa_7_a1o.ult(ssa_14_bw);
	bool ssa_61_pass2a = ssa_36_const1c && ssa_55_a2_zero_c;
	bool ssa_62_pass2b = ssa_61_pass2a && ssa_59_a3_high_zc;
	bool ssa_63_pass2 = ssa_62_pass2b && ssa_60_k_lt_bw;
	bool ssa_64_pass = ssa_53_pass1 || ssa_63_pass2;
	APInt ssa_65_res0_pt = ssa_4_a0z | ssa_12_const0;
	APInt ssa_66_res1_pt = ssa_5_a0o | ssa_12_const0;
	APInt ssa_67_res0_tmp1 = ssa_64_pass ? ssa_65_res0_pt : ssa_12_const0;
	APInt ssa_68_res1_tmp1 = ssa_64_pass ? ssa_66_res1_pt : ssa_12_const0;
	APInt ssa_69_res0_tmp2 = ssa_41_all_const ? ssa_48_res0_exact : ssa_67_res0_tmp1;
	APInt ssa_70_res1_tmp2 = ssa_41_all_const ? ssa_49_res1_exact : ssa_68_res1_tmp1;
	APInt ssa_71_res0 = ssa_44_any_ov ? ssa_13_all_ones : ssa_69_res0_tmp2;
	APInt ssa_72_res1 = ssa_44_any_ov ? ssa_13_all_ones : ssa_70_res1_tmp2;
	std::array<APInt, 2> ssa_73_r = std::array<APInt, 2>{ssa_71_res0, ssa_72_res1};
	return ssa_73_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_a0z = ssa_0_arg0[0];
	APInt ssa_5_a0o = ssa_0_arg0[1];
	APInt ssa_6_a1z = ssa_1_arg1[0];
	APInt ssa_7_a1o = ssa_1_arg1[1];
	APInt ssa_8_a2z = ssa_2_arg2[0];
	APInt ssa_9_a2o = ssa_2_arg2[1];
	APInt ssa_10_a3z = ssa_3_arg3[0];
	APInt ssa_11_a3o = ssa_3_arg3[1];
	APInt ssa_12_const0(ssa_4_a0z.getBitWidth(), 0);
	APInt ssa_13_const1(ssa_4_a0z.getBitWidth(), 1);
	APInt ssa_14_all_ones = APInt::getAllOnes(ssa_4_a0z.getBitWidth());
	APInt ssa_15_bw(ssa_4_a0z.getBitWidth(), ssa_4_a0z.getBitWidth());
	bool ssa_16_true = -1;
	APInt ssa_17_ov0 = ssa_4_a0z & ssa_5_a0o;
	APInt ssa_18_ov1 = ssa_6_a1z & ssa_7_a1o;
	APInt ssa_19_ov2 = ssa_8_a2z & ssa_9_a2o;
	APInt ssa_20_ov3 = ssa_10_a3z & ssa_11_a3o;
	bool ssa_21_ov0z = ssa_17_ov0.eq(ssa_12_const0);
	bool ssa_22_ov1z = ssa_18_ov1.eq(ssa_12_const0);
	bool ssa_23_ov2z = ssa_19_ov2.eq(ssa_12_const0);
	bool ssa_24_ov3z = ssa_20_ov3.eq(ssa_12_const0);
	bool ssa_25_ov0nz = ssa_17_ov0.ne(ssa_12_const0);
	bool ssa_26_ov1nz = ssa_18_ov1.ne(ssa_12_const0);
	bool ssa_27_ov2nz = ssa_19_ov2.ne(ssa_12_const0);
	bool ssa_28_ov3nz = ssa_20_ov3.ne(ssa_12_const0);
	APInt ssa_29_full0 = ssa_4_a0z | ssa_5_a0o;
	APInt ssa_30_full1 = ssa_6_a1z | ssa_7_a1o;
	APInt ssa_31_full2 = ssa_8_a2z | ssa_9_a2o;
	APInt ssa_32_full3 = ssa_10_a3z | ssa_11_a3o;
	bool ssa_33_full0c = ssa_29_full0.eq(ssa_14_all_ones);
	bool ssa_34_full1c = ssa_30_full1.eq(ssa_14_all_ones);
	bool ssa_35_full2c = ssa_31_full2.eq(ssa_14_all_ones);
	bool ssa_36_full3c = ssa_32_full3.eq(ssa_14_all_ones);
	bool ssa_37_const0c = ssa_33_full0c && ssa_21_ov0z;
	bool ssa_38_const1c = ssa_34_full1c && ssa_22_ov1z;
	bool ssa_39_const2c = ssa_35_full2c && ssa_23_ov2z;
	bool ssa_40_const3c = ssa_36_full3c && ssa_24_ov3z;
	bool ssa_41_const01 = ssa_37_const0c && ssa_38_const1c;
	bool ssa_42_const23 = ssa_39_const2c && ssa_40_const3c;
	bool ssa_43_all_const = ssa_41_const01 && ssa_42_const23;
	bool ssa_44_ov01 = ssa_25_ov0nz || ssa_26_ov1nz;
	bool ssa_45_ov23 = ssa_27_ov2nz || ssa_28_ov3nz;
	bool ssa_46_any_ov = ssa_44_ov01 || ssa_45_ov23;
	bool ssa_47_a1_min_ge_bw = ssa_7_a1o.uge(ssa_15_bw);
	APInt ssa_48_low_mask = ssa_12_const0;
	if (ssa_7_a1o.ule(ssa_7_a1o.getBitWidth()))
		ssa_48_low_mask.setLowBits(ssa_7_a1o.getZExtValue());
	else
		ssa_48_low_mask.setLowBits(ssa_7_a1o.getBitWidth());
	APInt ssa_49_m0 = ssa_10_a3z & ssa_9_a2o;
	APInt ssa_50_m1 = ssa_11_a3o & ssa_8_a2z;
	APInt ssa_51_m2 = ssa_49_m0 | ssa_50_m1;
	APInt ssa_52_m_low = ssa_51_m2 & ssa_48_low_mask;
	bool ssa_53_mismatch = ssa_52_m_low.ne(ssa_12_const0);
	bool ssa_54_bad_low_mismatch = ssa_38_const1c && ssa_53_mismatch;
	APInt ssa_55_sub0 = ssa_11_a3o - ssa_9_a2o;
	APInt ssa_56_ashr0;
	if (ssa_7_a1o.uge(ssa_7_a1o.getBitWidth()) && ssa_55_sub0.isSignBitSet()) {
		ssa_56_ashr0 = APInt(ssa_55_sub0.getBitWidth(), -1);
	} else if (ssa_7_a1o.uge(ssa_7_a1o.getBitWidth()) && ssa_55_sub0.isSignBitClear()) {
		ssa_56_ashr0 = APInt(ssa_55_sub0.getBitWidth(), 0);
	} else {
		ssa_56_ashr0 = ssa_55_sub0.ashr(ssa_7_a1o);
	}
	APInt ssa_57_shl0;
	if (ssa_7_a1o.uge(ssa_7_a1o.getBitWidth())) {
		ssa_57_shl0 = APInt(ssa_56_ashr0.getBitWidth(), 0);
	} else {
		ssa_57_shl0 = ssa_56_ashr0.shl(ssa_7_a1o);
	}
	bool ssa_58_ashr_exact_c = ssa_57_shl0.eq(ssa_55_sub0);
	bool ssa_59_k_lt_bw_c = ssa_7_a1o.ult(ssa_15_bw);
	bool ssa_60_constraint123 = ssa_58_ashr_exact_c && ssa_59_k_lt_bw_c;
	bool ssa_61_not_constraint123 = ssa_60_constraint123 != ssa_16_true;
	bool ssa_62_const123 = ssa_38_const1c && ssa_39_const2c;
	bool ssa_63_const123b = ssa_62_const123 && ssa_40_const3c;
	bool ssa_64_bad123 = ssa_63_const123b && ssa_61_not_constraint123;
	bool ssa_65_ssub_ov;
	ssa_5_a0o.ssub_ov(ssa_56_ashr0, ssa_65_ssub_ov);
	bool ssa_66_bad_subov = ssa_43_all_const && ssa_65_ssub_ov;
	bool ssa_67_unsat0 = ssa_46_any_ov || ssa_47_a1_min_ge_bw;
	bool ssa_68_unsat1 = ssa_67_unsat0 || ssa_64_bad123;
	bool ssa_69_unsat2 = ssa_68_unsat1 || ssa_66_bad_subov;
	bool ssa_70_unsat = ssa_69_unsat2 || ssa_54_bad_low_mismatch;
	APInt ssa_71_sub1 = ssa_5_a0o - ssa_56_ashr0;
	APInt ssa_72_res0_exact = ssa_71_sub1 ^ ssa_14_all_ones;
	APInt ssa_73_res1_exact = ssa_71_sub1 | ssa_12_const0;
	bool ssa_74_ashr0_zero = ssa_56_ashr0.eq(ssa_12_const0);
	bool ssa_75_pass1a = ssa_63_const123b && ssa_74_ashr0_zero;
	bool ssa_76_a2_zero = ssa_9_a2o.eq(ssa_12_const0);
	bool ssa_77_a2_zero_c = ssa_39_const2c && ssa_76_a2_zero;
	APInt ssa_78_bw_minus_k = ssa_15_bw - ssa_7_a1o;
	APInt ssa_79_high_mask = ssa_12_const0;
	if (ssa_78_bw_minus_k.ule(ssa_78_bw_minus_k.getBitWidth()))
		ssa_79_high_mask.setHighBits(ssa_78_bw_minus_k.getZExtValue());
	else
		ssa_79_high_mask.setHighBits(ssa_78_bw_minus_k.getBitWidth());
	APInt ssa_80_a3_high_z = ssa_10_a3z & ssa_79_high_mask;
	bool ssa_81_a3_high_zc = ssa_80_a3_high_z.eq(ssa_79_high_mask);
	bool ssa_82_pass2a = ssa_38_const1c && ssa_77_a2_zero_c;
	bool ssa_83_pass2b = ssa_82_pass2a && ssa_81_a3_high_zc;
	bool ssa_84_pass2 = ssa_83_pass2b && ssa_59_k_lt_bw_c;
	APInt ssa_85_a2_high_z = ssa_8_a2z & ssa_79_high_mask;
	bool ssa_86_a2_high_zc = ssa_85_a2_high_z.eq(ssa_79_high_mask);
	bool ssa_87_pass3a = ssa_38_const1c && ssa_86_a2_high_zc;
	bool ssa_88_pass3b = ssa_87_pass3a && ssa_81_a3_high_zc;
	bool ssa_89_pass3 = ssa_88_pass3b && ssa_59_k_lt_bw_c;
	bool ssa_90_pass_tmp = ssa_75_pass1a || ssa_84_pass2;
	bool ssa_91_pass = ssa_90_pass_tmp || ssa_89_pass3;
	APInt ssa_92_bw_minus1 = ssa_15_bw - ssa_13_const1;
	bool ssa_93_k_eq_bw_minus1 = ssa_7_a1o.eq(ssa_92_bw_minus1);
	bool ssa_94_cond_bw1a = ssa_37_const0c && ssa_38_const1c;
	bool ssa_95_cond_bw1 = ssa_94_cond_bw1a && ssa_93_k_eq_bw_minus1;
	bool ssa_96_not_all_const = ssa_43_all_const != ssa_16_true;
	bool ssa_97_cond_bw1_eff = ssa_95_cond_bw1 && ssa_96_not_all_const;
	APInt ssa_98_a0p1 = ssa_5_a0o + ssa_13_const1;
	APInt ssa_99_nota0 = ssa_5_a0o ^ ssa_14_all_ones;
	APInt ssa_100_nota0p1 = ssa_98_a0p1 ^ ssa_14_all_ones;
	APInt ssa_101_res0_bw1 = ssa_99_nota0 & ssa_100_nota0p1;
	APInt ssa_102_res1_bw1 = ssa_5_a0o & ssa_98_a0p1;
	APInt ssa_103_res0_pt = ssa_4_a0z | ssa_12_const0;
	APInt ssa_104_res1_pt = ssa_5_a0o | ssa_12_const0;
	APInt ssa_105_res0_tmp1 = ssa_91_pass ? ssa_103_res0_pt : ssa_12_const0;
	APInt ssa_106_res1_tmp1 = ssa_91_pass ? ssa_104_res1_pt : ssa_12_const0;
	APInt ssa_107_res0_tmp2 = ssa_43_all_const ? ssa_72_res0_exact : ssa_105_res0_tmp1;
	APInt ssa_108_res1_tmp2 = ssa_43_all_const ? ssa_73_res1_exact : ssa_106_res1_tmp1;
	APInt ssa_109_res0_tmp3 = ssa_97_cond_bw1_eff ? ssa_101_res0_bw1 : ssa_107_res0_tmp2;
	APInt ssa_110_res1_tmp3 = ssa_97_cond_bw1_eff ? ssa_102_res1_bw1 : ssa_108_res1_tmp2;
	APInt ssa_111_res0 = ssa_70_unsat ? ssa_14_all_ones : ssa_109_res0_tmp3;
	APInt ssa_112_res1 = ssa_70_unsat ? ssa_14_all_ones : ssa_110_res1_tmp3;
	std::array<APInt, 2> ssa_113_r = std::array<APInt, 2>{ssa_111_res0, ssa_112_res1};
	return ssa_113_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2, std::array<APInt, 2> ssa_3_arg3) {
	APInt ssa_4_a0z = ssa_0_arg0[0];
	APInt ssa_5_a0o = ssa_0_arg0[1];
	APInt ssa_6_a1z = ssa_1_arg1[0];
	APInt ssa_7_a1o = ssa_1_arg1[1];
	APInt ssa_8_a2z = ssa_2_arg2[0];
	APInt ssa_9_a2o = ssa_2_arg2[1];
	APInt ssa_10_a3z = ssa_3_arg3[0];
	APInt ssa_11_a3o = ssa_3_arg3[1];
	APInt ssa_12_const0(ssa_4_a0z.getBitWidth(), 0);
	APInt ssa_13_const1(ssa_4_a0z.getBitWidth(), 1);
	APInt ssa_14_all_ones = APInt::getAllOnes(ssa_4_a0z.getBitWidth());
	APInt ssa_15_bw(ssa_4_a0z.getBitWidth(), ssa_4_a0z.getBitWidth());
	bool ssa_16_true = -1;
	APInt ssa_17_ov0 = ssa_4_a0z & ssa_5_a0o;
	APInt ssa_18_ov1 = ssa_6_a1z & ssa_7_a1o;
	APInt ssa_19_ov2 = ssa_8_a2z & ssa_9_a2o;
	APInt ssa_20_ov3 = ssa_10_a3z & ssa_11_a3o;
	bool ssa_21_ov0z = ssa_17_ov0.eq(ssa_12_const0);
	bool ssa_22_ov1z = ssa_18_ov1.eq(ssa_12_const0);
	bool ssa_23_ov2z = ssa_19_ov2.eq(ssa_12_const0);
	bool ssa_24_ov3z = ssa_20_ov3.eq(ssa_12_const0);
	bool ssa_25_ov0nz = ssa_17_ov0.ne(ssa_12_const0);
	bool ssa_26_ov1nz = ssa_18_ov1.ne(ssa_12_const0);
	bool ssa_27_ov2nz = ssa_19_ov2.ne(ssa_12_const0);
	bool ssa_28_ov3nz = ssa_20_ov3.ne(ssa_12_const0);
	APInt ssa_29_full0 = ssa_4_a0z | ssa_5_a0o;
	APInt ssa_30_full1 = ssa_6_a1z | ssa_7_a1o;
	APInt ssa_31_full2 = ssa_8_a2z | ssa_9_a2o;
	APInt ssa_32_full3 = ssa_10_a3z | ssa_11_a3o;
	bool ssa_33_full0c = ssa_29_full0.eq(ssa_14_all_ones);
	bool ssa_34_full1c = ssa_30_full1.eq(ssa_14_all_ones);
	bool ssa_35_full2c = ssa_31_full2.eq(ssa_14_all_ones);
	bool ssa_36_full3c = ssa_32_full3.eq(ssa_14_all_ones);
	bool ssa_37_const0c = ssa_33_full0c && ssa_21_ov0z;
	bool ssa_38_const1c = ssa_34_full1c && ssa_22_ov1z;
	bool ssa_39_const2c = ssa_35_full2c && ssa_23_ov2z;
	bool ssa_40_const3c = ssa_36_full3c && ssa_24_ov3z;
	bool ssa_41_const01 = ssa_37_const0c && ssa_38_const1c;
	bool ssa_42_const23 = ssa_39_const2c && ssa_40_const3c;
	bool ssa_43_all_const = ssa_41_const01 && ssa_42_const23;
	bool ssa_44_ov01 = ssa_25_ov0nz || ssa_26_ov1nz;
	bool ssa_45_ov23 = ssa_27_ov2nz || ssa_28_ov3nz;
	bool ssa_46_any_ov = ssa_44_ov01 || ssa_45_ov23;
	bool ssa_47_a1_min_ge_bw = ssa_7_a1o.uge(ssa_15_bw);
	APInt ssa_48_low_mask = ssa_12_const0;
	if (ssa_7_a1o.ule(ssa_7_a1o.getBitWidth()))
		ssa_48_low_mask.setLowBits(ssa_7_a1o.getZExtValue());
	else
		ssa_48_low_mask.setLowBits(ssa_7_a1o.getBitWidth());
	APInt ssa_49_m0 = ssa_10_a3z & ssa_9_a2o;
	APInt ssa_50_m1 = ssa_11_a3o & ssa_8_a2z;
	APInt ssa_51_m2 = ssa_49_m0 | ssa_50_m1;
	APInt ssa_52_m_low = ssa_51_m2 & ssa_48_low_mask;
	bool ssa_53_mismatch = ssa_52_m_low.ne(ssa_12_const0);
	APInt ssa_54_sub0 = ssa_11_a3o - ssa_9_a2o;
	APInt ssa_55_ashr0;
	if (ssa_7_a1o.uge(ssa_7_a1o.getBitWidth()) && ssa_54_sub0.isSignBitSet()) {
		ssa_55_ashr0 = APInt(ssa_54_sub0.getBitWidth(), -1);
	} else if (ssa_7_a1o.uge(ssa_7_a1o.getBitWidth()) && ssa_54_sub0.isSignBitClear()) {
		ssa_55_ashr0 = APInt(ssa_54_sub0.getBitWidth(), 0);
	} else {
		ssa_55_ashr0 = ssa_54_sub0.ashr(ssa_7_a1o);
	}
	APInt ssa_56_shl0;
	if (ssa_7_a1o.uge(ssa_7_a1o.getBitWidth())) {
		ssa_56_shl0 = APInt(ssa_55_ashr0.getBitWidth(), 0);
	} else {
		ssa_56_shl0 = ssa_55_ashr0.shl(ssa_7_a1o);
	}
	bool ssa_57_ashr_exact_c = ssa_56_shl0.eq(ssa_54_sub0);
	bool ssa_58_k_lt_bw_c = ssa_7_a1o.ult(ssa_15_bw);
	bool ssa_59_constraint123 = ssa_57_ashr_exact_c && ssa_58_k_lt_bw_c;
	bool ssa_60_not_constraint123 = ssa_59_constraint123 != ssa_16_true;
	bool ssa_61_const123 = ssa_38_const1c && ssa_39_const2c;
	bool ssa_62_const123b = ssa_61_const123 && ssa_40_const3c;
	bool ssa_63_bad123 = ssa_62_const123b && ssa_60_not_constraint123;
	bool ssa_64_ssub_ov;
	ssa_5_a0o.ssub_ov(ssa_55_ashr0, ssa_64_ssub_ov);
	bool ssa_65_bad_subov = ssa_43_all_const && ssa_64_ssub_ov;
	bool ssa_66_unsat0 = ssa_46_any_ov || ssa_47_a1_min_ge_bw;
	bool ssa_67_unsat1 = ssa_66_unsat0 || ssa_63_bad123;
	bool ssa_68_unsat2 = ssa_67_unsat1 || ssa_65_bad_subov;
	bool ssa_69_unsat = ssa_68_unsat2 || ssa_53_mismatch;
	APInt ssa_70_sub1 = ssa_5_a0o - ssa_55_ashr0;
	APInt ssa_71_res0_exact = ssa_70_sub1 ^ ssa_14_all_ones;
	APInt ssa_72_res1_exact = ssa_70_sub1 | ssa_12_const0;
	bool ssa_73_ashr0_zero = ssa_55_ashr0.eq(ssa_12_const0);
	bool ssa_74_pass1a = ssa_62_const123b && ssa_73_ashr0_zero;
	bool ssa_75_a2_eq_a3 = ssa_9_a2o.eq(ssa_11_a3o);
	bool ssa_76_pass1b0 = ssa_39_const2c && ssa_40_const3c;
	bool ssa_77_pass1b = ssa_76_pass1b0 && ssa_75_a2_eq_a3;
	bool ssa_78_pass1 = ssa_74_pass1a || ssa_77_pass1b;
	bool ssa_79_a2_zero = ssa_9_a2o.eq(ssa_12_const0);
	bool ssa_80_a2_zero_c = ssa_39_const2c && ssa_79_a2_zero;
	APInt ssa_81_bw_minus_k = ssa_15_bw - ssa_7_a1o;
	APInt ssa_82_high_mask = ssa_12_const0;
	if (ssa_81_bw_minus_k.ule(ssa_81_bw_minus_k.getBitWidth()))
		ssa_82_high_mask.setHighBits(ssa_81_bw_minus_k.getZExtValue());
	else
		ssa_82_high_mask.setHighBits(ssa_81_bw_minus_k.getBitWidth());
	APInt ssa_83_a3_high_z = ssa_10_a3z & ssa_82_high_mask;
	bool ssa_84_a3_high_zc = ssa_83_a3_high_z.eq(ssa_82_high_mask);
	bool ssa_85_pass2a = ssa_38_const1c && ssa_80_a2_zero_c;
	bool ssa_86_pass2b = ssa_85_pass2a && ssa_84_a3_high_zc;
	bool ssa_87_pass2 = ssa_86_pass2b && ssa_58_k_lt_bw_c;
	APInt ssa_88_a2_high_z = ssa_8_a2z & ssa_82_high_mask;
	bool ssa_89_a2_high_zc = ssa_88_a2_high_z.eq(ssa_82_high_mask);
	bool ssa_90_pass3a = ssa_38_const1c && ssa_89_a2_high_zc;
	bool ssa_91_pass3b = ssa_90_pass3a && ssa_84_a3_high_zc;
	bool ssa_92_pass3 = ssa_91_pass3b && ssa_58_k_lt_bw_c;
	bool ssa_93_pass_tmp = ssa_78_pass1 || ssa_87_pass2;
	bool ssa_94_pass = ssa_93_pass_tmp || ssa_92_pass3;
	APInt ssa_95_bw_minus1 = ssa_15_bw - ssa_13_const1;
	bool ssa_96_k_eq_bw_minus1 = ssa_7_a1o.eq(ssa_95_bw_minus1);
	bool ssa_97_cond_bw1a = ssa_37_const0c && ssa_38_const1c;
	bool ssa_98_cond_bw1 = ssa_97_cond_bw1a && ssa_96_k_eq_bw_minus1;
	bool ssa_99_not_all_const = ssa_43_all_const != ssa_16_true;
	bool ssa_100_cond_bw1_eff = ssa_98_cond_bw1 && ssa_99_not_all_const;
	APInt ssa_101_a0p1 = ssa_5_a0o + ssa_13_const1;
	APInt ssa_102_nota0 = ssa_5_a0o ^ ssa_14_all_ones;
	APInt ssa_103_nota0p1 = ssa_101_a0p1 ^ ssa_14_all_ones;
	APInt ssa_104_res0_bw1 = ssa_102_nota0 & ssa_103_nota0p1;
	APInt ssa_105_res1_bw1 = ssa_5_a0o & ssa_101_a0p1;
	APInt ssa_106_res0_pt = ssa_4_a0z | ssa_12_const0;
	APInt ssa_107_res1_pt = ssa_5_a0o | ssa_12_const0;
	APInt ssa_108_res0_tmp1 = ssa_94_pass ? ssa_106_res0_pt : ssa_12_const0;
	APInt ssa_109_res1_tmp1 = ssa_94_pass ? ssa_107_res1_pt : ssa_12_const0;
	APInt ssa_110_res0_tmp2 = ssa_43_all_const ? ssa_71_res0_exact : ssa_108_res0_tmp1;
	APInt ssa_111_res1_tmp2 = ssa_43_all_const ? ssa_72_res1_exact : ssa_109_res1_tmp1;
	APInt ssa_112_res0_tmp3 = ssa_100_cond_bw1_eff ? ssa_104_res0_bw1 : ssa_110_res0_tmp2;
	APInt ssa_113_res1_tmp3 = ssa_100_cond_bw1_eff ? ssa_105_res1_bw1 : ssa_111_res1_tmp2;
	APInt ssa_114_res0 = ssa_69_unsat ? ssa_14_all_ones : ssa_112_res0_tmp3;
	APInt ssa_115_res1 = ssa_69_unsat ? ssa_14_all_ones : ssa_113_res1_tmp3;
	std::array<APInt, 2> ssa_116_r = std::array<APInt, 2>{ssa_114_res0, ssa_115_res1};
	return ssa_116_r;
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
