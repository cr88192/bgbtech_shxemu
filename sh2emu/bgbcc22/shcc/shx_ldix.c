int BGBCC_SHXC_EmitLdixVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, s32 imm)
{
	ccxl_type tty;
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	ty=type.val;
	
	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, type, &tty);

		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		
		nm1=BGBCC_SH_NMID_MOVL;
//		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

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

	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:	case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;
		}else
		{
//			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		}
		
		BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, cdreg);
		if(nm2>=0)
			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
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
	ccxl_type tty;
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz, asz, bsz;
	int i, j, k;

	ty=type.val;

	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, type, &tty);

		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		
		nm1=BGBCC_SH_NMID_MOVL;
//		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, csreg, ctreg, sz, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

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

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
				__debugbreak();

//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
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

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;

		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		
//		BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
//			nm1, cdreg, imm*sz, csreg);

		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, cdreg, imm*sz, ctreg);
		BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, ctreg, csreg, sz, 4);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
		return(1);
	}

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
	int tr0;
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

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;

		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		
//		BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
//			nm1, cdreg, imm*sz, csreg);

//		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
//			nm1, cdreg, imm*sz, ctreg);

		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, cdreg, csreg, sz, tr0);

		BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, tr0, ctreg, sz, 4);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

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

	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		i=BGBCC_SHXC_EmitLeaVRegVRegImm(ctx, sctx, type, dreg, sreg, imm);
		return(i);
	}
	
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

	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_L:	case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;
		}else
		{
//			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		}
		
//		if(cdreg==csreg)
//			__debugbreak();
		
		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, cdreg);
		if(nm2>=0)
			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
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

	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		i=BGBCC_SHXC_EmitLeaVRegVRegVReg(ctx, sctx, type, dreg, sreg, treg);
		return(i);
	}

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

	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_L:	case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;
	}

	if(nm1>=0)
	{
//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

		if(cdreg==csreg)
			__debugbreak();
		if(csreg==ctreg)
			__debugbreak();
		if(cdreg==ctreg)
			__debugbreak();

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

	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_L:	case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
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


int BGBCC_SHXC_EmitLeaShlRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg, int shl)
{
	int nm1, nm2, nm3, nm4;

	if(shl<0)
		return(0);
	if(!shl)
		return(1);

	nm1=-1; nm2=-1; nm3=-1; nm4=-1;
	switch(shl)
	{
	case 0:
		break;
	case 1:
		nm1=BGBCC_SH_NMID_SHLL;		break;
	case 2:
		nm1=BGBCC_SH_NMID_SHLL2;	break;
	case 3:
		nm1=BGBCC_SH_NMID_SHLL2;	nm2=BGBCC_SH_NMID_SHLL;		break;
	case 4:
		nm1=BGBCC_SH_NMID_SHLL2;	nm2=BGBCC_SH_NMID_SHLL2;	break;
	case 5:
		nm1=BGBCC_SH_NMID_SHLL2;	nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL;		break;
	case 6:
		nm1=BGBCC_SH_NMID_SHLL2;	nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL2;	break;
//	case 7:
//		nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2;
//		nm3=BGBCC_SH_NMID_SHLL2; nm3=BGBCC_SH_NMID_SHLL; break;
//	case 6: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLR2; break;
	case 7:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHAR;		break;
	case 8:
		nm1=BGBCC_SH_NMID_SHLL8;	break;
	case 9:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHLL;		break;
	case 10:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHLL2;	break;
	case 11:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL;		break;
	case 12:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL2;	break;
	case 13:
		nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL2; nm4=BGBCC_SH_NMID_SHLL;		break;
	case 14:
		nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL2; nm4=BGBCC_SH_NMID_SHLL2;		break;

	case 15: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHAR; break;
	case 16: nm1=BGBCC_SH_NMID_SHLL16; break;

	default: break;
	}

#if 1
	if((nm3>=0) && (dreg!=BGBCC_SH_REG_R0))
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD,
			BGBCC_SH_REG_R0, dreg);
		return(1);
	}
#endif

	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, dreg);
	if(nm2>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm2, dreg);
	if(nm3>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm3, dreg);
	if(nm4>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm4, dreg);

	return(1);
}

int BGBCC_SHXC_EmitLeaBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int treg;
	int nm1, nm2, nm3;
	int p0, p1, shl, bs, cbo, cmo;
	int i, j, k;

	switch(sc)
	{
	case 1: shl=0; bs=2; break;
	case 2: shl=1; bs=2; break;
	case 3: shl=0; bs=3; break;
	case 4: shl=2; bs=2; break;
	case 5: shl=0; bs=5; break;
	case 6: shl=1; bs=3; break;
	case 7: shl=0; bs=7; break;

	case 8: shl=3; bs=2; break;
	case 9: shl=0; bs=9; break;
	case 10: shl=1; bs=5; break;
	case 11: shl=0; bs=11; break;
	case 12: shl=2; bs=3; break;
	case 13: shl=0; bs=13; break;
	case 14: shl=1; bs=7; break;
	case 15: shl=0; bs=15; break;
	
	case 16: shl=4; bs=2; break;
	case 17: shl=0; bs=17; break;
	case 18: shl=1; bs=9; break;
	case 20: shl=2; bs=5; break;
	case 22: shl=1; bs=11; break;
	case 24: shl=3; bs=3; break;
	case 26: shl=1; bs=13; break;
	case 28: shl=2; bs=7; break;
	case 30: shl=1; bs=15; break;

	case 32: shl=5; bs=2; break;
	case 34: shl=1; bs=17; break;
	case 36: shl=2; bs=9; break;
	case 40: shl=3; bs=5; break;
	case 44: shl=2; bs=11; break;
	case 48: shl=4; bs=3; break;
	case 52: shl=2; bs=13; break;
	case 56: shl=3; bs=7; break;
	case 60: shl=2; bs=15; break;

	case 64: shl=6; bs=2; break;
	case 68: shl=2; bs=17; break;
	case 72: shl=3; bs=9; break;
	case 80: shl=4; bs=5; break;
	case 88: shl=3; bs=11; break;
	case 96: shl=5; bs=3; break;
	case 104: shl=3; bs=13; break;
	case 112: shl=4; bs=7; break;
	case 120: shl=3; bs=15; break;

	case 128: shl=7; bs=2; break;
	case 176: shl=4; bs=11; break;
	case 192: shl=6; bs=3; break;
	case 136: shl=3; bs=17; break;
	case 144: shl=4; bs=9; break;
	case 208: shl=4; bs=13; break;
	case 240: shl=4; bs=15; break;

	case 256: shl=8; bs=2; break;
	case 272: shl=4; bs=17; break;
	case 384: shl=7; bs=3; break;

	case 512: shl=9; bs=2; break;
	case 768: shl=8; bs=3; break;

	case 1024: shl=10; bs=2; break;
	case 2048: shl=11; bs=2; break;
	case 4096: shl=12; bs=2; break;
	case 8192: shl=13; bs=2; break;
	case 16384: shl=14; bs=2; break;
	case 32768: shl=15; bs=2; break;
	case 65536: shl=16; bs=2; break;

	default: shl=-1; bs=-1; break;
	}
	
	switch(bs)
	{
	case 2: cbo=1; break;
	case 3: cbo=3; break;
	case 5: cbo=3; break;
	case 7: cbo=4; break;
	case 9: cbo=4; break;
	case 11: cbo=5; break;
	case 13: cbo=5; break;
	case 15: cbo=6; break;
	case 17: cbo=6; break;
	}
	switch(shl)
	{
	case 0: cbo+=0; break;
	case 1: cbo+=1; break;
	case 2: cbo+=1; break;
	case 3: cbo+=2; break;
	case 4: cbo+=2; break;
//	case 5: cbo+=3; break;
//	case 6: cbo+=3; break;
	case 7: cbo+=2; break;
	case 8: cbo+=1; break;
	case 9: cbo+=2; break;
//	case 10: cbo+=3; break;
//	case 11: cbo+=3; break;
//	case 12: cbo+=3; break;
//	default: cbo+=4; break;

	default: cbo+=2; break;
	}
	
	cmo=3;
	if(sc>=128)cmo+=3;
	
	if((dreg==ireg) && (breg!=dreg) && (bs==2))
		cbo-=1;
	if((dreg==ireg) && (bs!=2))
		shl=-1;

	if((shl>=0) && (bs==2) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
			treg=BGBCC_SH_REG_R1;
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, treg);
			BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, treg, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, treg, dreg);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
			BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		}
		return(1);
	}

	if((shl>=0) && (bs==3) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
			treg=BGBCC_SH_REG_R1;
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, treg);
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL, treg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, treg);
			BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, treg, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, treg, dreg);
			return(1);
		}

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL, dreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		return(1);
	}

	if((shl>=0) && (bs==5) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
			treg=BGBCC_SH_REG_R1;
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, treg);
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL2, treg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, treg);
			BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, treg, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, treg, dreg);
			return(1);
		}

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL2, dreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		return(1);
	}

#if 0
	if((shl>=0) && ((bs==7)||(bs==9)) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				breg, BGBCC_SH_REG_R1);
			breg=BGBCC_SH_REG_R1;
		}

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL2, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL, dreg);
		if(bs==7)
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB, ireg, dreg);
		else
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		return(1);
	}

	if((shl>=0) && ((bs==11)||(bs==13)) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				breg, BGBCC_SH_REG_R1);
			breg=BGBCC_SH_REG_R1;
		}

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL, dreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL2, dreg);
		if(bs==11)
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB, ireg, dreg);
		else
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		return(1);
	}
#endif

//	if(shl<0)
	if(1)
	{
//		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//			ireg, dreg);

		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, sc);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MULL,
			ireg, BGBCC_SH_REG_R0);
		if(breg==dreg)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACL, BGBCC_SH_REG_R1);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_R1, dreg);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACL, dreg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		}
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
