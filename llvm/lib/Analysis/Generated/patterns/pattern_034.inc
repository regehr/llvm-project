namespace Pattern034 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_all = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_one(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_11_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_12_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_13_bw1 = ssa_12_bw - ssa_10_one;
	APInt ssa_14_signMask;
	if (ssa_13_bw1.uge(ssa_13_bw1.getBitWidth())) {
		ssa_14_signMask = APInt(ssa_9_all.getBitWidth(), 0);
	} else {
		ssa_14_signMask = ssa_9_all.shl(ssa_13_bw1);
	}
	APInt ssa_15_aKnown = ssa_3_a0 | ssa_4_a1;
	APInt ssa_16_bKnown = ssa_5_b0 | ssa_6_b1;
	APInt ssa_17_cKnown = ssa_7_c0 | ssa_8_c1;
	bool ssa_18_aFull = ssa_15_aKnown.eq(ssa_9_all);
	bool ssa_19_bFull = ssa_16_bKnown.eq(ssa_9_all);
	bool ssa_20_cFull = ssa_17_cKnown.eq(ssa_9_all);
	bool ssa_21_abFull = ssa_18_aFull && ssa_19_bFull;
	bool ssa_22_allFull = ssa_21_abFull && ssa_20_cFull;
	APInt ssa_23_sum1 = ssa_4_a1 + ssa_6_b1;
	APInt ssa_24_sum2 = ssa_23_sum1 + ssa_8_c1;
	APInt ssa_25_exactZero = ssa_24_sum2 ^ ssa_9_all;
	APInt ssa_26_a1msb = ssa_4_a1 & ssa_14_signMask;
	APInt ssa_27_b1msb = ssa_6_b1 & ssa_14_signMask;
	APInt ssa_28_c1msb = ssa_8_c1 & ssa_14_signMask;
	APInt ssa_29_a0msb = ssa_3_a0 & ssa_14_signMask;
	APInt ssa_30_b0msb = ssa_5_b0 & ssa_14_signMask;
	APInt ssa_31_c0msb = ssa_7_c0 & ssa_14_signMask;
	bool ssa_32_a1is1 = ssa_26_a1msb.eq(ssa_14_signMask);
	bool ssa_33_b1is1 = ssa_27_b1msb.eq(ssa_14_signMask);
	bool ssa_34_c1is1 = ssa_28_c1msb.eq(ssa_14_signMask);
	bool ssa_35_a0is1 = ssa_29_a0msb.eq(ssa_14_signMask);
	bool ssa_36_b0is1 = ssa_30_b0msb.eq(ssa_14_signMask);
	bool ssa_37_c0is1 = ssa_31_c0msb.eq(ssa_14_signMask);
	bool ssa_38_ab1 = ssa_32_a1is1 && ssa_33_b1is1;
	bool ssa_39_ac1 = ssa_32_a1is1 && ssa_34_c1is1;
	bool ssa_40_bc1 = ssa_33_b1is1 && ssa_34_c1is1;
	bool ssa_41_ab1c1 = ssa_38_ab1 || ssa_39_ac1;
	bool ssa_42_conflict = ssa_41_ab1c1 || ssa_40_bc1;
	bool ssa_43_any1ab = ssa_32_a1is1 || ssa_33_b1is1;
	bool ssa_44_any1 = ssa_43_any1ab || ssa_34_c1is1;
	bool ssa_45_all0ab = ssa_35_a0is1 && ssa_36_b0is1;
	bool ssa_46_all0 = ssa_45_all0ab && ssa_37_c0is1;
	APInt ssa_47_baseOne = ssa_44_any1 ? ssa_14_signMask : ssa_11_zero;
	APInt ssa_48_baseZero = ssa_46_all0 ? ssa_14_signMask : ssa_11_zero;
	APInt ssa_49_sel0 = ssa_22_allFull ? ssa_25_exactZero : ssa_48_baseZero;
	APInt ssa_50_sel1 = ssa_22_allFull ? ssa_24_sum2 : ssa_47_baseOne;
	APInt ssa_51_res0 = ssa_42_conflict ? ssa_9_all : ssa_49_sel0;
	APInt ssa_52_res1 = ssa_42_conflict ? ssa_9_all : ssa_50_sel1;
	std::array<APInt, 2> ssa_53_r = std::array<APInt, 2>{ssa_51_res0, ssa_52_res1};
	return ssa_53_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_all = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_one(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_11_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_12_aKnown = ssa_3_a0 | ssa_4_a1;
	APInt ssa_13_bKnown = ssa_5_b0 | ssa_6_b1;
	APInt ssa_14_cKnown = ssa_7_c0 | ssa_8_c1;
	APInt ssa_15_aTrail(ssa_12_aKnown.getBitWidth(), ssa_12_aKnown.countr_one());
	APInt ssa_16_bTrail(ssa_13_bKnown.getBitWidth(), ssa_13_bKnown.countr_one());
	APInt ssa_17_cTrail(ssa_14_cKnown.getBitWidth(), ssa_14_cKnown.countr_one());
	APInt ssa_18_minab = APIntOps::umin(ssa_15_aTrail, ssa_16_bTrail);
	APInt ssa_19_n = APIntOps::umin(ssa_18_minab, ssa_17_cTrail);
	APInt ssa_20_shl;
	if (ssa_19_n.uge(ssa_19_n.getBitWidth())) {
		ssa_20_shl = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_20_shl = ssa_10_one.shl(ssa_19_n);
	}
	APInt ssa_21_lowMask = ssa_20_shl - ssa_10_one;
	APInt ssa_22_aLow = ssa_4_a1 & ssa_21_lowMask;
	APInt ssa_23_bLow = ssa_6_b1 & ssa_21_lowMask;
	APInt ssa_24_cLow = ssa_8_c1 & ssa_21_lowMask;
	APInt ssa_25_sum1 = ssa_22_aLow + ssa_23_bLow;
	APInt ssa_26_sum2 = ssa_25_sum1 + ssa_24_cLow;
	APInt ssa_27_sumLow = ssa_26_sum2 & ssa_21_lowMask;
	APInt ssa_28_lowZero = ssa_21_lowMask ^ ssa_27_sumLow;
	APInt ssa_29_maxA = ssa_9_all ^ ssa_3_a0;
	APInt ssa_30_maxB = ssa_9_all ^ ssa_5_b0;
	APInt ssa_31_maxC = ssa_9_all ^ ssa_7_c0;
	APInt ssa_32_sumMax1 = ssa_29_maxA + ssa_30_maxB;
	bool ssa_33_ovMax1 = ssa_32_sumMax1.ult(ssa_29_maxA);
	APInt ssa_34_sumMax2 = ssa_32_sumMax1 + ssa_31_maxC;
	bool ssa_35_ovMax2 = ssa_34_sumMax2.ult(ssa_32_sumMax1);
	bool ssa_36_ovMax = ssa_33_ovMax1 || ssa_35_ovMax2;
	APInt ssa_37_maxSum = ssa_36_ovMax ? ssa_9_all : ssa_34_sumMax2;
	APInt ssa_38_sumMinBC = ssa_6_b1 + ssa_8_c1;
	bool ssa_39_ovMinBC = ssa_38_sumMinBC.ult(ssa_6_b1);
	APInt ssa_40_sumMin1 = ssa_4_a1 + ssa_6_b1;
	bool ssa_41_ovMin1 = ssa_40_sumMin1.ult(ssa_4_a1);
	APInt ssa_42_sumMin2 = ssa_40_sumMin1 + ssa_8_c1;
	bool ssa_43_ovMin2 = ssa_42_sumMin2.ult(ssa_40_sumMin1);
	bool ssa_44_ovMin12 = ssa_41_ovMin1 || ssa_43_ovMin2;
	bool ssa_45_ovMin = ssa_44_ovMin12 || ssa_39_ovMinBC;
	APInt ssa_46_minSum = ssa_44_ovMin12 ? ssa_11_zero : ssa_42_sumMin2;
	APInt ssa_47_diff = ssa_46_minSum ^ ssa_37_maxSum;
	APInt ssa_48_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	APInt ssa_49_lz(ssa_47_diff.getBitWidth(), ssa_47_diff.countl_zero());
	APInt ssa_50_shift = ssa_48_bw - ssa_49_lz;
	APInt ssa_51_highMask;
	if (ssa_50_shift.uge(ssa_50_shift.getBitWidth())) {
		ssa_51_highMask = APInt(ssa_9_all.getBitWidth(), 0);
	} else {
		ssa_51_highMask = ssa_9_all.shl(ssa_50_shift);
	}
	APInt ssa_52_highOnes = ssa_46_minSum & ssa_51_highMask;
	APInt ssa_53_highZero = ssa_51_highMask ^ ssa_52_highOnes;
	APInt ssa_54_bw1 = ssa_48_bw - ssa_10_one;
	APInt ssa_55_signMask;
	if (ssa_54_bw1.uge(ssa_54_bw1.getBitWidth())) {
		ssa_55_signMask = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_55_signMask = ssa_10_one.shl(ssa_54_bw1);
	}
	APInt ssa_56_a1msb = ssa_4_a1 & ssa_55_signMask;
	APInt ssa_57_b1msb = ssa_6_b1 & ssa_55_signMask;
	APInt ssa_58_c1msb = ssa_8_c1 & ssa_55_signMask;
	APInt ssa_59_a0msb = ssa_3_a0 & ssa_55_signMask;
	APInt ssa_60_b0msb = ssa_5_b0 & ssa_55_signMask;
	APInt ssa_61_c0msb = ssa_7_c0 & ssa_55_signMask;
	bool ssa_62_a1is1 = ssa_56_a1msb.eq(ssa_55_signMask);
	bool ssa_63_b1is1 = ssa_57_b1msb.eq(ssa_55_signMask);
	bool ssa_64_c1is1 = ssa_58_c1msb.eq(ssa_55_signMask);
	bool ssa_65_a0is1 = ssa_59_a0msb.eq(ssa_55_signMask);
	bool ssa_66_b0is1 = ssa_60_b0msb.eq(ssa_55_signMask);
	bool ssa_67_c0is1 = ssa_61_c0msb.eq(ssa_55_signMask);
	bool ssa_68_ab1 = ssa_62_a1is1 && ssa_63_b1is1;
	bool ssa_69_ac1 = ssa_62_a1is1 && ssa_64_c1is1;
	bool ssa_70_bc1 = ssa_63_b1is1 && ssa_64_c1is1;
	bool ssa_71_ab1c1 = ssa_68_ab1 || ssa_69_ac1;
	bool ssa_72_conflict = ssa_71_ab1c1 || ssa_70_bc1;
	bool ssa_73_any1ab = ssa_62_a1is1 || ssa_63_b1is1;
	bool ssa_74_any1 = ssa_73_any1ab || ssa_64_c1is1;
	bool ssa_75_all0ab = ssa_65_a0is1 && ssa_66_b0is1;
	bool ssa_76_all0 = ssa_75_all0ab && ssa_67_c0is1;
	APInt ssa_77_baseOne = ssa_74_any1 ? ssa_55_signMask : ssa_11_zero;
	APInt ssa_78_baseZero = ssa_76_all0 ? ssa_55_signMask : ssa_11_zero;
	APInt ssa_79_lowOne = ssa_27_sumLow | ssa_77_baseOne;
	APInt ssa_80_lowZ = ssa_28_lowZero | ssa_78_baseZero;
	APInt ssa_81_res1pre = ssa_79_lowOne | ssa_52_highOnes;
	APInt ssa_82_res0pre = ssa_80_lowZ | ssa_53_highZero;
	bool ssa_83_bad1 = ssa_72_conflict || ssa_45_ovMin;
	APInt ssa_84_res0 = ssa_83_bad1 ? ssa_9_all : ssa_82_res0pre;
	APInt ssa_85_res1 = ssa_83_bad1 ? ssa_9_all : ssa_81_res1pre;
	std::array<APInt, 2> ssa_86_r = std::array<APInt, 2>{ssa_84_res0, ssa_85_res1};
	return ssa_86_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_all = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_one(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_11_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_12_bw(ssa_3_a0.getBitWidth(), ssa_3_a0.getBitWidth());
	bool ssa_13_true = ssa_11_zero.eq(ssa_11_zero);
	APInt ssa_14_aKnown = ssa_3_a0 | ssa_4_a1;
	APInt ssa_15_maxB = ssa_9_all ^ ssa_5_b0;
	APInt ssa_16_maxC = ssa_9_all ^ ssa_7_c0;
	APInt ssa_17_nA(ssa_14_aKnown.getBitWidth(), ssa_14_aKnown.countr_one());
	APInt ssa_18_shlA;
	if (ssa_17_nA.uge(ssa_17_nA.getBitWidth())) {
		ssa_18_shlA = APInt(ssa_10_one.getBitWidth(), 0);
	} else {
		ssa_18_shlA = ssa_10_one.shl(ssa_17_nA);
	}
	APInt ssa_19_lowMaskA = ssa_18_shlA - ssa_10_one;
	APInt ssa_20_notLowMaskA = ssa_19_lowMaskA ^ ssa_9_all;
	APInt ssa_21_aLow = ssa_4_a1 & ssa_19_lowMaskA;
	APInt ssa_22_minBC = ssa_6_b1 + ssa_8_c1;
	bool ssa_23_ovMinBC = ssa_22_minBC.ult(ssa_6_b1);
	APInt ssa_24_maxBC = ssa_15_maxB + ssa_16_maxC;
	bool ssa_25_ovMaxBC = ssa_24_maxBC.ult(ssa_15_maxB);
	APInt ssa_26_minSumA = ssa_21_aLow + ssa_22_minBC;
	bool ssa_27_ovMinSumA = ssa_26_minSumA.ult(ssa_21_aLow);
	APInt ssa_28_maxSumA = ssa_21_aLow + ssa_24_maxBC;
	bool ssa_29_ovMaxSumA = ssa_28_maxSumA.ult(ssa_21_aLow);
	APInt ssa_30_diffHighA0 = ssa_26_minSumA ^ ssa_28_maxSumA;
	APInt ssa_31_diffHighA = ssa_30_diffHighA0 & ssa_20_notLowMaskA;
	bool ssa_32_highEqA0 = ssa_31_diffHighA.eq(ssa_11_zero);
	bool ssa_33_ovA1 = ssa_23_ovMinBC || ssa_25_ovMaxBC;
	bool ssa_34_ovA2 = ssa_27_ovMinSumA || ssa_29_ovMaxSumA;
	bool ssa_35_ovA = ssa_33_ovA1 || ssa_34_ovA2;
	bool ssa_36_noOvA = ssa_35_ovA != ssa_13_true;
	bool ssa_37_highEqA = ssa_32_highEqA0 && ssa_36_noOvA;
	APInt ssa_38_minLowA = ssa_26_minSumA & ssa_19_lowMaskA;
	APInt ssa_39_maxLowA = ssa_28_maxSumA & ssa_19_lowMaskA;
	APInt ssa_40_diffLowA = ssa_38_minLowA ^ ssa_39_maxLowA;
	APInt ssa_41_lzA(ssa_40_diffLowA.getBitWidth(), ssa_40_diffLowA.countl_zero());
	APInt ssa_42_shiftA = ssa_12_bw - ssa_41_lzA;
	APInt ssa_43_highMaskA;
	if (ssa_42_shiftA.uge(ssa_42_shiftA.getBitWidth())) {
		ssa_43_highMaskA = APInt(ssa_9_all.getBitWidth(), 0);
	} else {
		ssa_43_highMaskA = ssa_9_all.shl(ssa_42_shiftA);
	}
	APInt ssa_44_lowKnownMaskA = ssa_43_highMaskA & ssa_19_lowMaskA;
	APInt ssa_45_lowOnesA = ssa_38_minLowA & ssa_44_lowKnownMaskA;
	APInt ssa_46_lowZeroA = ssa_44_lowKnownMaskA ^ ssa_45_lowOnesA;
	APInt ssa_47_res1 = ssa_37_highEqA ? ssa_45_lowOnesA : ssa_11_zero;
	APInt ssa_48_res0 = ssa_37_highEqA ? ssa_46_lowZeroA : ssa_11_zero;
	std::array<APInt, 2> ssa_49_r = std::array<APInt, 2>{ssa_48_res0, ssa_47_res1};
	return ssa_49_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_all = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_zero(ssa_3_a0.getBitWidth(), 0);
	APInt ssa_11_one(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_12_aKnown = ssa_3_a0 | ssa_4_a1;
	APInt ssa_13_bKnown = ssa_5_b0 | ssa_6_b1;
	APInt ssa_14_cKnown = ssa_7_c0 | ssa_8_c1;
	APInt ssa_15_unkA = ssa_12_aKnown ^ ssa_9_all;
	APInt ssa_16_unkB = ssa_13_bKnown ^ ssa_9_all;
	APInt ssa_17_unkC = ssa_14_cKnown ^ ssa_9_all;
	APInt ssa_18_aMinus = ssa_15_unkA - ssa_11_one;
	APInt ssa_19_bMinus = ssa_16_unkB - ssa_11_one;
	APInt ssa_20_cMinus = ssa_17_unkC - ssa_11_one;
	APInt ssa_21_aPow = ssa_15_unkA & ssa_18_aMinus;
	APInt ssa_22_bPow = ssa_16_unkB & ssa_19_bMinus;
	APInt ssa_23_cPow = ssa_17_unkC & ssa_20_cMinus;
	bool ssa_24_aPow0 = ssa_21_aPow.eq(ssa_10_zero);
	bool ssa_25_bPow0 = ssa_22_bPow.eq(ssa_10_zero);
	bool ssa_26_cPow0 = ssa_23_cPow.eq(ssa_10_zero);
	bool ssa_27_abPow = ssa_24_aPow0 && ssa_25_bPow0;
	bool ssa_28_allPow = ssa_27_abPow && ssa_26_cPow0;
	APInt ssa_29_sum0ab = ssa_4_a1 + ssa_6_b1;
	APInt ssa_30_sum0 = ssa_29_sum0ab + ssa_8_c1;
	APInt ssa_31_sum1 = ssa_30_sum0 + ssa_15_unkA;
	APInt ssa_32_sum2 = ssa_30_sum0 + ssa_16_unkB;
	APInt ssa_33_sum3 = ssa_30_sum0 + ssa_17_unkC;
	APInt ssa_34_sum4 = ssa_31_sum1 + ssa_16_unkB;
	APInt ssa_35_sum5 = ssa_31_sum1 + ssa_17_unkC;
	APInt ssa_36_sum6 = ssa_32_sum2 + ssa_17_unkC;
	APInt ssa_37_sum7 = ssa_34_sum4 + ssa_17_unkC;
	APInt ssa_38_and01 = ssa_30_sum0 & ssa_31_sum1;
	APInt ssa_39_and23 = ssa_32_sum2 & ssa_33_sum3;
	APInt ssa_40_and45 = ssa_34_sum4 & ssa_35_sum5;
	APInt ssa_41_and67 = ssa_36_sum6 & ssa_37_sum7;
	APInt ssa_42_and0123 = ssa_38_and01 & ssa_39_and23;
	APInt ssa_43_and4567 = ssa_40_and45 & ssa_41_and67;
	APInt ssa_44_allAnd = ssa_42_and0123 & ssa_43_and4567;
	APInt ssa_45_or01 = ssa_30_sum0 | ssa_31_sum1;
	APInt ssa_46_or23 = ssa_32_sum2 | ssa_33_sum3;
	APInt ssa_47_or45 = ssa_34_sum4 | ssa_35_sum5;
	APInt ssa_48_or67 = ssa_36_sum6 | ssa_37_sum7;
	APInt ssa_49_or0123 = ssa_45_or01 | ssa_46_or23;
	APInt ssa_50_or4567 = ssa_47_or45 | ssa_48_or67;
	APInt ssa_51_allOr = ssa_49_or0123 | ssa_50_or4567;
	APInt ssa_52_knownZero = ssa_51_allOr ^ ssa_9_all;
	APInt ssa_53_res0 = ssa_28_allPow ? ssa_52_knownZero : ssa_10_zero;
	APInt ssa_54_res1 = ssa_28_allPow ? ssa_44_allAnd : ssa_10_zero;
	std::array<APInt, 2> ssa_55_r = std::array<APInt, 2>{ssa_53_res0, ssa_54_res1};
	return ssa_55_r;
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
