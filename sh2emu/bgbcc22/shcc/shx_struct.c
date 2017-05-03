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

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
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
	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:		case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
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

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
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
	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:		case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
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

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
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
	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:		case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
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

/** Emit logic to copy value-type given by registers and size. */
int BGBCC_SHXC_EmitValueCopyRegRegSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg, int sreg, int sz, int al)
{
	char *s0;
	int tr0, tr1;
	int nm1, nm2;
	int i, j, k;

	nm1=BGBCC_SH_NMID_MOVL;
	nm2=BGBCC_SH_NMID_MOVL;
	if(((sz&3)==1) && (al<=1))nm2=BGBCC_SH_NMID_MOVB;
	if(((sz&3)==2) && (al<=2))nm2=BGBCC_SH_NMID_MOVW;

	if(sz<=4)
	{
		if(al>=4)
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
//			tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHX_EmitOpLdRegReg(sctx, nm1, sreg, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegStReg(sctx, nm1, BGBCC_SH_REG_R0, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
//			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
		
		if(al==2)
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);

			nm1=BGBCC_SH_NMID_MOVW;
			if(sz>2)
			{
				BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1,	sreg, 0, tr0);
				BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm2,	sreg, 2, tr1);
				BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1,	tr0, dreg, 0);
				BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg, 2);
			}else
			{
				BGBCC_SHX_EmitOpLdRegReg(sctx, nm1, sreg, tr0);
				BGBCC_SHX_EmitOpRegStReg(sctx, nm1, tr0, dreg);
			}

			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
		
		nm1=BGBCC_SH_NMID_MOVB;
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		for(i=0; i<sz; i++)
		{
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, i, tr0);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
		}
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
	
//		BGBCC_SHX_EmitOpLdRegReg(sctx, nm2, sreg, BGBCC_SH_REG_R0);
//		BGBCC_SHX_EmitOpRegStReg(sctx, nm2, BGBCC_SH_REG_R0, dreg);
		return(1);
	}

	if((sz<=8) && (al>=4))
	{
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm2, sreg, 4, tr1);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg, 4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

	if((sz<=12) && (al>=4))
	{
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, 4, tr1);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 4);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm2, sreg, 8, tr0);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm2, tr0, dreg, 8);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

	if((sz<=16) && (al>=4))
	{
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, 4, tr1);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 4);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, 8, tr0);
		BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm2, sreg,12, tr1);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 8);
		BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg,12);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

#if 0
	if((sz<=64) && (al>=4))
	{
		i=0;
		while((i+8)<sz)
		{
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 0, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 4, BGBCC_SH_REG_R1);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, BGBCC_SH_REG_R0, dreg, i+ 0);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, BGBCC_SH_REG_R1, dreg, i+ 4);
			i+=8;
		}
		if((sz-i)>4)
		{
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm1, sreg, i+0, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm2, sreg, i+4, BGBCC_SH_REG_R1);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm1, BGBCC_SH_REG_R0, dreg, i+0);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm2, BGBCC_SH_REG_R1, dreg, i+4);
		}else
		{
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, nm2, sreg, i, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, nm2, BGBCC_SH_REG_R0, dreg, i);
		}
		return(1);
	}
#endif

#if 1
	if((sz<=64) && (al>=4))
	{
		switch((sz+3)/4)
		{
		case  1: s0="__memcpy32_4"; break;
		case  2: s0="__memcpy32_8"; break;
		case  3: s0="__memcpy32_12"; break;
		case  4: s0="__memcpy32_16"; break;
		case  5: s0="__memcpy32_20"; break;
		case  6: s0="__memcpy32_24"; break;
		case  7: s0="__memcpy32_28"; break;
		case  8: s0="__memcpy32_32"; break;
		case  9: s0="__memcpy32_36"; break;
		case 10: s0="__memcpy32_40"; break;
		case 11: s0="__memcpy32_44"; break;
		case 12: s0="__memcpy32_48"; break;
		case 13: s0="__memcpy32_52"; break;
		case 14: s0="__memcpy32_58"; break;
		case 15: s0="__memcpy32_60"; break;
		case 16: s0="__memcpy32_64"; break;
		default: s0=NULL; break;
		}
		
		if(s0)
		{
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
				dreg, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
				sreg, BGBCC_SH_REG_R5);
			BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
			return(1);
		}
	}
	
	if(al>=4)
	{
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
			sreg, BGBCC_SH_REG_R5);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R6, sz);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__memcpy32");
		return(1);
	}
#endif

	BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
		dreg, BGBCC_SH_REG_R4);
	BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
		sreg, BGBCC_SH_REG_R5);
	BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R6, sz);
	BGBCC_SHXC_EmitCallName(ctx, sctx, "memcpy");
	return(1);
}
