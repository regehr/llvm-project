namespace Pattern003 {
std::array<APInt, 2> partial_solution_0(std::array<APInt, 2> ssa_0_arg0, std::array<APInt, 2> ssa_1_arg1, std::array<APInt, 2> ssa_2_arg2) {
	APInt ssa_3_a0 = ssa_0_arg0[0];
	APInt ssa_4_a1 = ssa_0_arg0[1];
	APInt ssa_5_b0 = ssa_1_arg1[0];
	APInt ssa_6_b1 = ssa_1_arg1[1];
	APInt ssa_7_c0 = ssa_2_arg2[0];
	APInt ssa_8_c1 = ssa_2_arg2[1];
	APInt ssa_9_all = APInt::getAllOnes(ssa_3_a0.getBitWidth());
	APInt ssa_10_one(ssa_3_a0.getBitWidth(), 1);
	APInt ssa_11_knownMaskB = ssa_5_b0 | ssa_6_b1;
	APInt ssa_12_knownMaskC = ssa_7_c0 | ssa_8_c1;
	APInt ssa_13_bMax = ssa_9_all ^ ssa_5_b0;
	APInt ssa_14_cMax = ssa_9_all ^ ssa_7_c0;
	APInt ssa_15_sumMin_bc = ssa_6_b1 + ssa_8_c1;
	APInt ssa_16_sumMax_bc = ssa_13_bMax + ssa_14_cMax;
	APInt ssa_17_b1_and_c1 = ssa_6_b1 & ssa_8_c1;
	APInt ssa_18_b1_or_c1 = ssa_6_b1 | ssa_8_c1;
	APInt ssa_19_not_sumMin_bc = ssa_9_all ^ ssa_15_sumMin_bc;
	APInt ssa_20_tmp1 = ssa_18_b1_or_c1 & ssa_19_not_sumMin_bc;
	APInt ssa_21_carryMin_bc = ssa_17_b1_and_c1 | ssa_20_tmp1;
	APInt ssa_22_bMax_and_cMax = ssa_13_bMax & ssa_14_cMax;
	APInt ssa_23_bMax_or_cMax = ssa_13_bMax | ssa_14_cMax;
	APInt ssa_24_not_sumMax_bc = ssa_9_all ^ ssa_16_sumMax_bc;
	APInt ssa_25_tmp2 = ssa_23_bMax_or_cMax & ssa_24_not_sumMax_bc;
	APInt ssa_26_carryMax_bc = ssa_22_bMax_and_cMax | ssa_25_tmp2;
	APInt ssa_27_carryInMin_bc;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_27_carryInMin_bc = APInt(ssa_21_carryMin_bc.getBitWidth(), 0);
	} else {
		ssa_27_carryInMin_bc = ssa_21_carryMin_bc.shl(ssa_10_one);
	}
	APInt ssa_28_carryInMax_bc;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_28_carryInMax_bc = APInt(ssa_26_carryMax_bc.getBitWidth(), 0);
	} else {
		ssa_28_carryInMax_bc = ssa_26_carryMax_bc.shl(ssa_10_one);
	}
	APInt ssa_29_carryKnownZero_bc = ssa_9_all ^ ssa_28_carryInMax_bc;
	APInt ssa_30_carryKnown_bc = ssa_27_carryInMin_bc | ssa_29_carryKnownZero_bc;
	APInt ssa_31_knownMask_bc = ssa_11_knownMaskB & ssa_12_knownMaskC;
	APInt ssa_32_knownMask_bc2 = ssa_31_knownMask_bc & ssa_30_carryKnown_bc;
	APInt ssa_33_sumVal_bc = ssa_6_b1 ^ ssa_8_c1;
	APInt ssa_34_sumVal_bc2 = ssa_33_sumVal_bc ^ ssa_27_carryInMin_bc;
	APInt ssa_35_knownOne_bc = ssa_34_sumVal_bc2 & ssa_32_knownMask_bc2;
	APInt ssa_36_not_sumVal_bc2 = ssa_9_all ^ ssa_34_sumVal_bc2;
	APInt ssa_37_knownZero_bc = ssa_36_not_sumVal_bc2 & ssa_32_knownMask_bc2;
	APInt ssa_38_knownMaskA = ssa_3_a0 | ssa_4_a1;
	APInt ssa_39_knownMaskS = ssa_37_knownZero_bc | ssa_35_knownOne_bc;
	APInt ssa_40_aMax = ssa_9_all ^ ssa_3_a0;
	APInt ssa_41_sumMin_as = ssa_4_a1 + ssa_15_sumMin_bc;
	APInt ssa_42_sumMax_as = ssa_40_aMax + ssa_16_sumMax_bc;
	APInt ssa_43_a1_and_sMin = ssa_4_a1 & ssa_15_sumMin_bc;
	APInt ssa_44_a1_or_sMin = ssa_4_a1 | ssa_15_sumMin_bc;
	APInt ssa_45_not_sumMin_as = ssa_9_all ^ ssa_41_sumMin_as;
	APInt ssa_46_tmp3 = ssa_44_a1_or_sMin & ssa_45_not_sumMin_as;
	APInt ssa_47_carryMin_as = ssa_43_a1_and_sMin | ssa_46_tmp3;
	APInt ssa_48_aMax_and_sMax = ssa_40_aMax & ssa_16_sumMax_bc;
	APInt ssa_49_aMax_or_sMax = ssa_40_aMax | ssa_16_sumMax_bc;
	APInt ssa_50_not_sumMax_as = ssa_9_all ^ ssa_42_sumMax_as;
	APInt ssa_51_tmp4 = ssa_49_aMax_or_sMax & ssa_50_not_sumMax_as;
	APInt ssa_52_carryMax_as = ssa_48_aMax_and_sMax | ssa_51_tmp4;
	APInt ssa_53_carryInMin_as;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_53_carryInMin_as = APInt(ssa_47_carryMin_as.getBitWidth(), 0);
	} else {
		ssa_53_carryInMin_as = ssa_47_carryMin_as.shl(ssa_10_one);
	}
	APInt ssa_54_carryInMax_as;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_54_carryInMax_as = APInt(ssa_52_carryMax_as.getBitWidth(), 0);
	} else {
		ssa_54_carryInMax_as = ssa_52_carryMax_as.shl(ssa_10_one);
	}
	APInt ssa_55_carryKnownZero_as = ssa_9_all ^ ssa_54_carryInMax_as;
	APInt ssa_56_carryKnown_as = ssa_53_carryInMin_as | ssa_55_carryKnownZero_as;
	APInt ssa_57_knownMask_as = ssa_38_knownMaskA & ssa_39_knownMaskS;
	APInt ssa_58_knownMask_as2 = ssa_57_knownMask_as & ssa_56_carryKnown_as;
	APInt ssa_59_sumVal_as = ssa_4_a1 ^ ssa_35_knownOne_bc;
	APInt ssa_60_sumVal_as2 = ssa_59_sumVal_as ^ ssa_53_carryInMin_as;
	APInt ssa_61_knownOne_as = ssa_60_sumVal_as2 & ssa_58_knownMask_as2;
	APInt ssa_62_not_sumVal_as2 = ssa_9_all ^ ssa_60_sumVal_as2;
	APInt ssa_63_knownZero_as = ssa_62_not_sumVal_as2 & ssa_58_knownMask_as2;
	std::array<APInt, 2> ssa_64_r = std::array<APInt, 2>{ssa_63_knownZero_as, ssa_61_knownOne_as};
	return ssa_64_r;
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
	APInt ssa_11_knownMaskA = ssa_3_a0 | ssa_4_a1;
	APInt ssa_12_knownMaskB = ssa_5_b0 | ssa_6_b1;
	APInt ssa_13_knownMaskC = ssa_7_c0 | ssa_8_c1;
	APInt ssa_14_aMax = ssa_9_all ^ ssa_3_a0;
	APInt ssa_15_bMax = ssa_9_all ^ ssa_5_b0;
	APInt ssa_16_cMax = ssa_9_all ^ ssa_7_c0;
	APInt ssa_17_sumMin_ab = ssa_4_a1 + ssa_6_b1;
	APInt ssa_18_sumMax_ab = ssa_14_aMax + ssa_15_bMax;
	APInt ssa_19_a1_and_b1 = ssa_4_a1 & ssa_6_b1;
	APInt ssa_20_a1_or_b1 = ssa_4_a1 | ssa_6_b1;
	APInt ssa_21_not_sumMin_ab = ssa_9_all ^ ssa_17_sumMin_ab;
	APInt ssa_22_tmp1 = ssa_20_a1_or_b1 & ssa_21_not_sumMin_ab;
	APInt ssa_23_carryMin_ab = ssa_19_a1_and_b1 | ssa_22_tmp1;
	APInt ssa_24_aMax_and_bMax = ssa_14_aMax & ssa_15_bMax;
	APInt ssa_25_aMax_or_bMax = ssa_14_aMax | ssa_15_bMax;
	APInt ssa_26_not_sumMax_ab = ssa_9_all ^ ssa_18_sumMax_ab;
	APInt ssa_27_tmp2 = ssa_25_aMax_or_bMax & ssa_26_not_sumMax_ab;
	APInt ssa_28_carryMax_ab = ssa_24_aMax_and_bMax | ssa_27_tmp2;
	APInt ssa_29_carryInMin_ab;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_29_carryInMin_ab = APInt(ssa_23_carryMin_ab.getBitWidth(), 0);
	} else {
		ssa_29_carryInMin_ab = ssa_23_carryMin_ab.shl(ssa_10_one);
	}
	APInt ssa_30_carryInMax_ab;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_30_carryInMax_ab = APInt(ssa_28_carryMax_ab.getBitWidth(), 0);
	} else {
		ssa_30_carryInMax_ab = ssa_28_carryMax_ab.shl(ssa_10_one);
	}
	APInt ssa_31_carryKnownZero_ab = ssa_9_all ^ ssa_30_carryInMax_ab;
	APInt ssa_32_carryKnown_ab = ssa_29_carryInMin_ab | ssa_31_carryKnownZero_ab;
	APInt ssa_33_knownMask_ab = ssa_11_knownMaskA & ssa_12_knownMaskB;
	APInt ssa_34_knownMask_ab2 = ssa_33_knownMask_ab & ssa_32_carryKnown_ab;
	APInt ssa_35_sumVal_ab = ssa_4_a1 ^ ssa_6_b1;
	APInt ssa_36_sumVal_ab2 = ssa_35_sumVal_ab ^ ssa_29_carryInMin_ab;
	APInt ssa_37_knownOne_ab = ssa_36_sumVal_ab2 & ssa_34_knownMask_ab2;
	APInt ssa_38_not_sumVal_ab2 = ssa_9_all ^ ssa_36_sumVal_ab2;
	APInt ssa_39_knownZero_ab = ssa_38_not_sumVal_ab2 & ssa_34_knownMask_ab2;
	APInt ssa_40_knownMaskS1 = ssa_39_knownZero_ab | ssa_37_knownOne_ab;
	APInt ssa_41_sumMin_sc1 = ssa_17_sumMin_ab + ssa_8_c1;
	APInt ssa_42_sumMax_sc1 = ssa_18_sumMax_ab + ssa_16_cMax;
	APInt ssa_43_sMin1_and_c1 = ssa_17_sumMin_ab & ssa_8_c1;
	APInt ssa_44_sMin1_or_c1 = ssa_17_sumMin_ab | ssa_8_c1;
	APInt ssa_45_not_sumMin_sc1 = ssa_9_all ^ ssa_41_sumMin_sc1;
	APInt ssa_46_tmp3 = ssa_44_sMin1_or_c1 & ssa_45_not_sumMin_sc1;
	APInt ssa_47_carryMin_sc1 = ssa_43_sMin1_and_c1 | ssa_46_tmp3;
	APInt ssa_48_sMax1_and_cMax = ssa_18_sumMax_ab & ssa_16_cMax;
	APInt ssa_49_sMax1_or_cMax = ssa_18_sumMax_ab | ssa_16_cMax;
	APInt ssa_50_not_sumMax_sc1 = ssa_9_all ^ ssa_42_sumMax_sc1;
	APInt ssa_51_tmp4 = ssa_49_sMax1_or_cMax & ssa_50_not_sumMax_sc1;
	APInt ssa_52_carryMax_sc1 = ssa_48_sMax1_and_cMax | ssa_51_tmp4;
	APInt ssa_53_carryInMin_sc1;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_53_carryInMin_sc1 = APInt(ssa_47_carryMin_sc1.getBitWidth(), 0);
	} else {
		ssa_53_carryInMin_sc1 = ssa_47_carryMin_sc1.shl(ssa_10_one);
	}
	APInt ssa_54_carryInMax_sc1;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_54_carryInMax_sc1 = APInt(ssa_52_carryMax_sc1.getBitWidth(), 0);
	} else {
		ssa_54_carryInMax_sc1 = ssa_52_carryMax_sc1.shl(ssa_10_one);
	}
	APInt ssa_55_carryKnownZero_sc1 = ssa_9_all ^ ssa_54_carryInMax_sc1;
	APInt ssa_56_carryKnown_sc1 = ssa_53_carryInMin_sc1 | ssa_55_carryKnownZero_sc1;
	APInt ssa_57_knownMask_sc1 = ssa_40_knownMaskS1 & ssa_13_knownMaskC;
	APInt ssa_58_knownMask_sc12 = ssa_57_knownMask_sc1 & ssa_56_carryKnown_sc1;
	APInt ssa_59_sumVal_sc1 = ssa_37_knownOne_ab ^ ssa_8_c1;
	APInt ssa_60_sumVal_sc12 = ssa_59_sumVal_sc1 ^ ssa_53_carryInMin_sc1;
	APInt ssa_61_knownOne_sc1 = ssa_60_sumVal_sc12 & ssa_58_knownMask_sc12;
	APInt ssa_62_not_sumVal_sc12 = ssa_9_all ^ ssa_60_sumVal_sc12;
	APInt ssa_63_knownZero_sc1 = ssa_62_not_sumVal_sc12 & ssa_58_knownMask_sc12;
	APInt ssa_64_sumMin_ac = ssa_4_a1 + ssa_8_c1;
	APInt ssa_65_sumMax_ac = ssa_14_aMax + ssa_16_cMax;
	APInt ssa_66_a1_and_c1 = ssa_4_a1 & ssa_8_c1;
	APInt ssa_67_a1_or_c1 = ssa_4_a1 | ssa_8_c1;
	APInt ssa_68_not_sumMin_ac = ssa_9_all ^ ssa_64_sumMin_ac;
	APInt ssa_69_tmp5 = ssa_67_a1_or_c1 & ssa_68_not_sumMin_ac;
	APInt ssa_70_carryMin_ac = ssa_66_a1_and_c1 | ssa_69_tmp5;
	APInt ssa_71_aMax_and_cMax = ssa_14_aMax & ssa_16_cMax;
	APInt ssa_72_aMax_or_cMax = ssa_14_aMax | ssa_16_cMax;
	APInt ssa_73_not_sumMax_ac = ssa_9_all ^ ssa_65_sumMax_ac;
	APInt ssa_74_tmp6 = ssa_72_aMax_or_cMax & ssa_73_not_sumMax_ac;
	APInt ssa_75_carryMax_ac = ssa_71_aMax_and_cMax | ssa_74_tmp6;
	APInt ssa_76_carryInMin_ac;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_76_carryInMin_ac = APInt(ssa_70_carryMin_ac.getBitWidth(), 0);
	} else {
		ssa_76_carryInMin_ac = ssa_70_carryMin_ac.shl(ssa_10_one);
	}
	APInt ssa_77_carryInMax_ac;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_77_carryInMax_ac = APInt(ssa_75_carryMax_ac.getBitWidth(), 0);
	} else {
		ssa_77_carryInMax_ac = ssa_75_carryMax_ac.shl(ssa_10_one);
	}
	APInt ssa_78_carryKnownZero_ac = ssa_9_all ^ ssa_77_carryInMax_ac;
	APInt ssa_79_carryKnown_ac = ssa_76_carryInMin_ac | ssa_78_carryKnownZero_ac;
	APInt ssa_80_knownMask_ac = ssa_11_knownMaskA & ssa_13_knownMaskC;
	APInt ssa_81_knownMask_ac2 = ssa_80_knownMask_ac & ssa_79_carryKnown_ac;
	APInt ssa_82_sumVal_ac = ssa_4_a1 ^ ssa_8_c1;
	APInt ssa_83_sumVal_ac2 = ssa_82_sumVal_ac ^ ssa_76_carryInMin_ac;
	APInt ssa_84_knownOne_ac = ssa_83_sumVal_ac2 & ssa_81_knownMask_ac2;
	APInt ssa_85_not_sumVal_ac2 = ssa_9_all ^ ssa_83_sumVal_ac2;
	APInt ssa_86_knownZero_ac = ssa_85_not_sumVal_ac2 & ssa_81_knownMask_ac2;
	APInt ssa_87_knownMaskS2 = ssa_86_knownZero_ac | ssa_84_knownOne_ac;
	APInt ssa_88_sumMin_sb2 = ssa_64_sumMin_ac + ssa_6_b1;
	APInt ssa_89_sumMax_sb2 = ssa_65_sumMax_ac + ssa_15_bMax;
	APInt ssa_90_sMin2_and_b1 = ssa_64_sumMin_ac & ssa_6_b1;
	APInt ssa_91_sMin2_or_b1 = ssa_64_sumMin_ac | ssa_6_b1;
	APInt ssa_92_not_sumMin_sb2 = ssa_9_all ^ ssa_88_sumMin_sb2;
	APInt ssa_93_tmp7 = ssa_91_sMin2_or_b1 & ssa_92_not_sumMin_sb2;
	APInt ssa_94_carryMin_sb2 = ssa_90_sMin2_and_b1 | ssa_93_tmp7;
	APInt ssa_95_sMax2_and_bMax = ssa_65_sumMax_ac & ssa_15_bMax;
	APInt ssa_96_sMax2_or_bMax = ssa_65_sumMax_ac | ssa_15_bMax;
	APInt ssa_97_not_sumMax_sb2 = ssa_9_all ^ ssa_89_sumMax_sb2;
	APInt ssa_98_tmp8 = ssa_96_sMax2_or_bMax & ssa_97_not_sumMax_sb2;
	APInt ssa_99_carryMax_sb2 = ssa_95_sMax2_and_bMax | ssa_98_tmp8;
	APInt ssa_100_carryInMin_sb2;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_100_carryInMin_sb2 = APInt(ssa_94_carryMin_sb2.getBitWidth(), 0);
	} else {
		ssa_100_carryInMin_sb2 = ssa_94_carryMin_sb2.shl(ssa_10_one);
	}
	APInt ssa_101_carryInMax_sb2;
	if (ssa_10_one.uge(ssa_10_one.getBitWidth())) {
		ssa_101_carryInMax_sb2 = APInt(ssa_99_carryMax_sb2.getBitWidth(), 0);
	} else {
		ssa_101_carryInMax_sb2 = ssa_99_carryMax_sb2.shl(ssa_10_one);
	}
	APInt ssa_102_carryKnownZero_sb2 = ssa_9_all ^ ssa_101_carryInMax_sb2;
	APInt ssa_103_carryKnown_sb2 = ssa_100_carryInMin_sb2 | ssa_102_carryKnownZero_sb2;
	APInt ssa_104_knownMask_sb2 = ssa_87_knownMaskS2 & ssa_12_knownMaskB;
	APInt ssa_105_knownMask_sb22 = ssa_104_knownMask_sb2 & ssa_103_carryKnown_sb2;
	APInt ssa_106_sumVal_sb2 = ssa_84_knownOne_ac ^ ssa_6_b1;
	APInt ssa_107_sumVal_sb22 = ssa_106_sumVal_sb2 ^ ssa_100_carryInMin_sb2;
	APInt ssa_108_knownOne_sb2 = ssa_107_sumVal_sb22 & ssa_105_knownMask_sb22;
	APInt ssa_109_not_sumVal_sb22 = ssa_9_all ^ ssa_107_sumVal_sb22;
	APInt ssa_110_knownZero_sb2 = ssa_109_not_sumVal_sb22 & ssa_105_knownMask_sb22;
	APInt ssa_111_knownZero = ssa_63_knownZero_sc1 | ssa_110_knownZero_sb2;
	APInt ssa_112_knownOne = ssa_61_knownOne_sc1 | ssa_108_knownOne_sb2;
	std::array<APInt, 2> ssa_113_r = std::array<APInt, 2>{ssa_111_knownZero, ssa_112_knownOne};
	return ssa_113_r;
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
