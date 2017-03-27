/*
 * R0/R1/R2/R3: Return Value, Scratch
 *   R0: ReturnA, Default Const-Load
 *   R1: ReturnB, Scratch
 *   R2: Struct Return Pointer
 *   R3: Temp Opr Working Reg
 * R4/R5/R6/R7: Arg Passing/Scratch
 * R8..R11: Var Cache (Callee Save)
 * R12: Context Pointer (GOT)
 * R13: Var Cache (Callee Save)
 * R14: Frame Pointer
 * R15: Stack Pointer
 * FR5,FR4,FR7,FR6,FR9,FR8,FR11,FR10: Float/Double Args, Scratch
 * FR12..FR15: Var Cache (Callee Save)
 *
 * Frame (Frame-Ref Point)
 *   (- 4) Saved PR
 *   (- 8) Saved R14
 *   (-12) Saved R13
 *   (-16) Saved R12
 *   (-20) Saved R11
 *   (-24) Saved R10
 *   (-28) Saved R9
 *   (-32) Saved R8
 *   ...
 *   SP (Stack Ref Point)
 */


const byte bgbcc_shx_cachereg[6]={ 13, 11, 10, 9, 8, 255 };

int BGBCC_SHXC_EmitLoadFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int p0, p1;
	int ofs1;
	int i, j, k;

	ofs1=ofs+(sctx->frm_size);

	p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
		BGBCC_SH_REG_SP, ofs1, dreg);
	if(p0>0)
		return(1);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, BGBCC_SH_REG_R0, dreg);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int p0, p1;
	int ofs1;
	int i, j, k;

	ofs1=ofs+(sctx->frm_size);

	p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
		dreg, BGBCC_SH_REG_SP, ofs1);
	if(p0>0)
		return(1);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg, BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLdaFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int p0, p1;
	int ofs1;
	int i, j, k;

	ofs1=ofs+(sctx->frm_size);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		dreg, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_SP, dreg);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitStoreStackOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int p0, p1;
	int ofs1;
	int i, j, k;

	ofs1=ofs;

	p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
		dreg, BGBCC_SH_REG_SP, ofs1);
	if(p0>0)
		return(1);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg, BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitLoadBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int p0, p1;
	int i, j, k;

	p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, nmid,
		breg, ofs, dreg);
	if(p0>0)
		return(1);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, nmid,
			breg, BGBCC_SH_REG_R0, dreg);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int p0, p1;
	int i, j, k;

	p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, nmid,
		dreg, breg, ofs);
	if(p0>0)
		return(1);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
			dreg, BGBCC_SH_REG_R0, breg);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLeaBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int p0, p1;
	int i, j, k;

	p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, nmid,
		breg, ofs, dreg);
	if(p0>0)
		return(1);

	BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, ofs);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
	return(1);
}


int BGBCC_SHXC_EmitLoadBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	}

	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
	BGBCC_SHX_EmitOpLdReg2Reg(sctx, nmid,
		breg, BGBCC_SH_REG_R0, dreg);
	return(1);
}

int BGBCC_SHXC_EmitStoreBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	}

	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);

	p1=BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
		dreg, BGBCC_SH_REG_R0, breg);
	return(1);
}

int BGBCC_SHXC_EmitLeaBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	case 8: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL; break;
	case 16: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
	default: nm1=-1; nm2=-1; break;
	}

	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, dreg);
	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, dreg);
	if(nm2>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm2, dreg);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
	return(1);
}


int BGBCC_SHXC_SaveFrameOfsForReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int reg)
{
	int ofs;
	
	ofs=0;
	switch(reg)
	{
	case BGBCC_SH_REG_PR: ofs=-4; break;
	case BGBCC_SH_REG_R14: ofs=-8; break;
	case BGBCC_SH_REG_R13: ofs=-12; break;
	case BGBCC_SH_REG_R12: ofs=-16; break;
	case BGBCC_SH_REG_R11: ofs=-20; break;
	case BGBCC_SH_REG_R10: ofs=-24; break;
	case BGBCC_SH_REG_R9: ofs=-28; break;
	case BGBCC_SH_REG_R8: ofs=-32; break;
	}
	return(ofs);
}

int BGBCC_SHXC_EmitSaveFrameReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int reg)
{
	int ofs;
	ofs=BGBCC_SHXC_SaveFrameOfsForReg(ctx, sctx, reg);
	if(ofs!=0)
	{
		sctx->reg_save|=1<<(reg&15);
		BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}
	return(0);
}

int BGBCC_SHXC_EmitReloadSavedFrameReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int reg)
{
	int ofs;

	if(!(sctx->reg_save&(1<<(reg&15))))
		return(0);

	ofs=BGBCC_SHXC_SaveFrameOfsForReg(ctx, sctx, reg);
	if(ofs!=0)
	{
//		sctx->reg_save|=1<<(reg&15);
		BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}
	return(0);
}

int BGBCC_SHXC_GetGblIndexGotIndex(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int gblidx)
{
	int i, j, k;
	
	for(i=0; i<sctx->got_n_gblidx; i++)
		if(sctx->got_gblidx[i]==gblidx)
			return(i);

	if(!sctx->got_gblidx)
	{
		i=4096;
		sctx->got_gblidx=bgbcc_malloc(i*sizeof(u32));
		sctx->got_n_gblidx=0;
		sctx->got_m_gblidx=i;
	}
	
	if((sctx->got_n_gblidx+1)>=sctx->got_m_gblidx)
	{
		i=sctx->got_m_gblidx+(sctx->got_m_gblidx>>1);
		sctx->got_gblidx=bgbcc_realloc(sctx->got_gblidx, i*sizeof(u32));
		sctx->got_m_gblidx=i;
	}

	i=sctx->got_n_gblidx++;
	sctx->got_gblidx[i]=gblidx;
	return(i);
}

int BGBCC_SHXC_GetGblIndexLabel(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int gblidx)
{
	BGBCC_CCXL_RegisterInfo *gbl;
	int i;

	gbl=ctx->reg_globals[gblidx];

	i=gbl->fxoffs;
	if(i<=0)
	{
//		i=BGBCC_SHX_GenLabel(sctx);
		i=BGBCC_SHX_GetNamedLabel(sctx, gbl->name);
		gbl->fxoffs=i;
	}
	return(i);
}

int BGBCC_SHXC_GetVRegGotIndex(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg)
{
	int i, j, k;
	j=BGBCC_CCXL_GetRegID(ctx, sreg);
	i=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);
	return(i);
}

int BGBCC_SHXC_GetGblNameGotIndex(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	char *name)
{
	BGBCC_CCXL_RegisterInfo *gbl;
	int i, j, k;
	
	gbl=BGBCC_CCXL_GetGlobal(ctx, name);
	if(!gbl->regtype)
		gbl->regtype=CCXL_LITID_ASMFUNC;

	k=gbl->fxoffs;
	if(k<=0)
	{
//		k=BGBCC_SHX_GenLabel(sctx);
		k=BGBCC_SHX_GetNamedLabel(sctx, gbl->name);
		gbl->fxoffs=k;
	}

	j=gbl->gblid;
//	j=BGBCC_CCXL_GetRegID(ctx, sreg);
	i=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);
	return(i);
}

int BGBCC_SHXC_GetGblNameLabel(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	char *name)
{
	BGBCC_CCXL_RegisterInfo *gbl;
	int i, j, k;
	
	gbl=BGBCC_CCXL_GetGlobal(ctx, name);
	if(!gbl->regtype)
		gbl->regtype=CCXL_LITID_ASMFUNC;
	
	i=gbl->fxoffs;
	if(i<=0)
	{
//		i=BGBCC_SHX_GenLabel(sctx);
		i=BGBCC_SHX_GetNamedLabel(sctx, gbl->name);
		gbl->fxoffs=i;
	}
	return(i);
}

int BGBCC_SHXC_EmitLoadGblNameReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	char *name, int dreg)
{
	int p0, p1;
	int k;
	
	if(sctx->is_pic)
	{
		k=BGBCC_SHXC_GetGblNameGotIndex(ctx, sctx, name);
		p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, k*4);
		p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
		return(1);
	}else
	{
		k=BGBCC_SHXC_GetGblNameLabel(ctx, sctx, name);
		BGBCC_SHX_EmitLoadRegLabelAbs(sctx, dreg, k);
		return(1);
	}

	return(0);
}

int BGBCC_SHXC_EmitLoadFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
//		k=sctx->frm_offs_lcl+j*4;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		i=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, sreg))
	{
		if(sctx->is_pic)
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);

			p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
		}else
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, j);
			BGBCC_SHX_EmitLoadRegLabelAbs(sctx, dreg, k);
		}
			
		if(BGBCC_CCXL_IsRegSmallIntP(ctx, sreg))
		{
			p1=BGBCC_SHX_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
				dreg, dreg);
		}
		return(0);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register dreg, int sreg)
{
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, dreg))
	{
		j=dreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, dreg))
	{
		j=dreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, dreg))
	{
		j=dreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, dreg))
	{
		if(sctx->is_pic)
		{
			j=BGBCC_CCXL_GetRegID(ctx, dreg);
			k=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);

			p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, BGBCC_SH_REG_R0);
		}else
		{
			j=BGBCC_CCXL_GetRegID(ctx, dreg);
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, j);
			BGBCC_SHX_EmitLoadRegLabelAbs(sctx, BGBCC_SH_REG_R0, k);
		}
			
		if(BGBCC_CCXL_IsRegSmallIntP(ctx, dreg))
		{
			p1=BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVL,
				sreg, BGBCC_SH_REG_R0);
		}
		return(0);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLdaFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, sreg))
	{
		if(sctx->is_pic)
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);

			p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
		}else
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, j);
			BGBCC_SHX_EmitLoadRegLabelAbs(sctx, dreg, k);
		}
		
		return(0);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}


/* Try to get Variable as register.
 * Will return a value as a register if possible, but may fail.
 * Returns ZZR if it is unable to do so.
 */
int BGBCC_SHXC_EmitTryGetRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg, int fl)
{
	int creg;
	int i;

	/* value already in a register? */
	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=1<<i;
			if(fl&1)
				sctx->regalc_dirty|=1<<i;

			if(sctx->regalc_ltcnt[i]>0)
				sctx->regalc_ltcnt[i]--;
			sctx->regalc_utcnt[i]++;
			return(bgbcc_shx_cachereg[i]);
		}
	}

	return(BGBCC_SH_REG_ZZR);
}

/* Get Variable as register.
 * If no registers are free, evict least-recently-used.
 */
int BGBCC_SHXC_EmitGetRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg, int fl)
{
	int creg;
	int i;

	creg=BGBCC_SHXC_EmitTryGetRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	for(i=0; i<5; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

	/* Check for registers not holding a live value. */
	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;
		if(!((sctx->regalc_live)&(1<<i)))
		{
			sctx->regalc_map[i]=reg;
			sctx->regalc_utcnt[i]=1;
			sctx->regalc_live|=1<<i;
			if(fl&1)
				sctx->regalc_dirty|=1<<i;
			creg=bgbcc_shx_cachereg[i];
			if(fl&2)
				{ BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
			return(creg);
		}
	}

	/* Check for unallocated registers. */
	for(i=0; i<5; i++)
	{
		if((sctx->regalc_save)&(1<<i))
			continue;
		
		sctx->regalc_map[i]=reg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_save|=1<<i;
		sctx->regalc_live|=1<<i;
		if(fl&1)
			sctx->regalc_dirty|=1<<i;

		creg=bgbcc_shx_cachereg[i];		
		BGBCC_SHXC_EmitSaveFrameReg(ctx, sctx, creg);
		if(fl&2)
			{ BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

/* Dirty: Register is being modified. Needs old value and writeback. */
int BGBCC_SHXC_EmitGetRegisterDirty(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_SHXC_EmitGetRegister(ctx, sctx, reg, 3));
}

/* Write: Register is being written. Old value is not needed. */
int BGBCC_SHXC_EmitGetRegisterWrite(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_SHXC_EmitGetRegister(ctx, sctx, reg, 1));
}

/* Read: Register is being read. No writeback needed. */
int BGBCC_SHXC_EmitGetRegisterRead(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_SHXC_EmitGetRegister(ctx, sctx, reg, 2));
}

/* Dirty: Register is being modified. Needs old value and writeback. */
int BGBCC_SHXC_EmitTryGetRegisterDirty(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_SHXC_EmitTryGetRegister(ctx, sctx, reg, 3));
}

/* Write: Register is being written. Old value is not needed. */
int BGBCC_SHXC_EmitTryGetRegisterWrite(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_SHXC_EmitTryGetRegister(ctx, sctx, reg, 1));
}

/* Read: Register is being read. No writeback needed. */
int BGBCC_SHXC_EmitTryGetRegisterRead(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_SHXC_EmitTryGetRegister(ctx, sctx, reg, 2));
}

int BGBCC_SHXC_EmitReleaseRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	int creg;
	int i;

	/* value in register? */
	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=1<<i;
			if(sctx->regalc_utcnt[i]>0)
				sctx->regalc_utcnt[i]--;

			if(!sctx->regalc_utcnt[i])
			{
				if((sctx->regalc_dirty)&(1<<i))
				{
					creg=bgbcc_shx_cachereg[i];		
					BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);
					sctx->regalc_dirty&=~(1<<i);
				}

				sctx->regalc_live&=~(1<<i);
			}

			return(1);
		}
	}

	return(0);
}

int BGBCC_SHXC_EmitLoadVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int creg;
	
	creg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, creg, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
	return(1);

//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_SHXC_EmitStoreVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register dreg, int sreg)
{
	int creg;
	
	creg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg, creg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, sreg);
	return(1);
}

int BGBCC_SHXC_EmitMovVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, ccxl_register sreg)
{
	int cdreg, csreg;
	
	cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R3);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);
		return(1);
	}
}

int BGBCC_SHXC_EmitMovVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, s32 imm)
{
	int cdreg, csreg;
	
	cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, cdreg, imm);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, imm);
	BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
	return(1);
}


int BGBCC_SHXC_EmitOpNmidVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, int nmid,
	ccxl_register dreg, ccxl_register sreg)
{
	int cdreg, csreg;
	
	cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nmid, csreg, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
		BGBCC_SHX_EmitOpRegReg(sctx, nmid, cdreg, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nmid, csreg, BGBCC_SH_REG_R3);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R3);
		BGBCC_SHX_EmitOpRegReg(sctx, nmid,
			BGBCC_SH_REG_R3, BGBCC_SH_REG_R3);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);
		return(1);
	}
}


int BGBCC_SHXC_EmitJCmpVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg;
	int nm1, nm2;
	
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BT;
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

		BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_SHX_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_SHX_EmitOpLabel(sctx, nm2, lbl);
		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegZero(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	int csreg, ctreg;
	int nm1, nm2;
	
	csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);

	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		BGBCC_SHX_EmitOpReg(sctx, nm1, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_SHX_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_SHX_EmitOpLabel(sctx, nm2, lbl);
		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}
	
	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
	return(0);
}
