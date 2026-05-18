namespace Pattern260 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_lhs0 = ssa_0_lhs[0];
	APInt ssa_3_lhs1 = ssa_0_lhs[1];
	APInt ssa_4_rhs0 = ssa_1_rhs[0];
	APInt ssa_5_rhs1 = ssa_1_rhs[1];
	APInt ssa_6_bitwidth(ssa_4_rhs0.getBitWidth(), ssa_4_rhs0.getBitWidth());
	APInt ssa_7_allones = APInt::getAllOnes(ssa_4_rhs0.getBitWidth());
	APInt ssa_8_const0(ssa_4_rhs0.getBitWidth(), 0);
	APInt ssa_9_minshift = ssa_5_rhs1 & ssa_7_allones;
	bool ssa_10_uge = ssa_9_minshift.uge(ssa_6_bitwidth);
	APInt ssa_11_notlhs0 = ssa_2_lhs0 ^ ssa_7_allones;
	APInt ssa_12_maxlhs = ssa_3_lhs1 | ssa_11_notlhs0;
	APInt ssa_13_minlhs = ssa_3_lhs1 & ssa_7_allones;
	APInt ssa_14_notrhs0 = ssa_4_rhs0 ^ ssa_7_allones;
	APInt ssa_15_maxrhs = ssa_5_rhs1 | ssa_14_notrhs0;
	APInt ssa_16_minrhs = ssa_5_rhs1 & ssa_7_allones;
	APInt ssa_17_sumMin = ssa_13_minlhs + ssa_16_minrhs;
	APInt ssa_18_sumMax = ssa_12_maxlhs + ssa_15_maxrhs;
	bool ssa_19_ov;
	ssa_12_maxlhs.uadd_ov(ssa_15_maxrhs, ssa_19_ov);
	APInt ssa_20_diff = ssa_17_sumMin ^ ssa_18_sumMax;
	APInt ssa_21_clz(ssa_20_diff.getBitWidth(), ssa_20_diff.countl_zero());
	APInt ssa_22_prefixmask = ssa_8_const0;
	if (ssa_21_clz.ule(ssa_21_clz.getBitWidth()))
		ssa_22_prefixmask.setHighBits(ssa_21_clz.getZExtValue());
	else
		ssa_22_prefixmask.setHighBits(ssa_21_clz.getBitWidth());
	APInt ssa_23_notSumMin = ssa_17_sumMin ^ ssa_7_allones;
	APInt ssa_24_sumKnownZeroRaw = ssa_23_notSumMin & ssa_22_prefixmask;
	APInt ssa_25_sumKnownOneRaw = ssa_17_sumMin & ssa_22_prefixmask;
	APInt ssa_26_sumKnownZero = ssa_19_ov ? ssa_8_const0 : ssa_24_sumKnownZeroRaw;
	APInt ssa_27_sumKnownOne = ssa_19_ov ? ssa_8_const0 : ssa_25_sumKnownOneRaw;
	APInt ssa_28_knownMask = ssa_4_rhs0 | ssa_5_rhs1;
	bool ssa_29_isConst = ssa_28_knownMask.eq(ssa_7_allones);
	APInt ssa_30_shiftedZero;
	if (ssa_5_rhs1.uge(ssa_5_rhs1.getBitWidth())) {
		ssa_30_shiftedZero = APInt(ssa_26_sumKnownZero.getBitWidth(), 0);
	} else {
		ssa_30_shiftedZero = ssa_26_sumKnownZero.lshr(ssa_5_rhs1);
	}
	APInt ssa_31_shiftedOne;
	if (ssa_5_rhs1.uge(ssa_5_rhs1.getBitWidth())) {
		ssa_31_shiftedOne = APInt(ssa_27_sumKnownOne.getBitWidth(), 0);
	} else {
		ssa_31_shiftedOne = ssa_27_sumKnownOne.lshr(ssa_5_rhs1);
	}
	APInt ssa_32_highmaskConst = ssa_8_const0;
	if (ssa_5_rhs1.ule(ssa_5_rhs1.getBitWidth()))
		ssa_32_highmaskConst.setHighBits(ssa_5_rhs1.getZExtValue());
	else
		ssa_32_highmaskConst.setHighBits(ssa_5_rhs1.getBitWidth());
	APInt ssa_33_res0Const = ssa_30_shiftedZero | ssa_32_highmaskConst;
	APInt ssa_34_res1Const = ssa_31_shiftedOne & ssa_7_allones;
	APInt ssa_35_highmaskMin = ssa_8_const0;
	if (ssa_9_minshift.ule(ssa_9_minshift.getBitWidth()))
		ssa_35_highmaskMin.setHighBits(ssa_9_minshift.getZExtValue());
	else
		ssa_35_highmaskMin.setHighBits(ssa_9_minshift.getBitWidth());
	APInt ssa_36_res0Var = ssa_35_highmaskMin & ssa_7_allones;
	APInt ssa_37_res1Var = ssa_8_const0 & ssa_7_allones;
	APInt ssa_38_res0Sel = ssa_29_isConst ? ssa_33_res0Const : ssa_36_res0Var;
	APInt ssa_39_res1Sel = ssa_29_isConst ? ssa_34_res1Const : ssa_37_res1Var;
	APInt ssa_40_res0Final = ssa_10_uge ? ssa_7_allones : ssa_38_res0Sel;
	APInt ssa_41_res1Final = ssa_10_uge ? ssa_7_allones : ssa_39_res1Sel;
	std::array<APInt, 2> ssa_42_r = std::array<APInt, 2>{ssa_40_res0Final, ssa_41_res1Final};
	return ssa_42_r;
}
std::array<APInt, 2> partial_solution_1(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_lhs0 = ssa_0_lhs[0];
	APInt ssa_3_lhs1 = ssa_0_lhs[1];
	APInt ssa_4_rhs0 = ssa_1_rhs[0];
	APInt ssa_5_rhs1 = ssa_1_rhs[1];
	APInt ssa_6_bitwidth(ssa_4_rhs0.getBitWidth(), ssa_4_rhs0.getBitWidth());
	APInt ssa_7_allones = APInt::getAllOnes(ssa_4_rhs0.getBitWidth());
	APInt ssa_8_const0(ssa_4_rhs0.getBitWidth(), 0);
	APInt ssa_9_const1(ssa_4_rhs0.getBitWidth(), 1);
	APInt ssa_10_notlhs0 = ssa_2_lhs0 ^ ssa_7_allones;
	APInt ssa_11_maxlhs = ssa_3_lhs1 | ssa_10_notlhs0;
	APInt ssa_12_minlhs = ssa_3_lhs1 & ssa_7_allones;
	APInt ssa_13_notrhs0 = ssa_4_rhs0 ^ ssa_7_allones;
	APInt ssa_14_maxrhs = ssa_5_rhs1 | ssa_13_notrhs0;
	APInt ssa_15_minrhs = ssa_5_rhs1 & ssa_7_allones;
	APInt ssa_16_bwminus1 = ssa_6_bitwidth - ssa_9_const1;
	APInt ssa_17_maxshift = APIntOps::umin(ssa_14_maxrhs, ssa_16_bwminus1);
	APInt ssa_18_sumMin = ssa_12_minlhs + ssa_15_minrhs;
	APInt ssa_19_sumMax = ssa_11_maxlhs + ssa_14_maxrhs;
	bool ssa_20_ov;
	ssa_11_maxlhs.uadd_ov(ssa_14_maxrhs, ssa_20_ov);
	APInt ssa_21_sumMinB = ssa_20_ov ? ssa_8_const0 : ssa_18_sumMin;
	APInt ssa_22_sumMaxB = ssa_20_ov ? ssa_7_allones : ssa_19_sumMax;
	APInt ssa_23_minRes;
	if (ssa_17_maxshift.uge(ssa_17_maxshift.getBitWidth())) {
		ssa_23_minRes = APInt(ssa_21_sumMinB.getBitWidth(), 0);
	} else {
		ssa_23_minRes = ssa_21_sumMinB.lshr(ssa_17_maxshift);
	}
	APInt ssa_24_maxRes;
	if (ssa_15_minrhs.uge(ssa_15_minrhs.getBitWidth())) {
		ssa_24_maxRes = APInt(ssa_22_sumMaxB.getBitWidth(), 0);
	} else {
		ssa_24_maxRes = ssa_22_sumMaxB.lshr(ssa_15_minrhs);
	}
	APInt ssa_25_diff = ssa_23_minRes ^ ssa_24_maxRes;
	APInt ssa_26_clz(ssa_25_diff.getBitWidth(), ssa_25_diff.countl_zero());
	APInt ssa_27_prefixmask = ssa_8_const0;
	if (ssa_26_clz.ule(ssa_26_clz.getBitWidth()))
		ssa_27_prefixmask.setHighBits(ssa_26_clz.getZExtValue());
	else
		ssa_27_prefixmask.setHighBits(ssa_26_clz.getBitWidth());
	APInt ssa_28_notMinRes = ssa_23_minRes ^ ssa_7_allones;
	APInt ssa_29_res0 = ssa_28_notMinRes & ssa_27_prefixmask;
	APInt ssa_30_res1 = ssa_23_minRes & ssa_27_prefixmask;
	bool ssa_31_uge = ssa_15_minrhs.uge(ssa_6_bitwidth);
	APInt ssa_32_res0Final = ssa_31_uge ? ssa_7_allones : ssa_29_res0;
	APInt ssa_33_res1Final = ssa_31_uge ? ssa_7_allones : ssa_30_res1;
	std::array<APInt, 2> ssa_34_r = std::array<APInt, 2>{ssa_32_res0Final, ssa_33_res1Final};
	return ssa_34_r;
}
std::array<APInt, 2> partial_solution_2(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_lhs0 = ssa_0_lhs[0];
	APInt ssa_3_lhs1 = ssa_0_lhs[1];
	APInt ssa_4_rhs0 = ssa_1_rhs[0];
	APInt ssa_5_rhs1 = ssa_1_rhs[1];
	APInt ssa_6_bitwidth(ssa_4_rhs0.getBitWidth(), ssa_4_rhs0.getBitWidth());
	APInt ssa_7_allones = APInt::getAllOnes(ssa_4_rhs0.getBitWidth());
	APInt ssa_8_const0(ssa_4_rhs0.getBitWidth(), 0);
	APInt ssa_9_const1(ssa_4_rhs0.getBitWidth(), 1);
	APInt ssa_10_notlhs0 = ssa_2_lhs0 ^ ssa_7_allones;
	APInt ssa_11_maxlhs = ssa_3_lhs1 | ssa_10_notlhs0;
	APInt ssa_12_minlhs = ssa_3_lhs1 & ssa_7_allones;
	APInt ssa_13_notrhs0 = ssa_4_rhs0 ^ ssa_7_allones;
	APInt ssa_14_maxrhs = ssa_5_rhs1 | ssa_13_notrhs0;
	APInt ssa_15_minrhs = ssa_5_rhs1 & ssa_7_allones;
	APInt ssa_16_bwminus1 = ssa_6_bitwidth - ssa_9_const1;
	APInt ssa_17_maxrhsC0 = APIntOps::umin(ssa_14_maxrhs, ssa_16_bwminus1);
	APInt ssa_18_maxrhsC = ssa_17_maxrhsC0 & ssa_13_notrhs0;
	APInt ssa_19_sumMax = ssa_11_maxlhs + ssa_18_maxrhsC;
	bool ssa_20_ov;
	ssa_11_maxlhs.uadd_ov(ssa_18_maxrhsC, ssa_20_ov);
	APInt ssa_21_sumMaxB = ssa_20_ov ? ssa_7_allones : ssa_19_sumMax;
	APInt ssa_22_tz(ssa_4_rhs0.getBitWidth(), ssa_4_rhs0.countr_one());
	APInt ssa_23_lowMask = ssa_8_const0;
	if (ssa_22_tz.ule(ssa_22_tz.getBitWidth()))
		ssa_23_lowMask.setLowBits(ssa_22_tz.getZExtValue());
	else
		ssa_23_lowMask.setLowBits(ssa_22_tz.getBitWidth());
	APInt ssa_24_minNonZero = ssa_23_lowMask + ssa_9_const1;
	APInt ssa_25_minrhsNZ = APIntOps::umax(ssa_15_minrhs, ssa_24_minNonZero);
	APInt ssa_26_sumMinNZ = ssa_12_minlhs + ssa_25_minrhsNZ;
	APInt ssa_27_sumMinBNZ = ssa_20_ov ? ssa_8_const0 : ssa_26_sumMinNZ;
	APInt ssa_28_minResNZ;
	if (ssa_18_maxrhsC.uge(ssa_18_maxrhsC.getBitWidth())) {
		ssa_28_minResNZ = APInt(ssa_27_sumMinBNZ.getBitWidth(), 0);
	} else {
		ssa_28_minResNZ = ssa_27_sumMinBNZ.lshr(ssa_18_maxrhsC);
	}
	APInt ssa_29_maxResNZ;
	if (ssa_25_minrhsNZ.uge(ssa_25_minrhsNZ.getBitWidth())) {
		ssa_29_maxResNZ = APInt(ssa_21_sumMaxB.getBitWidth(), 0);
	} else {
		ssa_29_maxResNZ = ssa_21_sumMaxB.lshr(ssa_25_minrhsNZ);
	}
	APInt ssa_30_diffNZ = ssa_28_minResNZ ^ ssa_29_maxResNZ;
	APInt ssa_31_clzNZ(ssa_30_diffNZ.getBitWidth(), ssa_30_diffNZ.countl_zero());
	APInt ssa_32_prefixmaskNZ = ssa_8_const0;
	if (ssa_31_clzNZ.ule(ssa_31_clzNZ.getBitWidth()))
		ssa_32_prefixmaskNZ.setHighBits(ssa_31_clzNZ.getZExtValue());
	else
		ssa_32_prefixmaskNZ.setHighBits(ssa_31_clzNZ.getBitWidth());
	APInt ssa_33_notMinResNZ = ssa_28_minResNZ ^ ssa_7_allones;
	APInt ssa_34_res0NZ = ssa_33_notMinResNZ & ssa_32_prefixmaskNZ;
	APInt ssa_35_res1NZ = ssa_28_minResNZ & ssa_32_prefixmaskNZ;
	bool ssa_36_bZeroPossible = ssa_15_minrhs.eq(ssa_8_const0);
	bool ssa_37_bNonZeroPossible = ssa_18_maxrhsC.ne(ssa_8_const0);
	APInt ssa_38_res0Union = ssa_2_lhs0 & ssa_34_res0NZ;
	APInt ssa_39_res1Union = ssa_3_lhs1 & ssa_35_res1NZ;
	APInt ssa_40_res0Zsel = ssa_37_bNonZeroPossible ? ssa_38_res0Union : ssa_2_lhs0;
	APInt ssa_41_res1Zsel = ssa_37_bNonZeroPossible ? ssa_39_res1Union : ssa_3_lhs1;
	APInt ssa_42_res0All = ssa_36_bZeroPossible ? ssa_40_res0Zsel : ssa_34_res0NZ;
	APInt ssa_43_res1All = ssa_36_bZeroPossible ? ssa_41_res1Zsel : ssa_35_res1NZ;
	APInt ssa_44_rhsKnown = ssa_4_rhs0 | ssa_5_rhs1;
	bool ssa_45_rhsConst = ssa_44_rhsKnown.eq(ssa_7_allones);
	bool ssa_46_rhsIsOne = ssa_5_rhs1.eq(ssa_9_const1);
	bool ssa_47_rhsConstOne = ssa_45_rhsConst && ssa_46_rhsIsOne;
	APInt ssa_48_lhsT1(ssa_3_lhs1.getBitWidth(), ssa_3_lhs1.countr_one());
	bool ssa_49_lhsT1Gt0 = ssa_48_lhsT1.ne(ssa_8_const0);
	APInt ssa_50_lhsT1Minus1 = ssa_48_lhsT1 - ssa_9_const1;
	APInt ssa_51_lhsT1Safe = ssa_49_lhsT1Gt0 ? ssa_50_lhsT1Minus1 : ssa_8_const0;
	APInt ssa_52_lowMaskOut = ssa_8_const0;
	if (ssa_51_lhsT1Safe.ule(ssa_51_lhsT1Safe.getBitWidth()))
		ssa_52_lowMaskOut.setLowBits(ssa_51_lhsT1Safe.getZExtValue());
	else
		ssa_52_lowMaskOut.setLowBits(ssa_51_lhsT1Safe.getBitWidth());
	APInt ssa_53_res0Aug = ssa_42_res0All | ssa_52_lowMaskOut;
	APInt ssa_54_res0Rhs1 = ssa_47_rhsConstOne ? ssa_53_res0Aug : ssa_42_res0All;
	APInt ssa_55_lhsKnown = ssa_2_lhs0 | ssa_3_lhs1;
	bool ssa_56_lhsConst = ssa_55_lhsKnown.eq(ssa_7_allones);
	bool ssa_57_lhsIsZero = ssa_3_lhs1.eq(ssa_8_const0);
	bool ssa_58_lhsConstZero = ssa_56_lhsConst && ssa_57_lhsIsZero;
	APInt ssa_59_res0Zero = ssa_58_lhsConstZero ? ssa_7_allones : ssa_54_res0Rhs1;
	APInt ssa_60_res1Zero = ssa_58_lhsConstZero ? ssa_8_const0 : ssa_43_res1All;
	bool ssa_61_uge = ssa_15_minrhs.uge(ssa_6_bitwidth);
	APInt ssa_62_res0Final = ssa_61_uge ? ssa_7_allones : ssa_59_res0Zero;
	APInt ssa_63_res1Final = ssa_61_uge ? ssa_7_allones : ssa_60_res1Zero;
	std::array<APInt, 2> ssa_64_r = std::array<APInt, 2>{ssa_62_res0Final, ssa_63_res1Final};
	return ssa_64_r;
}
std::array<APInt, 2> partial_solution_3(std::array<APInt, 2> ssa_0_lhs, std::array<APInt, 2> ssa_1_rhs) {
	APInt ssa_2_lhs0 = ssa_0_lhs[0];
	APInt ssa_3_lhs1 = ssa_0_lhs[1];
	APInt ssa_4_rhs0 = ssa_1_rhs[0];
	APInt ssa_5_rhs1 = ssa_1_rhs[1];
	APInt ssa_6_allones = APInt::getAllOnes(ssa_2_lhs0.getBitWidth());
	APInt ssa_7_const0(ssa_2_lhs0.getBitWidth(), 0);
	APInt ssa_8_const1(ssa_2_lhs0.getBitWidth(), 1);
	APInt ssa_9_rhsKnown = ssa_4_rhs0 | ssa_5_rhs1;
	bool ssa_10_rhsConst = ssa_9_rhsKnown.eq(ssa_6_allones);
	bool ssa_11_rhsIsZero = ssa_5_rhs1.eq(ssa_7_const0);
	bool ssa_12_rhsConstZero = ssa_10_rhsConst && ssa_11_rhsIsZero;
	APInt ssa_13_notlhs0 = ssa_2_lhs0 ^ ssa_6_allones;
	APInt ssa_14_maxlhs = ssa_3_lhs1 | ssa_13_notlhs0;
	APInt ssa_15_minlhs = ssa_3_lhs1 & ssa_6_allones;
	APInt ssa_16_notrhs0 = ssa_4_rhs0 ^ ssa_6_allones;
	APInt ssa_17_maxrhs = ssa_5_rhs1 | ssa_16_notrhs0;
	APInt ssa_18_minrhs = ssa_5_rhs1 & ssa_6_allones;
	APInt ssa_19_sumMin = ssa_15_minlhs + ssa_18_minrhs;
	APInt ssa_20_sumMax = ssa_14_maxlhs + ssa_17_maxrhs;
	bool ssa_21_ov;
	ssa_14_maxlhs.uadd_ov(ssa_17_maxrhs, ssa_21_ov);
	APInt ssa_22_diff = ssa_19_sumMin ^ ssa_20_sumMax;
	APInt ssa_23_clz(ssa_22_diff.getBitWidth(), ssa_22_diff.countl_zero());
	APInt ssa_24_prefixmask = ssa_7_const0;
	if (ssa_23_clz.ule(ssa_23_clz.getBitWidth()))
		ssa_24_prefixmask.setHighBits(ssa_23_clz.getZExtValue());
	else
		ssa_24_prefixmask.setHighBits(ssa_23_clz.getBitWidth());
	APInt ssa_25_notSumMin = ssa_19_sumMin ^ ssa_6_allones;
	APInt ssa_26_sumKnownZeroRaw = ssa_25_notSumMin & ssa_24_prefixmask;
	APInt ssa_27_sumKnownOneRaw = ssa_19_sumMin & ssa_24_prefixmask;
	APInt ssa_28_sumKnownZeroBase = ssa_21_ov ? ssa_7_const0 : ssa_26_sumKnownZeroRaw;
	APInt ssa_29_sumKnownOneBase = ssa_21_ov ? ssa_7_const0 : ssa_27_sumKnownOneRaw;
	APInt ssa_30_knownMask = ssa_2_lhs0 | ssa_3_lhs1;
	APInt ssa_31_t(ssa_30_knownMask.getBitWidth(), ssa_30_knownMask.countr_one());
	APInt ssa_32_lowmask = ssa_7_const0;
	if (ssa_31_t.ule(ssa_31_t.getBitWidth()))
		ssa_32_lowmask.setLowBits(ssa_31_t.getZExtValue());
	else
		ssa_32_lowmask.setLowBits(ssa_31_t.getBitWidth());
	APInt ssa_33_lowVal = ssa_3_lhs1 & ssa_32_lowmask;
	APInt ssa_34_sumLow = ssa_33_lowVal + ssa_5_rhs1;
	APInt ssa_35_sumLowMasked = ssa_34_sumLow & ssa_32_lowmask;
	APInt ssa_36_notSumLow = ssa_35_sumLowMasked ^ ssa_6_allones;
	APInt ssa_37_sumKnownZeroLow = ssa_36_notSumLow & ssa_32_lowmask;
	APInt ssa_38_sumKnownOneLow = ssa_35_sumLowMasked & ssa_32_lowmask;
	APInt ssa_39_sumKnownZero = ssa_28_sumKnownZeroBase | ssa_37_sumKnownZeroLow;
	APInt ssa_40_sumKnownOne = ssa_29_sumKnownOneBase | ssa_38_sumKnownOneLow;
	APInt ssa_41_shiftedZero;
	if (ssa_5_rhs1.uge(ssa_5_rhs1.getBitWidth())) {
		ssa_41_shiftedZero = APInt(ssa_39_sumKnownZero.getBitWidth(), 0);
	} else {
		ssa_41_shiftedZero = ssa_39_sumKnownZero.lshr(ssa_5_rhs1);
	}
	APInt ssa_42_shiftedOne;
	if (ssa_5_rhs1.uge(ssa_5_rhs1.getBitWidth())) {
		ssa_42_shiftedOne = APInt(ssa_40_sumKnownOne.getBitWidth(), 0);
	} else {
		ssa_42_shiftedOne = ssa_40_sumKnownOne.lshr(ssa_5_rhs1);
	}
	APInt ssa_43_highmask = ssa_7_const0;
	if (ssa_5_rhs1.ule(ssa_5_rhs1.getBitWidth()))
		ssa_43_highmask.setHighBits(ssa_5_rhs1.getZExtValue());
	else
		ssa_43_highmask.setHighBits(ssa_5_rhs1.getBitWidth());
	APInt ssa_44_res0Const = ssa_41_shiftedZero | ssa_43_highmask;
	APInt ssa_45_res1Const = ssa_42_shiftedOne & ssa_6_allones;
	APInt ssa_46_res0ConstZ = ssa_12_rhsConstZero ? ssa_2_lhs0 : ssa_44_res0Const;
	APInt ssa_47_res1ConstZ = ssa_12_rhsConstZero ? ssa_3_lhs1 : ssa_45_res1Const;
	APInt ssa_48_tz(ssa_4_rhs0.getBitWidth(), ssa_4_rhs0.countr_one());
	APInt ssa_49_minNonZero;
	if (ssa_48_tz.uge(ssa_48_tz.getBitWidth())) {
		ssa_49_minNonZero = APInt(ssa_8_const1.getBitWidth(), 0);
	} else {
		ssa_49_minNonZero = ssa_8_const1.shl(ssa_48_tz);
	}
	bool ssa_50_ovNZ;
	ssa_14_maxlhs.uadd_ov(ssa_49_minNonZero, ssa_50_ovNZ);
	APInt ssa_51_sumMaxNZ = ssa_14_maxlhs + ssa_49_minNonZero;
	APInt ssa_52_sumMaxNZB = ssa_50_ovNZ ? ssa_6_allones : ssa_51_sumMaxNZ;
	APInt ssa_53_maxResNZ;
	if (ssa_49_minNonZero.uge(ssa_49_minNonZero.getBitWidth())) {
		ssa_53_maxResNZ = APInt(ssa_52_sumMaxNZB.getBitWidth(), 0);
	} else {
		ssa_53_maxResNZ = ssa_52_sumMaxNZB.lshr(ssa_49_minNonZero);
	}
	APInt ssa_54_clzNZ(ssa_53_maxResNZ.getBitWidth(), ssa_53_maxResNZ.countl_zero());
	APInt ssa_55_res0NZ = ssa_7_const0;
	if (ssa_54_clzNZ.ule(ssa_54_clzNZ.getBitWidth()))
		ssa_55_res0NZ.setHighBits(ssa_54_clzNZ.getZExtValue());
	else
		ssa_55_res0NZ.setHighBits(ssa_54_clzNZ.getBitWidth());
	APInt ssa_56_res1NZ = ssa_7_const0 & ssa_6_allones;
	bool ssa_57_bZeroPossible = ssa_5_rhs1.eq(ssa_7_const0);
	bool ssa_58_bNonZeroPossible = ssa_4_rhs0.ne(ssa_6_allones);
	APInt ssa_59_res0Union = ssa_2_lhs0 & ssa_55_res0NZ;
	APInt ssa_60_res1Union = ssa_3_lhs1 & ssa_56_res1NZ;
	APInt ssa_61_res0Zsel = ssa_58_bNonZeroPossible ? ssa_59_res0Union : ssa_2_lhs0;
	APInt ssa_62_res1Zsel = ssa_58_bNonZeroPossible ? ssa_60_res1Union : ssa_3_lhs1;
	APInt ssa_63_res0Var = ssa_57_bZeroPossible ? ssa_61_res0Zsel : ssa_55_res0NZ;
	APInt ssa_64_res1Var = ssa_57_bZeroPossible ? ssa_62_res1Zsel : ssa_56_res1NZ;
	APInt ssa_65_res0Final = ssa_10_rhsConst ? ssa_46_res0ConstZ : ssa_63_res0Var;
	APInt ssa_66_res1Final = ssa_10_rhsConst ? ssa_47_res1ConstZ : ssa_64_res1Var;
	std::array<APInt, 2> ssa_67_r = std::array<APInt, 2>{ssa_65_res0Final, ssa_66_res1Final};
	return ssa_67_r;
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
	std::array<APInt, 2> ssa_5_ = partial_solution_3(ssa_0_, ssa_1_);
	std::array<APInt, 2> ssa_6_ = meet(ssa_2_, ssa_3_);
	std::array<APInt, 2> ssa_7_ = meet(ssa_6_, ssa_4_);
	std::array<APInt, 2> ssa_8_ = meet(ssa_7_, ssa_5_);
	return ssa_8_;
}
}
