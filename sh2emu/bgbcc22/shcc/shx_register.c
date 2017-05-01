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
 *   (-36) Saved FR12
 *   (-40) Saved FR13
 *   (-44) Saved FR14
 *   (-48) Saved FR15
 *   ...
 *   SP (Stack Ref Point)
 */


const byte bgbcc_shx_cachereg[6]={ 13, 11, 10, 9, 8, 255 };

int BGBCC_SHXC_EmitLoadFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int dreg2, treg;
	int p0, p1;
	int ofs1;
	int i, j, k;

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_SHXC_EmitLoadFrameOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs+(sctx->frm_size);

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, ofs1+4, dreg2+1);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_SP, ofs1+0, dreg2+0);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs1);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, BGBCC_SH_REG_R0, dreg2+0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, BGBCC_SH_REG_R0, dreg2+1);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
		BGBCC_SH_REG_SP, ofs1, dreg);
	if(p0>0)
		return(1);

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//	treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, BGBCC_SH_REG_R0, dreg);
		if(p1>0)
		{
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int dreg2, treg;
	int p0, p1;
	int ofs1;
	int i, j, k;

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_SHXC_EmitStoreFrameOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs+(sctx->frm_size);

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, BGBCC_SH_REG_SP, ofs1+4);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				dreg2+0, BGBCC_SH_REG_SP, ofs1+0);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs1);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+0, BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
		dreg, BGBCC_SH_REG_SP, ofs1);
	if(p0>0)
		return(1);

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg, BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
		if(p1>0)
		{
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLdaFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int dreg2;
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
		{
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitStoreStackOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int dreg2;
	int p0, p1;
	int ofs1;
	int i, j, k;

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_SHXC_EmitStoreStackOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs;

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, BGBCC_SH_REG_SP, ofs1+4);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				dreg2+0, BGBCC_SH_REG_SP, ofs1+0);
			return(1);
		}

		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs1);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+0, BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
		dreg, BGBCC_SH_REG_SP, ofs1);
	if(p0>0)
		return(1);

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg, BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
		if(p1>0)
		{
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitLoadBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int dreg2;
	int p0, p1;
	int i, j, k;

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{	return(BGBCC_SHXC_EmitLoadBRegOfsFpReg(ctx, sctx,
			nmid, breg, ofs, dreg));	}

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, breg, ofs+4);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				dreg2+0, breg, ofs+0);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			breg, BGBCC_SH_REG_R0, dreg2+0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			breg, BGBCC_SH_REG_R0, dreg2+1);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, nmid,
		breg, ofs, dreg);
	if(p0>0)
		return(1);

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, nmid,
			breg, BGBCC_SH_REG_R0, dreg);
		if(p1>0)
		{
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int dreg2;
	int p0, p1;
	int i, j, k;

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_FR0+(dreg&7)*2;
	
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+1, breg, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+0, breg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, breg, ofs+4);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				dreg2+0, breg, ofs+0);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+0, breg, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, breg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, nmid,
		dreg, breg, ofs);
	if(p0>0)
		return(1);

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
			dreg, BGBCC_SH_REG_R0, breg);
		if(p1>0)
		{
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
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

//	p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, nmid,
//		breg, ofs, dreg);
//	if(p0>0)
//		return(1);

	if(breg!=dreg)
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, ofs);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
	}else
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R3, ofs);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, BGBCC_SH_REG_R3, dreg);
	}

	return(1);
}


int BGBCC_SHXC_EmitLoadBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int dreg2, sw;
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
	}

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
	if(nm2>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg) ||
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_SHXC_RegNormalizeReg(ctx, sctx, dreg);
		sw=BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg2);

		BGBCC_SHX_EmitOpLdReg2Reg(sctx, nmid,
			breg, BGBCC_SH_REG_R0, dreg2+(sw^0));
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, nmid,
			breg, BGBCC_SH_REG_R0, dreg2+(sw^1));
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	BGBCC_SHX_EmitOpLdReg2Reg(sctx, nmid,
		breg, BGBCC_SH_REG_R0, dreg);
	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	return(1);
}

int BGBCC_SHXC_EmitStoreBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int dreg2, sw;
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
	}

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
	if(nm2>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg) ||
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_SHXC_RegNormalizeReg(ctx, sctx, dreg);
		sw=BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg2);

		BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
			dreg2+(sw^0), breg, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
			dreg2+(sw^1), breg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	p1=BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
		dreg, BGBCC_SH_REG_R0, breg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	return(1);
}

int BGBCC_SHXC_ScratchSafeStompReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		if(sctx->sreg_live&(1<<(reg&15)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_SHXC_ScratchStompReg(ctx, sctx, reg));
	}

	return(BGBCC_SHXC_ScratchStompReg(ctx, sctx, reg));
}

int BGBCC_SHXC_ScratchStompReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=1<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1<<((reg&15)+0);
		sctx->sreg_live|=1<<((reg&15)+1);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_LR0)
	{
		if((reg&7)>=6)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=1<<(2*(reg&7)+0);
		sctx->sfreg_live|=1<<(2*(reg&7)+1);
		return(1);
	}

	return(0);
}

int BGBCC_SHXC_ScratchHoldReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held|=1<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1<<((reg&15)+0);
		sctx->sreg_held|=1<<((reg&15)+1);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_LR0)
	{
		if((reg&7)>=6)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held|=1<<(2*(reg&7)+0);
		sctx->sfreg_held|=1<<(2*(reg&7)+1);
		return(1);
	}

	return(0);
}

int BGBCC_SHXC_ScratchReleaseReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(1<<(reg&15));
		sctx->sreg_live&=~(1<<(reg&15));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held&=~(1<<(reg&15));
		sctx->sfreg_live&=~(1<<(reg&15));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(1<<((reg&15)+0));
		sctx->sreg_held&=~(1<<((reg&15)+1));
		sctx->sreg_live&=~(1<<((reg&15)+0));
		sctx->sreg_live&=~(1<<((reg&15)+1));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_LR0)
	{
		if((reg&7)>=6)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held&=~(1<<(2*(reg&7)+0));
		sctx->sfreg_held&=~(1<<(2*(reg&7)+1));
		sctx->sfreg_live&=~(1<<(2*(reg&7)+0));
		sctx->sfreg_live&=~(1<<(2*(reg&7)+1));
		return(1);
	}

	return(0);
}

int BGBCC_SHXC_ScratchAllocReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int cls)
{
	int i;

	if(!cls || (cls==BGBCC_SH_REGCLS_GR))
	{
		for(i=1; i<8; i++)
		{
			if(!(sctx->sreg_live&(1<<i)))
			{
				sctx->sreg_live|=(1<<i);
				return(BGBCC_SH_REG_R0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	if(cls==BGBCC_SH_REGCLS_FR)
	{
		for(i=0; i<12; i++)
		{
			if(!(sctx->sfreg_live&(1<<i)))
			{
				sctx->sfreg_live|=(1<<i);
				return(BGBCC_SH_REG_FR0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	if(cls==BGBCC_SH_REGCLS_GR2)
	{
		for(i=1; i<7; i++)
		{
			if(!(sctx->sreg_live&(3<<i)))
			{
				sctx->sreg_live|=(3<<i);
				return(BGBCC_SH_REG_R0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	if(cls==BGBCC_SH_REGCLS_FR2)
	{
		for(i=0; i<6; i++)
		{
			if(!(sctx->sfreg_live&(3<<(2*i))))
			{
				sctx->sfreg_live|=(3<<(2*i));
				return(BGBCC_SH_REG_DR0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	BGBCC_DBGBREAK
	return(-1);
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

int BGBCC_SHXC_SaveFrameOfsForFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int reg)
{
	int ofs;
	
	ofs=0;
	switch(reg)
	{
	case BGBCC_SH_REG_FR12: ofs=-36; break;
	case BGBCC_SH_REG_FR13: ofs=-40; break;
	case BGBCC_SH_REG_FR14: ofs=-44; break;
	case BGBCC_SH_REG_FR15: ofs=-48; break;
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
		if(sctx->reg_save&(1<<(reg&15)))
			return(0);

		sctx->reg_save|=1<<(reg&15);
		BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}

	ofs=BGBCC_SHXC_SaveFrameOfsForFpReg(ctx, sctx, reg);
	if(ofs!=0)
	{
		if(sctx->freg_save&(1<<(reg&15)))
			return(0);

		sctx->freg_save|=1<<(reg&15);
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

	ofs=BGBCC_SHXC_SaveFrameOfsForReg(ctx, sctx, reg);
	if(ofs!=0)
	{
		if(!(sctx->reg_save&(1<<(reg&15))))
			return(0);

//		sctx->reg_save|=1<<(reg&15);
		BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}

	ofs=BGBCC_SHXC_SaveFrameOfsForFpReg(ctx, sctx, reg);
	if(ofs!=0)
	{
		if(!(sctx->freg_save&(1<<(reg&15))))
			return(0);

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
	ccxl_type tty;
	char *s0;
	double f, g;
	int dreg2;
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;

		if(BGBCC_CCXL_TypeArrayP(ctx, ctx->cur_func->locals[j]->type) ||
			BGBCC_CCXL_TypeValueObjectP(ctx, ctx->cur_func->locals[j]->type))
		{
			k=ctx->cur_func->locals[j]->fxmoffs+(sctx->frm_offs_fix);
			i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

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
		if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg))
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			*(f32 *)(&k)=j;
			i=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg);
//			i=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
			return(i);
		}

		if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);

			f=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, j);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, k);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg2+0);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R1, dreg2+1);
			return(1);
		}
	
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		i=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(i);
	}

	if(BGBCC_CCXL_IsRegImmFloatP(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg))
	{
		if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			*(f32 *)(&k)=f;
//			i=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, k);
//			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg);
			i=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, k);
			return(i);
		}

		if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);

			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));

			BGBCC_SHX_EmitLoadRegImmPair32(sctx, dreg2, k, j);

#if 0
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, j);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, k);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg2+0);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R1, dreg2+1);
#endif
			return(1);
		}
	
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
		j=f;
		i=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(i);
	}

	if(BGBCC_CCXL_IsRegImmStringP(ctx, sreg))
	{
		s0=BGBCC_CCXL_GetRegImmStringValue(ctx, sreg);
		k=BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);

		if(sctx->is_pic)
		{
			BGBCC_CCXL_StubError(ctx);
		}else
		{
			BGBCC_SHX_EmitLoadRegLabelAbs(sctx, dreg, k);
		}
		return(1);
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

		tty=BGBCC_CCXL_GetRegType(ctx, sreg);

		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
			BGBCC_CCXL_TypeFunctionP(ctx, tty) ||
			BGBCC_CCXL_TypeArrayP(ctx, tty))
		{
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
			BGBCC_CCXL_TypeSmallLongP(ctx, tty) ||
			BGBCC_CCXL_TypePointerP(ctx, tty))
		{
//			p1=BGBCC_SHX_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
//				dreg, dreg);

			BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL, dreg, 0, dreg);

			return(1);
		}

		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
//			p1=BGBCC_SHX_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg, dreg);

			BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL, dreg, 0, dreg);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
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
	ccxl_type tty;
	int cdreg, treg, tsz;
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, dreg))
	{
		j=dreg.val&CCXL_REGID_BASEMASK;

		if(BGBCC_CCXL_TypeValueObjectP(ctx, ctx->cur_func->locals[j]->type))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx,
				ctx->cur_func->locals[j]->type);

			k=ctx->cur_func->locals[j]->fxmoffs+(sctx->frm_offs_fix);
			i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, treg);

			BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, treg, cdreg, tsz, 4);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(i);
		}

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
		treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);

		if(sctx->is_pic)
		{
			j=BGBCC_CCXL_GetRegID(ctx, dreg);
			k=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);

			p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, treg);
		}else
		{
			j=BGBCC_CCXL_GetRegID(ctx, dreg);
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, j);
			BGBCC_SHX_EmitLoadRegLabelAbs(sctx, treg, k);
		}

		tty=BGBCC_CCXL_GetRegType(ctx, dreg);

		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		{
			tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
			BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, treg, cdreg, tsz, 4);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeFunctionP(ctx, tty))
		{
			BGBCC_CCXL_StubError(ctx);
			return(0);
		}

		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
			BGBCC_CCXL_TypeSmallLongP(ctx, tty) ||
			BGBCC_CCXL_TypePointerP(ctx, tty))
		{
//			p1=BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVL,
//				dreg, treg);

			BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL, treg, 0, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
//			p1=BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg, treg);

			BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL, treg, 0, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
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

	if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
		BGBCC_CCXL_IsRegDoubleP(ctx, reg))
	{
		return(BGBCC_SHXC_EmitTryGetFpRegister(ctx, sctx, reg, fl));
	}

	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
	{
		return(BGBCC_SHXC_EmitTryGetLpRegister(ctx, sctx, reg, fl));
	}

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
	int creg, lng;
	int i;

	creg=BGBCC_SHXC_EmitTryGetRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
		BGBCC_CCXL_IsRegDoubleP(ctx, reg))
	{
		return(BGBCC_SHXC_EmitGetFpRegister(ctx, sctx, reg, fl));
	}
	
	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
	{
		return(BGBCC_SHXC_EmitGetLpRegister(ctx, sctx, reg, fl));
	}

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
	static int rchk=0;
	int creg;
	int i;

	if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
		BGBCC_CCXL_IsRegDoubleP(ctx, reg))
	{
		return(BGBCC_SHXC_EmitReleaseFpRegister(ctx, sctx, reg));
	}

	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
	{
		return(BGBCC_SHXC_EmitReleaseLpRegister(ctx, sctx, reg));
	}

	/* value in register? */
	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=1<<i;
//			if(sctx->regalc_utcnt[i]>0)
//				sctx->regalc_utcnt[i]--;

			if(sctx->regalc_utcnt[i]<=0)
				__debugbreak();

			sctx->regalc_utcnt[i]--;

			if(!sctx->regalc_utcnt[i])
			{
				if((sctx->regalc_dirty)&(1<<i))
				{
					creg=bgbcc_shx_cachereg[i];		
					if(!rchk)
					{
						rchk++;
						BGBCC_SHXC_EmitStoreFrameVRegReg(ctx,
							sctx, reg, creg);
						rchk--;
					}
					sctx->regalc_dirty&=~(1<<i);
				}

				sctx->regalc_live&=~(1<<i);
			}

			return(1);
		}
	}

	return(0);
}

int BGBCC_SHXC_EmitSyncRegisters(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	ccxl_register reg;
	int creg;
	int i;

	/* value in register? */
	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;

		if((sctx->regalc_dirty)&(1<<i))
		{
			reg=sctx->regalc_map[i];
			creg=bgbcc_shx_cachereg[i];		

			if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
				creg=bgbcc_shx_lcachereg[i];

			BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);

			sctx->regalc_utcnt[i]=0;
			sctx->regalc_dirty&=~(1<<i);
			sctx->regalc_live&=~(1<<i);
		}
	}

	return(0);
}

int BGBCC_SHXC_EmitLabelFlushRegisters(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	int i;

	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;

		sctx->regalc_map[i].val=-1;
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_dirty&=~(1<<i);
		sctx->regalc_live&=~(1<<i);
	}
}

int BGBCC_SHXC_EmitMovRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int sreg, int dreg)
{
	int sreg2, dreg2;

	if(sreg==dreg)
		return(1);

	if(BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, dreg))
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg, dreg);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg))
	{
		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, sreg, dreg);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg))
	{
		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_LDS, sreg, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, dreg))
	{
		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FLDS, sreg, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		sreg2=BGBCC_SH_REG_FR0+((sreg&7)*2);
		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
	
		if(BGBCC_SHXC_GetStatusFpscrSz(ctx, sctx))
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, sreg2, dreg2);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, sreg2+0, dreg2+0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, sreg2+1, dreg2+1);
		}
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);

//	nm=BGBCC_SH_NMID_MOV;
//	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, creg))
//		nm=BGBCC_SH_NMID_FMOV;
//	BGBCC_SHX_EmitOpRegReg(sctx, nm, creg, dreg);
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
//	return(1);
}

int BGBCC_SHXC_EmitLoadVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int creg, nm;
	
	creg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
//		nm=BGBCC_SH_NMID_MOV;
//		if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, creg))
//			nm=BGBCC_SH_NMID_FMOV;
//		BGBCC_SHX_EmitOpRegReg(sctx, nm, creg, dreg);
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx, creg, dreg);

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
//		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg, creg);
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx, sreg, creg);
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
//			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, csreg, cdreg);
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


int BGBCC_SHXC_EmitFpConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nm1, int nm2,
	ccxl_register dreg, ccxl_register sreg)
{
	int cdreg, csreg;
	
	cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
	csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
//	BGBCC_SHXC_EmitMovRegReg(ctx, sctx, csreg, cdreg);

	if(	BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, csreg) ||
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, cdreg))
		{ BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx); }
	else
		{ BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx); }

	csreg=BGBCC_SHXC_RegNormalizeReg(ctx, sctx, csreg);
	cdreg=BGBCC_SHXC_RegNormalizeReg(ctx, sctx, cdreg);
	if((nm1==BGBCC_SH_NMID_LDS) || (nm1==BGBCC_SH_NMID_FLDS))
		{ BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, BGBCC_SH_REG_FPUL); }
	else
		{ BGBCC_SHX_EmitOpReg(sctx, nm1, csreg); }

	if((nm2==BGBCC_SH_NMID_STS) || (nm2==BGBCC_SH_NMID_FSTS))
		{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_FPUL, cdreg); }
	else
		{ BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg); }

	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
	return(1);
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


int BGBCC_SHXC_EmitJCmpVRegVRegInt(
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
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegVRegInt(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegVRegFloat(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegZeroInt(
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
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}
	
	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegZero(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegZeroInt(ctx, sctx,
			type, sreg, cmp, lbl));
	}

#if 0
	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegZeroFloat(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
