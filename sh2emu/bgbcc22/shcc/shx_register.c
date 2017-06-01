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
 *   (-36) Saved FR15
 *   (-40) Saved FR14
 *   (-44) Saved FR13
 *   (-48) Saved FR12
 *   ...
 *   SP (Stack Ref Point)
 */


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
	
		p0=BGBCC_SHX_TryEmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, ofs1+4, dreg2+1);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_SP, ofs1+0, dreg2+0);
			return(1);
		}

		if(sctx->sreg_live&1)
		{
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);

			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				treg, ofs1);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_SP, treg);
			BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_MOVL, treg, dreg+0);
			BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_MOVL, treg, dreg+1);

			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
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

	p0=BGBCC_SHX_TryEmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
		BGBCC_SH_REG_SP, ofs1, dreg);
	if(p0>0)
		return(1);

	if(sctx->sreg_live&1)
	{
		treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);

		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			treg, ofs1);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_SP, treg);
		BGBCC_SHX_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL, treg, dreg);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
		return(1);
	}

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
	
		p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, BGBCC_SH_REG_SP, ofs1+4);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				dreg2+0, BGBCC_SH_REG_SP, ofs1+0);
			return(1);
		}

		if(sctx->sreg_live&1)
		{
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);

			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				treg, ofs1+4);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_SP, treg);
			BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVL, dreg+1, treg);
			BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_MOVL, dreg+0, treg);

			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
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

	p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
		dreg, BGBCC_SH_REG_SP, ofs1);
	if(p0>0)
		return(1);

	if(sctx->sreg_live&1)
	{
		treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);

		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			treg, ofs1);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_SP, treg);
		BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVL, dreg, treg);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
		return(1);
	}

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
	
		p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
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

	p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
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
	
		p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
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

	p0=BGBCC_SHX_TryEmitOpLdRegDispReg(sctx, nmid,
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

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg, breg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

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
	
		p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
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

	p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, nmid,
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
	int ctreg;
	int p0, p1;
	int i, j, k;

//	p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, nmid,
//		breg, ofs, dreg);
//	if(p0>0)
//		return(1);

	if(!ofs)
	{
		if(breg!=dreg)
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, breg, dreg);
		return(1);
	}

	if(breg!=dreg)
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, ofs);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
	}else
	{
		ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, ofs);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ctreg, dreg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);

//		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R3);
//		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R3, ofs);
//		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, BGBCC_SH_REG_R3, dreg);
//		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R3);
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
//		sw=BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

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
//		sw=BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

		BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
			dreg2+(sw^0), breg, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
			dreg2+(sw^1), breg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
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

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=7)
			{ BGBCC_DBGBREAK }
		if(sctx->sreg_live&(3<<(reg&15)))
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
		if((reg&15)>=7)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=3<<((reg&15)+0);
//		sctx->sreg_live|=1<<((reg&15)+1);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
	{
		if((reg&7)>=6)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=3<<(2*(reg&7)+0);
//		sctx->sfreg_live|=1<<(2*(reg&7)+1);
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
		sctx->sreg_held&=~(3<<((reg&15)+0));
//		sctx->sreg_held&=~(1<<((reg&15)+1));
		sctx->sreg_live&=~(3<<((reg&15)+0));
//		sctx->sreg_live&=~(1<<((reg&15)+1));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_LR0)
	{
		if((reg&7)>=6)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held&=~(3<<(2*(reg&7)+0));
//		sctx->sfreg_held&=~(1<<(2*(reg&7)+1));
		sctx->sfreg_live&=~(3<<(2*(reg&7)+0));
//		sctx->sfreg_live&=~(1<<(2*(reg&7)+1));
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

	if(!cls ||
		(cls==BGBCC_SH_REGCLS_GR) ||
		(cls==BGBCC_SH_REGCLS_VO_GR) ||
		(cls==BGBCC_SH_REGCLS_VO_REF) ||
		(cls==BGBCC_SH_REGCLS_AR_REF))
	{
//		for(i=1; i<8; i++)
		for(i=7; i>=0; i--)
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

	if((cls==BGBCC_SH_REGCLS_GR2) ||
		(cls==BGBCC_SH_REGCLS_VO_GR2))
	{
//		for(i=1; i<7; i++)
		for(i=6; i>=0; i--)
		{
			if(!(sctx->sreg_live&(3<<i)))
			{
				sctx->sreg_live|=(3<<i);
				return(BGBCC_SH_REG_LR0+i);
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
	case BGBCC_SH_REG_FR15: ofs=-36; break;
	case BGBCC_SH_REG_FR14: ofs=-40; break;
	case BGBCC_SH_REG_FR13: ofs=-44; break;
	case BGBCC_SH_REG_FR12: ofs=-48; break;
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

		if(!sctx->is_simpass && !(sctx->reg_vsave&(1<<(reg&15))))
			{ BGBCC_DBGBREAK }

		sctx->reg_save|=1<<(reg&15);
		BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}

	ofs=BGBCC_SHXC_SaveFrameOfsForFpReg(ctx, sctx, reg);
	if(ofs!=0)
	{
		if(sctx->freg_save&(1<<(reg&15)))
			return(0);

		if(!sctx->is_simpass && !(sctx->freg_vsave&(1<<(reg&15))))
			{ BGBCC_DBGBREAK }

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
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, k*4);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
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

int BGBCC_SHXC_EmitLoadFrameVRegByValReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, tr0, rcls;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->locals[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2))
		{
			p0=BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->regs[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2))
		{
			p0=BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

//		k=ctx->cur_func->regs[j]->fxoffs;

		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, tr0);

		BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, tr0, 0, dreg);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->args[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2))
		{
			p0=BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

#if 1
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
#endif

//		k=ctx->cur_func->regs[j]->fxoffs;

#if 0
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, tr0);
		BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, tr0, 0, dreg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
#endif
		return(1);
	}

	p0=BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
	return(p0);
}

int BGBCC_SHXC_EmitStoreFrameVRegByValReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, tr0, rcls;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->locals[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2))
		{
			p0=BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->regs[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2))
		{
			p0=BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

//		k=ctx->cur_func->regs[j]->fxoffs;

		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, tr0);

		BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, tr0, 0, dreg);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->args[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2))
		{
			p0=BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, dreg);
		return(1);
	}

	p0=BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
	return(p0);
}

int BGBCC_SHXC_EmitLoadFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	ccxl_type tty;
	char *s0;
	double f, g;
	s64 li, lj;
	int dreg2, treg;
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;

		tty=ctx->cur_func->locals[j]->type;
//		if(BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		if(BGBCC_CCXL_TypeArrayP(ctx, tty))
		{
			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);
			i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

#if 1
//		if(BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		if(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
		{
			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);
			i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

		if((ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_GR) ||
			(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_GR2))
		{
			k=ctx->cur_func->locals[j]->fxoffs;
			i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}
#endif

		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			k=1;
		}

//		k=sctx->frm_offs_lcl+j*4;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		tty=ctx->cur_func->regs[j]->type;
		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			k=1;
		}

		j=sreg.val&CCXL_REGID_BASEMASK;
//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;

		if((ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_GR) ||
			(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_GR2))
		{
			k=ctx->cur_func->args[j]->fxoffs;
			i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

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
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg);
//			i=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
			return(1);
		}

		if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);

			f=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));
			BGBCC_SHX_EmitLoadRegImmPair32(sctx, dreg2, k, j);
//			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, j);
//			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R1, k);
//			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg2+0);
//			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R1, dreg2+1);
			return(1);
		}

		if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			k=j>>31;
			BGBCC_SHX_EmitLoadRegImmPair32(sctx, dreg, j, k);
			return(1);
		}
	
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
	{
		if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_SHX_EmitLoadRegImm64P(sctx, dreg, li);
//			j=li;
//			k=li>>32;
//			BGBCC_SHX_EmitLoadRegImmPair32(sctx, dreg, j, k);
			return(1);
		}

		if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);

			f=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));
			BGBCC_SHX_EmitLoadRegImmPair32(sctx, dreg2, k, j);
			return(1);
		}

		j=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
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
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, k);
			return(1);
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

		if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
		{
			li=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			BGBCC_SHX_EmitLoadRegImm64P(sctx, dreg, li);
//			j=li;
//			k=li>>32;
//			BGBCC_SHX_EmitLoadRegImmPair32(sctx, dreg, j, k);
			return(1);
		}
	
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
		j=f;
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
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
#if 0
		if(BGBCC_CCXL_TypeFloatP(ctx, tty) &&
			BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg))
		{
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, treg);
			BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_FMOVS, treg, 0, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, tty) &&
			BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, treg);
			BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_FMOVS, treg, 0, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}
#endif

		treg=dreg;
		if(	BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg) ||
			BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg) ||
			BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg)	)
		{
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		}

		if(sctx->is_pic)
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);

			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
//			BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
			BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, treg);
		}else
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, j);
//			BGBCC_SHX_EmitLoadRegLabelAbs(sctx, dreg, k);
			BGBCC_SHX_EmitLoadRegLabelAbs(sctx, treg, k);
		}

		tty=BGBCC_CCXL_GetRegType(ctx, sreg);

		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
//			BGBCC_CCXL_TypeFunctionP(ctx, tty) ||
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

			if(treg!=dreg)
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeFunctionP(ctx, tty))
		{
			return(1);
		}

#if 1
		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
//			p1=BGBCC_SHX_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg, dreg);

//			BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
//				BGBCC_SH_NMID_FMOVS, dreg, 0, dreg);

			BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_FMOVS, treg, 0, dreg);

			if(treg!=dreg)
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}
#endif

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	if(BGBCC_CCXL_IsRegImmInt128P(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmFloat128P(ctx, sreg))
	{
		BGBCC_CCXL_GetRegImmX128Value(ctx, sreg, &li, &lj);
		k=BGBCC_SHXC_IndexLitInt128(ctx, sctx, li, lj);
	
		if(sctx->is_pic)
		{
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
		}else
		{
			BGBCC_SHX_EmitLoadRegLabelAbs(sctx, dreg, k);
		}

		return(1);
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
//			cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx,
				ctx->cur_func->locals[j]->type);

			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);
			if(ctx->cur_func->locals[j]->regcls!=BGBCC_SH_REGCLS_VO_REF)
				k=ctx->cur_func->locals[j]->fxoffs;

			i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, treg);

			BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, treg, sreg, tsz, 4);

//			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
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
//		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
//		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(sctx->is_pic)
		{
			j=BGBCC_CCXL_GetRegID(ctx, dreg);
			k=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);

			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
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
			BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, treg, sreg, tsz, 4);

//			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
			BGBCC_CCXL_TypeSmallLongP(ctx, tty) ||
			BGBCC_CCXL_TypePointerP(ctx, tty))
		{
//			p1=BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVL,
//				dreg, treg);

			BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL, treg, 0, sreg);

//			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
//			p1=BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg, treg);

			BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_FMOVS, treg, 0, sreg);

//			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeFunctionP(ctx, tty))
		{
			BGBCC_CCXL_StubError(ctx);
			return(0);
		}

		BGBCC_CCXL_StubError(ctx);
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

		if(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);

		i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->regs[j]->fxoffs;

//		if(ctx->cur_func->regs[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
//			k=(ctx->cur_func->regs[j]->fxmoffs)+(sctx->frm_offs_fix);

		i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;

//		if(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
//			k=(ctx->cur_func->args[j]->fxmoffs)+(sctx->frm_offs_fix);

		i=BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, sreg))
	{
		if(sctx->is_pic)
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_SHXC_GetGblIndexGotIndex(ctx, sctx, j);

			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
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


// const byte bgbcc_shx_cachereg[6]={ 13, 11, 10, 9, 8, 255 };
const byte bgbcc_shx_cachereg[8]={ 14, 13, 12, 11, 10, 9, 8, 255 };
const byte bgbcc_shx_maxreg=8;


/* Try to get Variable as register.
 * Will return a value as a register if possible, but may fail.
 * Returns ZZR if it is unable to do so.
 */
int BGBCC_SHXC_EmitTryGetRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg, int fl)
{
	static int rchk=0;
	ccxl_register reg1;
	int creg, excl, nsv;
	int i, bi;

	if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
		BGBCC_CCXL_IsRegDoubleP(ctx, reg))
	{
		return(BGBCC_SHXC_EmitTryGetFpRegister(ctx, sctx, reg, fl));
	}

	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
	{
		return(BGBCC_SHXC_EmitTryGetLpRegister(ctx, sctx, reg, fl));
	}

	excl=0;
	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

	/* value already in a register? */
//	for(i=0; i<5; i++)
	for(i=0; i<bgbcc_shx_maxreg; i++)
	{
		if(excl&(1<<i))
			continue;

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

	if((fl&3)==1)
	{
		bi=-1; nsv=0;
	
		/* Check for registers not holding a live value. */
//		for(i=0; i<5; i++)
		for(i=0; i<bgbcc_shx_maxreg; i++)
		{
			if(excl&(1<<i))
				continue;

			if(!((sctx->regalc_save)&(1<<i)))
			{
				nsv++;
				continue;
			}
			if(!((sctx->regalc_live)&(1<<i)))
			{
				reg1=sctx->regalc_map[i];
				if((bi<0) || 
					(sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
						bi=i;
				if(BGBCC_SHXC_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i; break; }
			}
		}

		/* Check if we are better off allocating another register */
		if(bi>=0)
		{
			reg1=sctx->regalc_map[bi];
			if((nsv>=2) && (sctx->regalc_ltcnt[bi]<=2) &&
				sctx->is_simpass &&
				!BGBCC_SHXC_CheckVRegIsZRegP(ctx, sctx, reg1))
			{
				bi=-1;
			}
		}
		
#if 1
		if(bi>=0)
		{
			i=bi;

			BGBCC_SHXC_EmitSyncRegisterIndex(ctx, sctx, i);

			sctx->regalc_ltcnt[i]=0;
			sctx->regalc_map[i]=reg;
			sctx->regalc_utcnt[i]=1;
			sctx->regalc_live|=1<<i;
			sctx->regalc_dirty|=1<<i;
			creg=bgbcc_shx_cachereg[i];
			return(creg);
		}
#endif
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
	static int rchk=0;
	ccxl_register reg1;
	int creg, lng, excl, bi, nsv;
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

	excl=0;
	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

//	for(i=0; i<5; i++)
	for(i=0; i<bgbcc_shx_maxreg; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

	bi=-1; nsv=0;
	/* Check for registers not holding a live value. */
//	for(i=0; i<5; i++)
	for(i=0; i<bgbcc_shx_maxreg; i++)
	{
		if(excl&(1<<i))
			continue;

		if(!((sctx->regalc_save)&(1<<i)))
		{
			nsv++;
			continue;
		}
		if(!((sctx->regalc_live)&(1<<i)))
		{
			reg1=sctx->regalc_map[i];
//			if((bi<0) || (sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
			if((bi<0) || (sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
				bi=i;
			if(BGBCC_SHXC_CheckVRegIsZRegP(ctx, sctx, reg1))
				{ bi=i; break; }
		}
	}

	/* Check if we are better off allocating another register */
	if(bi>=0)
	{
		reg1=sctx->regalc_map[bi];
		if((nsv>=2) && (sctx->regalc_ltcnt[bi]<=2) &&
			sctx->is_simpass && !BGBCC_SHXC_CheckVRegIsZRegP(ctx, sctx, reg1))
		{
			bi=-1;
		}
	}

#if 1
	if(bi>=0)
	{
		i=bi;

		BGBCC_SHXC_EmitSyncRegisterIndex(ctx, sctx, i);

		if(((reg.val&0xFFF)==0xFFF) && ((reg.val>>56)<4))
			{ BGBCC_DBGBREAK }

		sctx->regalc_ltcnt[i]=0;
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
#endif

	/* Check for unallocated registers. */
//	for(i=0; i<5; i++)
	for(i=0; i<bgbcc_shx_maxreg; i++)
	{
		if(excl&(1<<i))
			continue;

		if((sctx->regalc_save)&(1<<i))
			continue;
		
		sctx->regalc_ltcnt[i]=0;
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
	ccxl_register reg1;
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
//	for(i=0; i<5; i++)
	for(i=0; i<bgbcc_shx_maxreg; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;
			
		reg1=sctx->regalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
		{
			sctx->regalc_live|=1<<i;
//			if(sctx->regalc_utcnt[i]>0)
//				sctx->regalc_utcnt[i]--;

			if(sctx->regalc_utcnt[i]<=0)
				__debugbreak();

			sctx->regalc_utcnt[i]--;

			if(!sctx->regalc_utcnt[i])
			{
				BGBCC_SHXC_EmitSyncRegisterIndex(ctx, sctx, i);

				sctx->regalc_live&=~(1<<i);

				if((sctx->regalc_dirty)&(1<<i))
					if((reg1.val&0xFFF)==0xFFF)
						{ BGBCC_DBGBREAK }
			}

			return(1);
		}
	}

	return(0);
}

int BGBCC_SHXC_EmitSyncRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int rgix)
{
	static int rchk=0;
	ccxl_register reg;
	int creg;
	int i;

	i=rgix;

	if(!((sctx->regalc_save)&(1<<i)))
		return(0);

	if((sctx->regalc_dirty)&(1<<i))
	{
		reg=sctx->regalc_map[i];
		creg=bgbcc_shx_cachereg[i];		

		if((reg.val&CCXL_REGID_REGMASK)==CCXL_REGID_REGMASK)
			return(0);

		if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
			creg=bgbcc_shx_lcachereg[i+1];

		if(!rchk)
		{
			rchk=1;
			BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);
			rchk=0;
		}

		sctx->regalc_dirty&=~(1<<i);
	}

	return(1);
}

int BGBCC_SHXC_EmitSyncRegisters(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	int i;

	BGBCC_SHXC_EmitSyncFpRegisters(ctx, sctx);

	/* value in register? */
	for(i=0; i<5; i++)
	{
		BGBCC_SHXC_EmitSyncRegisterIndex(ctx, sctx, i);
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_live&=~(1<<i);
	}

	return(0);
}

int BGBCC_SHXC_EmitLabelFlushRegisters(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	int i;

	BGBCC_SHXC_EmitLabelFlushFpRegisters(ctx, sctx);

	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;

		sctx->regalc_map[i].val=-1;
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_dirty&=~(1<<i);
		sctx->regalc_live&=~(1<<i);
	}
	
	return(0);
}

int BGBCC_SHXC_EmitMovRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int sreg, int dreg)
{
	int sreg2, dreg2, sw;

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

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		sreg2=BGBCC_SH_REG_R0+(sreg&15);
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg2+0, dreg2+0);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg2+1, dreg2+1);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		sreg2=BGBCC_SH_REG_R0+(sreg&15);
		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		sw=(sctx->is_le!=0);

		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_LDS, sreg2+0, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg2+(0^sw));

		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_LDS, sreg2+1, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg2+(1^sw));
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		sreg2=BGBCC_SH_REG_FR0+((sreg&7)*2);
		sw=(sctx->is_le!=0);

		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FLDS, sreg2+0, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg2+(0^sw));
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FLDS, sreg2+1, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg2+(1^sw));

		return(1);
	}

	printf("BGBCC_SHXC_EmitMovRegReg: "
		"Bad MOV Regs Rm=%02X Rn=%02X\n", sreg, dreg);
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
	int tr0, rcls;

#if 1
	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);
	if((rcls==BGBCC_SH_REGCLS_VO_GR) || (rcls==BGBCC_SH_REGCLS_VO_GR2))
	{

		if((BGBCC_CCXL_IsRegLocalP(ctx, sreg) ||
			BGBCC_CCXL_IsRegArgP(ctx, sreg)) &&
			(BGBCC_CCXL_IsRegLocalP(ctx, dreg) ||
			BGBCC_CCXL_IsRegArgP(ctx, dreg)))
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, rcls);
//			BGBCC_SHXC_EmitLoadFrameVRegByValReg
			BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx, sreg, tr0);
			BGBCC_SHXC_EmitStoreFrameVRegByValReg(ctx, sctx, dreg, tr0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

			return(1);
		}
	}
#endif

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
//	if(0)
//	if(1)
	{
		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		if((cdreg&0xF0) != (csreg&0xF0))
		{
			printf("BGBCC_SHXC_EmitMovVRegVReg: "
				"Bad MOV Regs Rm=%02X(%016llX) Rn=%02X(%016llX)\n",
				csreg, sreg.val, cdreg, dreg.val);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			cdreg=BGBCC_SH_REG_ZZR;
			csreg=BGBCC_SH_REG_ZZR;
		}
	}else
	{
		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	}

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

//		if(BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR2);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR1);
//			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR1);
			return(1);
		}

//		if(BGBCC_CCXL_IsRegLongP(ctx, sreg))
		if(BGBCC_CCXL_TypeSgLongP(ctx, type))
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR2);
//			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			return(1);
		}

//		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R3);
//		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);

		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

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
	
	csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
	cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
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
	int cdreg, csreg, ctreg;
	
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
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitOpRegReg(sctx, nmid, csreg, ctreg);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}

		ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, ctreg);
		BGBCC_SHX_EmitOpRegReg(sctx, nmid, ctreg, ctreg);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
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
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHS;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGE;
			nm2=BGBCC_SH_NMID_BF;
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BF;
		}
		break;
	case CCXL_CMP_GE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHS;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGE;
			nm2=BGBCC_SH_NMID_BT;
		}
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

//		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

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

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegVRegLong(ctx, sctx,
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
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);

		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

		BGBCC_SHX_EmitOpReg(sctx, nm1, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_SHX_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_SHX_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegZero(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
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

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 0.0);
		return(BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 0.0);
		return(BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
