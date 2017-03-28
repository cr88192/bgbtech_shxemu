int BGBCC_SHXC_EmitLoadSlotVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	obj=ctx->reg_globals[gblid];
	fi=obj->fields[fid];

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
			nm1, csreg, fi->fxoffs, cdreg);
		if(nm2>=0)
			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreSlotVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	obj=ctx->reg_globals[gblid];
	fi=obj->fields[fid];

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
			nm1, cdreg, fi->fxoffs, csreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLoadSlotAddrVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	int csreg, ctreg, cdreg;
	int nm1, nm2, ty, sz;
	int i, j, k;

	obj=ctx->reg_globals[gblid];
	fi=obj->fields[fid];

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
			nm1, csreg, fi->fxoffs, cdreg);
		if(nm2>=0)
			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
