/*
 * Variant Arithmetic
 */

int BGBCC_SHXC_EmitBinaryVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2;
	int i, j, k;
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__lva_add"; break;
	case CCXL_BINOP_SUB:	s0="__lva_sub"; break;
	case CCXL_BINOP_MUL:	s0="__lva_mul"; break;
	case CCXL_BINOP_DIV:	s0="__lva_div"; break;
	case CCXL_BINOP_MOD:	s0="__lva_mod"; break;
	case CCXL_BINOP_AND:	s0="__lva_and"; break;
	case CCXL_BINOP_OR:		s0="__lva_or"; break;
	case CCXL_BINOP_XOR:	s0="__lva_xor"; break;
	case CCXL_BINOP_SHL:	s0="__lva_shl"; break;
	case CCXL_BINOP_SHR:	s0="__lva_shr"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2;
	int i, j, k;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_SHXC_EmitBinaryVRegVRegVariant(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__lva_add"; break;
	case CCXL_BINOP_SUB:	s0="__lva_sub"; break;
	case CCXL_BINOP_MUL:	s0="__lva_mul"; break;
	case CCXL_BINOP_DIV:	s0="__lva_div"; break;
	case CCXL_BINOP_MOD:	s0="__lva_mod"; break;
	case CCXL_BINOP_AND:	s0="__lva_and"; break;
	case CCXL_BINOP_OR:		s0="__lva_or"; break;
	case CCXL_BINOP_XOR:	s0="__lva_xor"; break;
	case CCXL_BINOP_SHL:	s0="__lva_shl"; break;
	case CCXL_BINOP_SHR:	s0="__lva_shr"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);

//	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
//	i=BGBCC_SHXC_EmitBinaryVRegVRegVariant(ctx, sctx, type, dreg, opr, treg);
//	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	s32 imm;
	int i, j, k;
	
	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:	s0="__lva_neg"; break;
	case CCXL_UNOP_NOT:	s0="__lva_not"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitConvToVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_toi32");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_toi64");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_tof32");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR0);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_tof64");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR0);
		return(1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_toptr");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

#if 0
	if(0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, opn);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitConvFromVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromi32");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromi64");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_FR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromf32");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromf64");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}


	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromptr");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
