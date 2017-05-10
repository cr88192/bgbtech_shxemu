
int BGBCC_SHXC_EmitBinaryVRegVRegInt128(
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
	case CCXL_BINOP_ADD:	s0="__addxli"; break;
	case CCXL_BINOP_SUB:	s0="__subxli"; break;
	case CCXL_BINOP_MUL:	s0="__smulxli"; break;
	case CCXL_BINOP_DIV:	s0="__sdivxli"; break;
	case CCXL_BINOP_AND:	s0="__andxli"; break;
	case CCXL_BINOP_OR:		s0="__orxli"; break;
	case CCXL_BINOP_XOR:	s0="__xorxli"; break;
	case CCXL_BINOP_SHL:	s0="__shlxli"; break;
	case CCXL_BINOP_SHR:	s0="__shrxli"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	int i;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_SHXC_EmitBinaryVRegVRegInt128(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_SHXC_EmitBinaryVRegVRegInt128(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegInt128(
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
	case CCXL_UNOP_NEG:	s0="__negxli"; break;
	case CCXL_UNOP_NOT:	s0="__notxli"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
