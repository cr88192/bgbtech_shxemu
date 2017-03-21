int BGBCC_SHXC_EmitLdixVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, s32 imm)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	ty=type.val;
	
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		
		BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, cdreg);
		if(nm2>=0)
			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLdixVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	ty=type.val;
	
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		
		BGBCC_SHXC_EmitLoadBRegIRegScReg(ctx, sctx,
			nm1, csreg, ctreg, sz, cdreg);
		if(nm2>=0)
			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}



int BGBCC_SHXC_EmitStixVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, s32 imm)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	ty=type.val;
	
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		
		BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
			nm1, cdreg, imm*sz, csreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStixVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	ty=type.val;
	
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		
		BGBCC_SHXC_EmitStoreBRegIRegScReg(ctx, sctx,
			nm1, cdreg, csreg, sz, ctreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitLeaVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, s32 imm)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	ty=type.val;
	
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		
		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, cdreg);
		if(nm2>=0)
			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLeaVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	ty=type.val;
	
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		
		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, csreg, ctreg, sz, cdreg);
		if(nm2>=0)
			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLdaVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;

	cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
	BGBCC_SHXC_EmitLdaFrameVRegReg(ctx, sctx, sreg, cdreg);
	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
	return(1);
}


int BGBCC_SHXC_EmitDiffPtrVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	ty=type.val;
	
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(nm1>=0)
	{
		switch(sz)
		{
		case 1: nm1=-1; nm2=-1; break;
		case 2: nm1=BGBCC_SH_NMID_SHLR; nm2=-1; break;
		case 4: nm1=BGBCC_SH_NMID_SHLR2; nm2=-1; break;
		case 8: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR; break;
		case 16: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR2; break;
		default: nm1=-1; nm2=-1; break;
		}

		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		
//		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
//			nm1, csreg, ctreg, sz, cdreg);
//		if(nm2>=0)
//			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB, ctreg, cdreg);

		if(nm1>=0)
			BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg);
		if(nm2>=0)
			BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
